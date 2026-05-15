#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ares.h"
#include "ares_nameser.h"
#include "ares_dns_record.h"

namespace harness_002 {

// Callback function for ares_query and ares_send
static void query_callback(void *arg, int status, int timeouts, 
                          const unsigned char *abuf, int alen) {
    // Simple callback - does nothing, just satisfies API requirements
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

// Callback function for ares_query_dnsrec and ares_send_dnsrec
static void dnsrec_callback(void *arg, ares_status_t status, 
                           size_t timeouts, const ares_dns_record_t *dnsrec) {
    // Simple callback - does nothing, just satisfies API requirements
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

// Callback function for ares_search
static void search_callback(void *arg, int status, int timeouts, 
                           const unsigned char *abuf, int alen) {
    // Simple callback - does nothing, just satisfies API requirements
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)abuf;
    (void)alen;
}

// Callback function for ares_search_dnsrec
static void search_dnsrec_callback(void *arg, ares_status_t status, 
                                  size_t timeouts, const ares_dns_record_t *dnsrec) {
    // Simple callback - does nothing, just satisfies API requirements
    (void)arg;
    (void)status;
    (void)timeouts;
    (void)dnsrec;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic parameters
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize c-ares library
    ares_library_init(ARES_LIB_INIT_ALL);

    // 1. ares_init/ares_init_options - initialize first channel
    ares_channel_t* channel1 = NULL;
    int init_result = ares_init(&channel1);
    if (init_result != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 2. ares_dup - duplicate the channel
    ares_channel_t* channel2 = NULL;
    int dup_result = ares_dup(&channel2, channel1);
    if (dup_result != ARES_SUCCESS) {
        // If duplication fails, we can still continue with channel1
        // but we need to handle this case
    }

    // 3. ares_reinit - reinitialize channel1 with new options
    // For ares_reinit, we need to set some options first
    // Create a simple options structure
    struct ares_options options;
    memset(&options, 0, sizeof(options));
    int optmask = 0;
    
    // Set some random options from fuzzer input
    uint8_t opt_flags = fdp.ConsumeIntegral<uint8_t>();
    
    if (opt_flags & 0x01) {
        options.flags = fdp.ConsumeIntegral<int>();
        optmask |= ARES_OPT_FLAGS;
    }
    
    if (opt_flags & 0x02 && fdp.remaining_bytes() >= 4) {
        options.timeout = fdp.ConsumeIntegral<int>();
        optmask |= ARES_OPT_TIMEOUTMS;
    }
    
    if (opt_flags & 0x04 && fdp.remaining_bytes() >= 4) {
        options.tries = fdp.ConsumeIntegral<int>();
        optmask |= ARES_OPT_TRIES;
    }
    
    // Try to reinitialize channel1 with new options
    ares_status_t reinit_status = ares_reinit(channel1);
    // Note: ares_reinit may fail if no options changed, but that's OK

    // Consume query parameters from fuzzer input
    // Fixed-size data first
    uint8_t dnsclass = fdp.ConsumeIntegral<uint8_t>() % 4;  // Limited to common classes
    uint16_t type = fdp.ConsumeIntegral<uint16_t>() % 65535; // DNS record type
    uint16_t query_id = fdp.ConsumeIntegral<uint16_t>();
    
    // Variable-size data (query name)
    std::string name = fdp.ConsumeRandomLengthString(253); // Max DNS name length - 2

    // 4. ares_query/ares_query_dnsrec - perform DNS queries
    // Use deprecated ares_query API
    ares_query(channel1, name.c_str(), dnsclass, type, query_callback, NULL);
    
    // Also use the new ares_query_dnsrec API if we have a valid channel2
    if (channel2 != NULL && fdp.remaining_bytes() > 10) {
        // Create a DNS record
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
                ares_query_dnsrec(channel2, name.c_str(), 
                                 static_cast<ares_dns_class_t>(dnsclass),
                                 static_cast<ares_dns_rec_type_t>(type),
                                 dnsrec_callback, NULL, &qid);
            }
            
            ares_dns_record_destroy(dnsrec);
        }
    }

    // 5. ares_send - test raw DNS packet sending
    // Create a simple DNS query packet
    // Minimum DNS query packet: 12 byte header + 5 byte name (example.com) + 4 byte QTYPE/QCLASS
    if (fdp.remaining_bytes() >= 30) {
        size_t packet_size = fdp.ConsumeIntegralInRange<size_t>(20, 512);
        if (packet_size > fdp.remaining_bytes()) {
            packet_size = fdp.remaining_bytes();
        }
        
        std::vector<unsigned char> dns_packet = fdp.ConsumeBytes<unsigned char>(packet_size);
        
        // Send the raw DNS packet
        ares_send(channel1, dns_packet.data(), dns_packet.size(), query_callback, NULL);
        
        // Also test ares_send_dnsrec if we have channel2
        if (channel2 != NULL && fdp.remaining_bytes() > 10) {
            // Create another DNS record for sending
            ares_dns_record_t* send_dnsrec = NULL;
            ares_status_t send_status = ares_dns_record_create(&send_dnsrec, 0, 
                                                              fdp.ConsumeIntegral<uint16_t>(),
                                                              ARES_OPCODE_QUERY, 
                                                              ARES_RCODE_NOERROR);
            
            if (send_status == ARES_SUCCESS && send_dnsrec != NULL) {
                // Add a question
                std::string send_name = fdp.ConsumeRandomLengthString(100);
                send_status = ares_dns_record_query_add(send_dnsrec, send_name.c_str(),
                                                       ARES_REC_TYPE_A,
                                                       ARES_CLASS_IN);
                
                if (send_status == ARES_SUCCESS) {
                    unsigned short send_qid = 0;
                    ares_send_dnsrec(channel2, send_dnsrec, dnsrec_callback, NULL, &send_qid);
                }
                
                ares_dns_record_destroy(send_dnsrec);
            }
        }
    }

    // 6. ares_process/ares_process_fd - process pending events
    // First get timeout value
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    // Get timeout for channel1
    ares_timeout(channel1, NULL, &tv);
    
    // Process using the deprecated ares_process API
    ares_process(channel1, NULL, NULL);
    
    // Also use ares_process_fd with invalid sockets (no actual sockets in fuzzer)
    ares_process_fd(channel1, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
    
    // Process channel2 if it exists
    if (channel2 != NULL) {
        ares_process_fd(channel2, ARES_SOCKET_BAD, ARES_SOCKET_BAD);
        
        // Test ares_process_fds with flags
        ares_process_flag_t flags = static_cast<ares_process_flag_t>(
            fdp.ConsumeIntegral<uint8_t>() % 2);
        ares_process_fds(channel2, NULL, 0, flags);
    }
    // 7. ares_search - test DNS search functionality
    // Use deprecated ares_search API
    std::string search_name = fdp.ConsumeRandomLengthString(100);
    ares_search(channel1, search_name.c_str(), dnsclass, type, search_callback, NULL);
    
    // Also test ares_search_dnsrec if we have channel2 and remaining data
    if (channel2 != NULL && fdp.remaining_bytes() > 10) {
        ares_dns_record_t* search_dnsrec = NULL;
        ares_status_t search_status = ares_dns_record_create(&search_dnsrec, 0,
                                                            fdp.ConsumeIntegral<uint16_t>(),
                                                            ARES_OPCODE_QUERY,
                                                            ARES_RCODE_NOERROR);
        
        if (search_status == ARES_SUCCESS && search_dnsrec != NULL) {
            // Add a search question
            std::string dnsrec_name = fdp.ConsumeRandomLengthString(100);
            search_status = ares_dns_record_query_add(search_dnsrec, dnsrec_name.c_str(),
                                                     ARES_REC_TYPE_A,
                                                     ARES_CLASS_IN);
            
            if (search_status == ARES_SUCCESS) {
                ares_search_dnsrec(channel2, search_dnsrec, search_dnsrec_callback, NULL);
            }
            
            ares_dns_record_destroy(search_dnsrec);
        }
    }

    // 8. ares_cancel - cancel all pending queries
    ares_cancel(channel1);
    if (channel2 != NULL) {
        ares_cancel(channel2);
    }

    // 9. ares_destroy - clean up channels
    ares_destroy(channel1);
    if (channel2 != NULL) {
        ares_destroy(channel2);
    }
    
    ares_library_cleanup();

    return 0;
}

} // namespace harness_002
