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
//<ID> 271
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 3
//<Quality> {"density":11,"unique_branches":{"ares_dns_rr_add_abin":[[940,5,940,38,0,0,4,1],[941,39,941,50,0,0,4,1],[945,7,945,59,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_local_dev","ares_set_local_dev","ares_set_socket_functions","ares_dns_record_create","ares_destroy","ares_library_cleanup","ares_dns_record_query_add","ares_dns_record_query_add","ares_dns_record_query_cnt","ares_dns_record_query_set_type","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_rr_add","ares_dns_rr_add_abin","ares_dns_rr_get_addr6","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_local_dev","ares_set_socket_functions","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_cnt","ares_dns_record_query_set_type","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_rr_add","ares_dns_rr_add_abin","ares_dns_rr_get_addr6","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Custom socket functions structure
static ares_socket_t  id_000023_my_asocket(int domain, int type, int protocol, void *user_data) {
    return socket(domain, type, protocol);
}

static int  id_000023_my_aclose(ares_socket_t socket_fd, void *user_data) {
    return close(socket_fd);
}

static int  id_000023_my_aconnect(ares_socket_t socket_fd, const struct sockaddr *addr, 
                      ares_socklen_t addrlen, void *user_data) {
    return connect(socket_fd, addr, addrlen);
}

static ares_ssize_t  id_000023_my_arecvfrom(ares_socket_t socket_fd, void *buf, size_t len,
                                 int flags, struct sockaddr *from, 
                                 ares_socklen_t *fromlen, void *user_data) {
    return recvfrom(socket_fd, buf, len, flags, from, fromlen);
}

static ares_ssize_t  id_000023_my_asendv(ares_socket_t socket_fd, const struct iovec *vec,
                              int len, void *user_data) {
    // Simplified sendv implementation
    ares_ssize_t total = 0;
    for (int i = 0; i < len; i++) {
        ssize_t sent = send(socket_fd, vec[i].iov_base, vec[i].iov_len, 0);
        if (sent < 0) return -1;
        total += sent;
    }
    return total;
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Create a channel
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 1. Set local device using first 32 bytes as device name (or less if size is smaller)
    const char *local_dev_name = "lo"; // default
    if (size > 0) {
        // Use up to 31 bytes for device name (plus null terminator)
        size_t dev_len = (size < 32) ? size : 31;
        char *dev_name = (char *)malloc(dev_len + 1);
        if (dev_name) {
            memcpy(dev_name, data, dev_len);
            dev_name[dev_len] = '\0';
            ares_set_local_dev(channel, dev_name);
            free(dev_name);
        }
    } else {
        ares_set_local_dev(channel, local_dev_name);
    }

    // 2. Set custom socket functions
    struct ares_socket_functions funcs;
    funcs.asocket =  id_000023_my_asocket;
    funcs.aclose =  id_000023_my_aclose;
    funcs.aconnect =  id_000023_my_aconnect;
    funcs.arecvfrom =  id_000023_my_arecvfrom;
    funcs.asendv =  id_000023_my_asendv;
    ares_set_socket_functions(channel, &funcs, nullptr);

    // 3. Create DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, 
                                                  12345, // ID
                                                  ARES_FLAG_QR | ARES_FLAG_AA, // Flags
                                                  ARES_OPCODE_QUERY, // Opcode
                                                  ARES_RCODE_NOERROR); // Rcode
    
    if (status != ARES_SUCCESS || !dnsrec) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }

    // 4. Add a query to the DNS record
    // Use data bytes 32-63 for query name (or less if size is smaller)
    const char *query_name = "example.com"; // default
    if (size > 32) {
        size_t name_len = (size - 32 < 32) ? size - 32 : 31;
        char *name = (char *)malloc(name_len + 1);
        if (name) {
            memcpy(name, data + 32, name_len);
            name[name_len] = '\0';
            status = ares_dns_record_query_add(dnsrec, name, 
                                               ARES_REC_TYPE_AAAA, 
                                               ARES_CLASS_IN);
            free(name);
        }
    } else {
        status = ares_dns_record_query_add(dnsrec, query_name, 
                                           ARES_REC_TYPE_AAAA, 
                                           ARES_CLASS_IN);
    }

    // 5. Set query type for the first query (index 0)
    if (ares_dns_record_query_cnt(dnsrec) > 0) {
        // Use data byte 64 to determine query type (mod 256)
        ares_dns_rec_type_t qtype = ARES_REC_TYPE_A; // default
        if (size > 64) {
            qtype = (ares_dns_rec_type_t)(data[64] % 16); // Limit to valid types
        }
        ares_dns_record_query_set_type(dnsrec, 0, qtype);
    }

    // 6. Get opcode and convert to string
    ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
    const char *opcode_str = ares_dns_opcode_tostr(opcode);
    (void)opcode_str; // Use variable to avoid unused warning

    // 7. Add an AAAA record to answer section
    ares_dns_rr_t *rr = nullptr;
    status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                    "example.com", ARES_REC_TYPE_AAAA,
                                    ARES_CLASS_IN, 300);
    
    if (status == ARES_SUCCESS && rr) {
        // 8. Set IPv6 address in the AAAA record using data bytes 65-80
        struct ares_in6_addr addr6;
        memset(&addr6, 0, sizeof(addr6));
        
        if (size > 80) {
            // Use bytes 65-80 for IPv6 address
            memcpy(&addr6, data + 65, 16);
        } else if (size > 65) {
            // Use available bytes
            memcpy(&addr6, data + 65, size - 65);
        }
        
        // Note: ares_dns_rr_set_addr6 is not in the provided API list,
        // so we use ares_dns_rr_add_abin as an alternative
        ares_dns_rr_add_abin(rr, ARES_RR_AAAA_ADDR, 
                            (const unsigned char *)&addr6, 
                            sizeof(struct ares_in6_addr));
        
        // 9. Get the IPv6 address back
        const struct ares_in6_addr *retrieved_addr = 
            ares_dns_rr_get_addr6(rr, ARES_RR_AAAA_ADDR);
        (void)retrieved_addr; // Use variable to avoid unused warning
    }

    // Clean up
    ares_dns_record_destroy(dnsrec);
    ares_destroy(channel);
    ares_library_cleanup();

    return 0;
}