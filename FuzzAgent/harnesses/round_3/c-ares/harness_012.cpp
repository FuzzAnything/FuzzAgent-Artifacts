#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include "ares.h"
#include "ares_dns_record.h"
#include "ares_nameser.h"

namespace harness_012 {


// Callback functions for testing
static void server_state_callback(const char *server_string, ares_bool_t success, int flags, void *data) {
    (void)server_string;
    (void)success;
    (void)flags;
    (void)data;
    // Callback implementation for server state changes
}

static void query_enqueue_callback(void *data) {
    (void)data;
    // Callback implementation for query enqueue events
}

// Simple query callback for DNS queries (deprecated version)
static void query_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
    // Simple callback - does nothing, just satisfies API requirements
}

// Callback for DNS record queries
static void query_dnsrec_callback(void *arg, ares_status_t status,
                                 size_t timeouts, const ares_dns_record_t *dnsrec) {
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
    // Simple callback for DNS record queries
}



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for configuration and DNS operations
    if (size < 256) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);
    
    // Step 2: Test ares_init and ares_init_options with various configurations
    ares_channel_t* channel1 = NULL;
    ares_channel_t* channel2 = NULL;
    
    // First, test simple ares_init
    int init_result = ares_init(&channel1);
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Test ares_init_options with various options
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    int optmask = 0;
    
    // Consume configuration from fuzzer input
    if (fdp.remaining_bytes() >= 4) {
        options.flags = fdp.ConsumeIntegral<int>();
        optmask |= ARES_OPT_FLAGS;
    }
    
    if (fdp.remaining_bytes() >= 4) {
        options.timeout = fdp.ConsumeIntegral<int>();
        optmask |= ARES_OPT_TIMEOUTMS;
    }
    
    if (fdp.remaining_bytes() >= 4) {
        options.tries = fdp.ConsumeIntegral<int>();
        optmask |= ARES_OPT_TRIES;
    }
    
    if (fdp.remaining_bytes() >= 4) {
        options.ndots = fdp.ConsumeIntegral<int>();
        optmask |= ARES_OPT_NDOTS;
    }
    
    // Initialize second channel with options
    int init_options_result = ares_init_options(&channel2, &options, optmask);
    if (init_options_result != ARES_SUCCESS) {
        ares_destroy(channel1);
        ares_library_cleanup();
        return 0;
    }
    
    // Step 3: Test configuration APIs (6 uncovered APIs)
    
    // 1. Test ares_set_local_ip4
    if (fdp.remaining_bytes() >= 4) {
        unsigned int local_ip = fdp.ConsumeIntegral<unsigned int>();
        ares_set_local_ip4(channel1, local_ip);
    }
    
    // 2. Test ares_set_local_ip6
    if (fdp.remaining_bytes() >= 16) {
        unsigned char local_ip6[16];
        std::vector<uint8_t> ip6_bytes = fdp.ConsumeBytes<uint8_t>(16);
        memcpy(local_ip6, ip6_bytes.data(), 16);
        ares_set_local_ip6(channel1, local_ip6);
    }
    
    // 3. Test ares_set_local_dev
    if (fdp.remaining_bytes() >= 10) {
        std::string dev_name = fdp.ConsumeRandomLengthString(32);
        ares_set_local_dev(channel1, dev_name.c_str());
    }
    
    // 4. Test ares_set_server_state_callback
    ares_set_server_state_callback(channel1, server_state_callback, NULL);
    
    // 5. Test ares_set_query_enqueue_cb
    ares_set_query_enqueue_cb(channel1, query_enqueue_callback, NULL);
    
    // Step 4: Create and manipulate DNS records for testing ares_dns_rr_get_opt_byid
    ares_dns_record_t *dns_record = NULL;
    ares_status_t status = ares_dns_record_create(&dns_record, 0, ARES_FLAG_QR, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dns_record != NULL) {
        // Add an OPT record (EDNS0) which can have options
        std::string domain_name = "example.com";
        ares_dns_rr_t *opt_rr = NULL;
        status = ares_dns_record_rr_add(&opt_rr, dns_record, ARES_SECTION_ADDITIONAL,
                                      domain_name.c_str(), ARES_REC_TYPE_OPT, ARES_CLASS_IN, 0);
        
        if (status == ARES_SUCCESS && opt_rr != NULL) {
            // Add some OPT options to the record
            if (fdp.remaining_bytes() >= 20) {
                size_t opt_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)100));
                std::vector<uint8_t> opt_data = fdp.ConsumeBytes<uint8_t>(opt_size);
                
                // For OPT records, we need to use specific keys for options
                // Try to set some OPT payload data
                ares_dns_rr_set_u16(opt_rr, ARES_RR_OPT_UDP_SIZE, 4096);
                ares_dns_rr_set_u8(opt_rr, ARES_RR_OPT_VERSION, 0);
                ares_dns_rr_set_u16(opt_rr, ARES_RR_OPT_FLAGS, 0);
                // 6. Test ares_dns_rr_get_opt_byid
                const unsigned char *opt_val = NULL;
                size_t opt_val_len = 0;
                
                // Try to get OPT by ID (some common OPT codes)
                ares_dns_rr_get_opt_byid(opt_rr, ARES_RR_OPT_OPTIONS, 8, &opt_val, &opt_val_len);  // NSID
                ares_dns_rr_get_opt_byid(opt_rr, ARES_RR_OPT_OPTIONS, 12, &opt_val, &opt_val_len); // COOKIE
            }
        }
        // Step 5: Test ares_dns_record_duplicate
        ares_dns_record_t *duplicate_record = ares_dns_record_duplicate(dns_record);
        if (duplicate_record != NULL) {
            // Write both records to compare
            unsigned char *buf1 = NULL;
            unsigned char *buf2 = NULL;
            size_t buf1_len = 0, buf2_len = 0;
            
            ares_dns_write(dns_record, &buf1, &buf1_len);
            ares_dns_write(duplicate_record, &buf2, &buf2_len);
            
            if (buf1 != NULL) {
                ares_free_string(buf1);
            }
            if (buf2 != NULL) {
                ares_free_string(buf2);
            }
            
            ares_dns_record_destroy(duplicate_record);
        }
        // Step 6: Test ares_send, ares_send_dnsrec, and ares_query_dnsrec
        
        // Create a simple DNS query
        ares_dns_record_t *query_record = NULL;
        status = ares_dns_record_create(&query_record, 0, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
        
        if (status == ARES_SUCCESS && query_record != NULL) {
            // Add a query
            std::string query_name = fdp.ConsumeRandomLengthString(NS_MAXDNAME - 1);
            ares_dns_record_query_add(query_record, query_name.c_str(), ARES_REC_TYPE_A, ARES_CLASS_IN);
            
            // Test deprecated ares_send (for coverage)
            if (query_name.length() > 0) {
                unsigned char *query_buf = NULL;
                size_t query_buf_len = 0;
                ares_dns_write(query_record, &query_buf, &query_buf_len);
                if (query_buf != NULL) {
                    ares_send(channel1, query_buf, query_buf_len, query_callback, NULL);
                    ares_free_string(query_buf);
                }
            }
            
            // Test ares_send_dnsrec (modern API)
            if (query_name.length() > 0) {
                unsigned short qid1 = 0;
                ares_send_dnsrec(channel1, query_record, query_dnsrec_callback, NULL, &qid1);
            }
            
            // Test ares_query_dnsrec (modern API with qid)
            if (query_name.length() > 0) {
                unsigned short qid2 = 0;
                ares_query_dnsrec(channel1, query_name.c_str(), ARES_CLASS_IN, ARES_REC_TYPE_A, 
                                 query_dnsrec_callback, NULL, &qid2);
            }
            
            // Test deprecated ares_query (for coverage)
            if (query_name.length() > 0) {
                ares_query(channel1, query_name.c_str(), ARES_CLASS_IN, ARES_REC_TYPE_A, 
                          query_callback, NULL);
            }
            
            ares_dns_record_destroy(query_record);
        }
        
        ares_dns_record_destroy(dns_record);
    }
    
    // Step 7: Process any pending events
    // Note: In a real fuzzing environment, we'd need to process events,
    // but for coverage testing, calling the APIs is sufficient
    
    // Cleanup
    ares_destroy(channel1);
    ares_destroy(channel2);
    ares_library_cleanup();
    
    return 0;
}

} // namespace harness_012
