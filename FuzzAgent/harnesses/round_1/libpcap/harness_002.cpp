#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>

namespace harness_002 {
/*
 * libpcap fuzzing harness - harness_002.cpp
 * Targets file I/O subsystem with complete pcap file processing lifecycle
 * 
 * Primary APIs targeted:
 * - pcap_open_offline (read pcap files from disk)
 * - pcap_dump_open (create dump file for writing packets)
 * - pcap_dump (write packets to dump file)
 * - pcap_dump_flush (flush dump buffer)
 * - pcap_dump_file (get FILE* from dump handle)
 * - pcap_dump_ftell (get current position in dump file)
 * - pcap_dump_close (close dump file)
 * - pcap_close (close pcap handle)
 * 
 * Differentiates from harness_000 (packet capture APIs) and harness_001 (BPF filter handling)
 * by focusing exclusively on file I/O operations for reading and writing pcap files.
 * Targets the 150+ undiscovered branches in file I/O subsystem identified by CoverageAnalyzerAgent.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic file operations
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Create temporary input file for pcap_open_offline
    char input_filename[] = "/tmp/libpcap_fuzz_input_XXXXXX";
    int input_fd = mkstemp(input_filename);
    if (input_fd < 0) {
        return 0;
    }
    
    // Write fuzzer data to input file (simulating pcap file)
    std::vector<uint8_t> input_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (write(input_fd, input_data.data(), input_data.size()) != static_cast<ssize_t>(input_data.size())) {
        close(input_fd);
        unlink(input_filename);
        return 0;
    }
    close(input_fd);
    
    // Create temporary output file for pcap_dump_open
    char output_filename[] = "/tmp/libpcap_fuzz_output_XXXXXX";
    int output_fd = mkstemp(output_filename);
    if (output_fd < 0) {
        unlink(input_filename);
        return 0;
    }
    close(output_fd);
    
    // Open the input file with pcap_open_offline
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap_handle = pcap_open_offline(input_filename, errbuf);
    if (pcap_handle == nullptr) {
        // Clean up temp files
        unlink(input_filename);
        unlink(output_filename);
        return 0;
    }
    
    // Open dump file for writing packets
    pcap_dumper_t* dumper = pcap_dump_open(pcap_handle, output_filename);
    if (dumper == nullptr) {
        // Try alternative: open with append mode
        dumper = pcap_dump_open_append(pcap_handle, output_filename);
    }
    
    // Test pcap_dump_file if dumper was successfully opened
    if (dumper != nullptr) {
        FILE* dump_file = pcap_dump_file(dumper);
        (void)dump_file;  // Use result to avoid unused variable warning
        
        // Test pcap_dump_ftell
        long ftell_pos = pcap_dump_ftell(dumper);
        (void)ftell_pos;
        // Test pcap_dump_ftell64 if available
        #ifdef pcap_dump_ftell64
        int64_t ftell64_pos = pcap_dump_ftell64(dumper);
        (void)ftell64_pos;
        #endif
    }
    
    // Read packets from input file and write to dump file
    struct pcap_pkthdr *header;
    const u_char* packet_data;
    int packet_count = 0;
    const int MAX_PACKETS = 100;  // Limit to prevent infinite loops
    
    while (packet_count < MAX_PACKETS) {
        int result = pcap_next_ex(pcap_handle, &header, &packet_data);
        
        if (result <= 0) {
            // No more packets or error
            break;
        }
        
        // Write packet to dump file if dumper is available
        if (dumper != nullptr) {
            pcap_dump(reinterpret_cast<u_char*>(dumper), header, packet_data);
            
            // Occasionally flush the dump buffer
            if (packet_count % 10 == 0) {
                pcap_dump_flush(dumper);
            }
        }
        
        packet_count++;
    }
    
    // Get statistics
    struct pcap_stat stats;
    int stats_result = pcap_stats(pcap_handle, &stats);
    (void)stats_result;
    
    // Close dump file if it was opened
    if (dumper != nullptr) {
        // Final flush before closing
        pcap_dump_flush(dumper);
        pcap_dump_close(dumper);
    }
    
    // Close pcap handle
    pcap_close(pcap_handle);
    
    // Test reading the dumped file back
    if (dumper != nullptr) {
        pcap_t* verify_handle = pcap_open_offline(output_filename, errbuf);
        if (verify_handle != nullptr) {
            // Just open and close to verify file was written correctly
            pcap_close(verify_handle);
        }
    }
    
    // Clean up temporary files
    unlink(input_filename);
    unlink(output_filename);
    
    return 0;
}

} // namespace harness_002
