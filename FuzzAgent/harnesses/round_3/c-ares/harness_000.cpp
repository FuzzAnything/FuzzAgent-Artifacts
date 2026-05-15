#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "ares.h"
#include "ares_nameser.h"

namespace harness_000 {


static void query_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    // Do nothing - just consume the callback
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

// Callback function for ares_gethostbyname
static void host_callback(void *arg, int status, int timeouts, 
                         const struct hostent *hostent) {
    // Simple callback - does nothing, just satisfies API requirements
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)hostent;
}

// Callback function for ares_query_dnsrec
static void dnsrec_callback(void *arg, ares_status_t status, 
                           size_t timeouts, const ares_dns_record_t *dnsrec) {
    // Simple callback - does nothing, just satisfies API requirements
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic parameters
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Initialize channel - ares_init returns int, not ares_status_t
    ares_channel_t* channel = NULL;
    int init_result = ares_init(&channel);
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Consume query parameters from fuzzer input
    // 1. Consume fixed-size data first
    uint8_t dnsclass = fdp.ConsumeIntegral<uint8_t>() % 4;  // Limited to common classes
    uint16_t type = fdp.ConsumeIntegral<uint16_t>() % 65535; // DNS record type
    uint16_t query_id = fdp.ConsumeIntegral<uint16_t>();

    // 2. Consume variable-size data (query name)
    std::string name = fdp.ConsumeRandomLengthString(253); // Max DNS name length - 2
    
    // Perform DNS query using the deprecated ares_query (still widely used)
    // Note: We use the deprecated API for broader compatibility
    ares_query(channel, name.c_str(), dnsclass, type, query_callback, NULL);

    // Also test ares_query_dnsrec if we have enough remaining data
    if (fdp.remaining_bytes() > 10) {
        // Create a DNS record for the new API
        ares_dns_record_t* dnsrec = NULL;
        ares_status_t status = ares_dns_record_create(&dnsrec, 0, query_id, 
                                                     ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
        
        if (status == ARES_SUCCESS && dnsrec != NULL) {
            // Add a question to the DNS record
            status = ares_dns_record_query_add(dnsrec, name.c_str(), 
                                               static_cast<ares_dns_rec_type_t>(type),
                                               static_cast<ares_dns_class_t>(dnsclass));
            
            if (status == ARES_SUCCESS) {
                unsigned short qid = 0;
                
                // Send the DNS record query
                ares_query_dnsrec(channel, name.c_str(), 
                                 static_cast<ares_dns_class_t>(dnsclass),
                                 static_cast<ares_dns_rec_type_t>(type),
                                 dnsrec_callback, NULL, &qid);
            }
            
            ares_dns_record_destroy(dnsrec);
        }
    }

    // Test ares_gethostbyname with remaining input
    if (fdp.remaining_bytes() > 5) {
        std::string hostname = fdp.ConsumeRandomLengthString(100);
        
        ares_gethostbyname(channel, hostname.c_str(), AF_INET, host_callback, NULL);
    }

    // Process any pending events (simplified - no actual socket handling)
    // In a real fuzzer, we'd need to handle sockets, but for API coverage
    // we can at least call the processing functions
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    ares_timeout(channel, NULL, &tv);
    
    // Note: We don't have actual sockets to process, but we call the function
    // to exercise that code path
    ares_process_fd(channel, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}

} // namespace harness_000
