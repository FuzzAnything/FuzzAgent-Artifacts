#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

namespace harness_005 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check: need at least source string type (1 byte), 
    // operation mode (1 byte), and some data for source strings
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation mode: 0 = test parsing only, 1 = test findalldevs_ex only, 
    // 2 = test createsrcstr only, 3 = full lifecycle test
    uint8_t mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume authentication type: 0 = no auth, 1 = password auth
    uint8_t auth_type = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume source type: 0 = local host, 1 = remote host, 2 = file directory
    uint8_t source_type_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    // Consume source string components
    size_t host_len = fdp.ConsumeIntegralInRange<size_t>(0, 128);
    std::string host = fdp.ConsumeBytesAsString(host_len);
    
    size_t port_len = fdp.ConsumeIntegralInRange<size_t>(0, 16);
    std::string port = fdp.ConsumeBytesAsString(port_len);
    
    size_t name_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string name = fdp.ConsumeBytesAsString(name_len);
    
    // Remaining bytes are for additional test strings
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;
    struct pcap_rmtauth auth;
    
    // Initialize authentication structure if needed
    if (auth_type == 1) {
        auth.type = RPCAP_RMTAUTH_PWD;
        auth.username = "fuzzer_user";
        auth.password = "fuzzer_pass";
    } else {
        memset(&auth, 0, sizeof(auth));
    }
    
    // Test 1: pcap_parsesrcstr parsing
    if (mode == 0 || mode == 3) {
        // Create a source string to parse
        char source_to_parse[PCAP_BUF_SIZE];
        int parsed_type;
        char parsed_host[PCAP_BUF_SIZE];
        char parsed_port[PCAP_BUF_SIZE];
        char parsed_name[PCAP_BUF_SIZE];
        
        // Test different source string formats based on source_type_choice
        if (source_type_choice == 0) {
            // Local host format: rpcap://
            if (!host.empty() && !port.empty()) {
                snprintf(source_to_parse, sizeof(source_to_parse), "rpcap://%s:%s/", 
                        host.c_str(), port.c_str());
            } else {
                snprintf(source_to_parse, sizeof(source_to_parse), "rpcap://");
            }
        } else if (source_type_choice == 1) {
            // File directory format: file://
            if (!name.empty()) {
                snprintf(source_to_parse, sizeof(source_to_parse), "file://%s", 
                        name.c_str());
            } else {
                snprintf(source_to_parse, sizeof(source_to_parse), "file:///tmp");
            }
        } else {
            // Local interface format (no protocol)
            if (!name.empty()) {
                snprintf(source_to_parse, sizeof(source_to_parse), "%s", 
                        name.c_str());
            } else {
                snprintf(source_to_parse, sizeof(source_to_parse), "");
            }
        }
        
        // Parse the source string
        int parse_result = pcap_parsesrcstr(source_to_parse, &parsed_type, 
                                           parsed_host, parsed_port, 
                                           parsed_name, errbuf);
        
        // Test parsing result - may fail for invalid strings, which is OK
        if (parse_result == 0) {
            // Successful parse - test the parsed components
            // parsed_type will be one of: PCAP_SRC_IFLOCAL, PCAP_SRC_IFREMOTE, PCAP_SRC_FILE
        }
    }
    
    // Test 2: pcap_findalldevs_ex device enumeration
    if (mode == 1 || mode == 3) {
        char source_for_find[PCAP_BUF_SIZE];
        struct pcap_rmtauth *auth_ptr = (auth_type == 1) ? &auth : nullptr;
        
        // Create source string for pcap_findalldevs_ex based on source_type_choice
        if (source_type_choice == 0) {
            // Local host devices
            if (pcap_createsrcstr(source_for_find, PCAP_SRC_IFLOCAL, 
                                 nullptr, nullptr, nullptr, errbuf) == 0) {
                int find_result = pcap_findalldevs_ex(source_for_find, auth_ptr, 
                                                     &alldevs, errbuf);
                
                if (find_result == 0 && alldevs != nullptr) {
                    // Successfully got device list - iterate through devices
                    pcap_if_t *dev = alldevs;
                    int device_count = 0;
                    while (dev != nullptr && device_count < 100) { // Limit iteration
                        // Access device information
                        const char *dev_name = dev->name;
                        const char *dev_desc = dev->description;
                        
                        // Test pcap_createsrcstr for each device (if mode 3)
                        if (mode == 3 && dev_name != nullptr) {
                            char reconstructed_source[PCAP_BUF_SIZE];
                            // Parse the device name to get components
                            int parsed_type;
                            char parsed_host[PCAP_BUF_SIZE];
                            char parsed_port[PCAP_BUF_SIZE];
                            char parsed_name[PCAP_BUF_SIZE];
                            
                            if (pcap_parsesrcstr(dev_name, &parsed_type,
                                                parsed_host, parsed_port,
                                                parsed_name, errbuf) == 0) {
                                // Recreate the source string
                                pcap_createsrcstr(reconstructed_source, parsed_type,
                                                 parsed_host, parsed_port,
                                                 parsed_name, errbuf);
                            }
                        }
                        
                        dev = dev->next;
                        device_count++;
                    }
                }
                
                // Clean up device list
                if (alldevs != nullptr) {
                    pcap_freealldevs(alldevs);
                    alldevs = nullptr;
                }
            }
        } else if (source_type_choice == 1 && !host.empty()) {
            // Remote host devices (requires host)
            if (pcap_createsrcstr(source_for_find, PCAP_SRC_IFREMOTE,
                                 host.c_str(), port.empty() ? "2002" : port.c_str(),
                                 nullptr, errbuf) == 0) {
                // Try to find devices on remote host (may fail if host not reachable)
                pcap_findalldevs_ex(source_for_find, auth_ptr, &alldevs, errbuf);
                
                // Clean up if we got any devices
                if (alldevs != nullptr) {
                    pcap_freealldevs(alldevs);
                    alldevs = nullptr;
                }
            }
        } else if (source_type_choice == 2 && !name.empty()) {
            // File directory listing
            if (pcap_createsrcstr(source_for_find, PCAP_SRC_FILE,
                                 nullptr, nullptr, name.c_str(), errbuf) == 0) {
                // Try to list files in directory (may fail if directory doesn't exist)
                pcap_findalldevs_ex(source_for_find, nullptr, &alldevs, errbuf);
                
                // Clean up if we got any file list
                if (alldevs != nullptr) {
                    pcap_freealldevs(alldevs);
                    alldevs = nullptr;
                }
            }
        }
    }
    
    // Test 3: pcap_createsrcstr source string creation
    if (mode == 2 || mode == 3) {
        char created_source[PCAP_BUF_SIZE];
        int create_type;
        
        // Test creating different types of source strings
        switch (source_type_choice) {
            case 0: // Local interface
                create_type = PCAP_SRC_IFLOCAL;
                pcap_createsrcstr(created_source, create_type,
                                 nullptr, nullptr, 
                                 name.empty() ? "eth0" : name.c_str(), 
                                 errbuf);
                break;
                
            case 1: // Remote interface
                create_type = PCAP_SRC_IFREMOTE;
                pcap_createsrcstr(created_source, create_type,
                                 host.empty() ? "localhost" : host.c_str(),
                                 port.empty() ? "2002" : port.c_str(),
                                 name.empty() ? nullptr : name.c_str(),
                                 errbuf);
                break;
                
            case 2: // File
                create_type = PCAP_SRC_FILE;
                pcap_createsrcstr(created_source, create_type,
                                 nullptr, nullptr,
                                 name.empty() ? "/tmp" : name.c_str(),
                                 errbuf);
                break;
        }
        
        // Test parsing the created source string
        if (mode == 3) {
            int parsed_type;
            char parsed_host[PCAP_BUF_SIZE];
            char parsed_port[PCAP_BUF_SIZE];
            char parsed_name[PCAP_BUF_SIZE];
            
            pcap_parsesrcstr(created_source, &parsed_type,
                            parsed_host, parsed_port, parsed_name, errbuf);
        }
    }
    
    // Test edge cases with remaining data
    if (!remaining_data.empty() && remaining_data.size() < PCAP_BUF_SIZE) {
        // Try to use remaining data as a source string directly
        char raw_source[PCAP_BUF_SIZE];
        memcpy(raw_source, remaining_data.data(), 
               std::min(remaining_data.size(), sizeof(raw_source) - 1));
        raw_source[std::min(remaining_data.size(), sizeof(raw_source) - 1)] = '\0';
        
        // Test parsing the raw source string
        int parsed_type;
        char parsed_host[PCAP_BUF_SIZE];
        char parsed_port[PCAP_BUF_SIZE];
        char parsed_name[PCAP_BUF_SIZE];
        
        pcap_parsesrcstr(raw_source, &parsed_type,
                        parsed_host, parsed_port, parsed_name, errbuf);
        
        // Test creating a source string from parsed components (if parse succeeded)
        if (pcap_parsesrcstr(raw_source, &parsed_type,
                            parsed_host, parsed_port, parsed_name, errbuf) == 0) {
            char recreated_source[PCAP_BUF_SIZE];
            pcap_createsrcstr(recreated_source, parsed_type,
                             parsed_host, parsed_port, parsed_name, errbuf);
        }
    }
    
    // Final cleanup
    if (alldevs != nullptr) {
        pcap_freealldevs(alldevs);
    }
    
    return 0;
}

} // namespace harness_005
