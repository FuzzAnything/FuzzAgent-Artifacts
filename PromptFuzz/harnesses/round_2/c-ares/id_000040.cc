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
//<ID> 982
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"ares_init_by_options":[[461,7,461,37,0,0,4,0]]},"library_calls":["ares_version","ares_init_options","ares_dup","ares_destroy","ares_set_socket_functions_ex","ares_destroy","ares_destroy","ares_set_server_state_callback","ares_set_local_ip6","ares_get_servers","ares_parse_srv_reply","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_rr_get_str","ares_dns_record_destroy","ares_destroy","ares_destroy"],"critical_calls":["ares_version","ares_init_options","ares_dup","ares_set_socket_functions_ex","ares_set_server_state_callback","ares_set_local_ip6","ares_get_servers","ares_parse_srv_reply","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_rr_add","ares_dns_rr_set_bin","ares_dns_rr_get_str","ares_dns_record_destroy","ares_destroy","ares_destroy"],"visited":0}
/**/


// Custom socket functions structure
static ares_socket_t  id_000040_dummy_socket(int domain, int type, int protocol, void *user_data) {
    return -1;  // Return invalid socket
}

static int  id_000040_dummy_close(ares_socket_t socket_fd, void *user_data) {
    return 0;  // Always succeed
}

static int  id_000040_dummy_setsockopt(ares_socket_t socket_fd, ares_socket_opt_t option, 
                           const void *optval, ares_socklen_t optlen, void *user_data) {
    return 0;  // Always succeed
}

static int  id_000040_dummy_connect(ares_socket_t socket_fd, const struct sockaddr *addr,
                        ares_socklen_t addrlen, unsigned int flags, void *user_data) {
    return 0;  // Always succeed
}

static ares_ssize_t  id_000040_dummy_recvfrom(ares_socket_t socket_fd, void *buf, size_t len,
                                  int flags, struct sockaddr *src_addr,
                                  ares_socklen_t *src_addr_len, void *user_data) {
    return -1;  // No data available
}

static ares_ssize_t  id_000040_dummy_sendto(ares_socket_t socket_fd, const void *buf, size_t len,
                                int flags, const struct sockaddr *dest_addr,
                                ares_socklen_t dest_addr_len, void *user_data) {
    return -1;  // Send failed
}

static int  id_000040_dummy_getsockname(ares_socket_t socket_fd, struct sockaddr *addr,
                            ares_socklen_t *addrlen, void *user_data) {
    return -1;  // Failed
}

static int  id_000040_dummy_bind(ares_socket_t socket_fd, unsigned int family,
                     const struct sockaddr *addr, socklen_t addrlen, void *user_data) {
    return -1;  // Failed
}

static unsigned int  id_000040_dummy_if_nametoindex(const char *ifname, void *user_data) {
    return 0;  // Invalid index
}

static const char * id_000040_dummy_if_indextoname(unsigned int ifindex, char *ifname,
                                       size_t ifnamelen, void *user_data) {
    return NULL;  // Failed
}

// Server state callback
static void  id_000040_server_state_callback(const char *name, ares_bool_t usable,
                                 int type, void *user_data) {
    // Do nothing in fuzzing context
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure minimal input size
    if (size < 4) {
	return 0;
	}
	
    
    // 1. Get c-ares version
    int version;
    const char *version_str = ares_version(&version);
    if (!version_str) {
	return 0;
	}
	
    
    // 2. Initialize ares channel
    ares_channel_t *channel = nullptr;
    ares_channel_t *dup_channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    if (ares_init_options(&channel, &options, optmask) != ARES_SUCCESS) {
        return 0;
    }
    
    // 3. Duplicate the channel (using ares_dup)
    if (ares_dup(&dup_channel, channel) != ARES_SUCCESS) {
        ares_destroy(channel);
        return 0;
    }
    
    // 4. Set custom socket functions
    struct ares_socket_functions_ex socket_funcs;
    memset(&socket_funcs, 0, sizeof(socket_funcs));
    socket_funcs.version = 0;
    socket_funcs.flags = 0;
    socket_funcs.asocket =  id_000040_dummy_socket;
    socket_funcs.aclose =  id_000040_dummy_close;
    socket_funcs.asetsockopt =  id_000040_dummy_setsockopt;
    socket_funcs.aconnect =  id_000040_dummy_connect;
    socket_funcs.arecvfrom =  id_000040_dummy_recvfrom;
    socket_funcs.asendto =  id_000040_dummy_sendto;
    socket_funcs.agetsockname =  id_000040_dummy_getsockname;
    socket_funcs.abind =  id_000040_dummy_bind;
    socket_funcs.aif_nametoindex =  id_000040_dummy_if_nametoindex;
    socket_funcs.aif_indextoname =  id_000040_dummy_if_indextoname;
    
    ares_status_t status = ares_set_socket_functions_ex(dup_channel, &socket_funcs, nullptr);
    if (status != ARES_SUCCESS) {
        ares_destroy(dup_channel);
        ares_destroy(channel);
        return 0;
    }
    
    // 5. Set server state callback
    ares_set_server_state_callback(dup_channel,  id_000040_server_state_callback, nullptr);
    
    // 6. Set local IPv6 address (use first 16 bytes of input if available)
    unsigned char local_ip6[16] = {0};
    if (size >= 16) {
        memcpy(local_ip6, data, 16);
    } else {
        // Fill with pattern if not enough data
        for (size_t i = 0; i < 16 && i < size; i++) {
            local_ip6[i] = data[i];
        }
    }
    ares_set_local_ip6(dup_channel, local_ip6);
    
    // 7. Get servers from channel
    struct ares_addr_node *servers = nullptr;
    if (ares_get_servers(dup_channel, &servers) == ARES_SUCCESS) {
        // Clean up servers list
        struct ares_addr_node *server = servers;
        while (server) {
            struct ares_addr_node *next = server->next;
            free(server);
            server = next;
        }
    }
    
    // 8. Parse input as SRV reply (use part of input data)
    struct ares_srv_reply *srv_out = nullptr;
    size_t parse_size = size > 1024 ? 1024 : size;  // Limit parse size
    int parse_result = ares_parse_srv_reply(data, parse_size, &srv_out);
    
    // Clean up SRV reply if parsed
    if (srv_out) {
        struct ares_srv_reply *srv = srv_out;
        while (srv) {
            struct ares_srv_reply *next = srv->next;
            if (srv->host) free(srv->host);
            free(srv);
            srv = next;
        }
    }
    
    // 9. Create DNS record and test ares_dns_rr_get_str
    ares_dns_record_t *dnsrec = nullptr;
    status = ares_dns_record_create(&dnsrec, 12345, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Add a query section
        status = ares_dns_record_query_add(dnsrec, "example.com", 
                                          ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        if (status == ARES_SUCCESS) {
            // Add answer section with CNAME record
            ares_dns_rr_t *rr = nullptr;
            status = ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER,
                                          "www.example.com", ARES_REC_TYPE_CNAME,
                                          ARES_CLASS_IN, 300);
            
            if (status == ARES_SUCCESS && rr) {
                // Set CNAME target using input data (limited to 255 bytes)
                size_t cname_len = size > 255 ? 255 : size;
                char cname_target[256] = {0};
                if (cname_len > 0) {
                    memcpy(cname_target, data, cname_len);
                    cname_target[cname_len] = '\0';
                } else {
                    strcpy(cname_target, "target.example.com");
                }
                
                // Use ares_dns_rr_set_bin to set the CNAME (as binary string)
                status = ares_dns_rr_set_bin(rr, ARES_RR_CNAME_CNAME,
                                           (const unsigned char*)cname_target,
                                           strlen(cname_target) + 1);
                
                // Now get the string using ares_dns_rr_get_str
                const char *cname_str = ares_dns_rr_get_str(rr, ARES_RR_CNAME_CNAME);
                // cname_str may be NULL if setting failed, which is OK for fuzzing
            }
        }
        
        // Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // Clean up channels
    ares_destroy(dup_channel);
    ares_destroy(channel);
    
    return 0;
}