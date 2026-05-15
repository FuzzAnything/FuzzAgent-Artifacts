#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include "ares.h"
#include "ares_dns.h"
#include "ares_dns_record.h"
#include "ares_nameser.h"

namespace harness_006 {
/* Fuzzing harness for c-ares library targeting channel cancellation and DNS mapping operations
 * This harness tests query cancellation functionality and DNS type/error mapping functions
 * Target APIs: ares_cancel (187 undiscovered branches), ares_dns_rr_key_tostr (118),
 *              ares_dns_rcode_tostr (42), ares_strerror (56)
 * Related mapping functions: ares_dns_section_tostr, ares_dns_rec_type_fromstr,
 *              ares_dns_class_fromstr, ares_dns_opt_get_datatype, ares_dns_opt_get_name
 * Required helper APIs: ares_library_init/ares_library_init_mem (library initialization),
 *              ares_init_options (create channel), ares_query/ares_create_query (DNS queries),
 *              ares_send (optional), ares_destroy (cleanup), ares_library_cleanup,
 *              ares_free_string
 * Follows invocation sequence: 1) ares_library_init, 2) ares_init_options to create channel,
 *              3) ares_query to create one or more DNS queries, 4) ares_cancel to cancel pending queries,
 *              5) ares_dns_rr_key_tostr, ares_dns_rcode_tostr, ares_dns_section_tostr,
 *              ares_strerror for mapping/error testing, 6) ares_destroy, ares_library_cleanup,
 *              ares_free_string cleanup
 * Differentiates from previous harnesses by focusing on query cancellation and DNS mapping
 */



// Callback for DNS queries that will be cancelled
static void query_callback(void* arg, int status, int timeouts, 
                          const unsigned char* abuf, int alen) {
    // This callback will be invoked with ARES_ECANCELLED when ares_cancel() is called
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for meaningful fuzzing
    // We need enough data for: initialization + query parameters + mapping values
    if (size < 50) {
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

    // 3. Create DNS queries that will be cancelled
    // We'll create multiple queries to test cancellation of all pending queries
    int num_queries = fdp.ConsumeIntegralInRange<int>(1, 5);
    void* query_args[5] = { NULL };
    
    for (int i = 0; i < num_queries && fdp.remaining_bytes() > 10; i++) {
        std::string query_name = fdp.ConsumeRandomLengthString(64);
        
        // Create query with callback - these will be cancelled
        // Note: ares_query returns void, not int (deprecated API)
        ares_query(channel, query_name.c_str(), ARES_CLASS_IN, 
                   ARES_REC_TYPE_A, query_callback, query_args[i]);
        
        // Try different query types for variety
        if (fdp.remaining_bytes() > 5) {
            std::string alt_name = fdp.ConsumeRandomLengthString(32);
            ares_query(channel, alt_name.c_str(), ARES_CLASS_IN,
                      ARES_REC_TYPE_AAAA, query_callback, NULL);
        }
    }

    // 4. Cancel all pending queries
    // This is the main target API with 187 undiscovered branches
    ares_cancel(channel);

    // 5. Test DNS mapping functions
    // a) Test ares_dns_rr_key_tostr with various key values
    // First, get valid RR keys for testing
    size_t key_count = 0;
    const ares_dns_rr_key_t* keys = ares_dns_rr_get_keys(ARES_REC_TYPE_A, &key_count);
    if (keys != NULL && key_count > 0) {
        for (size_t i = 0; i < key_count && i < 5; i++) {
            const char* key_str = ares_dns_rr_key_tostr(keys[i]);
            (void)key_str; // Consume result
        }
    }
    
    // Test with other record types
    keys = ares_dns_rr_get_keys(ARES_REC_TYPE_TXT, &key_count);
    if (keys != NULL && key_count > 0) {
        for (size_t i = 0; i < key_count && i < 3; i++) {
            const char* key_str = ares_dns_rr_key_tostr(keys[i]);
            (void)key_str;
        }
    }

    // b) Test ares_dns_rcode_tostr with various response codes
    for (int rcode = 0; rcode <= 15; rcode++) {
        const char* rcode_str = ares_dns_rcode_tostr((ares_dns_rcode_t)rcode);
        (void)rcode_str;
    }
    
    // Also test with fuzzer-generated rcode values
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 1; i++) {
        int rcode_val = fdp.ConsumeIntegralInRange<int>(0, 255);
        const char* rcode_str = ares_dns_rcode_tostr((ares_dns_rcode_t)rcode_val);
        (void)rcode_str;
    }

    // c) Test ares_dns_section_tostr with all section values
    for (int section = ARES_SECTION_ANSWER; section <= ARES_SECTION_ADDITIONAL; section++) {
        const char* section_str = ares_dns_section_tostr((ares_dns_section_t)section);
        (void)section_str;
    }

    // d) Test ares_strerror with various error codes
    // Test standard c-ares error codes
    for (int err = ARES_SUCCESS; err <= ARES_EDESTRUCTION; err++) {
        const char* err_str = ares_strerror(err);
        (void)err_str;
    }
    
    // Test with fuzzer-generated error codes
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 1; i++) {
        int err_code = fdp.ConsumeIntegral<int>();
        const char* err_str = ares_strerror(err_code);
        (void)err_str;
    }

    // e) Test ares_dns_rec_type_fromstr with various string inputs
    if (fdp.remaining_bytes() > 10) {
        std::vector<std::string> type_strings = {
            "A", "AAAA", "MX", "TXT", "CNAME", "NS", "SOA", "PTR", "SRV"
        };
        
        for (const auto& type_str : type_strings) {
            ares_dns_rec_type_t rec_type;
            ares_bool_t success = ares_dns_rec_type_fromstr(&rec_type, type_str.c_str());
            (void)success;
            (void)rec_type;
        }
        
        // Also test with fuzzer-generated strings
        for (int i = 0; i < 5 && fdp.remaining_bytes() > 2; i++) {
            std::string fuzz_str = fdp.ConsumeRandomLengthString(20);
            ares_dns_rec_type_t rec_type;
            ares_bool_t success = ares_dns_rec_type_fromstr(&rec_type, fuzz_str.c_str());
            (void)success;
            (void)rec_type;
        }
    }

    // f) Test ares_dns_class_fromstr with various string inputs
    if (fdp.remaining_bytes() > 10) {
        std::vector<std::string> class_strings = {
            "IN", "CH", "HS", "NONE", "ANY"
        };
        
        for (const auto& class_str : class_strings) {
            ares_dns_class_t dns_class;
            ares_bool_t success = ares_dns_class_fromstr(&dns_class, class_str.c_str());
            (void)success;
            (void)dns_class;
        }
        
        // Also test with fuzzer-generated strings
        for (int i = 0; i < 5 && fdp.remaining_bytes() > 2; i++) {
            std::string fuzz_str = fdp.ConsumeRandomLengthString(20);
            ares_dns_class_t dns_class;
            ares_bool_t success = ares_dns_class_fromstr(&dns_class, fuzz_str.c_str());
            (void)success;
            (void)dns_class;
        }
    }

    // g) Test ares_dns_opt_get_datatype and ares_dns_opt_get_name
    // Test with various RR keys and option values
    if (fdp.remaining_bytes() > 5) {
        // Test a few known option keys
        for (int opt_val = 0; opt_val < 10; opt_val++) {
            // Try with different RR keys
            for (int key_test = 0; key_test < 5; key_test++) {
                ares_dns_opt_datatype_t datatype = ares_dns_opt_get_datatype(
                    (ares_dns_rr_key_t)key_test, (unsigned short)opt_val);
                (void)datatype;
                
                const char* opt_name = ares_dns_opt_get_name(
                    (ares_dns_rr_key_t)key_test, (unsigned short)opt_val);
                (void)opt_name;
            }
        }
        
        // Test with fuzzer-generated values
        for (int i = 0; i < 5 && fdp.remaining_bytes() > 2; i++) {
            unsigned short opt_key = fdp.ConsumeIntegral<unsigned short>();
            unsigned short opt_val = fdp.ConsumeIntegral<unsigned short>();
            
            ares_dns_opt_datatype_t datatype = ares_dns_opt_get_datatype(
                (ares_dns_rr_key_t)opt_key, opt_val);
            (void)datatype;
            
            const char* opt_name = ares_dns_opt_get_name(
                (ares_dns_rr_key_t)opt_key, opt_val);
            (void)opt_name;
        }
    }

    // 6. Cleanup
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}

} // namespace harness_006
