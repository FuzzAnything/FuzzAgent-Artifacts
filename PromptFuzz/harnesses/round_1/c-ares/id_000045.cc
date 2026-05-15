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
//<ID> 911
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 3
//<Quality> {"density":17,"unique_branches":{"nameinfo_callback":[[211,9,211,47,0,0,4,1],[225,9,225,40,0,0,4,0],[229,11,229,46,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_socket_callback","ares_parse_txt_reply_ext","ares_free_data","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_class","ares_dns_rr_get_type","ares_dns_rr_get_ttl","ares_dns_rr_get_keys","ares_dns_rr_key_tostr","ares_dns_rr_key_datatype","ares_dns_rr_get_abin","ares_dns_rr_get_u32","ares_dns_record_rr_get","ares_dns_rr_get_ttl","ares_dns_record_destroy","ares_getnameinfo","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_socket_callback","ares_parse_txt_reply_ext","ares_free_data","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_rr_cnt","ares_dns_record_rr_get_const","ares_dns_rr_get_class","ares_dns_rr_get_type","ares_dns_rr_get_ttl","ares_dns_rr_get_keys","ares_dns_rr_key_tostr","ares_dns_rr_key_datatype","ares_dns_rr_get_abin","ares_dns_record_rr_get","ares_dns_rr_get_ttl","ares_dns_record_destroy","ares_getnameinfo","ares_fds","ares_process","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


// Dummy callbacks for the required APIs
static void  id_000045_dummy_nameinfo_callback(void *arg, int status, int timeouts, 
                                     const char *node, const char *service) {
    // Do nothing - this is a fuzzing context
}

static ares_socket_t  id_000045_dummy_socket_callback(ares_socket_t socket_fd, int type, void *data) {
    // Return the socket as-is
    return socket_fd;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
	//fuzzer shim end}




    // Early return if input is too small for meaningful processing
    if (size < 4) {
        return 0;
    }

    // 1. Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t *channel = nullptr;
    
    // 2. Create a channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // 3. Set socket callback (required by the API list)
    ares_set_socket_callback(channel,  id_000045_dummy_socket_callback, nullptr);

    // 4. Parse the input as a TXT reply
    struct ares_txt_ext *txt_out = nullptr;
    int txt_result = ares_parse_txt_reply_ext(data, (int)size, &txt_out);
    
    // Free TXT reply if parsed successfully
    if (txt_out) {
        ares_free_data(txt_out);
    }

    // 5. Parse the input as a DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_1, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec) {
        // 6. Get RCODE from DNS record
        ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
        
        // 7. Iterate through all sections and resource records
        for (int sect = ARES_SECTION_ANSWER; sect <= ARES_SECTION_ADDITIONAL; sect++) {
            size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, (ares_dns_section_t)sect);
            
            for (size_t idx = 0; idx < rr_cnt; idx++) {
                // 8. Get const resource record
                const ares_dns_rr_t *const_rr = ares_dns_record_rr_get_const(dnsrec, 
                                                                            (ares_dns_section_t)sect, 
                                                                            idx);
                
                if (const_rr) {
                    // 9. Get class and type of RR
                    ares_dns_class_t rr_class = ares_dns_rr_get_class(const_rr);
                    ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(const_rr);
                    
                    // 10. Get TTL of RR
                    unsigned int ttl = ares_dns_rr_get_ttl(const_rr);
                    
                    // 11. Get keys for this RR type
                    size_t key_cnt = 0;
                    const ares_dns_rr_key_t *keys = ares_dns_rr_get_keys(rr_type, &key_cnt);
                    
                    // 12. Process each key
                    for (size_t key_idx = 0; key_idx < key_cnt; key_idx++) {
                        ares_dns_rr_key_t key = keys[key_idx];
                        
                        // 13. Get key name
                        const char *key_str = ares_dns_rr_key_tostr(key);
                        
                        // 14. Get datatype of key
                        ares_dns_datatype_t datatype = ares_dns_rr_key_datatype(key);
                        
                        // Handle different datatypes based on the key
                        switch (datatype) {
                            case ARES_DATATYPE_BIN:
                            case ARES_DATATYPE_STR:
                            case ARES_DATATYPE_NAME: {
                                // 15. Get binary data
                                size_t len = 0;
                                const unsigned char *bin_data = ares_dns_rr_get_abin(const_rr, 
                                                                                    key, fuzz_uint64_t_2, &len);
                                // Can use bin_data and len here if needed
                                break;
                            }
                            case ARES_DATATYPE_U32: {
                                // 16. Get unsigned 32-bit value
                                unsigned int u32_val = ares_dns_rr_get_u32(const_rr, key);
                                break;
                            }
                            default:
                                // Handle other datatypes if needed
                                break;
                        }
                    }
                }
                
                // 17. Also get non-const version (even if we don't modify it)
                ares_dns_rr_t *nonconst_rr = ares_dns_record_rr_get(dnsrec, 
                                                                    (ares_dns_section_t)sect, 
                                                                    fuzz_uint64_t_3);
                // Just check it's non-null, we won't modify in fuzzing context
                if (nonconst_rr) {
                    // Could get properties here too
                    unsigned int ttl2 = ares_dns_rr_get_ttl(nonconst_rr);
                }
            }
        }
        
        // 18. Clean up DNS record
        ares_dns_record_destroy(dnsrec);
    }
    
    // 19. Test getnameinfo with a dummy socket address
    // Create a minimal valid sockaddr_in structure
    if (size >= sizeof(struct in_addr)) {
        struct sockaddr_in sa;
        memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        
        // Use first 4 bytes of input for IP address if available
        size_t ip_len = (size < sizeof(sa.sin_addr)) ? size : sizeof(sa.sin_addr);
        memcpy(&sa.sin_addr, data, ip_len);
        
        // 20. Call getnameinfo (async, callback will be called later)
        ares_getnameinfo(channel, (const struct sockaddr*)&sa, sizeof(sa), 
                         ARES_NI_NOFQDN,  id_000045_dummy_nameinfo_callback, nullptr);
        
        // Process any pending requests
        fd_set read_fds, write_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        
        int nfds = ares_fds(channel, &read_fds, &write_fds);
        if (nfds > 0) {
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            
            // 21. Process the channel
            ares_process(channel, &read_fds, &write_fds);
        }
    }
    
    // 22. Clean up channel and library
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}