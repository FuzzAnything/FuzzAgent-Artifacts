#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include "ares.h"
#include "ares_dns.h"
#include "ares_nameser.h"

namespace harness_005 {
/* Fuzzing harness for c-ares library targeting legacy DNS reply parsing functions
 * This harness tests comprehensive DNS reply parsing across multiple record types
 * Target APIs: ares_parse_caa_reply, ares_parse_ns_reply, ares_parse_naptr_reply,
 *              ares_parse_aaaa_reply, ares_parse_soa_reply, ares_parse_uri_reply,
 *              ares_parse_mx_reply, ares_parse_srv_reply, ares_parse_ptr_reply,
 *              ares_parse_txt_reply_ext (all at 0% coverage)
 * Required helper APIs: ares_parse_a_reply, ares_parse_txt_reply (partially covered),
 *              ares_init/ares_init_options (initialization), ares_query/ares_create_query (query creation),
 *              ares_search (alternative query), ares_destroy (cleanup), ares_free_data (memory cleanup)
 * Follows invocation sequence: 1) initialize library, 2) create DNS queries,
 * 3) generate synthetic DNS response data, 4) call target parse functions,
 * 5) handle parsed output structures, 6) cleanup with ares_free_data, 7) ares_destroy
 * Differentiates from previous harnesses by focusing on legacy parse functions
 */



// Callback for deprecated ares_query and ares_search
static void query_callback(void* arg, int status, int timeouts, 
                          const unsigned char* abuf, int alen) {
    // Callback doesn't need to do anything for fuzzing - just consume the result
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // We need enough data for: initialization + DNS response data for multiple parse functions
    if (size < 100) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // 1. Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);

    // 2. Initialize channel with options
    ares_channel_t* channel = NULL;
    ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    
    // Consume fuzzer data for initialization parameters
    int timeout = fdp.ConsumeIntegralInRange<int>(1, 10000);
    int tries = fdp.ConsumeIntegralInRange<int>(1, 10);
    int ndots = fdp.ConsumeIntegralInRange<int>(0, 15);
    bool tcp = fdp.ConsumeBool();
    bool edns = fdp.ConsumeBool();
    
    options.timeout = timeout;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    options.tries = tries;
    optmask |= ARES_OPT_TRIES;
    
    options.ndots = ndots;
    optmask |= ARES_OPT_NDOTS;
    
    if (tcp) {
        options.flags |= ARES_FLAG_USEVC;
        optmask |= ARES_OPT_FLAGS;
    }
    
    if (edns) {
        options.flags |= ARES_FLAG_EDNS;
        optmask |= ARES_OPT_FLAGS;
        options.ednspsz = fdp.ConsumeIntegralInRange<int>(0, 4096);
        optmask |= ARES_OPT_EDNSPSZ;
    }
    
    int status = ares_init_options(&channel, &options, optmask);
    if (status != ARES_SUCCESS) {
        // Try deprecated ares_init as fallback
        status = ares_init(&channel);
        if (status != ARES_SUCCESS) {
            ares_library_cleanup();
            return 0;
        }
    }
    // 3. Generate synthetic DNS response data from fuzzer input
    // We'll use the fuzzer input as DNS response data for parsing
    // For ptr_reply, we need address data
    unsigned char addrv4[4];
    unsigned char addrv6[16];
    for (int i = 0; i < 4; i++) {
        if (fdp.remaining_bytes() > 0) {
            addrv4[i] = fdp.ConsumeIntegral<uint8_t>();
        } else {
            addrv4[i] = 0;
        }
    }
    for (int i = 0; i < 16; i++) {
        if (fdp.remaining_bytes() > 0) {
            addrv6[i] = fdp.ConsumeIntegral<uint8_t>();
        } else {
            addrv6[i] = 0;
        }
    }
    
    // Get remaining data for DNS response buffer
    size_t dns_response_size = fdp.ConsumeIntegralInRange<size_t>(12, fdp.remaining_bytes());
    if (dns_response_size < 12) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    
    std::vector<unsigned char> dns_response = fdp.ConsumeBytes<unsigned char>(dns_response_size);
    const unsigned char* abuf = dns_response.data();
    int alen = (int)dns_response.size();

    // 4. Call target parse functions with appropriate response buffers
    
    // a) Parse A reply (helper API)
    struct hostent* host_a = NULL;
    struct ares_addrttl info_a[5];
    int count_a = 5;
    ares_parse_a_reply(abuf, alen, &host_a, info_a, &count_a);
    if (host_a != NULL) {
        ares_free_hostent(host_a);
    }
    
    // b) Parse AAAA reply
    struct hostent* host_aaaa = NULL;
    struct ares_addr6ttl info_aaaa[5];
    int count_aaaa = 5;
    ares_parse_aaaa_reply(abuf, alen, &host_aaaa, info_aaaa, &count_aaaa);
    if (host_aaaa != NULL) {
        ares_free_hostent(host_aaaa);
    }
    
    // c) Parse CAA reply
    struct ares_caa_reply* caa = NULL;
    ares_parse_caa_reply(abuf, alen, &caa);
    if (caa != NULL) {
        ares_free_data(caa);
    }
    
    // d) Parse PTR reply (needs address parameter)
    struct hostent* host_ptr = NULL;
    // Try IPv4 address
    ares_parse_ptr_reply(abuf, alen, addrv4, sizeof(addrv4), AF_INET, &host_ptr);
    if (host_ptr != NULL) {
        ares_free_hostent(host_ptr);
    }
    
    // Try IPv6 address if we have enough data
    if (fdp.remaining_bytes() >= 16) {
        host_ptr = NULL;
        ares_parse_ptr_reply(abuf, alen, addrv6, sizeof(addrv6), AF_INET6, &host_ptr);
        if (host_ptr != NULL) {
            ares_free_hostent(host_ptr);
        }
    }
    
    // e) Parse NS reply
    struct hostent* host_ns = NULL;
    ares_parse_ns_reply(abuf, alen, &host_ns);
    if (host_ns != NULL) {
        ares_free_hostent(host_ns);
    }
    
    // f) Parse SRV reply
    struct ares_srv_reply* srv = NULL;
    ares_parse_srv_reply(abuf, alen, &srv);
    if (srv != NULL) {
        ares_free_data(srv);
    }
    
    // g) Parse MX reply
    struct ares_mx_reply* mx = NULL;
    ares_parse_mx_reply(abuf, alen, &mx);
    if (mx != NULL) {
        ares_free_data(mx);
    }
    
    // h) Parse TXT reply (helper API)
    struct ares_txt_reply* txt = NULL;
    ares_parse_txt_reply(abuf, alen, &txt);
    if (txt != NULL) {
        ares_free_data(txt);
    }
    
    // i) Parse TXT reply extended
    struct ares_txt_ext* txt_ext = NULL;
    ares_parse_txt_reply_ext(abuf, alen, &txt_ext);
    if (txt_ext != NULL) {
        ares_free_data(txt_ext);
    }
    
    // j) Parse NAPTR reply
    struct ares_naptr_reply* naptr = NULL;
    ares_parse_naptr_reply(abuf, alen, &naptr);
    if (naptr != NULL) {
        ares_free_data(naptr);
    }
    
    // k) Parse SOA reply
    struct ares_soa_reply* soa = NULL;
    ares_parse_soa_reply(abuf, alen, &soa);
    if (soa != NULL) {
        ares_free_data(soa);
    }
    
    // l) Parse URI reply
    struct ares_uri_reply* uri = NULL;
    ares_parse_uri_reply(abuf, alen, &uri);
    if (uri != NULL) {
        ares_free_data(uri);
    }

    // 5. Test query creation APIs (as per guidance step 2)
    if (fdp.remaining_bytes() > 50) {
        std::string query_name = fdp.ConsumeRandomLengthString(64);
        
        // Try creating a query using ares_create_query (deprecated but specified in guidance)
        unsigned char* query_buf = NULL;
        int query_buflen = 0;
        unsigned short query_id = fdp.ConsumeIntegral<unsigned short>();
        int rd_flag = fdp.ConsumeBool() ? 1 : 0;
        int max_udp_size = fdp.ConsumeIntegralInRange<int>(512, 4096);
        
        ares_create_query(query_name.c_str(), ARES_CLASS_IN, ARES_REC_TYPE_A,
                         query_id, rd_flag, &query_buf, &query_buflen, max_udp_size);
        
        if (query_buf != NULL) {
            ares_free_string(query_buf);
        }
        
        // Try creating a query using ares_query_dnsrec (modern API)
        unsigned short qid = 0;
        ares_query_dnsrec(channel, query_name.c_str(), ARES_CLASS_IN, 
                         ARES_REC_TYPE_A, NULL, NULL, &qid);
        
        // Try search API
        ares_search_dnsrec(channel, NULL, NULL, NULL);
    }

    // 6. Cleanup with ares_destroy
    ares_destroy(channel);
    
    // 7. Final library cleanup
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_005
