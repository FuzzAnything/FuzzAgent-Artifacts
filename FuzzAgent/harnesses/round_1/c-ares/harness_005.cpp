#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include "ares.h"
#include "ares_dns.h"
#include "ares_nameser.h"

namespace harness_005 {

// Callback functions for async DNS operations
static void query_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    // Do nothing - just consume the callback
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

static void host_callback(void *arg, int status, int timeouts, 
                         const struct hostent *hostent) {
    // Do nothing - just consume the callback
    // Note: c-ares automatically frees the hostent structure after callback returns
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
}

// Fuzzer entry point for channel lifecycle and reinitialization operations
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful testing
    // We need enough for: options, domain names, IP addresses, etc.
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Global library initialization
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // 2. Create ares_options struct with random values
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    
    // Set random options from fuzzed data
    options.flags = fdp.ConsumeIntegral<int>();
    options.timeout = fdp.ConsumeIntegralInRange<int>(1, 30); // 1-30 seconds
    options.tries = fdp.ConsumeIntegralInRange<int>(1, 5);
    options.ndots = fdp.ConsumeIntegralInRange<int>(0, 15);
    options.udp_port = fdp.ConsumeIntegral<unsigned short>();
    options.tcp_port = fdp.ConsumeIntegral<unsigned short>();
    
    // Calculate optmask based on which options we're setting
    int optmask = 0;
    if (fdp.ConsumeBool()) optmask |= ARES_OPT_FLAGS;
    if (fdp.ConsumeBool()) optmask |= ARES_OPT_TIMEOUT;
    if (fdp.ConsumeBool()) optmask |= ARES_OPT_TRIES;
    if (fdp.ConsumeBool()) optmask |= ARES_OPT_NDOTS;
    if (fdp.ConsumeBool()) optmask |= ARES_OPT_UDP_PORT;
    if (fdp.ConsumeBool()) optmask |= ARES_OPT_TCP_PORT;
    
    // 3. Initialize channel with options
    ares_channel_t *channel = NULL;
    int init_result = ares_init_options(&channel, &options, optmask);
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 4. Perform some DNS operations to get active state
    if (fdp.remaining_bytes() > 8) {
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string domain_name = fdp.ConsumeBytesAsString(name_len);
        int dnsclass = fdp.ConsumeIntegralInRange<int>(C_IN, C_HS);
        int type = fdp.ConsumeIntegralInRange<int>(T_A, T_URI);
        
        ares_query(channel, domain_name.c_str(), dnsclass, type, 
                  query_callback, NULL);
    }
    
    // Also test host lookup
    if (fdp.remaining_bytes() > 4) {
        size_t hostname_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::string hostname = fdp.ConsumeBytesAsString(hostname_len);
        int family = fdp.ConsumeIntegralInRange<int>(AF_INET, AF_INET6);
        
        ares_gethostbyname(channel, hostname.c_str(), family, 
                          host_callback, NULL);
    }
    
    // 5. Duplicate the channel using ares_dup
    ares_channel_t *dup_channel = NULL;
    int dup_result = ares_dup(&dup_channel, channel);
    // Continue even if duplication fails - some configurations might not support it
    
    // 6. Modify channel configuration with various setter functions
    
    // Set sortlist if we have enough data
    if (fdp.remaining_bytes() > 4 && dup_result == ARES_SUCCESS && dup_channel != NULL) {
        size_t sortstr_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string sortstr = fdp.ConsumeBytesAsString(sortstr_len);
        
        ares_set_sortlist(dup_channel, sortstr.c_str());
    }
    
    // Set local IPv4 address
    if (fdp.remaining_bytes() >= 4 && dup_result == ARES_SUCCESS && dup_channel != NULL) {
        unsigned int local_ip4 = fdp.ConsumeIntegral<unsigned int>();
        ares_set_local_ip4(dup_channel, local_ip4);
    }
    
    // Set local IPv6 address (16 bytes)
    if (fdp.remaining_bytes() >= 16 && dup_result == ARES_SUCCESS && dup_channel != NULL) {
        std::vector<unsigned char> local_ip6 = fdp.ConsumeBytes<unsigned char>(16);
        ares_set_local_ip6(dup_channel, local_ip6.data());
    }
    
    // Set local device name
    if (fdp.remaining_bytes() > 4 && dup_result == ARES_SUCCESS && dup_channel != NULL) {
        size_t devname_len = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::string devname = fdp.ConsumeBytesAsString(devname_len);
        
        ares_set_local_dev(dup_channel, devname.c_str());
    }
    
    // 7. Reinitialize the original channel with ares_reinit
    ares_status_t reinit_status = ares_reinit(channel);
    // Continue regardless of reinit status - it might fail on some configurations
    
    // 8. Start new queries on both original and duplicated channels
    
    // On original channel
    if (fdp.remaining_bytes() > 8) {
        size_t name1_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string name1 = fdp.ConsumeBytesAsString(name1_len);
        int dnsclass1 = fdp.ConsumeIntegralInRange<int>(C_IN, C_HS);
        int type1 = fdp.ConsumeIntegralInRange<int>(T_A, T_URI);
        
        ares_query(channel, name1.c_str(), dnsclass1, type1, 
                  query_callback, NULL);
    }
    
    // On duplicated channel (if it exists)
    if (dup_result == ARES_SUCCESS && dup_channel != NULL && fdp.remaining_bytes() > 8) {
        size_t name2_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string name2 = fdp.ConsumeBytesAsString(name2_len);
        int dnsclass2 = fdp.ConsumeIntegralInRange<int>(C_IN, C_HS);
        int type2 = fdp.ConsumeIntegralInRange<int>(T_A, T_URI);
        
        ares_query(dup_channel, name2.c_str(), dnsclass2, type2, 
                  query_callback, NULL);
    }
    
    // 9. Test cancellation with ares_cancel
    // Cancel queries on original channel
    ares_cancel(channel);
    
    // Cancel queries on duplicated channel (if it exists)
    if (dup_result == ARES_SUCCESS && dup_channel != NULL) {
        ares_cancel(dup_channel);
    }
    
    // 10. Cleanup - destroy both channels
    ares_destroy(channel);
    if (dup_result == ARES_SUCCESS && dup_channel != NULL) {
        ares_destroy(dup_channel);
    }
    
    // Global cleanup
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_005
