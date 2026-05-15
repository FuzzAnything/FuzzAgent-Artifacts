#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <vector>
#include <string>

namespace harness_002 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need at least operation mode (1 byte), timestamp precision (1 byte),
    // and some data for pcap file content
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation mode: 0 = pcap_dump_open, 1 = pcap_dump_open_append
    uint8_t dump_mode = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume timestamp precision: 0 = microsecond, 1 = nanosecond
    uint8_t tstamp_precision = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume link type for pcap_open_dead_with_tstamp_precision
    int linktype = fdp.ConsumeIntegralInRange<int>(1, 255);
    int snaplen = fdp.ConsumeIntegralInRange<int>(64, 65535);
    
    // Consume dump filename length and filename
    size_t filename_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string dump_filename = fdp.ConsumeBytesAsString(filename_len);
    
    // Remaining bytes are for pcap file data
    std::vector<uint8_t> pcap_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    if (pcap_data.empty()) {
        return 0;
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = nullptr;
    pcap_dumper_t *dumper = nullptr;
    
    // Create temporary file for source pcap data
    char source_filename[] = "/tmp/libpcap_fuzz_src_XXXXXX";
    int src_fd = mkstemp(source_filename);
    if (src_fd < 0) {
        return 0;
    }
    
    // Write pcap data to temp file
    if (write(src_fd, pcap_data.data(), pcap_data.size()) != (ssize_t)pcap_data.size()) {
        close(src_fd);
        unlink(source_filename);
        return 0;
    }
    close(src_fd);
    
    // Step 1: Open source pcap file with timestamp precision
    if (tstamp_precision == 0) {
        // Use microsecond precision (default)
        pcap_handle = pcap_open_offline(source_filename, errbuf);
    } else {
        // Use nanosecond precision
        pcap_handle = pcap_open_offline_with_tstamp_precision(source_filename, 
                                                              PCAP_TSTAMP_PRECISION_NANO, 
                                                              errbuf);
    }
    
    if (pcap_handle == nullptr) {
        // Try to open as dead capture handle if offline open failed
        pcap_handle = pcap_open_dead_with_tstamp_precision(linktype, snaplen, 
                                                          tstamp_precision == 0 ? 
                                                          PCAP_TSTAMP_PRECISION_MICRO : 
                                                          PCAP_TSTAMP_PRECISION_NANO);
        if (pcap_handle == nullptr) {
            unlink(source_filename);
            return 0;
        }
    }
    
    // Step 2: Open dump file for writing
    if (dump_mode == 0) {
        // Regular pcap_dump_open (overwrites existing file)
        dumper = pcap_dump_open(pcap_handle, dump_filename.c_str());
    } else {
        // pcap_dump_open_append (appends to existing file)
        dumper = pcap_dump_open_append(pcap_handle, dump_filename.c_str());
    }
    
    if (dumper == nullptr) {
        // Dump open failed - just clean up and return
        // Don't call pcap_dump_file, pcap_dump_ftell, or pcap_dump_flush with NULL pointer
        // as these APIs expect valid dumper pointers
        pcap_close(pcap_handle);
        unlink(source_filename);
        return 0;
    }
    
    // Step 3: Test dump file related APIs
    FILE* dump_file = pcap_dump_file(dumper);
    
    // Test pcap_dump_ftell (traditional version)
    long ftell_result = pcap_dump_ftell(dumper);
    
    // Test pcap_dump_ftell64 (64-bit version if available)
    // Note: pcap_dump_ftell64 might not be available on all platforms
    // We'll use conditional compilation or just call if function exists
    
    // Step 4: Read packets from source and write to dump
    struct pcap_pkthdr* header;
    const u_char* pkt;
    int packet_count = 0;
    int read_result;
    
    // Read and dump packets (limit to 100 packets for performance)
    while (packet_count < 100) {
        read_result = pcap_next_ex(pcap_handle, &header, &pkt);
        
        if (read_result <= 0) {
            // No more packets or error
            break;
        }
        
        // Test pcap_dump function
        pcap_dump((u_char*)dumper, header, pkt);
        
        // Periodically test pcap_dump_flush
        if (packet_count % 10 == 0) {
            int flush_result = pcap_dump_flush(dumper);
        }
        
        packet_count++;
    }
    
    // Step 5: Test pcap_dump with various header values
    // Create synthetic packet headers for additional testing
    if (packet_count == 0) {
        // If no packets were read, create some synthetic ones
        for (int i = 0; i < 5; i++) {
            struct pcap_pkthdr synthetic_header;
            synthetic_header.ts.tv_sec = fdp.ConsumeIntegral<long>();
            synthetic_header.ts.tv_usec = fdp.ConsumeIntegral<long>();
            synthetic_header.caplen = fdp.ConsumeIntegralInRange<bpf_u_int32>(1, 1500);
            synthetic_header.len = synthetic_header.caplen;
            
            // Create synthetic packet data
            std::vector<uint8_t> synthetic_pkt = fdp.ConsumeBytes<uint8_t>(
                std::min(synthetic_header.caplen, (bpf_u_int32)1024));
            
            if (!synthetic_pkt.empty()) {
                pcap_dump((u_char*)dumper, &synthetic_header, synthetic_pkt.data());
            }
        }
    }
    
    // Final flush before closing
    int final_flush = pcap_dump_flush(dumper);
    
    // Step 6: Test pcap_dump_close
    pcap_dump_close(dumper);
    
    // Step 7: Test reopening dump file in append mode if we used regular mode initially
    if (dump_mode == 0 && packet_count > 0) {
        // Try to open the dump file in append mode
        dumper = pcap_dump_open_append(pcap_handle, dump_filename.c_str());
        if (dumper != nullptr) {
            // Add one more packet to appended file
            struct pcap_pkthdr append_header;
            append_header.ts.tv_sec = fdp.ConsumeIntegral<long>();
            append_header.ts.tv_usec = fdp.ConsumeIntegral<long>();
            append_header.caplen = fdp.ConsumeIntegralInRange<bpf_u_int32>(1, 1500);
            append_header.len = append_header.caplen;
            
            std::vector<uint8_t> append_pkt = fdp.ConsumeBytes<uint8_t>(
                std::min(append_header.caplen, (bpf_u_int32)1024));
            
            if (!append_pkt.empty()) {
                pcap_dump((u_char*)dumper, &append_header, append_pkt.data());
                pcap_dump_flush(dumper);
            }
            
            pcap_dump_close(dumper);
        }
    }
    
    // Clean up source pcap handle
    pcap_close(pcap_handle);
    
    // Clean up temporary files
    unlink(source_filename);
    
    // Try to clean up dump file if it was created
    if (access(dump_filename.c_str(), F_OK) == 0) {
        unlink(dump_filename.c_str());
    }
    
    return 0;
}

} // namespace harness_002
