#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <netinet/in.h>
#include <netdb.h>
#include "ares.h"
#include "ares_nameser.h"

namespace harness_009 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for initialization, sortlist, and hostname
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Step 2: Initialize ares channel
    ares_channel_t* channel = NULL;
    int init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Step 3: Test ares_set_sortlist with various sortlist strings
    // Consume sortlist string from fuzzer input
    // Sortlist format: "addr[/mask][;addr[/mask]]*"
    // Examples: "192.168.1.0/24;10.0.0.0/8", "fe80::/10", "1.2.3.4"
    std::string sortlist_str = fdp.ConsumeRandomLengthString(256);
    
    // Call ares_set_sortlist - this function parses the sortlist string
    // and configures address sorting rules for the channel
    int sortlist_result = ares_set_sortlist(channel, sortlist_str.c_str());
    // Note: ares_set_sortlist returns 0 on success, non-zero on failure
    // We don't need to check the result for fuzzing purposes
    
    // Step 4: Test ares_gethostbyname_file with various hostnames
    // Consume hostname from fuzzer input
    std::string hostname = fdp.ConsumeRandomLengthString(253); // Max DNS name length
    
    // Consume address family (AF_INET for IPv4, AF_INET6 for IPv6, AF_UNSPEC for any)
    int address_family_vals[] = {AF_INET, AF_INET6, AF_UNSPEC};
    size_t af_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(address_family_vals)/sizeof(address_family_vals[0]) - 1);
    int address_family = address_family_vals[af_idx];
    
    // Test ares_gethostbyname_file - this function performs hostname resolution
    // from local files (like /etc/hosts) without network queries
    struct hostent* host_result = NULL;
    int file_result = ares_gethostbyname_file(channel, hostname.c_str(), address_family, &host_result);
    
    // If a hostent structure was returned, we need to free it
    if (host_result != NULL) {
        ares_free_hostent(host_result);
        host_result = NULL;
    }
    
    // Step 5: Test with additional variations including edge cases
    // Test with various special hostname patterns
    if (fdp.remaining_bytes() > 20) {
        // Consume another hostname for testing
        std::string hostname2 = fdp.ConsumeRandomLengthString(200);
        
        // Test with different address families
        int af_vals[] = {AF_INET, AF_INET6, AF_UNSPEC};
        size_t af_idx2 = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(af_vals)/sizeof(af_vals[0]) - 1);
        int af2 = af_vals[af_idx2];
        
        struct hostent* host_result2 = NULL;
        int file_result2 = ares_gethostbyname_file(channel, hostname2.c_str(), af2, &host_result2);
        
        if (host_result2 != NULL) {
            ares_free_hostent(host_result2);
        }
        
        // Test with very short hostnames (edge cases)
        if (fdp.remaining_bytes() > 5) {
            std::string short_hostname = fdp.ConsumeRandomLengthString(10);
            struct hostent* short_result = NULL;
            int short_file_result = ares_gethostbyname_file(channel, short_hostname.c_str(), AF_UNSPEC, &short_result);
            
            if (short_result != NULL) {
                ares_free_hostent(short_result);
            }
        }
    }
    
    // Step 6: Test with different sortlist configurations
    if (fdp.remaining_bytes() > 30) {
        // Consume another sortlist configuration
        std::string sortlist_str2 = fdp.ConsumeRandomLengthString(150);
        int sortlist_result2 = ares_set_sortlist(channel, sortlist_str2.c_str());
        
        // Test another hostname with the new sortlist configuration
        std::string hostname3 = fdp.ConsumeRandomLengthString(180);
        struct hostent* host_result3 = NULL;
        int file_result3 = ares_gethostbyname_file(channel, hostname3.c_str(), AF_UNSPEC, &host_result3);
        
        if (host_result3 != NULL) {
            ares_free_hostent(host_result3);
        }
        
        // Test with empty sortlist string
        int empty_sortlist_result = ares_set_sortlist(channel, "");
        
        // Test with invalid sortlist format (should still handle gracefully)
        std::string invalid_sortlist = fdp.ConsumeRandomLengthString(50);
        int invalid_sortlist_result = ares_set_sortlist(channel, invalid_sortlist.c_str());
    }
    
    // Step 7: Test error handling with NULL channel (should fail gracefully)
    // We create a separate test case but don't pass NULL to actual API
    // as it would cause issues with the valid channel we're using
    
    // Step 8: Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_009
