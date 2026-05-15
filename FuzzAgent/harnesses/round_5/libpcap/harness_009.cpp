#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

namespace harness_009 {

// Packet handler callback for pcap_dump
static void pcap_dump_handler(u_char* dumper, const struct pcap_pkthdr* h, const u_char* bytes) {
    pcap_dump(dumper, h, bytes);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for packet headers and data
    if (size < 100) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different dump workflows
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Consume timestamp precision
    u_int tstamp_precision = fdp.ConsumeBool() ? PCAP_TSTAMP_PRECISION_NANO : PCAP_TSTAMP_PRECISION_MICRO;
    
    // Consume packet count for loops
    int packet_count = fdp.ConsumeIntegralInRange<int>(0, 10);
    
    // Consume snaplen value
    int snaplen = fdp.ConsumeIntegralInRange<int>(0, 65535);
    
    // Consume linktype
    int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);
    
    // Consume packet data for creating synthetic packets
    size_t max_packet_size = fdp.ConsumeIntegralInRange<size_t>(0, 2048);
    std::vector<uint8_t> packet_data = fdp.ConsumeBytes<uint8_t>(max_packet_size);
    
    // Create temporary filenames for dump files
    std::string dump_filename = "/tmp/libpcap_dump_fuzz_XXXXXX";
    std::string append_filename = "/tmp/libpcap_dump_append_fuzz_XXXXXX";
    
    char dump_tmp[256], append_tmp[256];
    strncpy(dump_tmp, dump_filename.c_str(), sizeof(dump_tmp) - 1);
    strncpy(append_tmp, append_filename.c_str(), sizeof(append_tmp) - 1);
    
    int dump_fd = mkstemp(dump_tmp);
    int append_fd = mkstemp(append_tmp);
    
    if (dump_fd < 0 || append_fd < 0) {
        if (dump_fd >= 0) close(dump_fd);
        if (append_fd >= 0) close(append_fd);
        return 0;
    }
    
    close(dump_fd);
    close(append_fd);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Create a dummy pcap_t handle for dump operations
    // We'll use pcap_open_dead to create a handle without actual capture device
    pcap_t* pcap = pcap_open_dead(linktype, snaplen);
    if (pcap == nullptr) {
        unlink(dump_tmp);
        unlink(append_tmp);
        return 0;
    }
    
    switch (operation) {
        case 0: {
            // Test basic pcap_dump_open and pcap_dump
            pcap_dumper_t* dumper = pcap_dump_open(pcap, dump_tmp);
            if (dumper != nullptr) {
                // Create synthetic packet headers and dump them
                for (int i = 0; i < packet_count && i * sizeof(struct pcap_pkthdr) < packet_data.size(); i++) {
                    struct pcap_pkthdr hdr;
                    hdr.ts.tv_sec = fdp.ConsumeIntegral<bpf_u_int32>();
                    hdr.ts.tv_usec = fdp.ConsumeIntegral<bpf_u_int32>();
                    hdr.caplen = fdp.ConsumeIntegralInRange<bpf_u_int32>(0, packet_data.size());
                    hdr.len = fdp.ConsumeIntegralInRange<bpf_u_int32>(hdr.caplen, packet_data.size());
                    const u_char* pkt_bytes = packet_data.data();
                    size_t bytes_available = packet_data.size();
                    if (packet_data.size() > 0) {
                        pkt_bytes = packet_data.data() + (i * sizeof(struct pcap_pkthdr)) % packet_data.size();
                        bytes_available = packet_data.size() - ((i * sizeof(struct pcap_pkthdr)) % packet_data.size());
                    }
                    size_t actual_caplen = hdr.caplen < bytes_available ? hdr.caplen : bytes_available;
                    // Validate caplen is within bounds before calling pcap_dump
                    if (actual_caplen > 0 && actual_caplen <= bytes_available) {
                        hdr.caplen = actual_caplen;
                        pcap_dump((u_char*)dumper, &hdr, pkt_bytes);
                    }
                }
                
                pcap_dump_close(dumper);
            }
            break;
        }
        
        case 1: {
            // Test pcap_dump_open with pcap_dump_flush
            pcap_dumper_t* dumper = pcap_dump_open(pcap, dump_tmp);
            if (dumper != nullptr) {
                // Dump a few packets
                for (int i = 0; i < 3 && i * sizeof(struct pcap_pkthdr) < packet_data.size(); i++) {
                    struct pcap_pkthdr hdr;
                    hdr.ts.tv_sec = fdp.ConsumeIntegral<bpf_u_int32>();
                    hdr.ts.tv_usec = fdp.ConsumeIntegral<bpf_u_int32>();
                    hdr.caplen = fdp.ConsumeIntegralInRange<bpf_u_int32>(0, 100);
                    const u_char* pkt_bytes = packet_data.data();
                    size_t bytes_available = packet_data.size();
                    if (packet_data.size() > 0) {
                        pkt_bytes = packet_data.data() + (i * 10) % packet_data.size();
                        bytes_available = packet_data.size() - ((i * 10) % packet_data.size());
                    }
                    size_t actual_caplen = hdr.caplen < bytes_available ? hdr.caplen : bytes_available;
                    
                    // Validate caplen is within bounds before calling pcap_dump
                    if (actual_caplen <= bytes_available) {
                        hdr.caplen = actual_caplen;
                        pcap_dump((u_char*)dumper, &hdr, pkt_bytes);
                    }
                }
                
                // Test flush operation
                pcap_dump_flush(dumper);
                
                // Test file position functions
                FILE* dump_file = pcap_dump_file(dumper);
                if (dump_file != nullptr) {
                    long pos = pcap_dump_ftell(dumper);
                    (void)pos; // Use to avoid warning
                    
                    #ifdef HAVE_FSEEKO
                    int64_t pos64 = pcap_dump_ftell64(dumper);
                    (void)pos64;
                    #endif
                }
                
                pcap_dump_close(dumper);
            }
            break;
        }
        
        case 2: {
            // Test pcap_dump_open_append
            // First create a file with some data
            pcap_dumper_t* initial_dumper = pcap_dump_open(pcap, append_tmp);
            if (initial_dumper != nullptr) {
                struct pcap_pkthdr hdr;
                hdr.ts.tv_sec = 1000;
                hdr.ts.tv_usec = 500000;
                hdr.caplen = packet_data.size() > 50 ? 50 : packet_data.size();
                hdr.len = hdr.caplen + 10;
                
                // Validate caplen is within bounds before calling pcap_dump
                if (hdr.caplen <= packet_data.size()) {
                    pcap_dump((u_char*)initial_dumper, &hdr, packet_data.data());
                }
            }
            
            // Now append to it
            pcap_dumper_t* append_dumper = pcap_dump_open_append(pcap, append_tmp);
            if (append_dumper != nullptr) {
                struct pcap_pkthdr hdr;
                hdr.ts.tv_sec = 2000;
                hdr.ts.tv_usec = 750000;
                hdr.caplen = packet_data.size() > 30 ? 30 : packet_data.size();
                hdr.len = hdr.caplen + 5;
                const u_char* pkt_bytes = packet_data.data();
                size_t bytes_available = packet_data.size();
                if (packet_data.size() > 0) {
                    pkt_bytes = packet_data.data() + 10 % packet_data.size();
                    bytes_available = packet_data.size() - (10 % packet_data.size());
                }
                
                // Validate caplen is within bounds before calling pcap_dump
                // Validate caplen is within bounds before calling pcap_dump
                if (hdr.caplen <= bytes_available) {
                    pcap_dump((u_char*)append_dumper, &hdr, pkt_bytes);
                }
                pcap_dump_close(append_dumper);
            }
            break;
        }
        
        case 3: {
            // Test pcap_dump_fopen (file handle version)
            FILE* output_file = fopen(dump_tmp, "wb");
            if (output_file != nullptr) {
                pcap_dumper_t* dumper = pcap_dump_fopen(pcap, output_file);
                if (dumper != nullptr) {
                    struct pcap_pkthdr hdr;
                    hdr.ts.tv_sec = fdp.ConsumeIntegral<bpf_u_int32>();
                    hdr.ts.tv_usec = fdp.ConsumeIntegral<bpf_u_int32>();
                    hdr.caplen = fdp.ConsumeIntegralInRange<bpf_u_int32>(0, packet_data.size());
                    hdr.len = fdp.ConsumeIntegralInRange<bpf_u_int32>(hdr.caplen, packet_data.size() + 20);
                    
                    // Validate caplen is within bounds before calling pcap_dump
                    if (hdr.caplen <= packet_data.size()) {
                        pcap_dump((u_char*)dumper, &hdr, packet_data.data());
                    }
                    // Note: pcap_dump_close will close the FILE* for us
                } else {
                    fclose(output_file);
                }
            }
            break;
        }
        
        case 4: {
            // Test error handling scenarios
            // Try to dump with NULL dumper (should be handled gracefully)
            pcap_dumper_t* dumper = pcap_dump_open(pcap, dump_tmp);
            if (dumper != nullptr) {
                // Test with empty/null packet data
                struct pcap_pkthdr hdr;
                hdr.ts.tv_sec = 0;
                hdr.ts.tv_usec = 0;
                hdr.caplen = 0;
                hdr.len = 0;
                
                pcap_dump((u_char*)dumper, &hdr, nullptr);
                
                // Test with zero-length packet
                pcap_dump((u_char*)dumper, &hdr, packet_data.data());
                
                // Test flush on empty file
                pcap_dump_flush(dumper);
                
                pcap_dump_close(dumper);
            }
            break;
        }
        
        case 5: {
            // Test multiple sequential dump operations
            pcap_dumper_t* dumper = pcap_dump_open(pcap, dump_tmp);
            if (dumper != nullptr) {
                // Create multiple packets with varying sizes
                for (int i = 0; i < 5; i++) {
                    struct pcap_pkthdr hdr;
                    hdr.ts.tv_sec = 1000 + i * 100;
                    hdr.ts.tv_usec = i * 100000;
                    hdr.caplen = (i * 20) % (packet_data.size() + 1);
                    hdr.len = hdr.caplen + (i * 5);
                    size_t offset = 0;
                    const u_char* pkt_bytes = packet_data.data();
                    size_t bytes_available = packet_data.size();
                    if (packet_data.size() > 0) {
                        offset = (i * 15) % packet_data.size();
                        pkt_bytes = packet_data.data() + offset;
                        bytes_available = packet_data.size() - offset;
                    }
                    size_t actual_caplen = hdr.caplen < bytes_available ? hdr.caplen : bytes_available;
                    
                    // Validate caplen is within bounds before calling pcap_dump
                    if (actual_caplen <= bytes_available) {
                        hdr.caplen = actual_caplen;
                        pcap_dump((u_char*)dumper, &hdr, pkt_bytes);
                    }
                    
                    // Flush every other packet
                    if (i % 2 == 0) {
                        pcap_dump_flush(dumper);
                    }
                }
                
                pcap_dump_close(dumper);
            }
            break;
        }
        
        case 6: {
            // Test boundary conditions
            pcap_dumper_t* dumper = pcap_dump_open(pcap, dump_tmp);
            if (dumper != nullptr) {
                // Test with maximum size packet header
                struct pcap_pkthdr hdr;
                hdr.ts.tv_sec = UINT32_MAX;
                hdr.ts.tv_usec = UINT32_MAX;
                hdr.caplen = UINT32_MAX;
                hdr.len = UINT32_MAX;
                
                // Validate caplen is within bounds before calling pcap_dump
                if (hdr.caplen <= packet_data.size()) {
                    pcap_dump((u_char*)dumper, &hdr, packet_data.data());
                }
                // Test with minimal valid packet
                hdr.ts.tv_sec = 1;
                hdr.ts.tv_usec = 1;
                hdr.caplen = 1;
                hdr.len = 1;
                
                if (packet_data.size() >= 1) {
                    pcap_dump((u_char*)dumper, &hdr, packet_data.data());
                }
                
                pcap_dump_close(dumper);
            }
            break;
        }
        
        case 7: {
            // Test pcap_dump with pcap_loop simulation
            pcap_dumper_t* dumper = pcap_dump_open(pcap, dump_tmp);
            if (dumper != nullptr) {
                // Simulate what pcap_loop would do with our handler
                for (int i = 0; i < packet_count; i++) {
                    struct pcap_pkthdr hdr;
                    hdr.ts.tv_sec = fdp.ConsumeIntegral<bpf_u_int32>();
                    hdr.ts.tv_usec = fdp.ConsumeIntegral<bpf_u_int32>();
                    hdr.caplen = fdp.ConsumeIntegralInRange<bpf_u_int32>(0, packet_data.size());
                    hdr.len = fdp.ConsumeIntegralInRange<bpf_u_int32>(hdr.caplen, packet_data.size() + 50);
                    
                    // Call the handler directly
                    pcap_dump_handler((u_char*)dumper, &hdr, packet_data.data());
                }
                
                pcap_dump_close(dumper);
            }
            break;
        }
    }
    
    // Cleanup
    pcap_close(pcap);
    unlink(dump_tmp);
    unlink(append_tmp);
    
    return 0;
}

} // namespace harness_009
