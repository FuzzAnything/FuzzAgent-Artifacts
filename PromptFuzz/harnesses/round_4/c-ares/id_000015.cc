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
//<ID> 236
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 30
//<Quality> {"density":13,"unique_branches":{"ares_free_data":[[69,7,69,34,0,0,4,0]],"ares_parse_mx_reply":[[43,7,43,19,0,0,4,1],[50,7,50,29,0,0,4,0],[50,7,50,29,0,0,4,1],[54,7,54,63,0,0,4,0],[54,7,54,63,0,0,4,1],[59,15,59,70,0,0,4,0],[59,15,59,70,0,0,4,1],[63,9,63,19,0,0,4,1],[69,9,69,51,0,0,4,0],[69,9,69,51,0,0,4,1],[70,9,70,53,0,0,4,0],[70,9,70,53,0,0,4,1],[76,9,76,24,0,0,4,1],[82,9,82,16,0,0,4,0],[82,9,82,16,0,0,4,1],[92,9,92,30,0,0,4,1],[100,7,100,29,0,0,4,0],[100,7,100,29,0,0,4,1],[101,9,101,16,0,0,4,1]],"ares_malloc_data":[[147,5,147,32,0,0,4,0]],"ares_strerror":[[34,5,34,22,0,0,4,0],[36,5,36,22,0,0,4,0],[40,5,40,24,0,0,4,0],[42,5,42,24,0,0,4,0],[44,5,44,22,0,0,4,0],[46,5,46,23,0,0,4,0],[48,5,48,24,0,0,4,0],[52,5,52,25,0,0,4,0]],"ares_set_socket_functions_ex":[[116,9,116,35,0,0,4,0]]},"library_calls":["ares_library_init","ares_init_options","ares_destroy","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_get_servers_csv","ares_free_string","ares_strerror","ares_parse_mx_reply","ares_strerror","ares_free_data","ares_dns_parse","ares_dns_record_get_flags","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_rr_get_const","ares_dns_rr_get_type","ares_dns_rr_get_class","ares_dns_class_tostr","ares_dns_rr_get_ttl","ares_dns_record_destroy","ares_set_socket_functions_ex","ares_strerror","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_get_servers_csv","ares_free_string","ares_strerror","ares_parse_mx_reply","ares_strerror","ares_free_data","ares_dns_parse","ares_dns_record_get_flags","ares_dns_record_get_opcode","ares_dns_opcode_tostr","ares_dns_record_rr_get_const","ares_dns_rr_get_type","ares_dns_rr_get_class","ares_dns_class_tostr","ares_dns_rr_get_ttl","ares_dns_record_destroy","ares_set_socket_functions_ex","ares_strerror","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    // Initialize channel with default options
    if (ares_init_options(&channel, &options, fuzz_int32_t_1) != ARES_SUCCESS) {
        channel = nullptr;
    }

    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        if (channel) ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        if (channel) ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Use ares_get_servers_csv - get servers in CSV format
    if (channel) {
        char *servers_csv = ares_get_servers_csv(channel);
        if (servers_csv) {
            fputs(servers_csv, out_file);
            fputc('\n', out_file);
            ares_free_string(servers_csv);
        }
    }

    // 2. Use ares_strerror - convert error code to string
    for (int i = 0; i < 10; i++) {
        const char *error_str = ares_strerror(fuzz_int32_t_2);
        if (error_str) {
            fputs(error_str, out_file);
            fputc('\n', out_file);
        }
    }

    // 3. Parse MX reply from input data
    struct ares_mx_reply *mx_reply = nullptr;
    if (size > 0) {
        int mx_result = ares_parse_mx_reply(data, size, &mx_reply);
        
        // Log error using ares_strerror
        if (mx_result != ARES_SUCCESS) {
            const char *error_msg = ares_strerror(mx_result);
            if (error_msg) {
                fputs("MX Parse Error: ", out_file);
                fputs(error_msg, out_file);
                fputc('\n', out_file);
            }
        }
        
        // Process and free MX reply if successful
        if (mx_reply) {
            struct ares_mx_reply *current = mx_reply;
            while (current) {
                if (current->host) {
                    fputs(current->host, out_file);
                    fputc(':', out_file);
                    fprintf(out_file, "%d", current->priority);
                    fputc('\n', out_file);
                }
                current = current->next;
            }
            ares_free_data(mx_reply);
        }
    }

    // 4. Parse DNS record from input data and get query info
    ares_dns_record_t *dnsrec = nullptr;
    if (size > 0) {
        ares_status_t parse_status = ares_dns_parse(data, size, fuzz_uint32_t_3, &dnsrec);
        
        if (parse_status == ARES_SUCCESS && dnsrec) {
            // Get number of queries
            size_t query_count = 0;
            // Note: ares_dns_record_query_get is not in the provided API list
            // We'll use available APIs to work with the DNS record
            
            // Get DNS record flags
            unsigned short flags = ares_dns_record_get_flags(dnsrec);
            fprintf(out_file, "DNS Flags: %04x\n", flags);
            
            // Get opcode
            ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
            const char *opcode_str = ares_dns_opcode_tostr(opcode);
            if (opcode_str) {
                fputs("Opcode: ", out_file);
                fputs(opcode_str, out_file);
                fputc('\n', out_file);
            }
            
            // Try to read sections
            for (int sect = ARES_SECTION_ANSWER; sect <= ARES_SECTION_ADDITIONAL; sect++) {
                size_t rr_count = 0;
                // Get RR count for this section (simplified - actual API may differ)
                const ares_dns_rr_t *rr = ares_dns_record_rr_get_const(
                    dnsrec, (ares_dns_section_t)sect, fuzz_uint64_t_4);
                if (rr) {
                    fprintf(out_file, "Section %d has RRs\n", sect);
                    
                    // Get RR type
                    ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                    fprintf(out_file, "RR Type: %d\n", (int)rr_type);
                    
                    // Get RR class
                    ares_dns_class_t rr_class = ares_dns_rr_get_class(rr);
                    const char *class_str = ares_dns_class_tostr(rr_class);
                    if (class_str) {
                        fputs("Class: ", out_file);
                        fputs(class_str, out_file);
                        fputc('\n', out_file);
                    }
                    
                    // Get TTL
                    unsigned int ttl = ares_dns_rr_get_ttl(rr);
                    fprintf(out_file, "TTL: %u\n", ttl);
                }
            }
            
            ares_dns_record_destroy(dnsrec);
        }
    }

    // 5. Set server state callback
    if (channel) {
        // Define a simple callback
        auto server_state_cb = [](const char *server, ares_bool_t up, 
                                  int rtt, void *user_data) {
            FILE *f = (FILE *)user_data;
            if (f && server) {
                fprintf(f, "Server %s is %s (RTT: %d)\n", 
                        server, up ? "up" : "down", rtt);
            }
        };
        
        // Note: ares_set_server_state_callback is not in the provided API list
        // We'll use ares_set_socket_functions_ex instead for demonstration
    }

    // 6. Use ares_set_socket_functions_ex
    if (channel) {
        struct ares_socket_functions_ex funcs = {0};
        funcs.version = 1;  // Set version
        
        // Set basic socket function pointers (simplified)
        // In a real fuzzer, these would be implemented to intercept socket operations
        funcs.asocket = [](int domain, int type, int protocol, void *user_data) -> ares_socket_t {
            return socket(domain, type, protocol);
        };
        
        funcs.aclose = [](ares_socket_t sock, void *user_data) -> int {
            return close(sock);
        };
        
        ares_status_t set_func_status = ares_set_socket_functions_ex(channel, &funcs, out_file);
        if (set_func_status != ARES_SUCCESS) {
            const char *error_msg = ares_strerror(set_func_status);
            if (error_msg) {
                fputs("Set socket functions error: ", out_file);
                fputs(error_msg, out_file);
                fputc('\n', out_file);
            }
        }
    }

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    if (channel) {
        ares_destroy(channel);
    }
    
    ares_library_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}