#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <climits>
#include <net/if.h>  // For IFNAMSIZ
#include <pcap/pcap.h>
#include <pcap/bpf.h>

namespace harness_023 {


// Forward declarations for Linux-specific internal functions
// We'll test Linux-specific memory-mapped packet capture infrastructure
extern "C" {
    // Memory-mapped ring buffer management functions (Linux-specific)
    // These are internal to pcap-linux.c
    int create_ring(pcap_t *handle);
    void destroy_ring(pcap_t *handle);
    int prepare_tpacket_socket(pcap_t *handle);
    
    // Linux-specific mmap read operations
    // Note: pcap_read_linux_mmap_v2 and pcap_read_linux_mmap_v3 are called internally
    // We'll trigger them through pcap_dispatch or pcap_loop
    
    // Other Linux-specific infrastructure
    int setup_mmapped(pcap_t *handle);
    int pcap_wait_for_frames_mmap(pcap_t *handle);
    int pcap_handle_packet_mmap(pcap_t *handle, pcap_handler callback, u_char *user, 
                               struct tpacket2_hdr *hdr, u_char *bp);
    
    // Linux-specific helper functions
    int fix_program(pcap_t *handle, struct sock_fprog *fcode);
    int fix_offset(pcap_t *handle, struct bpf_insn *p);
    int set_kernel_filter(pcap_t *handle, struct sock_fprog *fcode);
    int reset_kernel_filter(pcap_t *handle);
    
    // Protocol setting function
    int pcap_set_protocol_linux(pcap_t *p, int protocol);
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Need sufficient input for comprehensive Linux memory-mapped API testing
    // Minimum: scenario + flags + buffer params + packet data + various options
    if (Size < 512) {
        return 0;  // Need more input for meaningful testing
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios targeting Linux-specific mmap infrastructure
    // First byte: test scenario selector (0-7)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Second byte: memory-mapped ring version (2 for TPACKET_V2, 3 for TPACKET_V3)
    uint8_t mmap_version = fdp.ConsumeIntegral<uint8_t>() % 2 + 2; // 2 or 3
    
    // Third byte: ring buffer configuration flags
    uint8_t ring_flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Fourth and fifth bytes: ring block size
    uint32_t block_size = fdp.ConsumeIntegralInRange<uint32_t>(4096, 65536);
    
    // Sixth and seventh bytes: ring block count
    uint32_t block_count = fdp.ConsumeIntegralInRange<uint32_t>(1, 128);
    
    // Eighth byte: frame size
    uint32_t frame_size = fdp.ConsumeIntegralInRange<uint32_t>(2048, 16384);
    
    // Ninth byte: snapshot length
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    
    // Tenth byte: timeout in milliseconds
    int timeout = fdp.ConsumeIntegralInRange<int>(0, 10000);
    
    // Eleventh byte: buffer size
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1024, 1048576);
    
    // Twelfth byte: immediate mode flag
    int immediate_mode = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Thirteenth byte: promiscuous mode flag
    int promisc = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Fourteenth byte: non-blocking mode flag
    int nonblock = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Fifteenth byte: protocol for Linux-specific setting
    int linux_protocol = fdp.ConsumeIntegral<int>();
    
    // Sixteenth byte: direction (inbound/outbound/both)
    pcap_direction_t direction = static_cast<pcap_direction_t>(fdp.ConsumeIntegral<uint8_t>() % 3);
    
    // Consume device name - focus on Linux-specific devices
    std::string device_name;
    uint8_t device_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    switch (device_type) {
        case 0:
            device_name = "any";  // Linux special device for all interfaces
            break;
        case 1:
            device_name = "lo";   // Loopback interface
            break;
        case 2:
            device_name = "eth0"; // Common Ethernet interface name
            break;
        case 3:
            device_name = "wlan0"; // Common WiFi interface name
            break;
        case 4:
            device_name = "docker0"; // Docker bridge interface
            break;
        case 5:
            device_name = "veth0"; // Virtual Ethernet interface
            break;
        case 6:
            device_name = fdp.ConsumeRandomLengthString(32); // Random device name
            break;
        case 7:
            device_name = fdp.ConsumeRandomLengthString(IFNAMSIZ); // Max length name
            break;
    }
    
    // Consume packet data to simulate captured packets
    size_t packet_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 2048);
    std::vector<uint8_t> packet_data;
    if (fdp.remaining_bytes() >= packet_data_size) {
        packet_data = fdp.ConsumeBytes<uint8_t>(packet_data_size);
    } else {
        packet_data = fdp.ConsumeRemainingBytes<uint8_t>();
    }
    
    // Consume filter string (simpler for memory-mapped testing)
    std::string filter_str;
    uint8_t filter_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (filter_type) {
        case 0:
            filter_str = "ip";
            break;
        case 1:
            filter_str = "tcp port " + std::to_string(fdp.ConsumeIntegralInRange<uint16_t>(1, 65535));
            break;
        case 2:
            filter_str = "udp";
            break;
        case 3:
            filter_str = fdp.ConsumeRandomLengthString(128);
            break;
    }
    
    // Consume additional test parameters
    uint8_t additional_params[16];
    if (fdp.remaining_bytes() >= sizeof(additional_params)) {
        auto bytes = fdp.ConsumeBytes<uint8_t>(sizeof(additional_params));
        memcpy(additional_params, bytes.data(), sizeof(additional_params));
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = nullptr;
    int result = 0;
    struct bpf_program bpf;
    memset(&bpf, 0, sizeof(bpf));
    bpf_u_int32 netmask = 0xFFFFFF00; // Common netmask for testing
    
    // Test different scenarios targeting Linux-specific memory-mapped infrastructure
    switch (scenario) {
        case 0: {
            // Scenario 0: Basic memory-mapped capture setup
            // Test pcap_create with Linux-specific options to trigger mmap path
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                // Set Linux-specific protocol before activation
                result = pcap_set_protocol_linux(pcap_handle, linux_protocol);
                
                // Configure for memory-mapped capture
                pcap_set_snaplen(pcap_handle, snaplen);
                pcap_set_timeout(pcap_handle, timeout);
                pcap_set_buffer_size(pcap_handle, buffer_size);
                
                // Activate to trigger Linux-specific setup
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Set immediate mode and promisc after activation if supported
                    pcap_set_immediate_mode(pcap_handle, immediate_mode);
                    pcap_set_promisc(pcap_handle, promisc);
                    
                    // Try to set filter (may trigger kernel vs userland filtering)
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, netmask) == 0) {
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    }
                    
                    // Set direction (Linux-specific implementation)
                    pcap_setdirection(pcap_handle, direction);
                    
                    // Try to read a packet (may trigger mmap read path)
                    // Using pcap_dispatch with zero count to just check readiness
                    pcap_dispatch(pcap_handle, 0, nullptr, nullptr);
                    
                    // Get statistics (Linux-specific implementation)
                    struct pcap_stat stats;
                    pcap_stats(pcap_handle, &stats);
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Test error recovery and cleanup paths
            // Create multiple handles to test resource management
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                // Set extreme values to test boundary conditions
                pcap_set_snaplen(pcap_handle, 0);  // Zero snaplen
                pcap_set_timeout(pcap_handle, -1); // Negative timeout
                pcap_set_buffer_size(pcap_handle, 0); // Zero buffer
                
                // Try activation with bad parameters
                result = pcap_activate(pcap_handle);
                
                // Close handle (tests cleanup with partially initialized state)
                pcap_close(pcap_handle);
                
                // Create another handle with normal parameters
                pcap_handle = pcap_create(device_name.c_str(), errbuf);
                if (pcap_handle != nullptr) {
                    pcap_set_snaplen(pcap_handle, snaplen);
                    result = pcap_activate(pcap_handle);
                    
                    if (result >= 0) {
                        // Test with invalid filter
                        std::string invalid_filter = fdp.ConsumeRandomLengthString(256);
                        if (pcap_compile(pcap_handle, &bpf, invalid_filter.c_str(), 0, netmask) == 0) {
                            pcap_setfilter(pcap_handle, &bpf);
                            pcap_freecode(&bpf);
                        }
                    }
                    
                    pcap_close(pcap_handle);
                }
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Test pcap_open_live which may use mmap internally
            pcap_handle = pcap_open_live(device_name.c_str(), snaplen, promisc, timeout, errbuf);
            
            if (pcap_handle != nullptr) {
                // Test Linux-specific operations
                pcap_setdirection(pcap_handle, direction);
                
                if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, netmask) == 0) {
                    pcap_setfilter(pcap_handle, &bpf);
                    pcap_freecode(&bpf);
                }
                
                // Non-blocking mode (Linux-specific implementation)
                pcap_setnonblock(pcap_handle, nonblock, errbuf);
                int nb_result = pcap_getnonblock(pcap_handle, errbuf);
                
                // Try to inject a packet (Linux-specific implementation)
                if (!packet_data.empty()) {
                    pcap_inject(pcap_handle, packet_data.data(), packet_data.size());
                }
                
                // Get statistics
                struct pcap_stat stats;
                pcap_stats(pcap_handle, &stats);
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Test pcap_open_dead for filter compilation without device
            // This tests BPF compilation infrastructure shared with Linux backend
            pcap_handle = pcap_open_dead(DLT_EN10MB, snaplen);
            
            if (pcap_handle != nullptr) {
                // Try multiple filter compilations
                for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                    std::string test_filter;
                    if (i == 0) {
                        test_filter = filter_str;
                    } else {
                        test_filter = fdp.ConsumeRandomLengthString(64);
                    }
                    
                    if (pcap_compile(pcap_handle, &bpf, test_filter.c_str(), 1, netmask) == 0) {
                        pcap_freecode(&bpf);
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Test datalink type changes (Linux-specific)
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                pcap_set_snaplen(pcap_handle, snaplen);
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Get current datalink
                    int current_dlt = pcap_datalink(pcap_handle);
                    
                    // Try to set different datalink types
                    // Note: pcap_set_datalink may fail for incompatible types
                    int *dlt_list = nullptr;
                    int dlt_count = pcap_list_datalinks(pcap_handle, &dlt_list);
                    
                    if (dlt_count > 0 && dlt_list != nullptr) {
                        // Try first available datalink
                        pcap_set_datalink(pcap_handle, dlt_list[0]);
                        pcap_free_datalinks(dlt_list);
                    }
                    // Set back to original if changed
                    pcap_set_datalink(pcap_handle, current_dlt);
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Test with various buffer configurations
            // This may trigger different memory-mapped ring setups
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                // Try different buffer sizes to test ring allocation
                int buffer_sizes[] = {1024, 4096, 16384, 65536, 262144, 1048576};
                for (size_t i = 0; i < sizeof(buffer_sizes)/sizeof(buffer_sizes[0]); i++) {
                    pcap_set_buffer_size(pcap_handle, buffer_sizes[i]);
                }
                
                // Set final buffer size
                pcap_set_buffer_size(pcap_handle, buffer_size);
                pcap_set_snaplen(pcap_handle, snaplen);
                pcap_set_timeout(pcap_handle, timeout);
                
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Test filter
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, netmask) == 0) {
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    }
                    
                    // Change buffer size after activation (may not work)
                    pcap_set_buffer_size(pcap_handle, buffer_size * 2);
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 6: {
            // Scenario 6: Test pcap_loop with callback (exercises mmap read path)
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                pcap_set_snaplen(pcap_handle, snaplen);
                pcap_set_timeout(pcap_handle, timeout);
                
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Set non-blocking for this test
                    pcap_setnonblock(pcap_handle, 1, errbuf);
                    
                    // Simple packet handler callback
                    auto packet_handler = [](u_char *user, const struct pcap_pkthdr *hdr, const u_char *data) {
                        // Do nothing, just count
                        int *count = (int *)user;
                        (*count)++;
                    };
                    
                    int packet_count = 0;
                    
                    // Call pcap_loop with small count (may use mmap read internally)
                    pcap_loop(pcap_handle, 1, packet_handler, (u_char *)&packet_count);
                    
                    // Also try pcap_dispatch
                    pcap_dispatch(pcap_handle, 0, packet_handler, (u_char *)&packet_count);
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 7: {
            // Scenario 7: Comprehensive test mixing multiple operations
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                // Configure all options
                pcap_set_snaplen(pcap_handle, snaplen);
                pcap_set_timeout(pcap_handle, timeout);
                pcap_set_buffer_size(pcap_handle, buffer_size);
                
                // Set Linux-specific protocol
                result = pcap_set_protocol_linux(pcap_handle, linux_protocol);
                
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Set modes after activation
                    pcap_set_immediate_mode(pcap_handle, immediate_mode);
                    pcap_set_promisc(pcap_handle, promisc);
                    
                    // Test all major operations
                    
                    // 1. Filter operations
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, netmask) == 0) {
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    }
                    
                    // 2. Direction
                    pcap_setdirection(pcap_handle, direction);
                    
                    // 3. Non-blocking mode
                    pcap_setnonblock(pcap_handle, nonblock, errbuf);
                    pcap_getnonblock(pcap_handle, errbuf);
                    
                    // 4. Statistics
                    struct pcap_stat stats;
                    pcap_stats(pcap_handle, &stats);
                    
                    // 5. Datalink operations
                    int current_dlt = pcap_datalink(pcap_handle);
                    int *dlt_list2 = nullptr;
                    int dlt_count = pcap_list_datalinks(pcap_handle, &dlt_list2);
                    
                    if (dlt_count > 0 && dlt_list2 != nullptr) {
                        // Try to change datalink if possible
                        pcap_set_datalink(pcap_handle, dlt_list2[0]);
                        pcap_free_datalinks(dlt_list2);
                    }
                    
                    // 6. Try to read (may trigger mmap path)
                    struct pcap_pkthdr *pkthdr;
                    const u_char *pktdata;
                    int read_result = pcap_next_ex(pcap_handle, &pkthdr, &pktdata);
                    // 7. Inject test packet
                    if (!packet_data.empty()) {
                        pcap_inject(pcap_handle, packet_data.data(), packet_data.size());
                    }
                    
                    // 8. Break loop (tests Linux-specific breakloop implementation)
                    pcap_breakloop(pcap_handle);
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_023
