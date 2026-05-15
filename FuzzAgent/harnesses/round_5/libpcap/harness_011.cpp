#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_011 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for basic parameters
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector to test different API workflows
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    char errbuf[PCAP_ERRBUF_SIZE];
    char source_buf[PCAP_BUF_SIZE];
    char host_buf[PCAP_BUF_SIZE];
    char port_buf[PCAP_BUF_SIZE];
    char name_buf[PCAP_BUF_SIZE];
    
    // Initialize buffers
    memset(source_buf, 0, sizeof(source_buf));
    memset(host_buf, 0, sizeof(host_buf));
    memset(port_buf, 0, sizeof(port_buf));
    memset(name_buf, 0, sizeof(name_buf));
    memset(errbuf, 0, sizeof(errbuf));
    
    switch (operation) {
        case 0: {
            // Test pcap_createsrcstr and pcap_parsesrcstr together
            int type = fdp.ConsumeIntegralInRange<int>(PCAP_SRC_FILE, PCAP_SRC_IFREMOTE);
            
            // Consume strings for source creation
            std::string host = fdp.ConsumeRandomLengthString(64);
            std::string port = fdp.ConsumeRandomLengthString(16);
            std::string name = fdp.ConsumeRandomLengthString(128);
            
            // Create source string
            int create_result = pcap_createsrcstr(source_buf, type, 
                host.empty() ? NULL : host.c_str(),
                port.empty() ? NULL : port.c_str(),
                name.empty() ? NULL : name.c_str(),
                errbuf);
            
            if (create_result == 0 && source_buf[0] != '\0') {
                // Now parse the created source string
                int parsed_type;
                char parsed_host[PCAP_BUF_SIZE];
                char parsed_port[PCAP_BUF_SIZE];
                char parsed_name[PCAP_BUF_SIZE];
                
                memset(parsed_host, 0, sizeof(parsed_host));
                memset(parsed_port, 0, sizeof(parsed_port));
                memset(parsed_name, 0, sizeof(parsed_name));
                
                int parse_result = pcap_parsesrcstr(source_buf, &parsed_type,
                    parsed_host, parsed_port, parsed_name, errbuf);
                
                (void)parse_result;  // Result may be -1 for invalid strings, that's OK
                (void)parsed_type;
            }
            break;
        }
        
        case 1: {
            // Test pcap_open with various source strings
            int type = fdp.ConsumeIntegralInRange<int>(PCAP_SRC_FILE, PCAP_SRC_IFREMOTE);
            
            // Consume parameters for pcap_open
            std::string host = fdp.ConsumeRandomLengthString(64);
            std::string port = fdp.ConsumeRandomLengthString(16);
            std::string name = fdp.ConsumeRandomLengthString(128);
            
            // Create source string first
            int create_result = pcap_createsrcstr(source_buf, type,
                host.empty() ? NULL : host.c_str(),
                port.empty() ? NULL : port.c_str(),
                name.empty() ? NULL : name.c_str(),
                errbuf);
            
            if (create_result == 0 && source_buf[0] != '\0') {
                // Consume pcap_open parameters
                int snaplen = fdp.ConsumeIntegralInRange<int>(0, 65535);
                int flags = fdp.ConsumeIntegral<int>();
                int read_timeout = fdp.ConsumeIntegralInRange<int>(-1, 10000);
                
                // Try to open the source (may fail, that's OK)
                pcap_t* handle = pcap_open(source_buf, snaplen, flags, 
                    read_timeout, NULL, errbuf);
                
                if (handle != nullptr) {
                    // Test pcap_get_required_select_timeout if on UNIX
                    #ifndef _WIN32
                    const struct timeval* timeout = pcap_get_required_select_timeout(handle);
                    (void)timeout;
                    #endif
                    
                    // Test pcap_setsampling if available
                    struct pcap_samp* samp = pcap_setsampling(handle);
                    if (samp != nullptr) {
                        // Configure sampling parameters
                        samp->method = fdp.ConsumeIntegralInRange<int>(PCAP_SAMP_NOSAMP, PCAP_SAMP_FIRST_AFTER_N_MS);
                        samp->value = fdp.ConsumeIntegral<int>();
                    }
                    
                    pcap_close(handle);
                }
            }
            break;
        }
        
        case 2: {
            // Test direct pcap_parsesrcstr with fuzzed input strings
            std::string source_input = fdp.ConsumeRandomLengthString(256);
            
            if (!source_input.empty()) {
                int parsed_type;
                char parsed_host[PCAP_BUF_SIZE];
                char parsed_port[PCAP_BUF_SIZE];
                char parsed_name[PCAP_BUF_SIZE];
                
                memset(parsed_host, 0, sizeof(parsed_host));
                memset(parsed_port, 0, sizeof(parsed_port));
                memset(parsed_name, 0, sizeof(parsed_name));
                
                int parse_result = pcap_parsesrcstr(source_input.c_str(), &parsed_type,
                    parsed_host, parsed_port, parsed_name, errbuf);
                
                (void)parse_result;
                (void)parsed_type;
                
                // If parsing succeeded, try to create a source string from the parsed components
                if (parse_result == 0) {
                    char reconstructed[PCAP_BUF_SIZE];
                    memset(reconstructed, 0, sizeof(reconstructed));
                    
                    int create_result = pcap_createsrcstr(reconstructed, parsed_type,
                        parsed_host[0] != '\0' ? parsed_host : NULL,
                        parsed_port[0] != '\0' ? parsed_port : NULL,
                        parsed_name[0] != '\0' ? parsed_name : NULL,
                        errbuf);
                    
                    (void)create_result;
                }
            }
            break;
        }
        
        case 3: {
            // Test sampling configuration on a dead handle
            int linktype = fdp.ConsumeIntegralInRange<int>(0, 255);
            int snaplen = fdp.ConsumeIntegralInRange<int>(0, 65535);
            
            pcap_t* dead_handle = pcap_open_dead(linktype, snaplen);
            if (dead_handle != nullptr) {
                // Test pcap_setsampling on dead handle
                struct pcap_samp* samp = pcap_setsampling(dead_handle);
                if (samp != nullptr) {
                    samp->method = fdp.ConsumeIntegralInRange<int>(PCAP_SAMP_NOSAMP, PCAP_SAMP_FIRST_AFTER_N_MS);
                    samp->value = fdp.ConsumeIntegral<int>();
                }
                
                #ifndef _WIN32
                // Test pcap_get_required_select_timeout on dead handle
                const struct timeval* timeout = pcap_get_required_select_timeout(dead_handle);
                (void)timeout;
                #endif
                
                pcap_close(dead_handle);
            }
            break;
        }
        
        case 4: {
            // Test source string creation with edge cases
            int type = fdp.ConsumeIntegralInRange<int>(PCAP_SRC_FILE, PCAP_SRC_IFREMOTE);
            
            // Test with empty strings
            int result1 = pcap_createsrcstr(source_buf, type, 
                NULL, NULL, NULL, errbuf);
            
            // Test with very short strings
            std::string short_host = fdp.ConsumeBytesAsString(1);
            std::string short_port = fdp.ConsumeBytesAsString(1);
            std::string short_name = fdp.ConsumeBytesAsString(1);
            
            char source_buf2[PCAP_BUF_SIZE];
            memset(source_buf2, 0, sizeof(source_buf2));
            
            int result2 = pcap_createsrcstr(source_buf2, type,
                short_host.c_str(), short_port.c_str(), short_name.c_str(), errbuf);
            
            // Test with maximum length strings
            std::string max_host = fdp.ConsumeRandomLengthString(PCAP_BUF_SIZE - 1);
            std::string max_port = fdp.ConsumeRandomLengthString(PCAP_BUF_SIZE - 1);
            std::string max_name = fdp.ConsumeRandomLengthString(PCAP_BUF_SIZE - 1);
            
            char source_buf3[PCAP_BUF_SIZE];
            memset(source_buf3, 0, sizeof(source_buf3));
            
            int result3 = pcap_createsrcstr(source_buf3, type,
                max_host.c_str(), max_port.c_str(), max_name.c_str(), errbuf);
            
            (void)result1;
            (void)result2;
            (void)result3;
            
            // Try to parse any successfully created source strings
            if (source_buf[0] != '\0') {
                int parsed_type;
                char parsed_host[PCAP_BUF_SIZE];
                char parsed_port[PCAP_BUF_SIZE];
                char parsed_name[PCAP_BUF_SIZE];
                
                memset(parsed_host, 0, sizeof(parsed_host));
                memset(parsed_port, 0, sizeof(parsed_port));
                memset(parsed_name, 0, sizeof(parsed_name));
                
                pcap_parsesrcstr(source_buf, &parsed_type,
                    parsed_host, parsed_port, parsed_name, errbuf);
            }
            break;
        }
    }
    
    return 0;
}

} // namespace harness_011
