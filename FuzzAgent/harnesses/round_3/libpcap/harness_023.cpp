#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_023 {
/*
 * Harness for remote capture module (pcap-new.c) with 0% branch coverage
 * Primary Target: pcap_createsrcstr, pcap_parsesrcstr, pcap_open, pcap_setsampling
 * Secondary Targets: pcap_findalldevs_ex, remote authentication APIs
 * Coverage Goal: Target untested remote capture functionality in pcap-new.c module
 * Strategy: Test remote source string creation/parsing, remote device opening with
 *           authentication, sampling configuration, and remote device discovery
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for remote capture testing
    // Need enough data for source strings, authentication, and parameters
    if (size < 50) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size control parameters first
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 4);  // Which test scenario to run
    int src_type = fdp.ConsumeIntegralInRange<int>(2, 4);  // PCAP_SRC_FILE, PCAP_SRC_IFLOCAL, PCAP_SRC_IFREMOTE
    int auth_type = fdp.ConsumeIntegralInRange<int>(0, 1);  // RPCAP_RMTAUTH_NULL, RPCAP_RMTAUTH_PWD
    int sampling_method = fdp.ConsumeIntegralInRange<int>(0, 2);  // PCAP_SAMP_NOSAMP, PCAP_SAMP_1_EVERY_N, PCAP_SAMP_FIRST_AFTER_N_MS
    int sampling_value = fdp.ConsumeIntegralInRange<int>(0, 1000);
    
    // Consume pcap_open parameters
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);
    int flags = fdp.ConsumeIntegral<int>();
    int read_timeout = fdp.ConsumeIntegralInRange<int>(0, 10000);
    
    // Consume string parameters (host, port, name, etc.)
    size_t host_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string host = fdp.ConsumeBytesAsString(host_size);
    
    size_t port_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string port = fdp.ConsumeBytesAsString(port_size);
    
    size_t name_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
    std::string name = fdp.ConsumeBytesAsString(name_size);
    
    // Consume authentication credentials
    size_t username_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string username = fdp.ConsumeBytesAsString(username_size);
    
    size_t password_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string password = fdp.ConsumeBytesAsString(password_size);
    
    // Consume source string for parsing tests
    size_t source_str_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
    std::string source_str = fdp.ConsumeBytesAsString(source_str_size);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    int result;
    
    // Run different test scenarios based on consumed input
    switch (test_scenario) {
        case 0: {
            // Test 1: pcap_createsrcstr - create source strings
            char source[PCAP_BUF_SIZE];
            
            // Try creating source string with consumed parameters
            result = pcap_createsrcstr(source, src_type, 
                                      host.empty() ? NULL : host.c_str(),
                                      port.empty() ? NULL : port.c_str(),
                                      name.c_str(), errbuf);
            
            // If successful, try parsing it back
            if (result == 0) {
                int parsed_type;
                char parsed_host[PCAP_BUF_SIZE];
                char parsed_port[PCAP_BUF_SIZE];
                char parsed_name[PCAP_BUF_SIZE];
                
                pcap_parsesrcstr(source, &parsed_type, parsed_host, 
                                parsed_port, parsed_name, errbuf);
            }
            break;
        }
            
        case 1: {
            // Test 2: pcap_parsesrcstr - parse various source strings
            int parsed_type;
            char parsed_host[PCAP_BUF_SIZE];
            char parsed_port[PCAP_BUF_SIZE];
            char parsed_name[PCAP_BUF_SIZE];
            
            // Try parsing the consumed source string
            result = pcap_parsesrcstr(source_str.c_str(), &parsed_type, 
                                     parsed_host, parsed_port, parsed_name, errbuf);
            
            // If successful, try creating a new source string from parsed components
            if (result == 0) {
                char new_source[PCAP_BUF_SIZE];
                pcap_createsrcstr(new_source, parsed_type,
                                 strlen(parsed_host) > 0 ? parsed_host : NULL,
                                 strlen(parsed_port) > 0 ? parsed_port : NULL,
                                 parsed_name, errbuf);
            }
            break;
        }
            
        case 2: {
            // Test 3: pcap_open with remote authentication
            // First create a source string
            char source[PCAP_BUF_SIZE];
            
            // For remote testing, use IFREMOTE type
            int remote_type = PCAP_SRC_IFREMOTE;
            result = pcap_createsrcstr(source, remote_type,
                                      host.empty() ? NULL : host.c_str(),
                                      port.empty() ? NULL : port.c_str(),
                                      name.c_str(), errbuf);
            
            if (result == 0) {
                // Set up authentication structure
                struct pcap_rmtauth auth;
                auth.type = auth_type;
                
                // For password auth, set username and password
                if (auth_type == RPCAP_RMTAUTH_PWD) {
                    // Need to allocate strings that won't be freed by std::string
                    char* user_copy = username.empty() ? NULL : strdup(username.c_str());
                    char* pass_copy = password.empty() ? NULL : strdup(password.c_str());
                    
                    auth.username = user_copy;
                    auth.password = pass_copy;
                } else {
                    auth.username = NULL;
                    auth.password = NULL;
                }
                
                // Try to open the remote source
                pcap_t* pcap = pcap_open(source, snaplen, flags, read_timeout, 
                                       &auth, errbuf);
                
                if (pcap != nullptr) {
                    // Try to set sampling (even though it may only work on Windows)
                    struct pcap_samp* samp = pcap_setsampling(pcap);
                    if (samp != nullptr) {
                        samp->method = sampling_method;
                        samp->value = sampling_value;
                    }
                    
                    pcap_close(pcap);
                }
                
                // Free allocated strings if used
                if (auth_type == RPCAP_RMTAUTH_PWD) {
                    if (auth.username) free(auth.username);
                    if (auth.password) free(auth.password);
                }
            }
            break;
        }
            
        case 3: {
            // Test 4: pcap_findalldevs_ex for remote device discovery
            // Create a remote source string
            char source[PCAP_BUF_SIZE];
            
            result = pcap_createsrcstr(source, PCAP_SRC_IFREMOTE,
                                      host.empty() ? NULL : host.c_str(),
                                      port.empty() ? NULL : port.c_str(),
                                      "",  // Empty name for device listing
                                      errbuf);
            
            if (result == 0) {
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
                
                pcap_if_t* alldevs = nullptr;
                result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
                
                if (result == 0 && alldevs != nullptr) {
                    // Free the device list
                    pcap_freealldevs(alldevs);
                }
                
                // Free allocated strings if used
                if (auth_type == RPCAP_RMTAUTH_PWD) {
                    if (auth.username) free(auth.username);
                    if (auth.password) free(auth.password);
                }
            }
            break;
        }
            
        case 4: {
            // Test 5: Comprehensive test - chain all operations
            // Parse source string
            int parsed_type;
            char parsed_host[PCAP_BUF_SIZE];
            char parsed_port[PCAP_BUF_SIZE];
            char parsed_name[PCAP_BUF_SIZE];
            
            result = pcap_parsesrcstr(source_str.c_str(), &parsed_type,
                                     parsed_host, parsed_port, parsed_name, errbuf);
            
            if (result == 0) {
                // Create new source string from parsed components
                char new_source[PCAP_BUF_SIZE];
                result = pcap_createsrcstr(new_source, parsed_type,
                                          strlen(parsed_host) > 0 ? parsed_host : NULL,
                                          strlen(parsed_port) > 0 ? parsed_port : NULL,
                                          parsed_name, errbuf);
                
                if (result == 0 && parsed_type == PCAP_SRC_IFREMOTE) {
                    // Try to find devices on remote host
                    struct pcap_rmtauth auth;
                    auth.type = RPCAP_RMTAUTH_NULL;  // Use NULL auth for simplicity
                    auth.username = NULL;
                    auth.password = NULL;
                    
                    pcap_if_t* alldevs = nullptr;
                    pcap_findalldevs_ex(new_source, &auth, &alldevs, errbuf);
                    
                    if (alldevs != nullptr) {
                        pcap_freealldevs(alldevs);
                    }
                }
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_023
