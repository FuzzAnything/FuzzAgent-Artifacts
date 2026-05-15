#include <fuzzer/FuzzedDataProvider.h>
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>

namespace harness_018 {
/*
 * Harness for testing libpcap new API interface for advanced device discovery and source string parsing
 * Targets: pcap_open, pcap_findalldevs_ex, pcap_createsrcstr, pcap_parsesrcstr, pcap_setsampling, pcap_activate, pcap_next_ex, pcap_close, pcap_freealldevs
 * Coverage goal: Untested pcap-new.c module (0% coverage across 18 fuzzers)
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check: need sufficient bytes for various operations
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Create source strings for different interface types using pcap_createsrcstr
    
    // First, test creating source strings for different types
    char source_str[PCAP_BUF_SIZE];
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Consume parameters for source string creation
    uint8_t src_type = fdp.ConsumeIntegral<uint8_t>() % 3; // 0=FILE, 1=IFLOCAL, 2=IFREMOTE
    int actual_type;
    
    switch(src_type) {
        case 0:
            actual_type = PCAP_SRC_FILE;
            break;
        case 1:
            actual_type = PCAP_SRC_IFLOCAL;
            break;
        case 2:
            actual_type = PCAP_SRC_IFREMOTE;
            break;
        default:
            actual_type = PCAP_SRC_IFLOCAL;
    }
    
    // Consume host, port, and name parameters
    std::string host = fdp.ConsumeRandomLengthString(128);
    std::string port = fdp.ConsumeRandomLengthString(16);
    std::string name = fdp.ConsumeRandomLengthString(256);
    
    // Create source string
    int create_result = pcap_createsrcstr(source_str, actual_type, 
                                         host.empty() ? NULL : host.c_str(),
                                         port.empty() ? NULL : port.c_str(),
                                         name.empty() ? NULL : name.c_str(),
                                         errbuf);
    
    // Step 2: Parse and validate source strings using pcap_parsesrcstr
    int parsed_type;
    char parsed_host[PCAP_BUF_SIZE];
    char parsed_port[PCAP_BUF_SIZE];
    char parsed_name[PCAP_BUF_SIZE];
    
    // Try parsing the created source string (if creation was successful)
    if (create_result == 0) {
        int parse_result = pcap_parsesrcstr(source_str, &parsed_type, 
                                           parsed_host, parsed_port, 
                                           parsed_name, errbuf);
        // parse_result intentionally not checked to explore error paths
    }
    
    // Also try parsing random strings from fuzzed input
    std::string random_source = fdp.ConsumeRandomLengthString(512);
    if (!random_source.empty()) {
        pcap_parsesrcstr(random_source.c_str(), &parsed_type,
                        parsed_host, parsed_port, parsed_name, errbuf);
    }
    
    // Step 3: Discover available devices using pcap_findalldevs_ex
    pcap_if_t *alldevs = NULL;
    
    // Try with different source strings
    int find_result = pcap_findalldevs_ex(source_str, NULL, &alldevs, errbuf);
    
    // Also try with NULL source (should return error)
    if (fdp.ConsumeBool()) {
        pcap_findalldevs_ex(NULL, NULL, &alldevs, errbuf);
    }
    
    // Step 4: Open devices using pcap_open with various source strings
    pcap_t *pcap_handle = NULL;
    
    // Consume parameters for pcap_open
    int snaplen = fdp.ConsumeIntegralInRange<int>(1, 262144); // MAXIMUM_SNAPLEN
    int flags = 0;
    
    // Set flags based on fuzzed input
    if (fdp.ConsumeBool()) {
        flags |= PCAP_OPENFLAG_PROMISCUOUS;
    }
    if (fdp.ConsumeBool()) {
        flags |= PCAP_OPENFLAG_DATATX_UDP;
    }
    if (fdp.ConsumeBool()) {
        flags |= PCAP_OPENFLAG_NOCAPTURE_RPCAP;
    }
    if (fdp.ConsumeBool()) {
        flags |= PCAP_OPENFLAG_NOCAPTURE_LOCAL;
    }
    
    int read_timeout = fdp.ConsumeIntegralInRange<int>(0, 10000);
    
    // Try opening with the created source string
    if (create_result == 0) {
        pcap_handle = pcap_open(source_str, snaplen, flags, read_timeout, NULL, errbuf);
    }
    
    // Also try opening with random source string
    std::string random_open_source = fdp.ConsumeRandomLengthString(256);
    if (!random_open_source.empty()) {
        pcap_t *random_handle = pcap_open(random_open_source.c_str(), snaplen, flags, read_timeout, NULL, errbuf);
        if (random_handle != NULL) {
            pcap_close(random_handle);
        }
    }
    
    // Step 5: Configure packet sampling using pcap_setsampling (if device supports it)
    if (pcap_handle != NULL) {
        struct pcap_samp *sampling_ptr;
        sampling_ptr = pcap_setsampling(pcap_handle);
        
        // Step 6: Activate device (if not already activated by pcap_open)
        // pcap_open should have already activated, but try pcap_activate for completeness
        int activate_result = pcap_activate(pcap_handle);
        
        // Step 7: Attempt packet capture using pcap_next_ex
        if (activate_result >= 0) {
            struct pcap_pkthdr *pkthdr;
            const u_char *pktdata;
            
            // Try reading a few packets
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
                int next_result = pcap_next_ex(pcap_handle, &pkthdr, &pktdata);
                
                // Consume some bytes to simulate packet processing
                if (next_result == 1 && pkthdr != NULL && pktdata != NULL) {
                    size_t bytes_to_consume = fdp.ConsumeIntegralInRange<size_t>(0, 100);
                    std::vector<uint8_t> dummy = fdp.ConsumeBytes<uint8_t>(bytes_to_consume);
                }
            }
        }
        
        // Step 8: Clean up opened device
        pcap_close(pcap_handle);
        pcap_handle = NULL;
    }
    
    // Step 9: Free device list memory
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }
    
    // Additional testing: Try creating source strings with various combinations
    // to exercise different code paths in pcap_createsrcstr
    for (int i = 0; i < 2 && fdp.remaining_bytes() > 20; i++) {
        int test_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        std::string test_host = fdp.ConsumeRandomLengthString(64);
        std::string test_port = fdp.ConsumeRandomLengthString(10);
        std::string test_name = fdp.ConsumeRandomLengthString(128);
        
        char test_source[PCAP_BUF_SIZE];
        pcap_createsrcstr(test_source, 
                         test_type == 0 ? PCAP_SRC_FILE : 
                         test_type == 1 ? PCAP_SRC_IFLOCAL : PCAP_SRC_IFREMOTE,
                         test_host.empty() ? NULL : test_host.c_str(),
                         test_port.empty() ? NULL : test_port.c_str(),
                         test_name.empty() ? NULL : test_name.c_str(),
                         errbuf);
    }
    
    return 0;
}

} // namespace harness_018
