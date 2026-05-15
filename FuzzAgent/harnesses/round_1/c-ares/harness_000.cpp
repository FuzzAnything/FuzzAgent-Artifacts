#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ares.h"
#include "ares_dns.h"
#include "ares_nameser.h"

namespace harness_000 {


// Callback functions for async operations
static void dummy_callback(void *arg, int status, int timeouts, const unsigned char *abuf, int alen) {
    // Do nothing - just consume the callback
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

static void dummy_host_callback(void *arg, int status, int timeouts, const struct hostent *hostent) {
    // Do nothing - just consume the callback
    (void)arg;
    (void)status;
    (void)timeouts;
    if (hostent) {
        ares_free_hostent((struct hostent *)hostent);
    }
}

static void dummy_nameinfo_callback(void *arg, int status, int timeouts, const char *node, const char *service) {
    // Do nothing - just consume the callback
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)node;
    (void)service;
}

// Fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful testing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Create channel
    ares_channel_t *channel = NULL;
    int init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Consume data for various API parameters
    
    // 1. Test ares_gethostbyname (deprecated but still in API)
    if (fdp.remaining_bytes() > 4) {
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::string domain_name = fdp.ConsumeBytesAsString(name_len);
        int family = fdp.ConsumeIntegralInRange<int>(AF_INET, AF_INET6);
        
        ares_gethostbyname(channel, domain_name.c_str(), family, dummy_host_callback, NULL);
    }
    
    // 2. Test ares_query with various DNS types
    if (fdp.remaining_bytes() > 8) {
        size_t query_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string query_name = fdp.ConsumeBytesAsString(query_name_len);
        
        // Use fuzzed data for DNS class and type
        int dnsclass = fdp.ConsumeIntegralInRange<int>(C_IN, C_HS);
        int type = fdp.ConsumeIntegralInRange<int>(T_A, T_URI);
        
        ares_query(channel, query_name.c_str(), dnsclass, type, dummy_callback, NULL);
    }
    
    // 3. Test ares_getnameinfo
    if (fdp.remaining_bytes() > sizeof(struct sockaddr_in)) {
        // Create a sockaddr structure with fuzzed data
        struct sockaddr_in sa;
        memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        
        // Use fuzzed bytes for IP address
        if (fdp.remaining_bytes() >= 4) {
            std::vector<uint8_t> ip_bytes = fdp.ConsumeBytes<uint8_t>(4);
            memcpy(&sa.sin_addr, ip_bytes.data(), 4);
        }
        
        int flags = fdp.ConsumeIntegral<int>();
        ares_getnameinfo(channel, (struct sockaddr*)&sa, sizeof(sa), flags, 
                        dummy_nameinfo_callback, NULL);
    }
    
    // 4. Test parsing functions with remaining data as DNS response
    if (fdp.remaining_bytes() > 12) {
        std::vector<uint8_t> dns_response = fdp.ConsumeRemainingBytes<uint8_t>();
        const unsigned char *abuf = dns_response.data();
        int alen = (int)dns_response.size();
        
        // Test various parse functions
        struct hostent *host = NULL;
        struct ares_addrttl info[5];
        struct ares_addr6ttl info6[5];
        int count = 5;
        
        // a) Parse A record reply
        ares_parse_a_reply(abuf, alen, &host, info, &count);
        if (host) {
            ares_free_hostent(host);
            host = NULL;
        }
        
        // b) Parse AAAA record reply
        count = 5;
        ares_parse_aaaa_reply(abuf, alen, &host, info6, &count);
        if (host) {
            ares_free_hostent(host);
            host = NULL;
        }
        
        // c) Parse PTR reply with a dummy address
        unsigned char addrv4[4] = {0x08, 0x08, 0x08, 0x08}; // 8.8.8.8
        ares_parse_ptr_reply(abuf, alen, addrv4, sizeof(addrv4), AF_INET, &host);
        if (host) {
            ares_free_hostent(host);
            host = NULL;
        }
        
        // d) Parse NS reply
        ares_parse_ns_reply(abuf, alen, &host);
        if (host) {
            ares_free_hostent(host);
            host = NULL;
        }
        
        // e) Parse other reply types (just call them, ignore results)
        struct ares_srv_reply *srv = NULL;
        ares_parse_srv_reply(abuf, alen, &srv);
        if (srv) {
            ares_free_data(srv);
        }
        
        struct ares_mx_reply *mx = NULL;
        ares_parse_mx_reply(abuf, alen, &mx);
        if (mx) {
            ares_free_data(mx);
        }
        
        struct ares_txt_reply *txt = NULL;
        ares_parse_txt_reply(abuf, alen, &txt);
        if (txt) {
            ares_free_data(txt);
        }
        
        struct ares_soa_reply *soa = NULL;
        ares_parse_soa_reply(abuf, alen, &soa);
        if (soa) {
            ares_free_data(soa);
        }
    }
    
    // 5. Test server CSV setting (like in existing harness)
    if (fdp.remaining_bytes() > 4) {
        size_t csv_len = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        std::string csv_data = fdp.ConsumeBytesAsString(csv_len);
        ares_set_servers_csv(channel, csv_data.c_str());
    }
    
    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_000
