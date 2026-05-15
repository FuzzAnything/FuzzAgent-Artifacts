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

namespace harness_015 {


// Forward declarations for Linux-specific internal functions
extern "C" {
    // These functions are internal to pcap-linux.c but we need to call them
    // to test Linux-specific filtering paths
    int pcap_set_protocol_linux(pcap_t *p, int protocol);
    // Note: pcap_setfilter_linux is called internally through pcap_setfilter
    // We'll test it by calling pcap_setfilter on Linux pcap handles
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Minimum: scenario + protocol + filter complexity + device name + BPF params
    if (Size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios targeting Linux-specific APIs
    // First byte: test scenario selector (0-5)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    // Second byte: Linux protocol for pcap_set_protocol_linux
    int linux_protocol = fdp.ConsumeIntegral<int>();
    
    // Third byte: filter complexity indicator (affects kernel vs userland filtering)
    uint8_t filter_complexity = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Fourth and fifth bytes: snapshot length
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    
    // Sixth byte: flags for socket options and filtering behavior
    uint8_t flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Seventh byte: timeout in milliseconds
    int timeout = fdp.ConsumeIntegralInRange<int>(0, 10000);
    
    // Eighth byte: buffer size
    int buffer_size = fdp.ConsumeIntegralInRange<int>(1024, 1048576);
    
    // Ninth byte: immediate mode flag
    int immediate_mode = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Tenth byte: promiscuous mode flag
    int promisc = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Eleventh byte: non-blocking mode flag
    int nonblock = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Twelfth byte: direction (inbound/outbound/both)
    pcap_direction_t direction = static_cast<pcap_direction_t>(fdp.ConsumeIntegral<uint8_t>() % 3);
    
    // Consume device name (can be real device, "any", or synthetic)
    std::string device_name;
    uint8_t device_type = fdp.ConsumeIntegral<uint8_t>() % 5;
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
            device_name = fdp.ConsumeRandomLengthString(32); // Random device name
            break;
        case 4:
            device_name = fdp.ConsumeRandomLengthString(IFNAMSIZ); // Max length name
            break;
    }
    
    // Consume filter string - varied complexity to trigger different paths in pcap_setfilter_linux
    std::string filter_str;
    size_t max_filter_len = 1024;
    
    switch (filter_complexity) {
        case 0:
            // Simple filter - likely to work in kernel
            filter_str = fdp.ConsumeRandomLengthString(64);
            break;
        case 1:
            // Moderate complexity filter
            filter_str = "ip and tcp port " + std::to_string(fdp.ConsumeIntegralInRange<int>(1, 65535));
            break;
        case 2:
            // Complex filter with multiple conditions - may require userland filtering
            filter_str = "(ip or ip6) and (tcp or udp) and port " + 
                         std::to_string(fdp.ConsumeIntegralInRange<int>(1, 65535));
            break;
        case 3:
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
    }
    
    // BPF program structure
    struct bpf_program bpf;
    memset(&bpf, 0, sizeof(bpf));
    
    // Common variables
    pcap_t *pcap_handle = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE];
    int result;
    
    // Execute based on scenario
    switch (scenario) {
        case 0: {
            // Scenario 0: Basic Linux protocol setting test
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                // Set Linux-specific protocol
                pcap_set_protocol_linux(pcap_handle, linux_protocol);
                
                // Activate the handle
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Try to compile and set filter
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Test protocol setting with various pcap configurations
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                // Configure pcap handle
                pcap_set_snaplen(pcap_handle, snaplen);
                pcap_set_promisc(pcap_handle, promisc);
                pcap_set_timeout(pcap_handle, timeout);
                pcap_set_buffer_size(pcap_handle, buffer_size);
                pcap_set_immediate_mode(pcap_handle, immediate_mode);
                pcap_set_protocol_linux(pcap_handle, linux_protocol);
                
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Try multiple filter compilation attempts
                    for (int attempt = 0; attempt < 2; attempt++) {
                        if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), attempt, PCAP_NETMASK_UNKNOWN) == 0) {
                            pcap_setfilter(pcap_handle, &bpf);
                            pcap_freecode(&bpf);
                        }
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Test filter changes after activation (stress pcap_setfilter_linux)
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                pcap_set_protocol_linux(pcap_handle, linux_protocol);
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Set initial filter
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    }
                    
                    // Change filter multiple times
                    for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                        std::string new_filter = "tcp port " + 
                                                std::to_string(fdp.ConsumeIntegralInRange<int>(1, 65535));
                        
                        if (pcap_compile(pcap_handle, &bpf, new_filter.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                            pcap_setfilter(pcap_handle, &bpf);
                            pcap_freecode(&bpf);
                        }
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Test error paths and edge cases in Linux filtering
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                // Deliberately create problematic states
                if (flags & 0x01) {
                    // Set extremely large buffer size
                    pcap_set_buffer_size(pcap_handle, 100 * 1024 * 1024);
                }
                
                if (flags & 0x02) {
                    // Set very small snapshot length
                    pcap_set_snaplen(pcap_handle, 1);
                }
                
                if (flags & 0x04) {
                    // Set protocol to unusual value - FIX: avoid overflow
                    int protocol_value = linux_protocol;
                    // Avoid overflow by checking bounds
                    if (protocol_value > INT_MAX / 1000 && protocol_value > 0) {
                        protocol_value = INT_MAX / 1000;
                    } else if (protocol_value < INT_MIN / 1000 && protocol_value < 0) {
                        protocol_value = INT_MIN / 1000;
                    }
                    pcap_set_protocol_linux(pcap_handle, protocol_value * 1000);
                }
                
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Test with empty or malformed filter strings
                    std::vector<std::string> test_filters = {
                        "",  // Empty filter
                        "(", // Malformed - unmatched parenthesis
                        "invalid filter string with spaces and symbols !@#$%",
                        "len > 9999999999", // Extremely large value
                        filter_str
                    };
                    
                    for (const auto& test_filter : test_filters) {
                        if (pcap_compile(pcap_handle, &bpf, test_filter.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                            pcap_setfilter(pcap_handle, &bpf);
                            pcap_freecode(&bpf);
                        }
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 4: {
            // Scenario 4: Test async I/O integration with Linux filtering
            // This exercises the interaction between non-blocking mode and filtering
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                // Set up for async operation
                pcap_set_immediate_mode(pcap_handle, 1);
                pcap_setnonblock(pcap_handle, 1, errbuf);
                pcap_set_protocol_linux(pcap_handle, linux_protocol);
                
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    // Set initial filter
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    }
                    
                    // Rapid filter changes to stress the Linux filtering implementation
                    for (int i = 0; i < 5 && fdp.remaining_bytes() > 5; i++) {
                        std::string rapid_filter = "ip and tcp port " + 
                                                  std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1000));
                        
                        if (pcap_compile(pcap_handle, &bpf, rapid_filter.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                            pcap_setfilter(pcap_handle, &bpf);
                            pcap_freecode(&bpf);
                        }
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
        
        case 5: {
            // Scenario 5: Comprehensive Linux-specific API sequence
            // Tests interaction between multiple Linux-specific calls
            pcap_handle = pcap_create(device_name.c_str(), errbuf);
            
            if (pcap_handle != nullptr) {
                // Set all Linux-relevant options
                pcap_set_protocol_linux(pcap_handle, linux_protocol);
                pcap_set_snaplen(pcap_handle, snaplen);
                pcap_set_promisc(pcap_handle, promisc);
                pcap_set_timeout(pcap_handle, timeout);
                pcap_set_buffer_size(pcap_handle, buffer_size);
                pcap_set_immediate_mode(pcap_handle, immediate_mode);
                pcap_setdirection(pcap_handle, direction);
                
                // Alternate between blocking and non-blocking
                pcap_setnonblock(pcap_handle, nonblock, errbuf);
                
                result = pcap_activate(pcap_handle);
                
                if (result >= 0) {
                    struct pcap_stat stats;
                    
                    // Exercise filter compilation with optimization variations
                    for (int optimize = 0; optimize <= 1; optimize++) {
                        if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), optimize, PCAP_NETMASK_UNKNOWN) == 0) {
                            pcap_setfilter(pcap_handle, &bpf);
                            
                            // Get stats to see filter effect
                            pcap_stats(pcap_handle, &stats);
                            
                            pcap_freecode(&bpf);
                        }
                    }
                    
                    // Test with pcap_setfilter called multiple times (filter replacement)
                    std::string replacement_filter = "udp port " + 
                                                    std::to_string(fdp.ConsumeIntegralInRange<int>(1, 65535));
                    
                    if (pcap_compile(pcap_handle, &bpf, replacement_filter.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    }
                    
                    // Switch direction and set filter again
                    pcap_direction_t new_direction = static_cast<pcap_direction_t>(
                        (direction + 1) % 3);
                    pcap_setdirection(pcap_handle, new_direction);
                    
                    if (pcap_compile(pcap_handle, &bpf, filter_str.c_str(), 1, PCAP_NETMASK_UNKNOWN) == 0) {
                        pcap_setfilter(pcap_handle, &bpf);
                        pcap_freecode(&bpf);
                    }
                }
                
                pcap_close(pcap_handle);
            }
            break;
        }
    }
    
    // Clean up any remaining BPF program
    if (bpf.bf_insns != nullptr) {
        pcap_freecode(&bpf);
    }
    
    return 0;
}

} // namespace harness_015
