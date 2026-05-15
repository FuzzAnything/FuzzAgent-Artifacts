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

namespace harness_001 {
/* Fuzzing harness for c-ares library targeting DNS query processing APIs
 * This harness tests the complete DNS query lifecycle with fuzzer-generated input
 * Target APIs: ares_query, ares_search, ares_send, ares_process, ares_process_fd, ares_process_fds
 * Helper APIs: ares_init_options, ares_destroy
 */



// Callback for DNS queries
static void query_callback(void* arg, int status, int timeouts, 
                          const unsigned char* abuf, int alen) {
    // Callback doesn't need to do anything for fuzzing - just consume the result
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

// Callback for DNS queries with DNS record
static void query_dnsrec_callback(void* arg, ares_status_t status,
                                 size_t timeouts, const ares_dns_record_t* dnsrec) {
    // Callback doesn't need to do anything for fuzzing - just consume the result
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

// Callback for search operations
static void search_callback(void* arg, int status, int timeouts,
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
    if (size < 20) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize library
    ares_library_init(ARES_LIB_INIT_ALL);

    // Initialize channel with options
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
        ares_library_cleanup();
        return 0;
    }

    // Generate domain names for queries from fuzzer input
    std::vector<std::string> domains;
    size_t num_domains = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    for (size_t i = 0; i < num_domains && fdp.remaining_bytes() > 5; i++) {
        size_t domain_len = fdp.ConsumeIntegralInRange<size_t>(1, 100);
        std::string domain = fdp.ConsumeRandomLengthString(domain_len);
        // Ensure domain is null-terminated and has at least one character
        if (domain.empty()) {
            domain = "test";
        }
        domains.push_back(domain);
    }

    // Common DNS record types to test
    const ares_dns_rec_type_t dns_types[] = {
        ARES_REC_TYPE_A,
        ARES_REC_TYPE_AAAA,
        ARES_REC_TYPE_MX,
        ARES_REC_TYPE_NS,
        ARES_REC_TYPE_SOA,
        ARES_REC_TYPE_PTR,
        ARES_REC_TYPE_TXT,
        ARES_REC_TYPE_SRV,
        ARES_REC_TYPE_NAPTR,
        ARES_REC_TYPE_CAA
    };
    
    const size_t num_dns_types = sizeof(dns_types) / sizeof(dns_types[0]);

    // Test 1: ares_query (deprecated but still in API)
    if (!domains.empty() && fdp.remaining_bytes() > 1) {
        const std::string& domain = domains[0];
        int dnsclass = fdp.ConsumeIntegralInRange<int>(1, 4); // Usually 1 for IN
        int type_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_dns_types - 1);
        ares_dns_rec_type_t type = dns_types[type_idx];
        
        // Use both old and new query APIs
        ares_query(channel, domain.c_str(), dnsclass, type, query_callback, NULL);
        
        // Also test the newer ares_query_dnsrec API
        unsigned short qid;
        ares_query_dnsrec(channel, domain.c_str(), ARES_CLASS_IN, type, 
                         query_dnsrec_callback, NULL, &qid);
    }

    // Test 2: ares_search
    if (domains.size() > 1 && fdp.remaining_bytes() > 1) {
        const std::string& domain = domains[1 % domains.size()];
        int dnsclass = fdp.ConsumeIntegralInRange<int>(1, 4);
        int type_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_dns_types - 1);
        ares_dns_rec_type_t type = dns_types[type_idx];
        
        ares_search(channel, domain.c_str(), dnsclass, type, search_callback, NULL);
    }

    // Test 3: ares_send (with manually created DNS query)
    if (!domains.empty() && fdp.remaining_bytes() > 10) {
        // Get a domain for the query
        size_t domain_idx = fdp.ConsumeIntegralInRange<size_t>(0, domains.size() - 1);
        const std::string& domain = domains[domain_idx];
        
        // Create a simple DNS query using deprecated but public API
        unsigned char* qbuf = NULL;
        int qlen = 0;
        unsigned short query_id = fdp.ConsumeIntegral<unsigned short>();
        int rd_flag = fdp.ConsumeBool() ? 1 : 0;
        
        int create_status = ares_create_query(domain.c_str(), ARES_CLASS_IN, 
                                             ARES_REC_TYPE_A, query_id, rd_flag,
                                             &qbuf, &qlen, 512);
        
        if (create_status == ARES_SUCCESS && qbuf != NULL) {
            // Try to send it
            ares_send(channel, qbuf, qlen, query_callback, NULL);
            ares_free_string(qbuf);
        }
    }

    // Test 4: ares_process and related APIs
    // First, get file descriptors
    ares_socket_t socks[ARES_GETSOCK_MAXNUM];
    int numsocks = ares_getsock(channel, socks, ARES_GETSOCK_MAXNUM);
    
    if (numsocks > 0) {
        // Test ares_process_fd for each socket
        for (int i = 0; i < numsocks && i < ARES_GETSOCK_MAXNUM; i++) {
            ares_process_fd(channel, socks[i], ARES_SOCKET_BAD);
            ares_process_fd(channel, ARES_SOCKET_BAD, socks[i]);
        }
        
        // Test ares_process_fds with events array
        ares_fd_events_t events[ARES_GETSOCK_MAXNUM];
        size_t nevents = 0;
        
        for (int i = 0; i < numsocks && i < ARES_GETSOCK_MAXNUM && nevents < ARES_GETSOCK_MAXNUM; i++) {
            events[nevents].fd = socks[i];
            // Randomly assign read/write events
            unsigned int event_flags = 0;
            if (fdp.ConsumeBool()) event_flags |= ARES_FD_EVENT_READ;
            if (fdp.ConsumeBool()) event_flags |= ARES_FD_EVENT_WRITE;
            events[nevents].events = event_flags;
            nevents++;
        }
        
        if (nevents > 0) {
            unsigned int process_flags = fdp.ConsumeBool() ? ARES_PROCESS_FLAG_SKIP_NON_FD : ARES_PROCESS_FLAG_NONE;
            ares_process_fds(channel, events, nevents, process_flags);
        }
        
        // Test old ares_process API with fd_sets
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        for (int i = 0; i < numsocks && i < ARES_GETSOCK_MAXNUM; i++) {
            if (fdp.ConsumeBool()) {
                FD_SET(socks[i], &read_fds);
            }
            if (fdp.ConsumeBool()) {
                FD_SET(socks[i], &write_fds);
            }
        }
        
        ares_process(channel, &read_fds, &write_fds);
    }

    // Test timeout processing
    struct timeval tv;
    struct timeval* timeout_ptr = ares_timeout(channel, NULL, &tv);
    (void)timeout_ptr;

    // Process pending operations
    ares_process_pending_write(channel);

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}

} // namespace harness_001
