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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 189, nr_unique_branch: 220
//<Quality> {"density":7,"unique_branches":{"hostent_naddr":[[74,13,74,38,0,0,4,1]],"ares_append_addrinfo_cname":[[105,7,105,19,0,0,4,1],[109,7,109,12,0,0,4,0]],"ares_dns_opt_get_datatype":[[828,5,828,12,0,0,4,0]],"hostent_nalias":[[53,13,53,36,0,0,4,1]],"ares_inet_pton":[[439,7,439,20,0,0,4,1],[441,14,441,28,0,0,4,0],[448,7,448,19,0,0,4,0],[448,23,448,42,0,0,4,0],[448,23,448,42,0,0,4,1],[451,10,451,23,0,0,4,1]],"ares_inet_net_pton_ipv6":[[374,7,374,37,0,0,4,0],[374,7,374,37,0,0,4,1],[381,7,381,18,0,0,4,0],[381,7,381,18,0,0,4,1],[386,7,386,15,0,0,4,0]],"ares_freeaddrinfo_nodes":[[49,10,49,14,0,0,4,0],[49,10,49,14,0,0,4,1]],"ares_strdup":[[66,7,66,17,0,0,4,0]],"ares_inet_pton6":[[272,7,272,18,0,0,4,0],[272,7,272,18,0,0,4,1],[273,9,273,22,0,0,4,0],[280,10,280,31,0,0,4,0],[280,10,280,31,0,0,4,1],[283,9,283,58,0,0,4,0],[283,9,283,58,0,0,4,1],[286,9,286,20,0,0,4,0],[286,9,286,20,0,0,4,1],[287,11,287,28,0,0,4,0],[287,11,287,28,0,0,4,1],[292,11,292,23,0,0,4,1],[299,9,299,18,0,0,4,0],[299,9,299,18,0,0,4,1],[301,11,301,22,0,0,4,0],[301,11,301,22,0,0,4,1],[302,13,302,19,0,0,4,1],[307,18,307,30,0,0,4,0],[307,18,307,30,0,0,4,1],[310,11,310,33,0,0,4,1],[320,9,320,18,0,0,4,0],[320,9,320,18,0,0,4,1],[320,22,320,50,0,0,4,0],[321,9,321,61,0,0,4,1],[328,7,328,17,0,0,4,0],[328,7,328,17,0,0,4,1],[329,9,329,31,0,0,4,1],[335,7,335,21,0,0,4,1],[352,7,352,17,0,0,4,0]],"ai_nalias":[[64,28,64,41,0,0,4,0],[64,28,64,41,0,0,4,1]],"ai_naddr":[[85,26,85,38,0,0,4,0],[85,26,85,38,0,0,4,1],[86,9,86,24,0,0,4,0],[86,28,86,49,0,0,4,0],[86,28,86,49,0,0,4,1]],"ares_inet_net_pton_ipv4":[[87,7,87,16,0,0,4,0],[87,21,87,34,0,0,4,1],[87,38,87,51,0,0,4,1],[127,13,127,22,0,0,4,0],[135,25,135,34,0,0,4,0],[138,11,138,20,0,0,4,0],[151,7,151,16,0,0,4,0],[151,68,151,78,0,0,4,0],[159,11,159,20,0,0,4,0],[159,11,159,20,0,0,4,1],[162,14,162,35,0,0,4,0],[162,14,162,35,0,0,4,1],[163,9,163,19,0,0,4,0],[163,9,163,19,0,0,4,1],[169,7,169,17,0,0,4,0],[178,7,178,17,0,0,4,1],[185,16,185,28,0,0,4,1],[191,9,191,34,0,0,4,1],[198,9,198,18,0,0,4,0],[198,22,198,34,0,0,4,1],[203,10,203,35,0,0,4,0],[204,9,204,16,0,0,4,1],[93,25,93,52,6,0,4,1],[95,26,95,51,7,0,4,1],[95,55,95,80,7,0,4,1],[93,25,93,52,8,0,4,1],[95,55,95,80,9,0,4,1],[93,25,93,52,12,0,4,0],[93,25,93,52,12,0,4,1],[95,26,95,51,13,0,4,0],[95,26,95,51,13,0,4,1],[95,55,95,80,13,0,4,0],[95,55,95,80,13,0,4,1],[93,25,93,52,14,0,4,0],[93,25,93,52,14,0,4,1],[95,26,95,51,15,0,4,0],[95,26,95,51,15,0,4,1],[95,55,95,80,15,0,4,0],[95,55,95,80,15,0,4,1]],"ares_addrinfo2hostent":[[106,7,106,17,0,0,4,1],[106,21,106,33,0,0,4,1],[114,7,114,26,0,0,4,1],[122,7,122,24,0,0,4,0],[122,7,122,24,0,0,4,1],[122,28,122,46,0,0,4,1],[126,7,126,20,0,0,4,0],[128,9,128,17,0,0,4,1],[134,7,134,24,0,0,4,0],[134,7,134,24,0,0,4,1],[136,14,136,32,0,0,4,0],[140,7,140,30,0,0,4,0],[141,9,141,19,0,0,4,0],[141,9,141,19,0,0,4,1],[143,11,143,34,0,0,4,1],[148,11,148,34,0,0,4,0],[148,11,148,34,0,0,4,1],[148,38,148,46,0,0,4,1],[159,7,159,15,0,0,4,1],[164,7,164,15,0,0,4,0],[164,7,164,15,0,0,4,1],[167,30,167,43,0,0,4,0],[167,30,167,43,0,0,4,1],[168,11,168,31,0,0,4,1],[172,11,172,40,0,0,4,1],[183,7,183,20,0,0,4,1],[188,7,188,13,0,0,4,0],[188,7,188,13,0,0,4,1],[190,28,190,40,0,0,4,0],[190,28,190,40,0,0,4,1],[191,11,191,36,0,0,4,0],[191,11,191,36,0,0,4,1],[195,11,195,42,0,0,4,1],[198,11,198,29,0,0,4,1],[204,11,204,28,0,0,4,0],[214,7,214,27,0,0,4,0],[214,7,214,27,0,0,4,1],[214,31,214,55,0,0,4,0]],"ares_free_hostent":[[37,7,37,12,0,0,4,1],[42,29,42,30,0,0,4,0],[42,34,42,36,0,0,4,0],[42,34,42,36,0,0,4,1],[46,7,46,24,0,0,4,0],[48,15,48,43,0,0,4,0],[48,15,48,43,0,0,4,1]],"ares_parse_into_addrinfo":[[65,7,65,29,0,0,4,1],[70,7,70,19,0,0,4,0],[70,7,70,19,0,0,4,1],[75,15,75,26,0,0,4,0],[75,15,75,26,0,0,4,1],[80,9,80,51,0,0,4,0],[80,9,80,51,0,0,4,1],[98,9,98,37,0,0,4,0],[98,9,98,37,0,0,4,1],[106,11,106,24,0,0,4,1],[112,11,112,31,0,0,4,1],[117,11,117,30,0,0,4,1],[121,16,121,40,0,0,4,0],[121,16,121,40,0,0,4,1],[126,11,126,33,0,0,4,1],[129,16,129,43,0,0,4,0],[129,16,129,43,0,0,4,1],[134,11,134,33,0,0,4,1],[142,7,142,13,0,0,4,0],[142,7,142,13,0,0,4,1],[142,17,142,26,0,0,4,0],[142,17,142,26,0,0,4,1],[143,8,143,18,0,0,4,0],[149,7,149,23,0,0,4,0],[152,9,152,25,0,0,4,1],[158,7,158,12,0,0,4,0],[158,7,158,12,0,0,4,1],[158,16,158,24,0,0,4,0],[163,7,163,16,0,0,4,0],[163,7,163,16,0,0,4,1],[173,7,173,30,0,0,4,1]],"ares_dns_rcode_isvalid":[[53,5,53,28,0,0,4,0]],"ares_dns_rr_get_addr":[[762,7,762,60,0,0,4,1],[767,7,767,19,0,0,4,1]],"ares_append_addrinfo_node":[[145,7,145,19,0,0,4,1],[149,7,149,12,0,0,4,0],[149,7,149,12,0,0,4,1],[154,10,154,23,0,0,4,0],[154,10,154,23,0,0,4,1]],"ares_freeaddrinfo_cnames":[[37,10,37,14,0,0,4,0],[37,10,37,14,0,0,4,1]],"ares_append_ai_node":[[73,7,73,24,0,0,4,0],[73,7,73,24,0,0,4,1],[75,9,75,13,0,0,4,1],[86,14,86,32,0,0,4,0],[88,9,88,14,0,0,4,1],[104,7,104,12,0,0,4,1]],"ares_addrinfo_cat_nodes":[[166,7,166,12,0,0,4,0]],"ares_parse_aaaa_reply":[[61,7,61,15,0,0,4,1],[65,7,65,16,0,0,4,0],[73,7,73,29,0,0,4,0],[73,7,73,29,0,0,4,1],[78,7,78,29,0,0,4,0],[78,33,78,55,0,0,4,1],[82,7,82,19,0,0,4,0],[85,9,85,31,0,0,4,0],[85,35,85,57,0,0,4,1],[90,7,90,23,0,0,4,0],[90,27,90,40,0,0,4,1],[104,7,104,30,0,0,4,0],[104,7,104,30,0,0,4,1]],"ares_inet_net_pton":[[425,5,425,17,0,0,4,1],[427,5,427,18,0,0,4,0]],"ares_addrinfo_cat_cnames":[[126,7,126,12,0,0,4,0]],"ares_dns_rr_get_addr6":[[779,7,779,61,0,0,4,1],[784,7,784,19,0,0,4,1]],"ares_parse_a_reply":[[58,7,58,15,0,0,4,1],[62,7,62,16,0,0,4,0],[70,7,70,29,0,0,4,0],[70,7,70,29,0,0,4,1],[75,7,75,29,0,0,4,0],[75,7,75,29,0,0,4,1],[75,33,75,55,0,0,4,1],[79,7,79,19,0,0,4,0],[82,9,82,31,0,0,4,0],[82,9,82,31,0,0,4,1],[82,35,82,57,0,0,4,1],[87,7,87,23,0,0,4,0],[87,27,87,40,0,0,4,1],[102,7,102,30,0,0,4,0],[102,7,102,30,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_pending_write_cb","ares_parse_a_reply","ares_free_hostent","ares_parse_aaaa_reply","ares_free_hostent","ares_parse_naptr_reply","ares_getsock","ares_dns_opt_get_datatype","ares_inet_pton","ares_inet_pton","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_set_name","ares_dns_record_rr_add","ares_dns_rr_set_addr","ares_dns_rr_get_addr","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_pending_write_cb","ares_parse_a_reply","ares_free_hostent","ares_parse_aaaa_reply","ares_free_hostent","ares_parse_naptr_reply","ares_getsock","ares_dns_opt_get_datatype","ares_inet_pton","ares_inet_pton","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_set_name","ares_dns_record_rr_add","ares_dns_rr_set_addr","ares_dns_rr_get_addr","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

// Dummy callback function for ares_set_pending_write_cb
static void  id_000003_dummy_pending_write_cb(void *user_data) {
    // Do nothing - required callback for API
}

// Main fuzzing function
extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for basic operations
    if (size < 32) {
        return 0;
    }

    // Create a memory file for reading input
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // API 1: Set pending write callback
    ares_set_pending_write_cb(channel,  id_000003_dummy_pending_write_cb, nullptr);

    // API 2: Parse A reply using input data as DNS response
    struct hostent *host_a = nullptr;
    struct ares_addrttl addrttls[4];
    int naddrttls_a = 0;
    // Use a portion of input as DNS A reply (first half)
    size_t a_reply_size = size / 2;
    ares_parse_a_reply((const unsigned char *)data, a_reply_size, 
                      &host_a, addrttls, &naddrttls_a);
    if (host_a) {
        ares_free_hostent(host_a);
    }

    // API 3: Parse AAAA reply using second half of input
    struct hostent *host_aaaa = nullptr;
    struct ares_addr6ttl addr6ttls[4];
    int naddrttls_aaaa = 0;
    size_t aaaa_start = size / 2;
    size_t aaaa_size = size - aaaa_start;
    if (aaaa_size > 0) {
        ares_parse_aaaa_reply((const unsigned char *)data + aaaa_start, 
                             aaaa_size, &host_aaaa, addr6ttls, &naddrttls_aaaa);
        if (host_aaaa) {
            ares_free_hostent(host_aaaa);
        }
    }

    // API 4: Parse NAPTR reply using middle section of input
    struct ares_naptr_reply *naptr_out = nullptr;
    size_t naptr_start = size / 4;
    size_t naptr_size = size / 2;
    if (naptr_size > 0 && naptr_start + naptr_size <= size) {
        ares_parse_naptr_reply((const unsigned char *)data + naptr_start, 
                              naptr_size, &naptr_out);
        // Free NAPTR reply chain if allocated
        while (naptr_out) {
            struct ares_naptr_reply *next = naptr_out->next;
            free(naptr_out->flags);
            free(naptr_out->service);
            free(naptr_out->regexp);
            free(naptr_out->replacement);
            free(naptr_out);
            naptr_out = next;
        }
    }

    // API 5: Get sockets from channel
    ares_socket_t socks[ARES_GETSOCK_MAXNUM];
    int numsocks = ares_getsock(channel, socks, ARES_GETSOCK_MAXNUM);

    // API 6: Get datatype for DNS option (using first byte as key, second as opt)
    if (size >= 2) {
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)data[0];
        unsigned short opt = (unsigned short)data[1];
        ares_dns_opt_datatype_t datatype = ares_dns_opt_get_datatype(key, opt);
        // datatype variable intentionally unused - just checking it doesn't crash
        (void)datatype;
    }

    // API 7: Convert IP address string from input
    char ip_str[64] = {0};
    size_t ip_str_len = (size < 63) ? size : 63;
    memcpy(ip_str, data, ip_str_len);
    ip_str[ip_str_len] = '\0';
    
    // Try IPv4 conversion
    struct in_addr addr4;
    ares_inet_pton(AF_INET, ip_str, &addr4);
    
    // Try IPv6 conversion
    struct in6_addr addr6;
    ares_inet_pton(AF_INET6, ip_str, &addr6);

    // API 8: Create DNS record and set query name
    ares_dns_record_t *dnsrec = nullptr;
    if (ares_dns_record_create(&dnsrec, 12345, 
                               ARES_FLAG_QR | ARES_FLAG_RD,
                               ARES_OPCODE_QUERY,
                               ARES_RCODE_NOERROR) == ARES_SUCCESS) {
        // Add a query first
        ares_dns_record_query_add(dnsrec, "example.com", 
                                 ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        // Extract name from input for setting query name
        char query_name[256] = {0};
        size_t name_len = (size < 255) ? size : 255;
        for (size_t i = 0; i < name_len; i++) {
            if (data[i] >= 32 && data[i] <= 126) { // Printable ASCII
                query_name[i] = data[i];
            } else {
                query_name[i] = 'a' + (data[i] % 26);
            }
        }
        query_name[name_len] = '\0';
        
        // API 9: Set query name
        ares_dns_record_query_set_name(dnsrec, 0, query_name);
        
        // Get address from DNS RR if we had one (use the A reply parsing result)
        // This demonstrates ares_dns_rr_get_addr but we need a DNS RR first
        // Create a DNS RR for demonstration
        ares_dns_rr_t *rr = nullptr;
        if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                   "example.com", ARES_REC_TYPE_A,
                                   ARES_CLASS_IN, 300) == ARES_SUCCESS) {
            // Set an address for the RR
            struct in_addr demo_addr;
            demo_addr.s_addr = 0x01020304;
            ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &demo_addr);
            
            // Get the address back
            const struct in_addr *retrieved_addr = ares_dns_rr_get_addr(rr, ARES_RR_A_ADDR);
            (void)retrieved_addr; // Avoid unused variable warning
        }
        
        ares_dns_record_destroy(dnsrec);
    }

    // Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}