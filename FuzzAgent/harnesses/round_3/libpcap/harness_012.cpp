#include <fuzzer/FuzzedDataProvider.h>
#include <pcap/pcap.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_012 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing of remote capture APIs
    // We need: source type, host, port, name strings, auth type, sampling method, etc.
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume test scenario selector
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 3);
    
    // Consume source type for testing
    int source_type = fdp.ConsumeIntegralInRange<int>(2, 4); // PCAP_SRC_FILE (2), PCAP_SRC_IFLOCAL (3), PCAP_SRC_IFREMOTE (4)
    
    // Consume authentication type
    int auth_type = fdp.ConsumeIntegralInRange<int>(0, 1); // RPCAP_RMTAUTH_NULL (0), RPCAP_RMTAUTH_PWD (1)
    
    // Consume sampling method
    int sampling_method = fdp.ConsumeIntegralInRange<int>(0, 2); // PCAP_SAMP_NOSAMP (0), PCAP_SAMP_1_EVERY_N (1), PCAP_SAMP_FIRST_AFTER_N_MS (2)
    
    // Consume sampling value
    int sampling_value = fdp.ConsumeIntegralInRange<int>(0, 1000);
    
    // Consume pcap_open flags
    int open_flags = fdp.ConsumeIntegral<int>();
    
    // Consume snaplen
    int snaplen = fdp.ConsumeIntegralInRange<int>(68, 65535);
    
    // Consume read timeout
    int read_timeout = fdp.ConsumeIntegralInRange<int>(0, 10000);
    
    // Consume string parameters with reasonable limits
    size_t host_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string host = fdp.ConsumeBytesAsString(host_len);
    
    size_t port_len = fdp.ConsumeIntegralInRange<size_t>(0, 32);
    std::string port = fdp.ConsumeBytesAsString(port_len);
    
    size_t name_len = fdp.ConsumeIntegralInRange<size_t>(0, 256);
    std::string name = fdp.ConsumeBytesAsString(name_len);
    
    // Consume username and password for authentication
    size_t username_len = fdp.ConsumeIntegralInRange<size_t>(0, 128);
    std::string username = fdp.ConsumeBytesAsString(username_len);
    
    size_t password_len = fdp.ConsumeIntegralInRange<size_t>(0, 128);
    std::string password = fdp.ConsumeBytesAsString(password_len);
    
    // Error buffer for all API calls
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Step 1: Test pcap_parsesrcstr with various source string formats
    // Create source strings based on type
    std::string source_str;
    if (source_type == PCAP_SRC_FILE) {
        source_str = "file://" + name;
    } else if (source_type == PCAP_SRC_IFLOCAL || source_type == PCAP_SRC_IFREMOTE) {
        if (!host.empty()) {
            source_str = "rpcap://" + host;
            if (!port.empty()) {
                source_str += ":" + port;
            }
            source_str += "/" + name;
        } else {
            source_str = "rpcap://" + name;
        }
    } else {
        // Use raw string from remaining input
        source_str = fdp.ConsumeRemainingBytesAsString();
    }
    
    // Parse the source string
    int parsed_type = 0;
    char parsed_host[PCAP_BUF_SIZE] = {0};
    char parsed_port[PCAP_BUF_SIZE] = {0};
    char parsed_name[PCAP_BUF_SIZE] = {0};
    
    if (pcap_parsesrcstr(source_str.c_str(), &parsed_type, parsed_host, parsed_port, parsed_name, errbuf) == -1) {
        // Parsing failed, but that's OK - we still want to test other APIs
        // Reset to defaults
        parsed_type = source_type;
        strncpy(parsed_host, host.c_str(), PCAP_BUF_SIZE - 1);
        strncpy(parsed_port, port.c_str(), PCAP_BUF_SIZE - 1);
        strncpy(parsed_name, name.c_str(), PCAP_BUF_SIZE - 1);
    }
    
    // Step 2: Test pcap_createsrcstr to construct source strings
    char created_source[PCAP_BUF_SIZE] = {0};
    if (pcap_createsrcstr(created_source, parsed_type, 
                         parsed_host[0] ? parsed_host : nullptr,
                         parsed_port[0] ? parsed_port : nullptr,
                         parsed_name[0] ? parsed_name : nullptr,
                         errbuf) == -1) {
        // Creation failed, use original source string
        strncpy(created_source, source_str.c_str(), PCAP_BUF_SIZE - 1);
    }
    
    // Step 3: Test pcap_findalldevs_ex to discover available devices
    pcap_if_t* alldevs = nullptr;
    
    // Prepare authentication structure if needed
    struct pcap_rmtauth auth;
    memset(&auth, 0, sizeof(auth));
    auth.type = auth_type;
    
    if (auth_type == RPCAP_RMTAUTH_PWD) {
        // Note: We need to allocate memory for username/password strings
        // Since these are stack variables, we need to ensure they're properly null-terminated
        // For fuzzing, we'll just use empty strings if allocation fails
        auth.username = const_cast<char*>(username.c_str());
        auth.password = const_cast<char*>(password.c_str());
    }
    
    // Try to find devices with the created source string
    int find_result = pcap_findalldevs_ex(created_source, &auth, &alldevs, errbuf);
    
    // Step 4: Test pcap_open with different source types and flags
    pcap_t* pcap_handle = nullptr;
    
    if (test_scenario == 0 || test_scenario == 1) {
        // Try to open a capture handle
        pcap_handle = pcap_open(created_source, snaplen, open_flags, read_timeout, 
                               (source_type == PCAP_SRC_IFREMOTE) ? &auth : nullptr, 
                               errbuf);
        
        if (pcap_handle != nullptr) {
            // Step 5: Test pcap_setsampling on opened handles
            // Note: According to documentation, sampling only works on Windows local captures
            // but we should still call the API to test its code paths
            struct pcap_samp* samp = pcap_setsampling(pcap_handle);
            if (samp != nullptr) {
                // Configure sampling parameters
                samp->method = sampling_method;
                samp->value = sampling_value;
                // The sampling is now configured on the handle
            }
            
            // Step 6: Cleanup pcap handle
            pcap_close(pcap_handle);
        }
    }
    
    // Step 7: Test iterating through found devices and opening them
    if (test_scenario == 2 && alldevs != nullptr) {
        // Try to open each found device (limited to first few to avoid excessive testing)
        int device_count = 0;
        for (pcap_if_t* dev = alldevs; dev != nullptr && device_count < 3; dev = dev->next) {
            // Create source string for this specific device
            char device_source[PCAP_BUF_SIZE] = {0};
            if (pcap_createsrcstr(device_source, source_type,
                                 parsed_host[0] ? parsed_host : nullptr,
                                 parsed_port[0] ? parsed_port : nullptr,
                                 dev->name,
                                 errbuf) == 0) {
                // Try to open this device
                pcap_t* dev_handle = pcap_open(device_source, snaplen, open_flags, 
                                              read_timeout,
                                              (source_type == PCAP_SRC_IFREMOTE) ? &auth : nullptr,
                                              errbuf);
                if (dev_handle != nullptr) {
                    // Test sampling on this device too
                    struct pcap_samp* dev_samp = pcap_setsampling(dev_handle);
                    if (dev_samp != nullptr) {
                        dev_samp->method = sampling_method;
                        dev_samp->value = sampling_value;
                    }
                    pcap_close(dev_handle);
                }
            }
            device_count++;
        }
    }
    
    // Step 8: Cleanup device list
    if (alldevs != nullptr) {
        pcap_freealldevs(alldevs);
    }
    
    return 0;
}

} // namespace harness_012
