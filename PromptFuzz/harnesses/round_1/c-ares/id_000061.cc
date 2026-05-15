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
//<ID> 1138
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"ares_dns_rr_get_u8":[[796,7,796,56,0,0,4,0]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_servers_csv","ares_dns_record_create","ares_destroy","ares_library_cleanup","ares_dns_record_destroy","ares_destroy","ares_library_cleanup","ares_dns_record_rr_add","ares_dns_record_destroy","ares_destroy","ares_library_cleanup","ares_dns_rr_set_addr","ares_dns_rr_set_opt","ares_set_socket_callback","ares_search","ares_parse_aaaa_reply","ares_free_hostent","ares_fds","ares_process_fd","ares_queue_wait_empty","ares_dns_rr_get_ttl","ares_dns_rr_get_u8","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_servers_csv","ares_dns_record_create","ares_dns_record_rr_add","ares_dns_rr_set_addr","ares_dns_rr_set_opt","ares_set_socket_callback","ares_search","ares_parse_aaaa_reply","ares_free_hostent","ares_fds","ares_process_fd","ares_queue_wait_empty","ares_dns_rr_get_ttl","ares_dns_rr_get_u8","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" {
}

// Dummy callbacks
static void  id_000061_dummy_callback(void* arg, int status, int timeouts, 
                          const unsigned char* abuf, int alen) {}

static void  id_000061_dummy_host_callback(void* arg, int status, 
                               int timeouts, struct hostent* host) {}

static ares_socket_t  id_000061_dummy_socket_callback(ares_socket_t socket_fd,
                                          int type, void* user_data) {
    return socket_fd;
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<41) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	
    
    // 1. Initialize library
    int init_flags = ARES_LIB_INIT_ALL;
    if (ares_library_init(init_flags) != ARES_SUCCESS) {
        return 0;
    }
    
    // 2. Create channel
    ares_channel_t* channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 3. Set servers from CSV (use first 50 bytes as CSV string)
    char csv_buf[51] = {0};
    size_t csv_len = size > 50 ? 50 : size;
    memcpy(csv_buf, data, csv_len);
    csv_buf[csv_len] = '\0';
    ares_set_servers_csv(channel, csv_buf);
    
    // 4. Create DNS record
    ares_dns_record_t* dnsrec = nullptr;
    uint16_t id = *reinterpret_cast<const uint16_t*>(data + 50);
    uint16_t flags = *reinterpret_cast<const uint16_t*>(data + 52);
    ares_dns_opcode_t opcode = ARES_OPCODE_QUERY;
    ares_dns_rcode_t rcode = ARES_RCODE_NOERROR;
    
    if (ares_dns_record_create(&dnsrec, id, flags, opcode, rcode) != ARES_SUCCESS) {
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    
    // 5. Add resource record (skip 54 bytes used so far)
    size_t offset = 54;
    if (offset + 20 >= size) {
        ares_dns_record_destroy(dnsrec);
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    
    ares_dns_rr_t* rr = nullptr;
    const char* rr_name = "example.com";
    ares_dns_rec_type_t rr_type = ARES_REC_TYPE_A;
    ares_dns_class_t rr_class = ARES_CLASS_IN;
    unsigned int ttl = 300;
    
    if (ares_dns_record_rr_add(&rr, dnsrec, ARES_SECTION_ANSWER, 
                              fuzz_str_1, rr_type, rr_class, fuzz_uint32_t_2) != ARES_SUCCESS) {
        ares_dns_record_destroy(dnsrec);
        ares_destroy(channel);
        ares_library_cleanup();
        return 0;
    }
    
    // 6. Set address in RR
    struct in_addr addr;
    if (offset + sizeof(struct in_addr) <= size) {
        memcpy(&addr, data + offset, sizeof(struct in_addr));
        offset += sizeof(struct in_addr);
        ares_dns_rr_set_addr(rr, ARES_RR_A_ADDR, &addr);
    }
    
    // 7. Set option in RR (skip 2 bytes for opt, use next 2 for val_len)
    if (offset + 4 <= size) {
        unsigned short opt = *reinterpret_cast<const unsigned short*>(data + offset);
        offset += 2;
        unsigned short val_len = *reinterpret_cast<const unsigned short*>(data + offset);
        offset += 2;
        
        if (val_len > 0 && offset + val_len <= size) {
            ares_dns_rr_set_opt(rr, ARES_RR_OPT_OPTIONS, opt, 
                               data + offset, val_len);
            offset += val_len;
        }
    }
    
    // 8. Set socket callback
    ares_set_socket_callback(channel,  id_000061_dummy_socket_callback, nullptr);
    
    // 9. Perform DNS search
    ares_search(channel, fuzz_str_3, C_IN, T_AAAA, 
                 id_000061_dummy_callback, nullptr);
    
    // 10. Parse AAAA reply from remaining data
    if (offset < size) {
        size_t remaining = size - offset;
        struct hostent* host = nullptr;
        struct ares_addr6ttl addrttls[10];
        int naddrttls = 0;
        
        ares_parse_aaaa_reply(data + offset, remaining, 
                             &host, addrttls, &naddrttls);
        if (host) {
            ares_free_hostent(host);
        }
    }
    
    // 11. Get file descriptors
    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    int nfds = ares_fds(channel, &read_fds, &write_fds);
    
    // 12. Process file descriptors
    if (nfds > 0) {
        // Create a file from input data for processing
        FILE* in_file = fmemopen((void*)data, size, "rb");
        if (in_file) {
            int fd = fuzz_fileno(in_file);
            ares_process_fd(channel, fd, ARES_SOCKET_BAD);
            assert_file_closed(&in_file);;
        }
    }
    
    // 13. Wait for queue to empty
    ares_queue_wait_empty(channel, fuzz_int32_t_4);
    
    // 14. Get TTL from RR
    if (rr) {
        unsigned int rr_ttl = ares_dns_rr_get_ttl(rr);
        (void)rr_ttl; // Prevent unused warning
    }
    
    // 15. Get U8 value from RR
    if (rr) {
        unsigned char u8_val = ares_dns_rr_get_u8(rr, ARES_RR_A_ADDR);
        (void)u8_val; // Prevent unused warning
    }
    
    // 16. Clean up
    ares_dns_record_destroy(dnsrec);
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}