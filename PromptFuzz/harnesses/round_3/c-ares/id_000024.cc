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
//<ID> 449
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_socket_functions","ares_parse_soa_reply","ares_dns_rec_type_fromstr","ares_query_dnsrec","ares_dns_record_rr_get","ares_dns_record_get_rcode","ares_dns_rcode_tostr","ares_cancel","ares_free_data","ares_free_data","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_socket_functions","ares_parse_soa_reply","ares_dns_rec_type_fromstr","ares_query_dnsrec","ares_dns_record_rr_get","ares_dns_record_get_rcode","ares_dns_rcode_tostr","ares_cancel","ares_free_data","ares_free_data","ares_destroy","ares_library_cleanup"],"visited":0}
/*Here's a fuzz driver that uses the specified c-ares APIs to achieve DNS query/response processing:

*/


// Global variables for callback context
static ares_dns_record_t * id_000024_g_dnsrec_callback = nullptr;
static bool  id_000024_g_callback_called = false;

// Callback function for ares_query_dnsrec
static void  id_000024_query_callback(void *arg, ares_status_t status, 
                          size_t timeouts, const ares_dns_record_t *dnsrec) {
    (void)arg;
    (void)timeouts;
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Duplicate the DNS record for later use
         id_000024_g_dnsrec_callback = ares_dns_record_duplicate(dnsrec);
    }
     id_000024_g_callback_called = true;
}

// Dummy socket functions for ares_set_socket_functions
static ares_socket_t  id_000024_dummy_socket(int domain, int type, int protocol, void *user_data) {
    (void)domain;
    (void)type;
    (void)protocol;
    (void)user_data;
    return ARES_SOCKET_BAD;
}

static int  id_000024_dummy_close(ares_socket_t socket, void *user_data) {
    (void)socket;
    (void)user_data;
    return -1;
}

static int  id_000024_dummy_connect(ares_socket_t socket, const struct sockaddr *addr,
                        ares_socklen_t addrlen, void *user_data) {
    (void)socket;
    (void)addr;
    (void)addrlen;
    (void)user_data;
    return -1;
}

static ares_ssize_t  id_000024_dummy_recvfrom(ares_socket_t socket, void *buffer,
                                  size_t length, int flags,
                                  struct sockaddr *addr,
                                  ares_socklen_t *addrlen, void *user_data) {
    (void)socket;
    (void)buffer;
    (void)length;
    (void)flags;
    (void)addr;
    (void)addrlen;
    (void)user_data;
    return -1;
}

static ares_ssize_t  id_000024_dummy_sendv(ares_socket_t socket, const struct iovec *vector,
                               int count, void *user_data) {
    (void)socket;
    (void)vector;
    (void)count;
    (void)user_data;
    return -1;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
	//fuzzer shim end}




    // Reset global state
     id_000024_g_dnsrec_callback = nullptr;
     id_000024_g_callback_called = false;
    
    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // Create a channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // Set up dummy socket functions (required API)
    struct ares_socket_functions funcs = {
         id_000024_dummy_socket,
         id_000024_dummy_close,
         id_000024_dummy_connect,
         id_000024_dummy_recvfrom,
         id_000024_dummy_sendv
    };
    ares_set_socket_functions(channel, &funcs, nullptr);
    
    // Parse input as SOA reply (required API)
    struct ares_soa_reply *soa_reply = nullptr;
    if (size > 0) {
        // Use input data as DNS response buffer
        int parse_result = ares_parse_soa_reply(data, static_cast<int>(size), &soa_reply);
        (void)parse_result; // Result is intentionally ignored for fuzzing
    }
    
    // Convert string to DNS record type (required API)
    ares_dns_rec_type_t qtype;
    ares_bool_t type_converted = ares_dns_rec_type_fromstr(&qtype, fuzz_str_1);
    (void)type_converted;
    
    // Prepare domain name for query (extract from input if possible)
    char domain_name[256] = "example.com";
    if (size > 0 && data[0] != 0) {
        // Use first part of input as domain name (up to 255 chars)
        size_t name_len = (size < 255) ? size : 255;
        memcpy(domain_name, data, name_len);
        domain_name[name_len] = '\0';
        
        // Ensure it's a valid C string (replace non-printable chars)
        for (size_t i = 0; i < name_len; i++) {
            if (domain_name[i] < 32 || domain_name[i] > 126) {
                domain_name[i] = 'a';
            }
        }
    }
    
    // Perform DNS query (required API)
    unsigned short qid = 0;
    ares_status_t query_status = ares_query_dnsrec(
        channel,
        fuzz_str_2,
        ARES_CLASS_IN,
        qtype,
         id_000024_query_callback,
        nullptr,
        &qid
    );
    (void)query_status;
    
    // Get a DNS record resource (required API)
    if ( id_000024_g_dnsrec_callback) {
        ares_dns_rr_t *rr = ares_dns_record_rr_get(
             id_000024_g_dnsrec_callback,
            ARES_SECTION_ANSWER,
            fuzz_uint64_t_3
        );
        (void)rr; // Result is intentionally ignored for fuzzing
    }
    
    // Convert RCODE to string (required API)
    if ( id_000024_g_dnsrec_callback) {
        ares_dns_rcode_t rcode = ares_dns_record_get_rcode( id_000024_g_dnsrec_callback);
        const char *rcode_str = ares_dns_rcode_tostr(rcode);
        (void)rcode_str; // Result is intentionally ignored for fuzzing
    }
    
    // Cancel all pending requests (required API)
    ares_cancel(channel);
    
    // Clean up allocated resources
    if (soa_reply) {
        ares_free_data(soa_reply); // Required API
    }
    
    if ( id_000024_g_dnsrec_callback) {
        ares_free_data( id_000024_g_dnsrec_callback); // Required API
    }
    
    // Destroy channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();
    
    return 0;
}