#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <arpa/inet.h>
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"
#include "ares_nameser.h"

namespace harness_004 {


// Callback for deprecated ares_search
static void search_callback(void *arg, int status, int timeouts, 
                           const unsigned char *abuf, int alen) {
    // Do nothing - just consume the callback
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

// Callback for modern ares_search_dnsrec
static void search_dnsrec_callback(void *arg, ares_status_t status, 
                                  size_t timeouts, const ares_dns_record_t *dnsrec) {
    // Do nothing - just consume the callback
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

// Callback for ares_gethostbyaddr
static void hostbyaddr_callback(void *arg, int status, int timeouts, 
                               const struct hostent *hostent) {
    // Do nothing - just consume the callback
    // Note: c-ares automatically frees the hostent structure after callback returns
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
}

// Fuzzer entry point for DNS search and query creation operations
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful testing
    // We need enough for: domain name, DNS class/type, IP address, query ID, etc.
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Global library initialization
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // 2. Channel initialization
    ares_channel_t *channel = NULL;
    int init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 3. Test ares_create_query - create DNS query buffer
    if (fdp.remaining_bytes() > 32) {
        // Consume parameters for query creation
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string query_name = fdp.ConsumeBytesAsString(name_len);
        
        int dnsclass = fdp.ConsumeIntegralInRange<int>(C_IN, C_HS);
        int type = fdp.ConsumeIntegralInRange<int>(T_A, T_URI);
        unsigned short id = fdp.ConsumeIntegral<unsigned short>();
        int rd = fdp.ConsumeBool() ? 1 : 0;  // Recursion desired flag
        
        // Determine max UDP size (0 for no EDNS, or positive value)
        int max_udp_size = 0;
        if (fdp.ConsumeBool()) {
            max_udp_size = fdp.ConsumeIntegralInRange<int>(512, 4096);
        }
        
        unsigned char *buf = NULL;
        int buflen = 0;
        
        // Test ares_create_query (with max_udp_size parameter)
        int create_result = ares_create_query(query_name.c_str(), dnsclass, type, 
                                             id, rd, &buf, &buflen, max_udp_size);
        (void)create_result; // Result can be ignored for fuzzing
        
        // Free the buffer if allocated
        if (buf != NULL) {
            ares_free_string(buf);
        }
        
        // Also test ares_mkquery (legacy version without max_udp_size)
        if (fdp.remaining_bytes() > 16) {
            unsigned char *buf2 = NULL;
            int buflen2 = 0;
            
            int mkquery_result = ares_mkquery(query_name.c_str(), dnsclass, type,
                                             id, rd, &buf2, &buflen2);
            (void)mkquery_result; // Result can be ignored for fuzzing
            
            if (buf2 != NULL) {
                ares_free_string(buf2);
            }
        }
    }
    
    // 4. Test ares_search (deprecated) - DNS search with various record types
    if (fdp.remaining_bytes() > 24) {
        size_t search_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::string search_name = fdp.ConsumeBytesAsString(search_name_len);
        
        // Try different DNS record types
        int search_dnsclass = fdp.ConsumeIntegralInRange<int>(C_IN, C_HS);
        int search_type = fdp.ConsumeIntegralInRange<int>(T_A, T_URI);
        
        // Call ares_search (deprecated but still in API)
        ares_search(channel, search_name.c_str(), search_dnsclass, search_type,
                   search_callback, NULL);
    }
    
    // 5. Test ares_search_dnsrec (modern) - DNS search with DNS record object
    if (fdp.remaining_bytes() > 48) {
        // First create a DNS record for the search
        ares_dns_record_t *dnsrec = NULL;
        ares_status_t status;
        
        // Create a basic DNS query record
        unsigned short dns_id = fdp.ConsumeIntegral<unsigned short>();
        unsigned short dns_flags = fdp.ConsumeIntegral<unsigned short>();
        ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
        ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
        
        status = ares_dns_record_create(&dnsrec, dns_id, dns_flags, opcode, rcode);
        if (status == ARES_SUCCESS && dnsrec != NULL) {
            // Add a query to the record
            size_t dns_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 128);
            std::string dns_name = fdp.ConsumeBytesAsString(dns_name_len);
            
            ares_dns_rec_type_t qtype = static_cast<ares_dns_rec_type_t>(
                fdp.ConsumeIntegralInRange<int>(ARES_REC_TYPE_A, ARES_REC_TYPE_RAW_RR));
            ares_dns_class_t qclass = static_cast<ares_dns_class_t>(
                fdp.ConsumeIntegralInRange<int>(ARES_CLASS_IN, ARES_CLASS_ANY));
            
            // Try to add query (may fail if name is invalid, but that's OK)
            ares_dns_record_query_add(dnsrec, dns_name.c_str(), qtype, qclass);
            
            // Now use this DNS record with ares_search_dnsrec
            ares_search_dnsrec(channel, dnsrec, search_dnsrec_callback, NULL);
            
            // Clean up the DNS record
            ares_dns_record_destroy(dnsrec);
        }
    }
    
    // 6. Test ares_gethostbyaddr - reverse DNS lookup
    if (fdp.remaining_bytes() > 20) {
        // Determine address family (IPv4 or IPv6)
        int addr_family = fdp.ConsumeBool() ? AF_INET : AF_INET6;
        size_t addr_len = (addr_family == AF_INET) ? 4 : 16;
        
        if (fdp.remaining_bytes() >= addr_len) {
            // Consume address bytes
            std::vector<uint8_t> addr_bytes = fdp.ConsumeBytes<uint8_t>(addr_len);
            
            // Call ares_gethostbyaddr
            ares_gethostbyaddr(channel, addr_bytes.data(), addr_len, addr_family,
                              hostbyaddr_callback, NULL);
        }
    }
    
    // 7. Cleanup
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_004
