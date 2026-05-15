#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <ares.h>
}
extern "C" {
#include <ares_dns.h>
}
extern "C" {
#include <ares_nameser.h>
}
//<ID> 357
//<Prompt> []
/*<Combination>: [
*/
//<score> 5.5, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_socket_functions","ares_destroy","ares_library_cleanup","ares_create_query","ares_free_string","ares_destroy","ares_library_cleanup","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_ttl","ares_dns_rr_get_type","ares_dns_rr_get_opt_byid","ares_dns_record_destroy","ares_send","ares_process_pending_write","ares_cancel","ares_free_string","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_socket_functions","ares_create_query","ares_dns_parse","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_ttl","ares_dns_rr_get_type","ares_dns_rr_get_opt_byid","ares_dns_record_destroy","ares_send","ares_process_pending_write","ares_cancel","ares_free_string","ares_destroy","ares_library_cleanup"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified c-ares APIs to process DNS responses:

*/


extern "C" {
}

// Custom socket functions structure
struct ares_socket_functions  id_000015_my_socket_funcs;

// Socket function implementations
static ares_socket_t  id_000015_my_asocket(int domain, int type, int protocol, void *user_data) {
    return socket(domain, type, protocol);
}

static int  id_000015_my_aclose(ares_socket_t socket_fd, void *user_data) {
    return close(socket_fd);
}

static int  id_000015_my_aconnect(ares_socket_t socket_fd, const struct sockaddr *addr,
                      ares_socklen_t addrlen, void *user_data) {
    return connect(socket_fd, addr, addrlen);
}

static ares_ssize_t  id_000015_my_arecvfrom(ares_socket_t socket_fd, void *buf, size_t len,
                                int flags, struct sockaddr *from,
                                ares_socklen_t *fromlen, void *user_data) {
    return recvfrom(socket_fd, buf, len, flags, from, fromlen);
}

static ares_ssize_t  id_000015_my_asendv(ares_socket_t socket_fd, const struct iovec *vec,
                             int len, void *user_data) {
    // Simple implementation - send each vector
    ares_ssize_t total_sent = 0;
    for (int i = 0; i < len; i++) {
        ssize_t sent = send(socket_fd, vec[i].iov_base, vec[i].iov_len, 0);
        if (sent < 0) return -1;
        total_sent += sent;
    }
    return total_sent;
}

// Callback function for ares_send
static void  id_000015_my_callback(void *arg, int status, int timeouts,
                       const unsigned char *abuf, int alen) {
    // Process the DNS response
    if (status == ARES_SUCCESS && abuf != NULL && alen > 0) {
        // Parse the DNS response
        ares_dns_record_t *dnsrec = NULL;
        ares_status_t parse_status = ares_dns_parse(abuf, alen, 0, &dnsrec);
        
        if (parse_status == ARES_SUCCESS && dnsrec != NULL) {
            // Get the number of answer records
            size_t answer_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
            
            for (size_t i = 0; i < answer_cnt; i++) {
                // Get the resource record
                ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, i);
                if (rr != NULL) {
                    // Get TTL
                    unsigned int ttl = ares_dns_rr_get_ttl(rr);
                    
                    // Get record type
                    ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                    
                    // Try to get OPT record data if available
                    if (rr_type == ARES_REC_TYPE_OPT) {
                        const unsigned char *opt_val = NULL;
                        size_t opt_len = 0;
                        
                        // Try to get option by ID (using a sample ID 8 for EDNS client subnet)
                        ares_bool_t has_opt = ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS,
                                                                      8, &opt_val, &opt_len);
                        // Optionally use the result
                        (void)has_opt;
                    }
                }
            }
            
            // Clean up
            ares_dns_record_destroy(dnsrec);
        }
    }
}

// Helper function to convert string to DNS record type
static void  id_000015_test_rec_type_fromstr() {
    ares_dns_rec_type_t qtype;
    const char *type_str = "A";
    ares_bool_t success = ares_dns_rec_type_fromstr(&qtype, type_str);
    (void)success; // Use the result to avoid unused variable warning
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    if (size < sizeof(uint16_t)) {
        return 0;
    }
    
    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Create a channel
    ares_channel_t *channel = NULL;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize options
    memset(&options, 0, sizeof(options));
    options.timeout = 2000;
    optmask |= ARES_OPT_TIMEOUTMS;
    
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Set up custom socket functions
     id_000015_my_socket_funcs.asocket =  id_000015_my_asocket;
     id_000015_my_socket_funcs.aclose =  id_000015_my_aclose;
     id_000015_my_socket_funcs.aconnect =  id_000015_my_aconnect;
     id_000015_my_socket_funcs.arecvfrom =  id_000015_my_arecvfrom;
     id_000015_my_socket_funcs.asendv =  id_000015_my_asendv;
    
    ares_set_socket_functions(channel, & id_000015_my_socket_funcs, NULL);
    
    // Test string to record type conversion
     id_000015_test_rec_type_fromstr();
    
    // Create a query from the input data
    // Use the first part of data as query name if possible
    size_t name_len = 0;
    for (size_t i = 0; i < size && i < 256; i++) {
        if (data[i] == 0) {
            name_len = i;
            break;
        }
    }
    
    if (name_len == 0 || name_len >= size) {
        name_len = (size > 255) ? 255 : size;
    }
    
    char *query_name = (char *)malloc(name_len + 1);
    if (!query_name) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    
    memcpy(query_name, data, name_len);
    query_name[name_len] = '\0';
    
    // Create a DNS query
    unsigned char *qbuf = NULL;
    int qlen = 0;
    
    // Use ares_create_query to create the query
    int query_result = ares_create_query(query_name, 
                                        C_IN,           // DNS class
                                        T_A,           // Query type A
                                        fuzz_uint16_t_2,        // Query ID
                                        1,             // Recursion desired
                                        &qbuf, 
                                        &qlen, 
                                        fuzz_int32_t_3);          // Max UDP size
    
    free(query_name);
    
    if (query_result != ARES_SUCCESS || qbuf == NULL) {
        if (qbuf) ares_free_string(qbuf);
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    
    // Use remaining data as mock response for parsing
    if (size > name_len) {
        const uint8_t *response_data = data + name_len;
        size_t response_size = size - name_len;
        
        if (response_size > 0) {
            // Parse the response data as DNS
            ares_dns_record_t *dnsrec = NULL;
            ares_status_t parse_status = ares_dns_parse(response_data, response_size, fuzz_uint32_t_4, &dnsrec);
            
            if (parse_status == ARES_SUCCESS && dnsrec != NULL) {
                // Get the number of answer records
                size_t answer_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
                
                for (size_t i = 0; i < answer_cnt && i < 10; i++) { // Limit to 10 records
                    // Get the resource record
                    ares_dns_rr_t *rr = ares_dns_record_rr_get(dnsrec, ARES_SECTION_ANSWER, fuzz_uint64_t_5);
                    if (rr != NULL) {
                        // Get TTL
                        unsigned int ttl = ares_dns_rr_get_ttl(rr);
                        
                        // Get record type
                        ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                        
                        // Try to get OPT record data if available
                        if (rr_type == ARES_REC_TYPE_OPT) {
                            const unsigned char *opt_val = NULL;
                            size_t opt_len = 0;
                            
                            // Try to get option by ID (using various IDs)
                            for (unsigned short opt_id = 0; opt_id < 16; opt_id++) {
                                ares_bool_t has_opt = ares_dns_rr_get_opt_byid(
                                    rr, ARES_RR_OPT_OPTIONS, fuzz_uint16_t_6, &opt_val, &opt_len);
                                (void)has_opt; // Use the result
                            }
                        }
                    }
                }
                
                ares_dns_record_destroy(dnsrec);
            }
        }
    }
    
    // Send the query
    ares_send(channel, qbuf, fuzz_int32_t_7,  id_000015_my_callback, NULL);
    
    // Process pending writes
    ares_process_pending_write(channel);
    
    // Cancel all pending requests
    ares_cancel(channel);
    
    // Clean up
    ares_free_string(qbuf);
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}