#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include "ares.h"
#include "ares_dns.h"          // For DNS record types and classes
#include "ares_dns_record.h"   // For DNS record functions
#include "ares_nameser.h"      // For T_A, T_AAAA, T_MX, T_TXT, C_IN constants

namespace harness_008 {
/*
 * Fuzzing harness for c-ares DNS library - Thread-safe DNS Query Queue Operations
 * Targets 4 completely uncovered thread-safe queue and search APIs with 446+ undiscovered branches:
 *   - ares_queue_wait_empty (14 undiscovered branches) - Thread synchronization for queue emptiness
 *   - ares_queue_active_queries (2 undiscovered branches) - Active query monitoring
 *   - ares_search (218 undiscovered branches) - Advanced DNS search (deprecated but still uncovered)
 *   - ares_search_dnsrec (212 undiscovered branches) - Modern DNS search with DNS records
 * 
 * Semantic Focus: Thread-safe DNS query queue operations, concurrency aspects, thread synchronization,
 * DNS search functionality, and query lifecycle management. Tests c-ares' threading utilities and
 * advanced search capabilities missing from existing harnesses.
 * Follows exact invocation sequence from coverage guidance for maximum branch coverage.
 */



// Custom memory functions for ares_library_init_mem
static void* custom_malloc(size_t size) {
    return malloc(size);
}

static void custom_free(void* ptr) {
    free(ptr);
}

static void* custom_realloc(void* ptr, size_t size) {
    return realloc(ptr, size);
}

// DNS query callback for ares_query
static void query_callback(void* arg, int status, int timeouts,
                          const unsigned char* abuf, int alen) {
    (void)arg; (void)status; (void)timeouts; (void)abuf; (void)alen;
    // For fuzzing purposes, we don't need to process the callback result
    // This callback is invoked when ares_query completes
}

// DNS record callback for ares_search_dnsrec and ares_query_dnsrec
static void dnsrec_callback(void* arg, ares_status_t status,
                           size_t timeouts,
                           const ares_dns_record_t* dnsrec) {
    (void)arg; (void)status; (void)timeouts; (void)dnsrec;
    // For fuzzing purposes, we don't need to process the callback result
    // This callback is invoked when ares_search_dnsrec or ares_query_dnsrec completes
}

// Search callback for ares_search (deprecated)
static void search_callback(void* arg, int status, int timeouts,
                           const unsigned char* abuf, int alen) {
    (void)arg; (void)status; (void)timeouts; (void)abuf; (void)alen;
    // For fuzzing purposes, we don't need to process the callback result
    // This callback is invoked when ares_search completes
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for various configurations and domain names
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Define valid enum values for DNS classes
    const ares_dns_class_t valid_classes[] = {
        ARES_CLASS_IN,      // 1
        ARES_CLASS_CHAOS,   // 3  
        ARES_CLASS_HESIOD,  // 4
        ARES_CLASS_NONE,    // 254
        ARES_CLASS_ANY      // 255
    };
    const size_t num_valid_classes = sizeof(valid_classes) / sizeof(valid_classes[0]);

    // Define valid enum values for DNS record types (common ones)
    const ares_dns_rec_type_t valid_rec_types[] = {
        ARES_REC_TYPE_A,      // 1
        ARES_REC_TYPE_NS,     // 2
        ARES_REC_TYPE_CNAME,  // 5
        ARES_REC_TYPE_SOA,    // 6
        ARES_REC_TYPE_PTR,    // 12
        ARES_REC_TYPE_HINFO,  // 13
        ARES_REC_TYPE_MX,     // 15
        ARES_REC_TYPE_TXT,    // 16
        ARES_REC_TYPE_SIG,    // 24
        ARES_REC_TYPE_AAAA,   // 28
        ARES_REC_TYPE_SRV,    // 33
        ARES_REC_TYPE_NAPTR,  // 35
        ARES_REC_TYPE_OPT,    // 41
        ARES_REC_TYPE_TLSA,   // 52
        ARES_REC_TYPE_SVCB,   // 64
        ARES_REC_TYPE_HTTPS,  // 65
        ARES_REC_TYPE_ANY,    // 255
        ARES_REC_TYPE_URI,    // 256
        ARES_REC_TYPE_CAA,    // 257
        // Note: ARES_REC_TYPE_RAW_RR (65536) is typically for internal use
    };
    const size_t num_valid_rec_types = sizeof(valid_rec_types) / sizeof(valid_rec_types[0]);

    // PHASE 1: Memory-aware library initialization
    // Step 1: Call ares_library_init_mem for memory-aware initialization
    int init_flags = fdp.ConsumeIntegral<int>();
    // Mask flags to valid range
    init_flags &= (ARES_LIB_INIT_WIN32 | ARES_LIB_INIT_ALL);
    
    int mem_init_result = ares_library_init_mem(init_flags,
                                                custom_malloc,
                                                custom_free,
                                                custom_realloc);
    if (mem_init_result != ARES_SUCCESS) {
        // If memory init fails, try regular initialization
        // but we still want to proceed with fuzzing
    }

    // Step 2: Call ares_library_init to initialize library (if memory init failed or as fallback)
    int lib_init_result = ares_library_init(init_flags);
    if (lib_init_result != ARES_SUCCESS) {
        // Library initialization failed, cannot proceed
        return 0;
    }

    // PHASE 2: Channel initialization
    // Step 3: Call ares_init to create channel
    ares_channel_t* channel = nullptr;
    int channel_init_result = ares_init(&channel);
    if (channel_init_result != ARES_SUCCESS || channel == nullptr) {
        ares_library_cleanup();
        return 0;
    }

    // PHASE 3: Create DNS queries
    // Step 4: Create DNS queries using ares_query
    // Consume parameters for DNS queries from fuzzer input
    uint8_t num_queries = fdp.ConsumeIntegralInRange<uint8_t>(0, 5);
    std::vector<unsigned short> query_ids;
    for (uint8_t i = 0; i < num_queries && fdp.remaining_bytes() > 32; i++) {
        // Consume domain name
        size_t max_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 255);
        std::string domain_name = fdp.ConsumeRandomLengthString(max_name_len);
        
        // Consume DNS type and class from valid enum values
        ares_dns_rec_type_t rec_type = fdp.PickValueInArray(valid_rec_types);
        ares_dns_class_t rec_class = fdp.PickValueInArray(valid_classes);
        
        // For the deprecated ares_query function, we need integer values
        int dns_type = static_cast<int>(rec_type);
        int dns_class = static_cast<int>(rec_class);
        
        unsigned short qid = 0;
        ares_status_t query_status = ares_query_dnsrec(channel,
                                                      domain_name.c_str(),
                                                      rec_class,
                                                      rec_type,
                                                      dnsrec_callback,
                                                      nullptr,
                                                      &qid);
        
        if (query_status == ARES_SUCCESS && qid != 0) {
            query_ids.push_back(qid);
        }
        
        // Also test the deprecated ares_query for coverage
        ares_query(channel,
                  domain_name.c_str(),
                  dns_class,
                  dns_type,
                  query_callback,
                  nullptr);
    }

    // PHASE 4: Queue monitoring
    // Step 5: Call ares_queue_active_queries to monitor query queue state
    size_t active_queries = ares_queue_active_queries(channel);
    (void)active_queries; // Use the result to avoid unused variable warning

    // PHASE 5: DNS searches
    // Step 6: Call ares_search and ares_search_dnsrec to perform DNS searches
    
    // First, test ares_search_dnsrec with a DNS record
    if (fdp.remaining_bytes() > 64) {
        // Create a simple DNS record for testing
        ares_dns_record_t* dnsrec = nullptr;
        // Consume parameters for DNS record from fuzzer input
        unsigned short id = fdp.ConsumeIntegral<unsigned short>();
        unsigned short flags = fdp.ConsumeIntegral<unsigned short>();
        // Use valid opcode values: 0, 1, 2, 4, 5 (3 is not valid)
        const int valid_opcodes[] = {0, 1, 2, 4, 5};
        ares_dns_opcode_t opcode = static_cast<ares_dns_opcode_t>(
            fdp.PickValueInArray(valid_opcodes));
        
        // Use valid rcode values: 0-23
        ares_dns_rcode_t rcode = static_cast<ares_dns_rcode_t>(
            fdp.ConsumeIntegralInRange<int>(0, 23));
        
        ares_status_t dnsrec_status = ares_dns_record_create(&dnsrec,
                                                            id,
                                                            flags,
                                                            opcode,
                                                            rcode);
        
        if (dnsrec_status == ARES_SUCCESS && dnsrec != nullptr) {
            // Add a question to the DNS record
            std::string search_name = fdp.ConsumeRandomLengthString(128);
            ares_dns_rec_type_t search_type = fdp.PickValueInArray(valid_rec_types);
            ares_dns_class_t search_class = fdp.PickValueInArray(valid_classes);
            
            ares_status_t question_status = ares_dns_record_query_add(
                dnsrec,
                search_name.c_str(),
                search_type,
                search_class);
            
            (void)question_status; // Use result
            
            // Perform the search with the DNS record
            ares_status_t search_status = ares_search_dnsrec(channel,
                                                            dnsrec,
                                                            dnsrec_callback,
                                                            nullptr);
            (void)search_status; // Use result
            
            // Clean up the DNS record
            ares_dns_record_destroy(dnsrec);
        }
    }
    
    // Also test the deprecated ares_search function
    if (fdp.remaining_bytes() > 32) {
        std::string search_domain = fdp.ConsumeRandomLengthString(128);
        int search_class_int = static_cast<int>(fdp.PickValueInArray(valid_classes));
        int search_type_int = static_cast<int>(fdp.PickValueInArray(valid_rec_types));
        
        ares_search(channel,
                   search_domain.c_str(),
                   search_class_int,
                   search_type_int,
                   search_callback,
                   nullptr);
    }

    // PHASE 6: Thread synchronization
    // Step 7: Call ares_queue_wait_empty to wait for query queue to empty
    if (!query_ids.empty() && active_queries > 0) {
        // Only call wait_empty if we have active queries
        // Use a reasonable timeout from fuzzer input (0-1000ms)
        int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 1000);
        ares_queue_wait_empty(channel, timeout_ms);
    }

    // PHASE 7: Cleanup
    // Step 8: Call ares_destroy to clean up channel
    ares_destroy(channel);
    
    // Step 9: Call ares_library_cleanup to clean up library
    ares_library_cleanup();

    return 0;
}

} // namespace harness_008
