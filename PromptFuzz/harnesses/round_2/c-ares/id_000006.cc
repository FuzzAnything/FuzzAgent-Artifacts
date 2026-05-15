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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 209, nr_unique_branch: 171
//<Quality> {"density":11,"unique_branches":{"ares_dns_write_rr_caa":[[867,7,867,29,0,0,4,1],[873,7,873,29,0,0,4,1],[879,7,879,19,0,0,4,1],[879,23,879,36,0,0,4,1]],"ares_nameoffset_find":[[98,39,98,52,0,0,4,1],[102,44,102,56,0,0,4,0],[102,44,102,56,0,0,4,1],[108,9,108,33,0,0,4,0],[108,9,108,33,0,0,4,1],[113,9,113,30,0,0,4,0],[113,9,113,30,0,0,4,1],[113,34,113,73,0,0,4,0],[113,34,113,73,0,0,4,1],[122,9,122,50,0,0,4,0],[122,9,122,50,0,0,4,1],[129,9,129,24,0,0,4,0],[129,9,129,24,0,0,4,1],[129,28,129,55,0,0,4,1]],"ares_dns_rr_get_bin":[[848,8,848,58,0,0,4,0],[848,8,848,58,0,0,4,1],[849,8,849,59,0,0,4,1],[851,7,851,18,0,0,4,1],[856,7,856,59,0,0,4,1],[869,7,869,18,0,0,4,1],[874,7,874,22,0,0,4,1]],"ares_malloc_data":[[154,5,154,38,0,0,4,0]],"ares_dns_write_rr_aaaa":[[526,7,526,19,0,0,4,1]],"ares_dns_write_rr_be32":[[294,7,294,57,0,0,4,1]],"ares_dns_write_rr_mx":[[433,7,433,29,0,0,4,1]],"ares_dns_write_buf":[[1121,7,1121,29,0,0,4,0],[1126,7,1126,29,0,0,4,0],[1131,7,1131,29,0,0,4,0]],"ares_dns_rr_get_opt_byid":[[1063,7,1063,10,0,0,4,0],[1066,7,1066,14,0,0,4,0],[1070,7,1070,57,0,0,4,1],[1075,7,1075,19,0,0,4,1],[1075,23,1075,36,0,0,4,0],[1075,23,1075,36,0,0,4,1],[1080,15,1080,22,0,0,4,0],[1080,15,1080,22,0,0,4,1],[1082,9,1082,23,0,0,4,1],[1085,9,1085,27,0,0,4,0],[1085,9,1085,27,0,0,4,1],[1090,7,1090,15,0,0,4,0],[1090,7,1090,15,0,0,4,1],[1090,19,1090,33,0,0,4,1],[1094,7,1094,10,0,0,4,0],[1097,7,1097,14,0,0,4,0]],"ares_set_socket_functions_ex":[[101,15,101,67,0,0,4,1],[102,9,102,44,0,0,4,1],[106,7,106,60,0,0,4,0]],"ares_array_claim_at":[[355,14,355,33,0,0,4,0],[359,9,359,31,0,0,4,1]],"ares_get_servers_ports":[[1174,7,1174,22,0,0,4,1],[1180,56,1180,68,0,0,4,0],[1180,56,1180,68,0,0,4,1],[1186,9,1186,19,0,0,4,1],[1190,9,1190,18,0,0,4,1],[1202,9,1202,37,0,0,4,0],[1211,7,1211,29,0,0,4,1]],"ares_dns_write_rr_str":[[202,7,202,18,0,0,4,1],[207,7,207,16,0,0,4,1],[213,7,213,29,0,0,4,1],[217,7,217,15,0,0,4,0],[217,7,217,15,0,0,4,1]],"ares_dns_write_binstr":[[238,9,238,18,0,0,4,1],[244,9,244,31,0,0,4,1],[249,9,249,12,0,0,4,0],[249,9,249,12,0,0,4,1],[251,11,251,33,0,0,4,1],[258,12,258,23,0,0,4,1]],"ares_dns_get_opt_rr_const":[[1553,15,1553,71,0,0,4,1]],"ares_dns_write_rr_name":[[186,7,186,19,0,0,4,1]],"ares_dns_write_rr_soa":[[360,7,360,29,0,0,4,1],[367,7,367,29,0,0,4,1],[373,7,373,29,0,0,4,1],[379,7,379,29,0,0,4,1],[385,7,385,29,0,0,4,1],[391,7,391,29,0,0,4,1]],"ares_dns_name_write":[[382,7,382,19,0,0,4,1],[384,9,384,20,0,0,4,0],[384,24,384,49,0,0,4,0],[384,24,384,49,0,0,4,1],[392,7,392,18,0,0,4,1],[392,22,392,52,0,0,4,0],[392,22,392,52,0,0,4,1],[417,9,417,20,0,0,4,1],[426,7,426,18,0,0,4,0],[430,9,430,31,0,0,4,1],[437,7,437,19,0,0,4,1],[437,24,437,35,0,0,4,1],[437,39,437,69,0,0,4,0],[437,39,437,69,0,0,4,1]],"ares_dns_write_header":[[52,7,52,35,0,0,4,0],[62,7,62,35,0,0,4,0],[67,7,67,35,0,0,4,0],[77,7,77,35,0,0,4,0],[84,7,84,35,0,0,4,0],[89,7,89,35,0,0,4,0],[94,7,94,25,0,0,4,0],[94,29,94,70,0,0,4,0]],"ares_dns_write_rr_uri":[[834,7,834,29,0,0,4,1],[840,7,840,29,0,0,4,1],[847,7,847,21,0,0,4,1],[847,25,847,49,0,0,4,1]],"ares_array_move":[[126,7,126,18,0,0,4,1],[126,22,126,48,0,0,4,1],[126,52,126,77,0,0,4,1],[131,7,131,26,0,0,4,1],[140,7,140,25,0,0,4,1]],"ares_parse_dns_name_escape":[[234,9,234,26,0,0,4,1],[242,7,242,24,0,0,4,1]],"ares_dns_write_rr":[[962,9,962,23,0,0,4,0],[969,9,969,31,0,0,4,0],[1007,7,1007,27,0,0,4,0],[1010,7,1010,28,0,0,4,0],[1013,7,1013,31,0,0,4,0],[1016,7,1016,29,0,0,4,0],[1019,7,1019,29,0,0,4,0],[1022,7,1022,31,0,0,4,0],[1025,7,1025,28,0,0,4,0],[1028,7,1028,29,0,0,4,0],[1034,7,1034,30,0,0,4,0],[1037,7,1037,29,0,0,4,0],[1046,7,1046,29,0,0,4,1],[1055,7,1055,31,0,0,4,0],[1058,7,1058,29,0,0,4,0],[1061,7,1061,29,0,0,4,0],[1064,7,1064,32,0,0,4,0],[1069,9,1069,31,0,0,4,0]],"ares_dns_write_rr_opt":[[673,9,673,12,0,0,4,1]],"ares_dns_write_rr_hinfo":[[417,7,417,29,0,0,4,1]],"ares_dns_write_rr_srv":[[541,7,541,29,0,0,4,1],[547,7,547,29,0,0,4,1],[553,7,553,29,0,0,4,1]],"ares_dns_write_rr_abin":[[271,7,271,15,0,0,4,1],[275,15,275,22,0,0,4,0],[275,15,275,22,0,0,4,1],[282,9,282,31,0,0,4,1]],"ares_dns_rr_del_opt_byid":[[1437,7,1437,57,0,0,4,1],[1442,7,1442,22,0,0,4,1],[1447,7,1447,23,0,0,4,1],[1452,17,1452,26,0,0,4,0],[1454,9,1454,23,0,0,4,1],[1457,9,1457,27,0,0,4,0],[1457,9,1457,27,0,0,4,1]],"ares_dns_rr_get_u32":[[830,7,830,57,0,0,4,1],[835,7,835,18,0,0,4,1]],"ares_dns_write_rr_https":[[781,7,781,29,0,0,4,1],[788,7,788,29,0,0,4,1],[793,15,793,68,0,0,4,0],[793,15,793,68,0,0,4,1],[802,9,802,31,0,0,4,1],[808,9,808,31,0,0,4,1],[813,9,813,12,0,0,4,0],[813,9,813,12,0,0,4,1],[813,16,813,23,0,0,4,0],[815,11,815,33,0,0,4,1]],"ares_nameoffset_create":[[55,7,55,20,0,0,4,1]],"ares_free_data":[[94,7,94,40,0,0,4,0]],"ares_dns_write_rr_u8":[[314,7,314,56,0,0,4,1]],"ares_dns_write_rr_raw_rr":[[899,7,899,15,0,0,4,1],[909,7,909,29,0,0,4,1],[914,7,914,29,0,0,4,1],[920,7,920,29,0,0,4,1],[926,7,926,19,0,0,4,0],[926,7,926,19,0,0,4,1],[930,7,930,20,0,0,4,1]],"ares_split_dns_name":[[307,9,307,26,0,0,4,1]],"ares_dns_write_rr_a":[[328,7,328,19,0,0,4,1]],"ares_dns_rcode_isvalid":[[58,5,58,28,0,0,4,0]]},"library_calls":["ares_library_init","ares_threadsafety","ares_init","ares_library_cleanup","ares_dns_parse","ares_dns_record_duplicate","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_rr_get_opt_byid","ares_dns_rr_del_opt_byid","ares_dns_rr_get_opt_byid","ares_dns_rr_del_opt_byid","ares_dns_record_destroy","ares_dns_record_destroy","ares_set_socket_functions_ex","ares_get_servers_ports","ares_free_data","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_threadsafety","ares_init","ares_dns_parse","ares_dns_record_duplicate","ares_dns_record_rr_cnt","ares_dns_record_rr_get","ares_dns_rr_get_type","ares_dns_rr_get_opt_byid","ares_dns_rr_del_opt_byid","ares_dns_rr_get_opt_byid","ares_dns_rr_del_opt_byid","ares_dns_record_destroy","ares_dns_record_destroy","ares_set_socket_functions_ex","ares_get_servers_ports","ares_free_data","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" {
}

// Custom socket functions structure
static ares_socket_t  id_000006_dummy_asocket(int domain, int type, int protocol, void *user_data) {
    return -1;
}

static int  id_000006_dummy_aclose(ares_socket_t socket_fd, void *user_data) {
    return 0;
}

static int  id_000006_dummy_asetsockopt(ares_socket_t socket_fd, ares_socket_opt_t optname, 
                           const void *optval, ares_socklen_t optlen, void *user_data) {
    return 0;
}

static int  id_000006_dummy_aconnect(ares_socket_t socket_fd, const struct sockaddr *addr,
                        ares_socklen_t addrlen, unsigned int flags, void *user_data) {
    return 0;
}

static ares_ssize_t  id_000006_dummy_arecvfrom(ares_socket_t socket_fd, void *buf, size_t len,
                                   int flags, struct sockaddr *from,
                                   ares_socklen_t *fromlen, void *user_data) {
    return -1;
}

static ares_ssize_t  id_000006_dummy_asendto(ares_socket_t socket_fd, const void *buf, size_t len,
                                 int flags, const struct sockaddr *to,
                                 ares_socklen_t tolen, void *user_data) {
    return -1;
}

static int  id_000006_dummy_agetsockname(ares_socket_t socket_fd, struct sockaddr *name,
                             ares_socklen_t *namelen, void *user_data) {
    return 0;
}

static int  id_000006_dummy_abind(ares_socket_t socket_fd, unsigned int flags,
                      const struct sockaddr *addr, socklen_t addrlen, void *user_data) {
    return 0;
}

static unsigned int  id_000006_dummy_aif_nametoindex(const char *ifname, void *user_data) {
    return 0;
}

static const char*  id_000006_dummy_aif_indextoname(unsigned int ifindex, char *ifname,
                                       size_t ifnamelen, void *user_data) {
    return NULL;
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    // Check thread safety
    if (!ares_threadsafety()) {
        // Library is not thread-safe, but we continue anyway
    }

    // Create a channel
    ares_channel_t *channel = NULL;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Parse the input data as DNS record
    ares_dns_record_t *dnsrec = NULL;
    ares_status_t parse_status = ares_dns_parse(data, size, 0, &dnsrec);
    
    if (parse_status == ARES_SUCCESS && dnsrec != NULL) {
        // Duplicate the DNS record
        ares_dns_record_t *duplicate_rec = ares_dns_record_duplicate(dnsrec);
        
        if (duplicate_rec != NULL) {
            // Get the number of answer records
            size_t answer_cnt = ares_dns_record_rr_cnt(duplicate_rec, ARES_SECTION_ANSWER);
            
            if (answer_cnt > 0) {
                // Get the first answer record
                ares_dns_rr_t *rr = ares_dns_record_rr_get(duplicate_rec, ARES_SECTION_ANSWER, 0);
                
                if (rr != NULL) {
                    // Check if this is an OPT record
                    ares_dns_rec_type_t rr_type = ares_dns_rr_get_type(rr);
                    
                    if (rr_type == ARES_REC_TYPE_OPT) {
                        // Try to get OPT options using get_opt_byid
                        const unsigned char *opt_val = NULL;
                        size_t opt_val_len = 0;
                        
                        // Try to get option ID 8 (EDNS Client Subnet)
                        if (ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, 8, 
                                                    &opt_val, &opt_val_len)) {
                            // Option exists, try to delete it
                            ares_status_t del_status = ares_dns_rr_del_opt_byid(rr, 
                                                                              ARES_RR_OPT_OPTIONS, 
                                                                              8);
                            // We don't check the result since this is fuzzing
                        }
                        
                        // Try another common option ID (3 for NSID)
                        if (ares_dns_rr_get_opt_byid(rr, ARES_RR_OPT_OPTIONS, 3,
                                                    &opt_val, &opt_val_len)) {
                            ares_dns_rr_del_opt_byid(rr, ARES_RR_OPT_OPTIONS, 3);
                        }
                    }
                }
            }
            
            // Clean up duplicate record
            ares_dns_record_destroy(duplicate_rec);
        }
        
        // Clean up original parsed record
        ares_dns_record_destroy(dnsrec);
    }

    // Set up custom socket functions
    struct ares_socket_functions_ex socket_funcs;
    memset(&socket_funcs, 0, sizeof(socket_funcs));
    socket_funcs.version = 0;  // Use version 0 for base functionality
    socket_funcs.flags = 0;
    socket_funcs.asocket =  id_000006_dummy_asocket;
    socket_funcs.aclose =  id_000006_dummy_aclose;
    socket_funcs.asetsockopt =  id_000006_dummy_asetsockopt;
    socket_funcs.aconnect =  id_000006_dummy_aconnect;
    socket_funcs.arecvfrom =  id_000006_dummy_arecvfrom;
    socket_funcs.asendto =  id_000006_dummy_asendto;
    socket_funcs.agetsockname =  id_000006_dummy_agetsockname;
    socket_funcs.abind =  id_000006_dummy_abind;
    socket_funcs.aif_nametoindex =  id_000006_dummy_aif_nametoindex;
    socket_funcs.aif_indextoname =  id_000006_dummy_aif_indextoname;

    // Set custom socket functions on the channel
    ares_status_t set_func_status = ares_set_socket_functions_ex(channel, &socket_funcs, NULL);

    // Get servers and ports from the channel
    struct ares_addr_port_node *servers = NULL;
    int get_servers_status = ares_get_servers_ports(channel, &servers);
    
    if (get_servers_status == ARES_SUCCESS && servers != NULL) {
        // Free the servers list
        ares_free_data(servers);
    }

    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file != NULL) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        // We could use the fd for something, but just having it is enough for fuzzing
        assert_file_closed(&in_file);;
    }

    // Create an output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file != NULL) {
        // Write some dummy data
        fwrite("test", 1, 4, out_file);
        assert_file_closed(&out_file);;
        
        // Remove the output file
        unlink("output_file");
    }

    // Clean up channel
    ares_destroy(channel);
    
    // Clean up library
    ares_library_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}