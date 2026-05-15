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

namespace harness_016 {


// Forward declarations for Linux-specific internal functions
// We'll try to trigger these through pcap API calls
extern "C" {
    // Note: pcap_setfilter_linux is called internally through pcap_setfilter
    // We'll test it by calling pcap_setfilter on Linux pcap handles with various
    // filter programs to trigger kernel vs userland filtering decisions
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Need sufficient input for comprehensive Linux filtering API testing
    // Minimum: scenario + linktype + filter complexity + device + BPF params + various flags
    if (Size < 256) {
        return 0;  // Need more input for meaningful testing
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios targeting Linux-specific filtering logic
    // First byte: test scenario selector (0-7)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Second byte: link type - affects fix_program behavior
    int linktype = fdp.ConsumeIntegral<uint8_t>();
    
    // Third byte: filter complexity level (0-7)
    uint8_t filter_complexity = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Fourth and fifth bytes: snapshot length
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    
    // Sixth byte: optimize flag for compilation
    int optimize = fdp.ConsumeBool() ? 1 : 0;
    
    // Seventh byte: immediate mode flag
    int immediate_mode = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Eighth byte: promiscuous mode flag
    int promisc = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Ninth byte: timeout in milliseconds
    int timeout = fdp.ConsumeIntegralInRange<int>(0, 10000);
    
    // Tenth byte: buffer size
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1024, 1048576);
    
    // Eleventh byte: direction (inbound/outbound/both)
    pcap_direction_t direction = static_cast<pcap_direction_t>(fdp.ConsumeIntegral<uint8_t>() % 3);
    
    // Twelfth byte: cooked mode flag (SOCK_DGRAM vs SOCK_RAW)
    int cooked_mode = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume device name - can be real device, "any", or synthetic
    std::string device_name;
    uint8_t device_type = fdp.ConsumeIntegral<uint8_t>() % 6;
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
            device_name = fdp.ConsumeRandomLengthString(32); // Random device name
            break;
        case 5:
            device_name = fdp.ConsumeRandomLengthString(IFNAMSIZ); // Max length name
            break;
    }
    
    // Consume filter string - varied complexity to trigger different paths in pcap_setfilter_linux
    // The complexity affects whether filter can work in kernel or requires userland filtering
    std::string filter_str;
    size_t max_filter_len = 1024;
    
    switch (filter_complexity) {
        case 0:
            // Simple filter - likely to work in kernel (short BPF programs)
            filter_str = fdp.ConsumeRandomLengthString(32);
            break;
        case 1:
            // Simple BPF-like filter
            filter_str = "ip";
            break;
        case 2:
            // Moderate complexity - may work in kernel
            filter_str = "tcp port " + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 65535));
            break;
        case 3:
            // Complex filter with multiple conditions - may require userland filtering
            filter_str = "(ip or ip6) and (tcp or udp) and port " + 
                         std::to_string(fdp.ConsumeIntegralInRange<int>(1, 65535));
            break;
        case 4:
            // Very complex filter - will likely require userland filtering
            filter_str = "((ip and not (host " + 
                        std::to_string(fdp.ConsumeIntegralInRange<int>(1, 255)) + "." +
                        std::to_string(fdp.ConsumeIntegralInRange<int>(0, 255)) + "." +
                        std::to_string(fdp.ConsumeIntegralInRange<int>(0, 255)) + "." +
                        std::to_string(fdp.ConsumeIntegralInRange<int>(1, 254)) + 
                        ")) or (tcp and portrange " +
                        std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1000)) + "-" +
                        std::to_string(fdp.ConsumeIntegralInRange<int>(1001, 2000)) + "))";
            break;
        case 5:
            // Extremely long filter - may exceed USHRT_MAX instruction limit
            filter_str = fdp.ConsumeRandomLengthString(512);
            break;
        case 6:
            // Empty filter string - edge case
            filter_str = "";
            break;
        case 7:
            // Filter with special characters
            filter_str = "ip and (tcp or udp) and port not " + 
                        std::to_string(fdp.ConsumeIntegralInRange<int>(1, 65535));
            break;
    }
    
    // Consume netmask for filter compilation
    bpf_u_int32 netmask = fdp.ConsumeIntegral<bpf_u_int32>();
    
    // Consume remaining bytes for additional filter strings (for multiple filter changes)
    std::vector<std::string> additional_filters;
    size_t max_additional_filters = 5;
    for (size_t i = 0; i < max_additional_filters && fdp.remaining_bytes() > 10; i++) {
        additional_filters.push_back(fdp.ConsumeRandomLengthString(64));
    }
    
    // BPF program structure
    struct bpf_program bpf;
    memset(&bpf, 0, sizeof(bpf));
    
    // Common variables
    pcap_t *pcap_handle = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    int result;
    
    // Execute based on scenario - each scenario targets different aspects of Linux filtering
    switch (scenario) {
        case 0: {
            // Scenario 0: Test basic Linux pcap_create + pcap_activate + pcap_setfilter
            // This tests the normal path through pcap_setfilter_linux
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                // Configure basic parameters
                pcap_set_snaplen(pcap_handle, snaplen);
                pcap_set_promisc(pcap_handle, promisc);
                pcap_set_timeout(pcap_handle, timeout);
                pcap_set_buffer_size(pcap_handle, buffer_size);
                pcap_set_immediate_mode(pcap_handle, immediate_mode);
                
                // Activate the handle
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Try to compile and set filter
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), optimize, netmask) == 0) {
                        // This will call pcap_setfilter_linux internally
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Test multiple filter changes to stress filter installation/removal
            // This tests the kernel filter vs userland filter switching logic
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                pcap_set_snaplen(pcap_handle, snaplen);
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Set initial filter
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), optimize, netmask) == 0) {
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    }
                    
                    // Change filter multiple times with different complexities
                    for (size_t i = 0; i < additional_filters.size() && i < 3; i++) {
                        struct bpf_program new_bpf;
                        memset(&new_bpf, 0, sizeof(new_bpf));
                        
                        if (pcap_compile(pcap_handle, &new_bpf, additional_filters[i].c_str(), 
                                        optimize, netmask) == 0) {
                            pcap_setfilter(pcap_handle, &new_bpf);
                            pcap_freecode(&new_bpf);
                        }
                    }
                    
                    // Set direction which may affect filtering
                    pcap_setdirection(pcap_handle, direction);
                    
                    // Try filter one more time
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), optimize, netmask) == 0) {
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Test with pcap_open_dead (dummy handle) for filter compilation testing
            // This tests BPF compilation without actual device
            pcap_handle = pcap_open_dead(linktype, snaplen);
            
            if (pcap_handle != nullptr) {
                // Try to compile filter (should work even without activation)
                if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), optimize, netmask) == 0) {
                    // Note: pcap_setfilter won't work on dead handle, but compilation is tested
                    pcap_freecode(&bpf);
                }
                
                // Test with different optimization levels
                for (int opt_level = 0; opt_level <= 1; opt_level++) {
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), opt_level, netmask) == 0) {
                        pcap_freecode(&bpf);
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Test error paths and edge cases in filter handling
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                // Set extreme values to test boundary conditions
                pcap_set_snaplen(pcap_handle, 0);  // Zero snaplen
                pcap_set_timeout(pcap_handle, -1); // Negative timeout
                
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Test with empty filter string
                    if (filter_str.empty()) {
                        // Empty filter should compile to "accept all"
                        if (pcap_compile(pcap_handle, &bpf, "", optimize, netmask) == 0) {
                            pcap_setfilter(pcap_handle, &bpf);
                            pcap_freecode(&bpf);
                        }
                    }
                    
                    // Test with NULL filter program (should fail gracefully)
                    // We can't pass NULL to pcap_setfilter, but we can test compilation failure
                    std::string invalid_filter = fdp.ConsumeRandomLengthString(10);
                    
                    // Try compilation with various netmasks
                    bpf_u_int32 test_netmasks[] = {0, 0xFFFFFFFF, netmask};
                    for (size_t i = 0; i < 3; i++) {
                        struct bpf_program test_bpf;
                        memset(&test_bpf, 0, sizeof(test_bpf));
                        
                        if (pcap_compile(pcap_handle, &test_bpf, filter_str.c_str(), 
                                        optimize, test_netmasks[i]) == 0) {
                            pcap_setfilter(pcap_handle, &test_bpf);
                            pcap_freecode(&test_bpf);
                        }
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Test filter performance with large/complex BPF programs
            // This targets the USHRT_MAX check in pcap_setfilter_linux
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                pcap_set_snaplen(pcap_handle, snaplen);
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Create a very long filter string to test BPF program size limits
                    std::string long_filter;
                    if (filter_complexity == 5) { // Extremely long filter case
                        // Use the already generated long filter
                        long_filter = filter_str;
                    } else {
                        // Build a long filter by repeating conditions
                        long_filter = "ip";
                        for (int i = 0; i < 20 && fdp.remaining_bytes() > 5; i++) {
                            long_filter += " or tcp port " + 
                                         std::to_string(fdp.ConsumeIntegralInRange<int>(1, 65535));
                        }
                    }
                    
                    // Try to compile the long filter
                    if (pcap_compile(pcap_handle, &bpf, long_filter.c_str(), optimize, netmask) == 0) {
                        // This may trigger the USHRT_MAX check in pcap_setfilter_linux
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    }
                    
                    // Also test with the original filter
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), optimize, netmask) == 0) {
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Test filter with pcap_open_live (alternative to pcap_create)
            // Different code path for live capture
            pcap_handle = pcap_open_live(device_name.c_str(), snaplen, promisc, timeout, errbuf);
            
            if (pcap_handle != nullptr) {
                // Set various handle properties that might affect filtering
                pcap_set_buffer_size(pcap_handle, buffer_size);
                pcap_set_immediate_mode(pcap_handle, immediate_mode);
                pcap_setdirection(pcap_handle, direction);
                
                // Compile and set filter
                if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), optimize, netmask) == 0) {
                    pcap_setfilter(pcap_handle, &bpf);
                    pcap_freecode(&bpf);
                }
                
                // Test non-blocking mode with filter
                char nonblock_errbuf[PCAP_ERRBUF_SIZE];
                memset(nonblock_errbuf, 0, sizeof(nonblock_errbuf));
                pcap_setnonblock(pcap_handle, immediate_mode, nonblock_errbuf);
                
                // Try filter again in non-blocking mode
                if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), optimize, netmask) == 0) {
                    pcap_setfilter(pcap_handle, &bpf);
                    pcap_freecode(&bpf);
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 6: {
            // Scenario 6: Test cooked mode (SOCK_DGRAM) vs raw mode (SOCK_RAW) filtering
            // This affects the fix_offset logic in fix_program
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                // Note: We can't directly set cooked mode, but certain device types
                // or configurations may trigger it internally
                pcap_set_snaplen(pcap_handle, snaplen);
                pcap_set_promisc(pcap_handle, promisc);
                
                // For "any" device, may use cooked mode
                if (device_name == "any") {
                    // "any" device often uses cooked mode
                    // Test filtering on this special device
                }
                
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Test various filter types that may trigger different fix_program paths
                    std::vector<std::string> test_filters;
                    test_filters.push_back(filter_str);
                    test_filters.push_back("ether proto 0x0800");  // IP
                    test_filters.push_back("ether dst 00:00:00:00:00:00");
                    test_filters.push_back("ip and tcp");
                    
                    for (const auto& test_filter : test_filters) {
                        struct bpf_program test_bpf;
                        memset(&test_bpf, 0, sizeof(test_bpf));
                        
                        if (pcap_compile(pcap_handle, &test_bpf, test_filter.c_str(), 
                                        optimize, netmask) == 0) {
                            pcap_setfilter(pcap_handle, &test_bpf);
                            pcap_freecode(&test_bpf);
                        }
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 7: {
            // Scenario 7: Comprehensive stress test - multiple handles, multiple filters
            // Create and test multiple pcap handles in sequence
            std::vector<pcap_t*> handles;
            
            // Create 2-3 handles with different configurations
            for (int handle_idx = 0; handle_idx < 3 && fdp.remaining_bytes() > 50; handle_idx++) {
                pcap_t* test_handle = pcap_create(device_name.c_str(), errbuf);
                
                if (test_handle != nullptr) {
                    // Vary configuration for each handle
                    pcap_set_snaplen(test_handle, snaplen + handle_idx * 100);
                    pcap_set_promisc(test_handle, (promisc + handle_idx) % 2);
                    pcap_set_timeout(test_handle, timeout + handle_idx * 100);
                    
                    result = pcap_activate(test_handle);
                    
                    if (result >= 0) {
                        handles.push_back(test_handle);
                        
                        // Set filter on this handle
                        struct bpf_program handle_bpf;
                        memset(&handle_bpf, 0, sizeof(handle_bpf));
                        
                        // Use different filter for each handle
                        std::string handle_filter;
                        if (handle_idx < additional_filters.size()) {
                            handle_filter = additional_filters[handle_idx];
                        } else {
                            handle_filter = filter_str;
                        }
                        
                        if (pcap_compile(test_handle, &handle_bpf, handle_filter.c_str(), 
                                        optimize, netmask) == 0) {
                            pcap_setfilter(test_handle, &handle_bpf);
                            pcap_freecode(&handle_bpf);
                        }
                    } else {
                        pcap_close(test_handle);
                    }
                }
            }
            
            // Clean up all handles
            for (pcap_t* handle : handles) {
                pcap_close(handle);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_016
