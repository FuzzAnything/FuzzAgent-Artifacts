#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

namespace harness_009 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing of offline pcap functions
    // Need enough data for pcap file content and control parameters
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size control parameters first
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 4);  // Which test scenario to run
    int tstamp_precision = fdp.ConsumeIntegralInRange<int>(0, 1);  // 0=microseconds, 1=nanoseconds
    int loop_count = fdp.ConsumeIntegralInRange<int>(0, 10);  // For pcap_loop
    int dispatch_packet_count = fdp.ConsumeIntegralInRange<int>(-1, 10);  // For pcap_dispatch (-1 means all)
    int read_method = fdp.ConsumeIntegralInRange<int>(0, 2);  // 0=pcap_next, 1=pcap_dispatch, 2=pcap_loop
    
    // Consume filter string (if any)
    size_t filter_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string filter_str;
    if (filter_size > 0) {
        filter_str = fdp.ConsumeBytesAsString(filter_size);
    }
    
    // Get remaining bytes as potential pcap file content
    std::vector<uint8_t> pcap_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (pcap_data.size() < 24) {  // Minimum pcap header size
        return 0;
    }
    
    // Create temporary file for testing
    char temp_filename[] = "/tmp/libpcap_offline_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd < 0) {
        return 0;
    }
    
    // Write pcap data to temporary file
    if (write(fd, pcap_data.data(), pcap_data.size()) != static_cast<ssize_t>(pcap_data.size())) {
        close(fd);
        unlink(temp_filename);
        return 0;
    }
    close(fd);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap = nullptr;
    int* tstamp_types = nullptr;
    
    // Test different offline opening scenarios
    switch (test_scenario) {
        case 0: {
            // Scenario 0: pcap_open_offline (basic file path opening)
            pcap = pcap_open_offline(temp_filename, errbuf);
            break;
        }
            
        case 1: {
            // Scenario 1: pcap_open_offline_with_tstamp_precision
            pcap = pcap_open_offline_with_tstamp_precision(temp_filename, tstamp_precision, errbuf);
            break;
        }
            
        case 2: {
            // Scenario 2: pcap_fopen_offline (FILE* based opening)
            FILE* f = fopen(temp_filename, "rb");
            if (f == nullptr) {
                unlink(temp_filename);
                return 0;
            }
            pcap = pcap_fopen_offline(f, errbuf);
            // Note: pcap_fopen_offline takes ownership of the FILE*
            break;
        }
            
        case 3: {
            // Scenario 3: pcap_fopen_offline_with_tstamp_precision
            FILE* f = fopen(temp_filename, "rb");
            if (f == nullptr) {
                unlink(temp_filename);
                return 0;
            }
            pcap = pcap_fopen_offline_with_tstamp_precision(f, tstamp_precision, errbuf);
            // Note: takes ownership of the FILE*
            break;
        }
            
        case 4: {
            // Scenario 4: Create a dead pcap handle for testing pcap_offline_filter
            int linktype = DLT_EN10MB;  // Ethernet
            int snaplen = 65535;
            pcap = pcap_open_dead(linktype, snaplen);
            if (pcap == nullptr) {
                unlink(temp_filename);
                return 0;
            }
            
            // Also open the file to get packet data for filtering
            pcap_t* file_pcap = pcap_open_offline(temp_filename, errbuf);
            if (file_pcap != nullptr) {
                // Test pcap_offline_filter with a simple filter
                struct bpf_program fp;
                if (pcap_compile(pcap, &fp, "ip", 1, PCAP_NETMASK_UNKNOWN) == 0) {
                    // Read packets and test filtering
                    struct pcap_pkthdr header;
                    const u_char* packet;
                    
                    while ((packet = pcap_next(file_pcap, &header)) != nullptr) {
                        // Test pcap_offline_filter on each packet
                        int filter_result = pcap_offline_filter(&fp, &header, packet);
                        (void)filter_result;
                        
                        // Also test pcap_is_swapped
                        int swapped = pcap_is_swapped(file_pcap);
                        (void)swapped;
                    }
                    pcap_freecode(&fp);
                }
                pcap_close(file_pcap);
            }
            // Cleanup will happen at the end
            break;
        }
    }
    
    // If pcap handle was successfully opened (or created as dead handle)
    if (pcap != nullptr) {
        // Test timestamp type listing and management
        int tstamp_count = pcap_list_tstamp_types(pcap, &tstamp_types);
        if (tstamp_count > 0 && tstamp_types != nullptr) {
            // Test pcap_free_tstamp_types
            pcap_free_tstamp_types(tstamp_types);
            tstamp_types = nullptr;
            
            // List again to verify cleanup was handled
            tstamp_count = pcap_list_tstamp_types(pcap, &tstamp_types);
        }
        
        // Test pcap_is_swapped (for endianness detection)
        int swapped = pcap_is_swapped(pcap);
        (void)swapped;
        
        // Set up filter if we have one
        struct bpf_program fp;
        int filter_compiled = 0;
        if (!filter_str.empty()) {
            if (pcap_compile(pcap, &fp, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                filter_compiled = 1;
                if (pcap_setfilter(pcap, &fp) == 0) {
                    // Filter successfully set
                }
            }
        }
        
        // Test different packet reading methods
        switch (read_method) {
            case 0: {
                // Method 0: pcap_next - read packets one by one
                struct pcap_pkthdr header;
                const u_char* packet;
                int packets_read = 0;
                
                while ((packet = pcap_next(pcap, &header)) != nullptr && packets_read < 20) {
                    packets_read++;
                    
                    // Test pcap_offline_filter if we have a compiled filter
                    if (filter_compiled) {
                        int filter_result = pcap_offline_filter(&fp, &header, packet);
                        (void)filter_result;
                    }
                }
                break;
            }
                
            case 1: {
                // Method 1: pcap_dispatch - read with callback
                auto packet_handler = [](u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
                    // Simple packet handler that does nothing
                    (void)user;
                    (void)h;
                    (void)bytes;
                };
                
                pcap_dispatch(pcap, dispatch_packet_count, packet_handler, nullptr);
                break;
            }
                
            case 2: {
                // Method 2: pcap_loop - similar to dispatch but continues until EOF or error
                auto packet_handler = [](u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
                    // Simple packet handler
                    (void)user;
                    (void)h;
                    (void)bytes;
                };
                
                pcap_loop(pcap, loop_count, packet_handler, nullptr);
                break;
            }
        }
        
        // Clean up filter if compiled
        if (filter_compiled) {
            pcap_freecode(&fp);
        }
        
        // Clean up timestamp types if allocated
        if (tstamp_types != nullptr) {
            pcap_free_tstamp_types(tstamp_types);
        }
        
        // Test error paths with invalid inputs (on already opened handle)
        // These should fail gracefully
        
        // Try to compile invalid filter
        struct bpf_program invalid_fp;
        if (pcap_compile(pcap, &invalid_fp, "invalid filter syntax", 1, PCAP_NETMASK_UNKNOWN) == 0) {
            pcap_freecode(&invalid_fp);
        }
        
        // Try to set invalid datalink (should fail on savefile)
        pcap_set_datalink(pcap, 99999);
        
        // Close the pcap handle
        pcap_close(pcap);
    }
    
    // Test additional error cases with invalid file paths
    // This tests error handling in the offline opening functions
    
    if (fdp.remaining_bytes() == 0) {  // Only if we have no more data to avoid infinite loops
        // Try to open non-existent file
        pcap_t* invalid_pcap = pcap_open_offline("/nonexistent/file.pcap", errbuf);
        if (invalid_pcap != nullptr) {
            pcap_close(invalid_pcap);
        }
        
        // Try to open with invalid precision
        invalid_pcap = pcap_open_offline_with_tstamp_precision(temp_filename, 999, errbuf);
        if (invalid_pcap != nullptr) {
            pcap_close(invalid_pcap);
        }
    }
    
    // Clean up temporary file
    unlink(temp_filename);
    
    return 0;
}

} // namespace harness_009
