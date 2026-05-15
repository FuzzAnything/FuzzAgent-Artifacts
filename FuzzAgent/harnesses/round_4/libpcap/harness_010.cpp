#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <pcap/pcap.h>

namespace harness_010 {


int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Minimum size needed for meaningful testing of remote capture APIs
    // Need enough data for: scenario selection, source string creation, 
    // authentication parameters, and device enumeration
    if (Size < 32) {
        return 0;  // Not enough input for meaningful testing
    }

    FuzzedDataProvider fdp(Data, Size);
    
    // Split input for different test scenarios
    // First byte: test scenario selector (0-3)
    uint8_t scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Second byte: source type for pcap_createsrcstr
    uint8_t src_type_byte = fdp.ConsumeIntegral<uint8_t>();
    int src_type;
    
    // Map input byte to valid source types
    switch (src_type_byte % 4) {
        case 0: src_type = PCAP_SRC_FILE; break;
        case 1: src_type = PCAP_SRC_IFLOCAL; break;
        case 2: src_type = PCAP_SRC_IFREMOTE; break;
        default: src_type = PCAP_SRC_IFREMOTE; break;  // Default to remote
    }
    
    // Third byte: authentication type for pcap_rmtauth
    uint8_t auth_type_byte = fdp.ConsumeIntegral<uint8_t>();
    int auth_type = (auth_type_byte % 2 == 0) ? RPCAP_RMTAUTH_NULL : RPCAP_RMTAUTH_PWD;
    // Split remaining input into strings for host, port, name, username, password
    // Use reasonable maximum lengths to prevent excessive memory usage
    std::string host_str = fdp.ConsumeRandomLengthString(128);
    std::string port_str = fdp.ConsumeRandomLengthString(16);
    std::string name_str = fdp.ConsumeRandomLengthString(128);
    std::string user_str = fdp.ConsumeRandomLengthString(64);
    std::string pass_str = fdp.ConsumeRandomLengthString(64);
    
    // Reserve 2 bytes for sampling configuration before consuming remaining bytes
    uint8_t sampling_method_byte = 0;
    uint8_t sampling_value_byte = 10; // default
    
    if (fdp.remaining_bytes() >= 2) {
        sampling_method_byte = fdp.ConsumeIntegral<uint8_t>();
        sampling_value_byte = fdp.ConsumeIntegral<uint8_t>();
    }
    
    // Use remaining bytes for source string input for parsing tests
    std::string source_input = fdp.ConsumeRemainingBytesAsString();
    char source_errbuf[PCAP_ERRBUF_SIZE];
    memset(source_errbuf, 0, sizeof(source_errbuf));
    
    // Buffer for created source string (needs to be large enough)
    char source_buffer[PCAP_BUF_SIZE];
    memset(source_buffer, 0, sizeof(source_buffer));
    
    // Variables for parsed source string components
    int parsed_type = 0;
    char parsed_host[PCAP_BUF_SIZE];
    char parsed_port[PCAP_BUF_SIZE];
    char parsed_name[PCAP_BUF_SIZE];
    char parse_errbuf[PCAP_ERRBUF_SIZE];
    
    memset(parsed_host, 0, sizeof(parsed_host));
    memset(parsed_port, 0, sizeof(parsed_port));
    memset(parsed_name, 0, sizeof(parsed_name));
    memset(parse_errbuf, 0, sizeof(parse_errbuf));
    
    // Step 1: Call pcap_createsrcstr to create source string
    // This creates a source string from components
    int create_result = pcap_createsrcstr(
        source_buffer,           // output buffer
        src_type,                // source type
        host_str.empty() ? NULL : host_str.c_str(),  // host
        port_str.empty() ? NULL : port_str.c_str(),  // port  
        name_str.empty() ? NULL : name_str.c_str(),  // name
        source_errbuf            // error buffer
    );
    
    // Whether creation succeeded or failed, both are interesting for coverage
    
    // Step 2: Call pcap_parsesrcstr to parse and validate source strings
    // Try parsing both the created source string and raw input
    int parse_result1 = -1;
    int parse_result2 = -1;
    
    // Parse the created source string if creation was successful
    if (create_result == 0) {
        parse_result1 = pcap_parsesrcstr(
            source_buffer,       // source string to parse
            &parsed_type,        // parsed type
            parsed_host,         // parsed host
            parsed_port,         // parsed port
            parsed_name,         // parsed name
            parse_errbuf         // error buffer
        );
    }
    
    // Also try parsing the raw input string
    // Reset parsed variables
    memset(parsed_host, 0, sizeof(parsed_host));
    memset(parsed_port, 0, sizeof(parsed_port));
    memset(parsed_name, 0, sizeof(parsed_name));
    memset(parse_errbuf, 0, sizeof(parse_errbuf));
    
    if (!source_input.empty()) {
        parse_result2 = pcap_parsesrcstr(
            source_input.c_str(),  // raw input as source string
            &parsed_type,          // parsed type
            parsed_host,           // parsed host
            parsed_port,           // parsed port
            parsed_name,           // parsed name
            parse_errbuf           // error buffer
        );
    }
    
    // Step 3: Set up authentication structure for remote capture
    struct pcap_rmtauth auth;
    auth.type = auth_type;
    
    // Set username and password based on authentication type
    if (auth_type == RPCAP_RMTAUTH_PWD) {
        // For password authentication, need to provide credentials
        // Use fuzzed strings
        auth.username = user_str.empty() ? NULL : const_cast<char*>(user_str.c_str());
        auth.password = pass_str.empty() ? NULL : const_cast<char*>(pass_str.c_str());
    } else {
        // For NULL authentication, both can be NULL
        auth.username = NULL;
        auth.password = NULL;
    }
    
    // Step 4: Call pcap_findalldevs_ex to enumerate devices
    // Try with different source strings based on scenario
    pcap_if_t *alldevs = NULL;
    char find_errbuf[PCAP_ERRBUF_SIZE];
    memset(find_errbuf, 0, sizeof(find_errbuf));
    
    int find_result = -1;
    const char* find_source = NULL;
    
    switch (scenario) {
        case 0:
            // Use created source string if available
            find_source = (create_result == 0) ? source_buffer : "rpcap://";
            break;
        case 1:
            // Use raw input as source string
            find_source = source_input.empty() ? "rpcap://" : source_input.c_str();
            break;
        case 2:
            // Try local interface enumeration
            find_source = "rpcap://";
            break;
        case 3:
            // Try file source
            find_source = "file://";
            break;
    }
    
    // Actually call pcap_findalldevs_ex
    find_result = pcap_findalldevs_ex(
        find_source,           // source string
        (auth_type == RPCAP_RMTAUTH_NULL && find_source && strstr(find_source, "rpcap://")) ? &auth : NULL,  // auth only for remote
        &alldevs,              // device list
        find_errbuf            // error buffer
    );
    
    // Step 5: If devices were found, try to open one and configure sampling
    pcap_t *pcap_handle = NULL;
    if (find_result == 0 && alldevs != NULL) {
        // Try to open the first device in the list
        pcap_if_t *dev = alldevs;
        
        // Create a source string for the specific device
        char dev_source[PCAP_BUF_SIZE];
        memset(dev_source, 0, sizeof(dev_source));
        char dev_errbuf[PCAP_ERRBUF_SIZE];
        memset(dev_errbuf, 0, sizeof(dev_errbuf));
        
        // Try to create source string for the device
        // For remote devices, we need proper source string
        if (strstr(find_source, "rpcap://")) {
            // For remote device
            if (pcap_createsrcstr(dev_source, PCAP_SRC_IFREMOTE, 
                                 host_str.empty() ? "localhost" : host_str.c_str(),
                                 port_str.empty() ? NULL : port_str.c_str(),
                                 dev->name, dev_errbuf) == 0) {
                // Try to open the device
                char open_errbuf[PCAP_ERRBUF_SIZE];
                memset(open_errbuf, 0, sizeof(open_errbuf));
                
                // Use pcap_open for remote devices (pcap_open_live is for local)
                pcap_handle = pcap_open(dev_source, 65535, PCAP_OPENFLAG_PROMISCUOUS,
                                       1000, &auth, open_errbuf);
            }
        } else {
            // For local device, try pcap_open_live
            char live_errbuf[PCAP_ERRBUF_SIZE];
            memset(live_errbuf, 0, sizeof(live_errbuf));
            
            pcap_handle = pcap_open_live(dev->name, 65535, 1, 1000, live_errbuf);
        }
        
        // Step 6: If device was opened, try to configure sampling
        if (pcap_handle != NULL) {
            // Call pcap_setsampling to get sampling structure
            struct pcap_samp *samp = pcap_setsampling(pcap_handle);
            
            // If sampling structure is returned, we can configure it
            if (samp != NULL) {
                // Configure sampling based on pre-stored fuzzed input bytes
                // Use the sampling bytes we reserved earlier
                uint8_t sampling_method = sampling_method_byte % 3;  // 0-2
                uint8_t sampling_value = sampling_value_byte;
                
                samp->method = sampling_method;
                samp->value = sampling_value;
                
                // Note: pcap_setsampling returns a pointer to internal structure
                // We don't need to free it
            }
            
            // Step 7: Close the opened device handle
            pcap_close(pcap_handle);
            pcap_handle = NULL;
        }
    }
    
    // Step 8: Free device list memory if allocated
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
    
    return 0;
}

} // namespace harness_010
