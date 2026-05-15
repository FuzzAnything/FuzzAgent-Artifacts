#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>

namespace harness_026 {
/*
 * Harness targeting 5 related APIs from pcap-new.c with minimal coverage (1% function lines hit):
 * pcap_findalldevs_ex, pcap_open, pcap_setsampling, pcap_parsesrcstr, pcap_createsrcstr
 * 
 * Strategy: Implement a comprehensive logical device discovery and opening sequence
 * with focus on edge cases and error paths that haven't been thoroughly tested.
 * 
 * Semantic Differentiation from existing harnesses (018, 024):
 * 1. More comprehensive testing of authentication structures for remote devices
 * 2. Deeper exploration of pcap_open flag combinations and their interactions
 * 3. Systematic testing of pcap_setsampling return values and structure modification
 * 4. Focus on source string parsing edge cases (empty strings, maximum lengths, special chars)
 * 5. Complete lifecycle testing with proper resource cleanup in all code paths
 * 
 * Coverage Goal: Target the remaining 99% of uncovered lines in pcap-new.c APIs
 * by exploring deeper parameter validation and state transition paths.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for comprehensive testing of all 5 APIs
    // Need space for source strings, authentication data, flags, and parameters
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Phase 1: Consume control parameters for test scenario selection
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 7);
    int focus_error_paths = fdp.ConsumeBool() ? 1 : 0;
    
    // Phase 2: Consume source string components with varied lengths
    // Test edge cases with empty strings, maximum lengths, and special values
    size_t host_len = fdp.ConsumeIntegralInRange<size_t>(0, PCAP_BUF_SIZE - 1);
    std::string host = fdp.ConsumeBytesAsString(host_len);
    
    size_t port_len = fdp.ConsumeIntegralInRange<size_t>(0, 16);
    std::string port = fdp.ConsumeBytesAsString(port_len);
    
    size_t name_len = fdp.ConsumeIntegralInRange<size_t>(0, PCAP_BUF_SIZE - 1);
    std::string name = fdp.ConsumeBytesAsString(name_len);
    
    size_t filename_len = fdp.ConsumeIntegralInRange<size_t>(0, PCAP_BUF_SIZE - 1);
    std::string filename = fdp.ConsumeBytesAsString(filename_len);
    
    // Phase 3: Consume authentication data
    int auth_type = fdp.ConsumeIntegralInRange<int>(0, 1); // RPCAP_RMTAUTH_NULL or RPCAP_RMTAUTH_PWD
    size_t username_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string username = fdp.ConsumeBytesAsString(username_len);
    
    size_t password_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string password = fdp.ConsumeBytesAsString(password_len);
    
    // Phase 4: Consume pcap_open parameters with edge cases
    int snaplen = fdp.ConsumeIntegralInRange<int>(-100, 262144); // Test negative and extreme values
    int flags = fdp.ConsumeIntegral<int>(); // All combinations
    int read_timeout = fdp.ConsumeIntegralInRange<int>(-1000, 10000); // Negative and positive
    
    // Phase 5: Consume sampling parameters
    int sampling_method = fdp.ConsumeIntegralInRange<int>(-1, 3); // Test invalid values
    int sampling_value = fdp.ConsumeIntegralInRange<int>(-100, 10000);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    int result;
    pcap_t* pcap_handle = nullptr;
    pcap_if_t* alldevs = nullptr;
    
    // Phase 6: Execute comprehensive test scenarios
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Complete lifecycle for file source with error path focus
            char source[PCAP_BUF_SIZE];
            
            if (focus_error_paths) {
                // Test error paths: empty filename, too long strings, etc.
                result = pcap_createsrcstr(source, PCAP_SRC_FILE, 
                                          host.c_str(), port.c_str(), 
                                          "", errbuf);
            } else {
                result = pcap_createsrcstr(source, PCAP_SRC_FILE, 
                                          NULL, NULL, 
                                          filename.c_str(), errbuf);
            }
            
            if (result == 0) {
                // Parse back to verify
                int parsed_type;
                char parsed_host[PCAP_BUF_SIZE];
                char parsed_port[PCAP_BUF_SIZE];
                char parsed_name[PCAP_BUF_SIZE];
                
                pcap_parsesrcstr(source, &parsed_type, parsed_host, 
                                parsed_port, parsed_name, errbuf);
                
                // Find devices (for file source, lists files in directory)
                pcap_findalldevs_ex(source, NULL, &alldevs, errbuf);
                
                if (alldevs != nullptr) {
                    pcap_freealldevs(alldevs);
                    alldevs = nullptr;
                }
                
                // Try to open the file (may fail but tests error paths)
                if (!filename.empty()) {
                    pcap_handle = pcap_open(source, snaplen, flags, 
                                           read_timeout, NULL, errbuf);
                    
                    if (pcap_handle != nullptr) {
                        // Test sampling (returns NULL for file sources but tests the API)
                        struct pcap_samp* samp = pcap_setsampling(pcap_handle);
                        if (samp != nullptr) {
                            // Even if sampling is not supported, test structure access
                            samp->method = sampling_method;
                            samp->value = sampling_value;
                        }
                        pcap_close(pcap_handle);
                    }
                }
            }
            break;
        }
            
        case 1: {
            // Scenario 2: Local interface with comprehensive flag testing
            char source[PCAP_BUF_SIZE];
            
            result = pcap_createsrcstr(source, PCAP_SRC_IFLOCAL, 
                                      NULL, NULL, 
                                      name.c_str(), errbuf);
            
            if (result == 0) {
                // Parse verification
                int parsed_type;
                char parsed_host[PCAP_BUF_SIZE];
                char parsed_port[PCAP_BUF_SIZE];
                char parsed_name[PCAP_BUF_SIZE];
                
                pcap_parsesrcstr(source, &parsed_type, parsed_host, 
                                parsed_port, parsed_name, errbuf);
                
                // Find local devices
                result = pcap_findalldevs_ex(source, NULL, &alldevs, errbuf);
                
                if (result == 0 && alldevs != nullptr) {
                    // Try to open first few devices with different flag combinations
                    pcap_if_t* dev = alldevs;
                    int device_count = 0;
                    
                    while (dev != nullptr && device_count < 3) {
                        if (dev->name != nullptr) {
                            // Test with different flag combinations
                            int test_flags = flags;
                            
                            // Test each flag individually and in combination
                            for (int flag_test = 0; flag_test < 4; flag_test++) {
                                switch (flag_test) {
                                    case 0: test_flags = 0; break;
                                    case 1: test_flags = PCAP_OPENFLAG_PROMISCUOUS; break;
                                    case 2: test_flags = PCAP_OPENFLAG_MAX_RESPONSIVENESS; break;
                                    case 3: test_flags = flags; break; // Use fuzzed combination
                                }
                                
                                pcap_handle = pcap_open(dev->name, snaplen, test_flags,
                                                       read_timeout, NULL, errbuf);
                                
                                if (pcap_handle != nullptr) {
                                    // Test sampling API
                                    struct pcap_samp* samp = pcap_setsampling(pcap_handle);
                                    if (samp != nullptr) {
                                        // Test valid and invalid method values
                                        if (sampling_method >= 0 && sampling_method <= 2) {
                                            samp->method = sampling_method;
                                            samp->value = sampling_value;
                                        }
                                    }
                                    pcap_close(pcap_handle);
                                    pcap_handle = nullptr;
                                }
                            }
                        }
                        dev = dev->next;
                        device_count++;
                    }
                    
                    pcap_freealldevs(alldevs);
                    alldevs = nullptr;
                }
            }
            break;
        }
            
        case 2: {
            // Scenario 3: Remote interface with authentication testing
            char source[PCAP_BUF_SIZE];
            
            // Create remote source string with potentially empty components
            result = pcap_createsrcstr(source, PCAP_SRC_IFREMOTE,
                                      host.empty() ? NULL : host.c_str(),
                                      port.empty() ? NULL : port.c_str(),
                                      name.empty() ? "" : name.c_str(),
                                      errbuf);
            
            if (result == 0 || focus_error_paths) {
                // Always try parsing, even if creation failed (tests error handling)
                int parsed_type;
                char parsed_host[PCAP_BUF_SIZE];
                char parsed_port[PCAP_BUF_SIZE];
                char parsed_name[PCAP_BUF_SIZE];
                
                pcap_parsesrcstr(source, &parsed_type, parsed_host,
                                parsed_port, parsed_name, errbuf);
                
                // Set up authentication structure
                struct pcap_rmtauth auth;
                auth.type = auth_type;
                
                // Test both NULL and non-NULL authentication
                if (auth_type == 1 && !username.empty() && !password.empty()) {
                    // Use allocated strings that we'll need to free
                    char* user_copy = strdup(username.c_str());
                    char* pass_copy = strdup(password.c_str());
                    
                    auth.username = user_copy;
                    auth.password = pass_copy;
                    
                    // Find remote devices with authentication
                    result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
                    
                    free(user_copy);
                    free(pass_copy);
                } else {
                    auth.username = NULL;
                    auth.password = NULL;
                    
                    // Find remote devices without authentication
                    result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
                }
                
                if (result == 0 && alldevs != nullptr) {
                    // Try to open devices if any were found
                    pcap_if_t* dev = alldevs;
                    int remote_device_count = 0;
                    
                    while (dev != nullptr && remote_device_count < 2) {
                        if (dev->name != nullptr) {
                            // Re-create source string for this specific device
                            char device_source[PCAP_BUF_SIZE];
                            pcap_createsrcstr(device_source, PCAP_SRC_IFREMOTE,
                                            host.empty() ? NULL : host.c_str(),
                                            port.empty() ? NULL : port.c_str(),
                                            dev->name, errbuf);
                            
                            // Try opening with various parameters
                            pcap_handle = pcap_open(device_source, snaplen, flags,
                                                   read_timeout, &auth, errbuf);
                            
                            if (pcap_handle != nullptr) {
                                // Test sampling on remote handle
                                struct pcap_samp* samp = pcap_setsampling(pcap_handle);
                                if (samp != nullptr) {
                                    samp->method = sampling_method;
                                    samp->value = sampling_value;
                                }
                                pcap_close(pcap_handle);
                                pcap_handle = nullptr;
                            }
                        }
                        dev = dev->next;
                        remote_device_count++;
                    }
                    
                    pcap_freealldevs(alldevs);
                    alldevs = nullptr;
                }
            }
            break;
        }
            
        case 3: {
            // Scenario 4: Direct API testing without full lifecycle
            // Test pcap_parsesrcstr with random strings from input
            size_t random_str_len = fdp.ConsumeIntegralInRange<size_t>(0, PCAP_BUF_SIZE * 2);
            std::string random_source = fdp.ConsumeBytesAsString(random_str_len);
            
            if (!random_source.empty()) {
                int parsed_type;
                char parsed_host[PCAP_BUF_SIZE];
                char parsed_port[PCAP_BUF_SIZE];
                char parsed_name[PCAP_BUF_SIZE];
                
                pcap_parsesrcstr(random_source.c_str(), &parsed_type,
                                parsed_host, parsed_port, parsed_name, errbuf);
                
                // Try to find devices with this potentially invalid source
                pcap_findalldevs_ex(random_source.c_str(), NULL, &alldevs, errbuf);
                
                if (alldevs != nullptr) {
                    pcap_freealldevs(alldevs);
                    alldevs = nullptr;
                }
            }
            break;
        }
            
        case 4: {
            // Scenario 5: Edge case testing with boundary values
            // Test maximum length strings and NULL pointers
            char source[PCAP_BUF_SIZE];
            
            // Test with maximum possible lengths
            std::string max_host = fdp.ConsumeBytesAsString(PCAP_BUF_SIZE - 1);
            std::string max_name = fdp.ConsumeBytesAsString(PCAP_BUF_SIZE - 1);
            
            result = pcap_createsrcstr(source, PCAP_SRC_IFREMOTE,
                                      max_host.c_str(), "8888",
                                      max_name.c_str(), errbuf);
            
            // Test parsing of the created string
            if (result == 0) {
                int parsed_type;
                char parsed_host[PCAP_BUF_SIZE];
                char parsed_port[PCAP_BUF_SIZE];
                char parsed_name[PCAP_BUF_SIZE];
                
                pcap_parsesrcstr(source, &parsed_type, parsed_host,
                                parsed_port, parsed_name, errbuf);
            }
            
            // Test pcap_open with extreme parameter values
            pcap_handle = pcap_open("any", 
                                   fdp.ConsumeIntegralInRange<int>(-1000, 1000000),
                                   fdp.ConsumeIntegral<int>(),
                                   fdp.ConsumeIntegralInRange<int>(-10000, 100000),
                                   NULL, errbuf);
            
            if (pcap_handle != nullptr) {
                struct pcap_samp* samp = pcap_setsampling(pcap_handle);
                if (samp != nullptr) {
                    // Test extreme sampling values
                    samp->method = fdp.ConsumeIntegralInRange<int>(-10, 10);
                    samp->value = fdp.ConsumeIntegralInRange<int>(-10000, 100000);
                }
                pcap_close(pcap_handle);
            }
            break;
        }
            
        case 5: {
            // Scenario 6: Memory and resource cleanup testing
            // Repeated API calls to test resource management
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 100; i++) {
                char source[PCAP_BUF_SIZE];
                std::string test_name = fdp.ConsumeRandomLengthString(100);
                
                pcap_createsrcstr(source, PCAP_SRC_IFLOCAL, NULL, NULL,
                                 test_name.c_str(), errbuf);
                
                pcap_if_t* local_devs = nullptr;
                pcap_findalldevs_ex(source, NULL, &local_devs, errbuf);
                
                if (local_devs != nullptr) {
                    pcap_freealldevs(local_devs);
                }
                
                // Try opening with different timeout values
                pcap_handle = pcap_open(test_name.c_str(), 65535, 0,
                                       fdp.ConsumeIntegralInRange<int>(0, 1000),
                                       NULL, errbuf);
                
                if (pcap_handle != nullptr) {
                    pcap_setsampling(pcap_handle);
                    pcap_close(pcap_handle);
                }
            }
            break;
        }
            
        case 6: {
            // Scenario 7: Mixed source type testing
            // Create and test all three source types in sequence
            char file_source[PCAP_BUF_SIZE];
            char local_source[PCAP_BUF_SIZE];
            char remote_source[PCAP_BUF_SIZE];
            
            // File source
            pcap_createsrcstr(file_source, PCAP_SRC_FILE, NULL, NULL,
                             filename.c_str(), errbuf);
            
            // Local interface source
            pcap_createsrcstr(local_source, PCAP_SRC_IFLOCAL, NULL, NULL,
                             name.c_str(), errbuf);
            
            // Remote interface source
            pcap_createsrcstr(remote_source, PCAP_SRC_IFREMOTE,
                             host.c_str(), port.c_str(),
                             name.c_str(), errbuf);
            
            // Parse all three
            int types[3];
            char hosts[3][PCAP_BUF_SIZE];
            char ports[3][PCAP_BUF_SIZE];
            char names[3][PCAP_BUF_SIZE];
            
            pcap_parsesrcstr(file_source, &types[0], hosts[0], ports[0], names[0], errbuf);
            pcap_parsesrcstr(local_source, &types[1], hosts[1], ports[1], names[1], errbuf);
            pcap_parsesrcstr(remote_source, &types[2], hosts[2], ports[2], names[2], errbuf);
            
            // Find devices for each source type
            pcap_if_t* devices[3] = {nullptr, nullptr, nullptr};
            
            pcap_findalldevs_ex(file_source, NULL, &devices[0], errbuf);
            pcap_findalldevs_ex(local_source, NULL, &devices[1], errbuf);
            pcap_findalldevs_ex(remote_source, NULL, &devices[2], errbuf);
            
            // Clean up
            for (int i = 0; i < 3; i++) {
                if (devices[i] != nullptr) {
                    pcap_freealldevs(devices[i]);
                }
            }
            break;
        }
            
        case 7: {
            // Scenario 8: Error injection and recovery testing
            // Test APIs with deliberately bad parameters
            
            // Test with NULL pointers where not allowed
            if (focus_error_paths) {
                pcap_createsrcstr(NULL, PCAP_SRC_FILE, NULL, NULL, "test", errbuf);
                pcap_parsesrcstr(NULL, NULL, NULL, NULL, NULL, errbuf);
                pcap_findalldevs_ex(NULL, NULL, NULL, errbuf);
                
                // Test with invalid type values
                char bad_source[PCAP_BUF_SIZE];
                pcap_createsrcstr(bad_source, -1, NULL, NULL, "test", errbuf);
                pcap_createsrcstr(bad_source, 999, NULL, NULL, "test", errbuf);
                
                // Test pcap_open with bad parameters
                pcap_handle = pcap_open(NULL, 0, 0, 0, NULL, errbuf);
                if (pcap_handle != nullptr) {
                    pcap_close(pcap_handle);
                }
            }
            break;
        }
    }
    
    // Final cleanup (in case any resources weren't cleaned up in scenarios)
    if (alldevs != nullptr) {
        pcap_freealldevs(alldevs);
    }
    
    if (pcap_handle != nullptr) {
        pcap_close(pcap_handle);
    }
    
    return 0;
}

} // namespace harness_026
