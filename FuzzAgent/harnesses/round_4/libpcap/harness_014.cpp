#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <pcap/pcap.h>

namespace harness_014 {


int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Need sufficient input for meaningful remote capture API testing
    // Minimum: 1 byte scenario + auth type + source string + some flags
    if (Size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios
    // First byte: test scenario selector (0-3)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Second byte: authentication type
    uint8_t auth_type = fdp.ConsumeIntegral<uint8_t>() % 3; // 0=RPCAP_RMTAUTH_NULL, 1=RPCAP_RMTAUTH_PWD
    
    // Third and fourth bytes: snaplen
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 65535);
    
    // Fifth byte: flags (combination of PCAP_OPENFLAG_*)
    int flags = 0;
    uint8_t flags_byte = fdp.ConsumeIntegral<uint8_t>();
    if (flags_byte & 0x01) flags |= PCAP_OPENFLAG_PROMISCUOUS;
    if (flags_byte & 0x02) flags |= PCAP_OPENFLAG_DATATX_UDP;
    if (flags_byte & 0x04) flags |= PCAP_OPENFLAG_NOCAPTURE_LOCAL;
    if (flags_byte & 0x08) flags |= PCAP_OPENFLAG_NOCAPTURE_RPCAP;
    if (flags_byte & 0x10) flags |= PCAP_OPENFLAG_MAX_RESPONSIVENESS;
    
    // Sixth byte: read timeout in milliseconds
    int read_timeout = fdp.ConsumeIntegralInRange<int>(100, 10000);
    
    // Seventh byte: sampling method
    uint8_t sampling_method = fdp.ConsumeIntegral<uint8_t>() % 3; // 0=PCAP_SAMP_NOSAMP, 1=PCAP_SAMP_1_EVERY_N, 2=PCAP_SAMP_FIRST_AFTER_N_MS
    
    // Eighth byte: sampling value
    int sampling_value = fdp.ConsumeIntegralInRange<int>(1, 1000);
    
    // Consume strings for source, username, password, host, port, name
    size_t max_string_len = 256;
    std::string source_str = fdp.ConsumeRandomLengthString(max_string_len);
    std::string username = fdp.ConsumeRandomLengthString(max_string_len);
    std::string password = fdp.ConsumeRandomLengthString(max_string_len);
    std::string host = fdp.ConsumeRandomLengthString(max_string_len);
    std::string port = fdp.ConsumeRandomLengthString(16); // Port max 16 chars
    std::string name = fdp.ConsumeRandomLengthString(max_string_len);
    
    // Prepare error buffer
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    // Prepare authentication structure
    struct pcap_rmtauth auth;
    memset(&auth, 0, sizeof(auth));
    
    if (auth_type == 1) { // RPCAP_RMTAUTH_PWD
        auth.type = 1; // RPCAP_RMTAUTH_PWD
        auth.username = const_cast<char*>(username.c_str());
        auth.password = const_cast<char*>(password.c_str());
    } else {
        // RPCAP_RMTAUTH_NULL or default
        auth.type = 0; // RPCAP_RMTAUTH_NULL
        auth.username = nullptr;
        auth.password = nullptr;
    }
    
    pcap_if_t *alldevs = nullptr;
    pcap_t *pcap_handle = nullptr;
    struct pcap_samp *samp_struct = nullptr;
    
    switch (scenario) {
        case 0: {
            // Scenario 0: Test pcap_findalldevs_ex with various source strings
            int find_result = pcap_findalldevs_ex(source_str.c_str(), &auth, &alldevs, errbuf);
            
            if (find_result == 0 && alldevs != nullptr) {
                // Successfully found devices, iterate through them
                pcap_if_t *dev = alldevs;
                int device_count = 0;
                while (dev != nullptr && device_count < 10) { // Limit iteration
                    // Check device fields
                    if (dev->name != nullptr) {
                        // Could try to get more info about each device
                        // but we're mainly testing pcap_findalldevs_ex paths
                    }
                    dev = dev->next;
                    device_count++;
                }
                
                // Clean up
                pcap_freealldevs(alldevs);
                alldevs = nullptr;
            }
            break;
        }
        
        case 1: {
            // Scenario 1: Test pcap_open with remote source
            pcap_handle = pcap_open(source_str.c_str(), snaplen, flags, read_timeout, &auth, errbuf);
            
            if (pcap_handle != nullptr) {
                // Test pcap_setsampling on the open handle
                samp_struct = pcap_setsampling(pcap_handle);
                if (samp_struct != nullptr) {
                    // Set sampling parameters based on fuzzer input
                    samp_struct->method = sampling_method;
                    samp_struct->value = sampling_value;
                }
                
                // Clean up
                pcap_close(pcap_handle);
                pcap_handle = nullptr;
            }
            break;
        }
        
        case 2: {
            // Scenario 2: Full lifecycle - find devices, open one, set sampling
            // First find devices
            int find_result = pcap_findalldevs_ex(source_str.c_str(), &auth, &alldevs, errbuf);
            
            if (find_result == 0 && alldevs != nullptr) {
                // Try to open the first device
                pcap_if_t *first_dev = alldevs;
                if (first_dev != nullptr && first_dev->name != nullptr) {
                    // Create a source string for this device
                    std::string device_source = "rpcap://";
                    device_source += host;
                    if (!port.empty()) {
                        device_source += ":";
                        device_source += port;
                    }
                    device_source += "/";
                    device_source += first_dev->name;
                    
                    pcap_handle = pcap_open(device_source.c_str(), snaplen, flags, read_timeout, &auth, errbuf);
                    
                    if (pcap_handle != nullptr) {
                        // Test pcap_setsampling
                        samp_struct = pcap_setsampling(pcap_handle);
                        if (samp_struct != nullptr) {
                            samp_struct->method = sampling_method;
                            samp_struct->value = sampling_value;
                        }
                        
                        pcap_close(pcap_handle);
                        pcap_handle = nullptr;
                    }
                }
                
                pcap_freealldevs(alldevs);
                alldevs = nullptr;
            }
            break;
        }
        
        case 3: {
            // Scenario 3: Test pcap_createsrcstr and pcap_parsesrcstr helpers
            char created_source[PCAP_BUF_SIZE];
            memset(created_source, 0, sizeof(created_source));
            
            // Determine source type from fuzzer input
            int source_type = fdp.ConsumeIntegral<uint8_t>() % 4; // 0=PCAP_SRC_FILE, 1=PCAP_SRC_IFLOCAL, 2=PCAP_SRC_IFREMOTE
            
            int create_result = pcap_createsrcstr(created_source, source_type, 
                                                  host.empty() ? nullptr : host.c_str(),
                                                  port.empty() ? nullptr : port.c_str(),
                                                  name.empty() ? nullptr : name.c_str(),
                                                  errbuf);
            
            if (create_result == 0) {
                // Successfully created source string, now parse it back
                int parsed_type;
                char parsed_host[PCAP_BUF_SIZE];
                char parsed_port[PCAP_BUF_SIZE];
                char parsed_name[PCAP_BUF_SIZE];
                char parse_errbuf[PCAP_ERRBUF_SIZE];
                
                memset(parsed_host, 0, sizeof(parsed_host));
                memset(parsed_port, 0, sizeof(parsed_port));
                memset(parsed_name, 0, sizeof(parsed_name));
                memset(parse_errbuf, 0, sizeof(parse_errbuf));
                
                int parse_result = pcap_parsesrcstr(created_source, &parsed_type,
                                                    parsed_host, parsed_port, parsed_name,
                                                    parse_errbuf);
                // Result doesn't matter for fuzzing - we're exercising the code paths
            }
            
            // Also test with the fuzzer-provided source string
            int parsed_type2;
            char parsed_host2[PCAP_BUF_SIZE];
            char parsed_port2[PCAP_BUF_SIZE];
            char parsed_name2[PCAP_BUF_SIZE];
            char parse_errbuf2[PCAP_ERRBUF_SIZE];
            
            memset(parsed_host2, 0, sizeof(parsed_host2));
            memset(parsed_port2, 0, sizeof(parsed_port2));
            memset(parsed_name2, 0, sizeof(parsed_name2));
            memset(parse_errbuf2, 0, sizeof(parse_errbuf2));
            
            int parse_result2 = pcap_parsesrcstr(source_str.c_str(), &parsed_type2,
                                                parsed_host2, parsed_port2, parsed_name2,
                                                parse_errbuf2);
            // Result doesn't matter for fuzzing
            break;
        }
    }
    
    // Ensure any remaining handles are cleaned up
    if (alldevs != nullptr) {
        pcap_freealldevs(alldevs);
    }
    
    if (pcap_handle != nullptr) {
        pcap_close(pcap_handle);
    }
    
    return 0;
}

} // namespace harness_014
