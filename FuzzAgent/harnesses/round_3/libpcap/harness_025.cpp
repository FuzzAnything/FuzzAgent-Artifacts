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

namespace harness_025 {
/*
 * Harness for pcap-linux.c module targeting memory-mapped read functions
 * Primary Target: pcap_read_linux_mmap_v2 (0% coverage), pcap_read_linux_mmap_v3 (0% coverage)
 * Secondary Targets: pcap_activate_linux, packet_mmap_acquire/release, pcap_handle_packet_mmap
 * Coverage Goal: Target 516+ undiscovered branches in Linux memory-mapped packet capture
 * Strategy: Comprehensive testing of mmap ring buffer operations with focus on TPACKET_V2/V3
 *           differences, VLAN handling, timestamp precision, and edge cases
 * Semantic Diversity: More focused on actual packet read paths than harness_021, with
 *                     deeper testing of mmap ring buffer state transitions
 */


// Custom packet handler callback to test mmap packet processing
static void test_mmap_packet_handler(u_char* user, const struct pcap_pkthdr* hdr, const u_char* data) {
    // Track packet count and verify basic packet structure
    int* packet_count = reinterpret_cast<int*>(user);
    (*packet_count)++;
    
    // Test packet header fields
    if (hdr != nullptr) {
        // Verify header fields are reasonable
        (void)hdr->caplen;
        (void)hdr->len;
        (void)hdr->ts.tv_sec;
        (void)hdr->ts.tv_usec;
    }
    
    // Test packet data access (limited to avoid excessive processing)
    if (data != nullptr && hdr != nullptr && hdr->caplen > 0) {
        // Access first few bytes
        (void)data[0];
        if (hdr->caplen > 1) (void)data[1];
        if (hdr->caplen > 2) (void)data[2];
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for comprehensive mmap testing
    // Need device names, control parameters, and simulated packet data
    if (size < 200) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Phase 1: Consume control parameters for mmap configuration
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 5);
    int use_mmap_mode = 1;  // Always try mmap mode for this harness
    int promisc_mode = fdp.ConsumeBool() ? 1 : 0;
    int immediate_mode = fdp.ConsumeBool() ? 1 : 0;
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 5000);
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1024, 1048576);
    
    // TPACKET version influence attempt
    int try_influence_tpacket = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Device selection with focus on Linux interfaces that might support mmap
    std::string device_name;
    int device_choice = fdp.ConsumeIntegralInRange<int>(0, 4);
    switch (device_choice) {
        case 0: device_name = "any"; break;      // Linux "any" device
        case 1: device_name = "lo"; break;       // Loopback
        case 2: device_name = "eth0"; break;     // Ethernet
        case 3: device_name = "wlan0"; break;    // Wireless
        case 4: device_name = "docker0"; break;  // Docker bridge
    }
    
    // Simulated packet data for callback testing
    size_t packet_data_size = fdp.ConsumeIntegralInRange<size_t>(64, 4096);
    std::vector<uint8_t> simulated_packet = fdp.ConsumeBytes<uint8_t>(packet_data_size);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* pcap_handle = nullptr;
    int packet_counter = 0;
    
    // Phase 2: Create and configure pcap handle for mmap testing
    pcap_handle = pcap_create(device_name.c_str(), errbuf);
    if (pcap_handle == nullptr) {
        // Fallback to pcap_open_live if create fails
        pcap_handle = pcap_open_live(device_name.c_str(), snaplen, promisc_mode, timeout_ms, errbuf);
        if (pcap_handle == nullptr) {
            // Both methods failed, test error handling functions
            (void)pcap_strerror(PCAP_ERROR);
            (void)pcap_geterr(nullptr);
            return 0;
        }
    } else {
        // Configure handle before activation
        pcap_set_snaplen(pcap_handle, snaplen);
        pcap_set_promisc(pcap_handle, promisc_mode);
        pcap_set_timeout(pcap_handle, timeout_ms);
        pcap_set_buffer_size(pcap_handle, buffer_size);
        pcap_set_immediate_mode(pcap_handle, immediate_mode);
        
        // Attempt to influence TPACKET version through various configurations
        switch (try_influence_tpacket) {
            case 0:
                // Default configuration
                break;
            case 1:
                // Larger buffer might affect mmap configuration
                pcap_set_buffer_size(pcap_handle, buffer_size * 2);
                break;
            case 2:
                // Different timeout might affect ring buffer setup
                pcap_set_timeout(pcap_handle, timeout_ms / 2);
                break;
            case 3:
                // Very small buffer for edge case
                pcap_set_buffer_size(pcap_handle, 512);
                break;
        }
        
        // Activate handle - this triggers pcap_activate_linux and mmap setup
        int activate_result = pcap_activate(pcap_handle);
        if (activate_result < 0 && activate_result != PCAP_WARNING) {
            // Activation failed, test cleanup and error paths
            pcap_close(pcap_handle);
            
            // Test with dead handle for filter compilation paths
            pcap_t* dead_handle = pcap_open_dead(DLT_EN10MB, snaplen);
            if (dead_handle != nullptr) {
                struct bpf_program bpf;
                std::string filter_str = fdp.ConsumeRandomLengthString(128);
                if (pcap_compile(dead_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                    pcap_freecode(&bpf);
                }
                pcap_close(dead_handle);
            }
            return 0;
        }
    }
    
    // Phase 3: Execute different mmap test scenarios
    switch (test_scenario) {
        case 0: {
            // Scenario 0: Basic mmap read with pcap_next_ex
            // This may call pcap_read_linux_mmap_v2 or v3 depending on tp_version
            struct pcap_pkthdr* header = nullptr;
            const u_char* packet_data = nullptr;
            
            // Try to read a packet (may return immediately if no packets available)
            int read_result = pcap_next_ex(pcap_handle, &header, &packet_data);
            (void)read_result;  // PCAP_ERROR_BREAK, 0, 1, or other values
            
            // Test with pcap_dispatch which uses the read_op callback mechanism
            int max_packets = fdp.ConsumeIntegralInRange<int>(1, 10);
            int dispatch_result = pcap_dispatch(pcap_handle, max_packets, 
                test_mmap_packet_handler, reinterpret_cast<u_char*>(&packet_counter));
            (void)dispatch_result;
            break;
        }
            
        case 1: {
            // Scenario 1: Extended mmap read testing with different max_packet values
            // Test edge cases for max_packets parameter
            int max_packets_values[] = {0, 1, 5, 10, -1};  // -1 for unlimited
            size_t max_idx = fdp.ConsumeIntegralInRange<size_t>(0, 4);
            int max_packets = max_packets_values[max_idx];
            
            int dispatch_result = pcap_dispatch(pcap_handle, max_packets,
                [](u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
                    (void)user; (void)h; (void)bytes;
                    return;
                }, nullptr);
            (void)dispatch_result;
            
            // Test pcap_loop with limited count
            int loop_result = pcap_loop(pcap_handle, 3, test_mmap_packet_handler,
                                       reinterpret_cast<u_char*>(&packet_counter));
            (void)loop_result;
            break;
        }
            
        case 2: {
            // Scenario 2: Break loop testing with mmap reads
            // Set break loop before attempting reads
            pcap_breakloop(pcap_handle);
            
            // Try to dispatch packets - should return PCAP_ERROR_BREAK
            int dispatch_result = pcap_dispatch(pcap_handle, 10,
                test_mmap_packet_handler, reinterpret_cast<u_char*>(&packet_counter));
            (void)dispatch_result;
            
            // Clear break loop by setting handle->break_loop = 0 indirectly
            // There's no public API, but we can create a new handle
            pcap_close(pcap_handle);
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            if (pcap_handle != nullptr) {
                pcap_set_snaplen(pcap_handle, snaplen);
                pcap_activate(pcap_handle);
            }
            break;
        }
            
        case 3: {
            // Scenario 3: Non-blocking mode with mmap
            int nonblock_result = pcap_setnonblock(pcap_handle, 1, errbuf);
            (void)nonblock_result;
            
            if (nonblock_result == 0) {
                // Try immediate reads in non-blocking mode
                struct pcap_pkthdr* header = nullptr;
                const u_char* packet_data = nullptr;
                
                for (int i = 0; i < 5; i++) {
                    int read_result = pcap_next_ex(pcap_handle, &header, &packet_data);
                    (void)read_result;
                }
                
                // Test getnonblock
                char nonblock_errbuf[PCAP_ERRBUF_SIZE];
                int current_nonblock = pcap_getnonblock(pcap_handle, nonblock_errbuf);
                (void)current_nonblock;
                
                // Switch back to blocking
                pcap_setnonblock(pcap_handle, 0, errbuf);
            }
            
            // Test statistics with mmap mode
            struct pcap_stat stats;
            int stats_result = pcap_stats(pcap_handle, &stats);
            (void)stats_result;
            break;
        }
            
        case 4: {
            // Scenario 4: Filter testing with mmap mode
            // Test different filter complexities that might affect kernel vs userland filtering
            struct bpf_program bpf;
            std::string filter_str;
            
            int filter_type = fdp.ConsumeIntegralInRange<int>(0, 3);
            switch (filter_type) {
                case 0:
                    filter_str = "tcp or udp";
                    break;
                case 1:
                    filter_str = "port " + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 65535));
                    break;
                case 2:
                    filter_str = "vlan and ip";
                    break;
                case 3:
                    filter_str = fdp.ConsumeRandomLengthString(256);
                    break;
            }
            
            bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
            
            // Compile and set filter - may affect filter_in_userland flag
            if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, netmask) == 0) {
                int setfilter_result = pcap_setfilter(pcap_handle, &bpf);
                (void)setfilter_result;
                pcap_freecode(&bpf);
                
                // Test reading with filter applied
                int dispatch_result = pcap_dispatch(pcap_handle, 5,
                    test_mmap_packet_handler, reinterpret_cast<u_char*>(&packet_counter));
                (void)dispatch_result;
            }
            break;
        }
            
        case 5: {
            // Scenario 5: Timestamp precision and VLAN testing
            // These affect pcap_handle_packet_mmap which is called by mmap read functions
            
            // Test setting datalink type (affects VLAN offset calculation)
            int dlt_values[] = {DLT_EN10MB, DLT_LINUX_SLL, DLT_RAW};
            size_t dlt_idx = fdp.ConsumeIntegralInRange<size_t>(0, 2);
            int setdlt_result = pcap_set_datalink(pcap_handle, dlt_values[dlt_idx]);
            (void)setdlt_result;
            
            // Test direction setting
            int direction_values[] = {PCAP_D_IN, PCAP_D_OUT, PCAP_D_INOUT};
            size_t dir_idx = fdp.ConsumeIntegralInRange<size_t>(0, 2);
            int setdir_result = pcap_setdirection(pcap_handle, 
                static_cast<pcap_direction_t>(direction_values[dir_idx]));
            (void)setdir_result;
            
            // Test with simulated packet data using pcap_inject
            if (simulated_packet.size() > 0) {
                int inject_result = pcap_inject(pcap_handle, simulated_packet.data(), 
                                               simulated_packet.size());
                (void)inject_result;
            }
            
            // Complex dispatch with multiple callbacks
            int complex_dispatch = pcap_dispatch(pcap_handle, -1,  // Unlimited
                [](u_char* user, const struct pcap_pkthdr* h, const u_char* bytes) {
                    int* counter = reinterpret_cast<int*>(user);
                    (*counter)++;
                    
                    // Test accessing packet data with bounds checking
                    if (h != nullptr && bytes != nullptr && h->caplen > 0) {
                        // Test VLAN tag extraction simulation
                        // (pcap_handle_packet_mmap handles VLAN_TPID and VLAN_VALID)
                        (void)bytes[0];
                        if (h->caplen > 14) (void)bytes[14];  // Potential Ethernet type
                    }
                    return;
                }, reinterpret_cast<u_char*>(&packet_counter));
            (void)complex_dispatch;
            break;
        }
    }
    
    // Phase 4: Final cleanup and additional tests
    if (pcap_handle != nullptr) {
        // Test file descriptor access before close
        int selectable_fd = pcap_get_selectable_fd(pcap_handle);
        (void)selectable_fd;
        
        // Test handle properties
        int linktype = pcap_datalink(pcap_handle);
        int snapshot = pcap_snapshot(pcap_handle);
        int major = pcap_major_version(pcap_handle);
        int minor = pcap_minor_version(pcap_handle);
        (void)linktype; (void)snapshot; (void)major; (void)minor;
        
        // Final statistics
        struct pcap_stat final_stats;
        pcap_stats(pcap_handle, &final_stats);
        
        // Close handle - triggers pcap_cleanup_linux
        pcap_close(pcap_handle);
    }
    
    return 0;
}

} // namespace harness_025
