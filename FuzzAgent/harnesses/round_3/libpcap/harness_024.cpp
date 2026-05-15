#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_024 {
/*
 * Harness for pcap-new.c module targeting 0% branch coverage despite 69% line coverage
 * Primary Target: Complete lifecycle testing of newer libpcap interface APIs
 * Focus: pcap_open, pcap_findalldevs_ex, pcap_createsrcstr, pcap_parsesrcstr, 
 *        pcap_setsampling, pcap_close with logical invocation sequence
 * Coverage Goal: Target critical coverage gaps in remote capture module lifecycle
 * Strategy: Test complete device enumeration and capture initialization sequence
 *            with edge cases, error paths, and semantic diversity from harness_023
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for comprehensive lifecycle testing
    // Need enough data for source strings, auth, parameters, and test scenarios
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Phase 1: Consume fixed-size control parameters
    int lifecycle_stage = fdp.ConsumeIntegralInRange<int>(0, 4);  // Which lifecycle stage to focus on
    int source_type = fdp.ConsumeIntegralInRange<int>(2, 4);  // PCAP_SRC_FILE, PCAP_SRC_IFLOCAL, PCAP_SRC_IFREMOTE
    int auth_type = fdp.ConsumeIntegralInRange<int>(0, 1);    // RPCAP_RMTAUTH_NULL, RPCAP_RMTAUTH_PWD
    int sampling_method = fdp.ConsumeIntegralInRange<int>(0, 2);  // PCAP_SAMP_NOSAMP, PCAP_SAMP_1_EVERY_N, PCAP_SAMP_FIRST_AFTER_N_MS
    int sampling_value = fdp.ConsumeIntegralInRange<int>(0, 1000);
    
    // pcap_open parameters
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);
    int flags = fdp.ConsumeIntegral<int>();
    int read_timeout = fdp.ConsumeIntegralInRange<int>(0, 10000);
    
    // Phase 2: Consume string parameters
    size_t host_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string host = fdp.ConsumeBytesAsString(host_size);
    
    size_t port_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string port = fdp.ConsumeBytesAsString(port_size);
    
    size_t name_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string name = fdp.ConsumeBytesAsString(name_size);
    
    size_t filename_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string filename = fdp.ConsumeBytesAsString(filename_size);
    
    // Authentication credentials
    size_t username_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string username = fdp.ConsumeBytesAsString(username_size);
    
    size_t password_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string password = fdp.ConsumeBytesAsString(password_size);
    
    // Source string for parsing tests
    size_t source_str_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    std::string source_str = fdp.ConsumeBytesAsString(source_str_size);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    int result;
    pcap_t* pcap_handle = nullptr;
    
    // Phase 3: Execute different lifecycle test scenarios
    switch (lifecycle_stage) {
        case 0: {
            // Test 1: Complete lifecycle for local file source
            char source[PCAP_BUF_SIZE];
            
            // Create source string for file
            result = pcap_createsrcstr(source, PCAP_SRC_FILE, NULL, NULL, 
                                      filename.c_str(), 
                                      errbuf);
            
            if (result == 0) {
                // Parse it back to verify
                int parsed_type;
                char parsed_host[PCAP_BUF_SIZE];
                char parsed_port[PCAP_BUF_SIZE];
                char parsed_name[PCAP_BUF_SIZE];
                
                pcap_parsesrcstr(source, &parsed_type, parsed_host, 
                                parsed_port, parsed_name, errbuf);
                
                // Try to find devices (for file, this should list files in directory)
                pcap_if_t* alldevs = nullptr;
                pcap_findalldevs_ex(source, NULL, &alldevs, errbuf);
                
                if (alldevs != nullptr) {
                    pcap_freealldevs(alldevs);
                }
            }
            break;
        }
            
        case 1: {
            // Test 2: Complete lifecycle for local interface
            char source[PCAP_BUF_SIZE];
            
            // Create source string for local interface
            result = pcap_createsrcstr(source, PCAP_SRC_IFLOCAL, NULL, NULL, 
                                      name.c_str(), 
                                      errbuf);
            
            if (result == 0) {
                // Parse to verify
                int parsed_type;
                char parsed_host[PCAP_BUF_SIZE];
                char parsed_port[PCAP_BUF_SIZE];
                char parsed_name[PCAP_BUF_SIZE];
                
                pcap_parsesrcstr(source, &parsed_type, parsed_host, 
                                parsed_port, parsed_name, errbuf);
                
                // Find local devices
                pcap_if_t* alldevs = nullptr;
                pcap_findalldevs_ex(source, NULL, &alldevs, errbuf);
                
                if (alldevs != nullptr) {
                    // Try to open first device if available
                    if (alldevs->name != nullptr) {
                        pcap_handle = pcap_open(alldevs->name, snaplen, flags, 
                                               read_timeout, NULL, errbuf);
                        
                        if (pcap_handle != nullptr) {
                            // Test sampling (may return NULL on non-Windows)
                            struct pcap_samp* samp = pcap_setsampling(pcap_handle);
                            if (samp != nullptr) {
                                samp->method = sampling_method;
                                samp->value = sampling_value;
                            }
                            
                            pcap_close(pcap_handle);
                        }
                    }
                    
                    pcap_freealldevs(alldevs);
                }
            }
            break;
        }
            
        case 2: {
            // Test 3: Complete lifecycle for remote interface with authentication
            char source[PCAP_BUF_SIZE];
            
            // Create remote source string
            result = pcap_createsrcstr(source, PCAP_SRC_IFREMOTE,
                                      host.c_str(),
                                      port.empty() ? NULL : port.c_str(),
                                      name.empty() ? "" : name.c_str(),
                                      errbuf);
            
            if (result == 0) {
                // Parse to verify
                int parsed_type;
                char parsed_host[PCAP_BUF_SIZE];
                char parsed_port[PCAP_BUF_SIZE];
                char parsed_name[PCAP_BUF_SIZE];
                
                pcap_parsesrcstr(source, &parsed_type, parsed_host,
                                parsed_port, parsed_name, errbuf);
                
                // Set up authentication
                struct pcap_rmtauth auth;
                auth.type = auth_type;
                
                if (auth_type == RPCAP_RMTAUTH_PWD) {
                    char* user_copy = username.empty() ? NULL : strdup(username.c_str());
                    char* pass_copy = password.empty() ? NULL : strdup(password.c_str());
                    
                    auth.username = user_copy;
                    auth.password = pass_copy;
                } else {
                    auth.username = NULL;
                    auth.password = NULL;
                }
                
                // Find remote devices
                pcap_if_t* alldevs = nullptr;
                result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
                
                if (result == 0 && alldevs != nullptr) {
                    // Try to open first remote device if available
                    if (alldevs->name != nullptr) {
                        pcap_handle = pcap_open(alldevs->name, snaplen, flags,
                                               read_timeout, &auth, errbuf);
                        
                        if (pcap_handle != nullptr) {
                            // Test sampling
                            struct pcap_samp* samp = pcap_setsampling(pcap_handle);
                            if (samp != nullptr) {
                                samp->method = sampling_method;
                                samp->value = sampling_value;
                            }
                            
                            pcap_close(pcap_handle);
                        }
                    }
                    
                    pcap_freealldevs(alldevs);
                }
                
                // Clean up authentication strings
                if (auth_type == RPCAP_RMTAUTH_PWD) {
                    if (auth.username) free(auth.username);
                    if (auth.password) free(auth.password);
                }
            }
            break;
        }
            
        case 3: {
            // Test 4: Error path testing - invalid source strings and parameters
            
            // Test 1: Parse invalid source string
            int parsed_type;
            char parsed_host[PCAP_BUF_SIZE];
            char parsed_port[PCAP_BUF_SIZE];
            char parsed_name[PCAP_BUF_SIZE];
            
            pcap_parsesrcstr(source_str.c_str(), &parsed_type,
                           parsed_host, parsed_port, parsed_name, errbuf);
            
            // Test 2: Create source string with invalid type
            char invalid_source[PCAP_BUF_SIZE];
            int invalid_type = fdp.ConsumeIntegralInRange<int>(-10, 10);
            if (invalid_type < 2 || invalid_type > 4) {
                pcap_createsrcstr(invalid_source, invalid_type,
                                host.empty() ? NULL : host.c_str(),
                                port.empty() ? NULL : port.c_str(),
                                name.c_str(),
                                errbuf);
            }
            
            // Test 3: pcap_open with NULL source
            pcap_t* null_pcap = pcap_open(NULL, snaplen, flags, read_timeout, NULL, errbuf);
            if (null_pcap != nullptr) {
                pcap_close(null_pcap);
            }
            
            // Test 4: pcap_findalldevs_ex with NULL source
            pcap_if_t* null_devs = nullptr;
            pcap_findalldevs_ex(NULL, NULL, &null_devs, errbuf);
            if (null_devs != nullptr) {
                pcap_freealldevs(null_devs);
            }
            
            break;
        }
            
        case 4: {
            // Test 5: Edge case testing - boundary values and special conditions
            
            // Test with maximum buffer sizes
            char large_source[PCAP_BUF_SIZE];
            
            // Create source with maximum allowed host/port/name
            std::string large_host(255, 'x');
            std::string large_port(255, '9');
            std::string large_name(255, 'i');
            
            pcap_createsrcstr(large_source, source_type,
                            large_host.c_str(), large_port.c_str(),
                            large_name.c_str(), errbuf);
            
            // Test with empty strings
            char empty_source[PCAP_BUF_SIZE];
            pcap_createsrcstr(empty_source, source_type,
                            "", "", "", errbuf);
            
            // Test sampling with invalid pcap handle
            pcap_t* dead_pcap = pcap_open_dead(DLT_EN10MB, snaplen);
            if (dead_pcap != nullptr) {
                // pcap_setsampling may fail on dead handles
                struct pcap_samp* dead_samp = pcap_setsampling(dead_pcap);
                if (dead_samp != nullptr) {
                    dead_samp->method = sampling_method;
                    dead_samp->value = sampling_value;
                }
                pcap_close(dead_pcap);
            }
            
            // Test multiple open/close cycles
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 10; i++) {
                pcap_t* temp_pcap = pcap_open_dead(DLT_EN10MB, snaplen + i);
                if (temp_pcap != nullptr) {
                    pcap_close(temp_pcap);
                }
            }
            
            break;
        }
    }
    
    return 0;
}

} // namespace harness_024
