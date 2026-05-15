#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ares.h"

namespace harness_001 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 4) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Parse the fuzzer input as DNS reply data
    // The input will be used directly as DNS response data
    
    // Test various DNS reply parsers with the same input data
    // This follows the pattern from ares-test-fuzz.c
    
    struct hostent *host = NULL;
    struct ares_addrttl info[5];
    struct ares_addr6ttl info6[5];
    unsigned char addrv4[4] = { 0x10, 0x20, 0x30, 0x40 };
    struct ares_srv_reply *srv = NULL;
    struct ares_mx_reply *mx = NULL;
    struct ares_txt_reply *txt = NULL;
    struct ares_soa_reply *soa = NULL;
    struct ares_naptr_reply *naptr = NULL;
    struct ares_caa_reply *caa = NULL;
    struct ares_uri_reply *uri = NULL;
    int count = 5;

    // Convert the fuzzer input to const unsigned char* for the parsers
    auto dns_bytes = fdp.ConsumeRemainingBytes<unsigned char>();
    const unsigned char* dns_data = dns_bytes.data();
    int dns_size = static_cast<int>(dns_bytes.size());
    // Test A record parser
    ares_parse_a_reply(dns_data, dns_size, &host, info, &count);
    if (host) {
        ares_free_hostent(host);
        host = NULL;
    }

    // Test AAAA record parser
    count = 5;
    ares_parse_aaaa_reply(dns_data, dns_size, &host, info6, &count);
    if (host) {
        ares_free_hostent(host);
        host = NULL;
    }

    // Test PTR record parser
    ares_parse_ptr_reply(dns_data, dns_size, addrv4, sizeof(addrv4), AF_INET, &host);
    if (host) {
        ares_free_hostent(host);
        host = NULL;
    }

    // Test NS record parser
    ares_parse_ns_reply(dns_data, dns_size, &host);
    if (host) {
        ares_free_hostent(host);
        host = NULL;
    }

    // Test SRV record parser
    ares_parse_srv_reply(dns_data, dns_size, &srv);
    if (srv) {
        ares_free_data(srv);
    }

    // Test MX record parser
    ares_parse_mx_reply(dns_data, dns_size, &mx);
    if (mx) {
        ares_free_data(mx);
    }

    // Test TXT record parser
    ares_parse_txt_reply(dns_data, dns_size, &txt);
    if (txt) {
        ares_free_data(txt);
    }

    // Test SOA record parser
    ares_parse_soa_reply(dns_data, dns_size, &soa);
    if (soa) {
        ares_free_data(soa);
    }

    // Test NAPTR record parser
    ares_parse_naptr_reply(dns_data, dns_size, &naptr);
    if (naptr) {
        ares_free_data(naptr);
    }

    // Test CAA record parser
    ares_parse_caa_reply(dns_data, dns_size, &caa);
    if (caa) {
        ares_free_data(caa);
    }

    // Test URI record parser
    ares_parse_uri_reply(dns_data, dns_size, &uri);
    if (uri) {
        ares_free_data(uri);
    }

    // Also test the newer DNS record parser
    // We need to recreate the FDP since we consumed all data
    FuzzedDataProvider fdp2(data, size); // Recreate with original data
    
    // Try to parse as a DNS record using the new API
    ares_dns_record_t* dnsrec = NULL;
    auto dns_bytes2 = fdp2.ConsumeRemainingBytes<unsigned char>();
    ares_status_t status = ares_dns_parse(dns_bytes2.data(),
                                         static_cast<int>(dns_bytes2.size()), 0, &dnsrec);
    if (status == ARES_SUCCESS && dnsrec != NULL) {
        // Exercise some getter functions on the parsed record
        ares_dns_record_get_id(dnsrec);
        ares_dns_record_get_opcode(dnsrec);
        ares_dns_record_get_rcode(dnsrec);
        ares_dns_record_get_flags(dnsrec);
        ares_dns_record_query_cnt(dnsrec);
        
        // Clean up
        ares_dns_record_destroy(dnsrec);
    }

    // Clean up library
    ares_library_cleanup();

    return 0;
}

} // namespace harness_001
