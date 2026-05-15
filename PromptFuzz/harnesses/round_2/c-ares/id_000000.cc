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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 70, nr_unique_branch: 32
//<Quality> {"density":10,"unique_branches":{"ares_timeout":[[140,7,140,22,0,0,4,1],[140,26,140,39,0,0,4,1]],"ares_dns_record_query_set_type":[[312,7,312,21,0,0,4,1],[312,25,312,58,0,0,4,1],[313,7,313,51,0,0,4,1]],"ares_strcpy":[[100,7,100,23,0,0,4,0]],"ares_dns_name_write":[[440,9,440,31,0,0,4,0]],"ares_getsock":[[40,7,40,22,0,0,4,1],[40,26,40,39,0,0,4,1],[48,57,48,70,0,0,4,0],[48,57,48,70,0,0,4,1],[53,61,53,73,0,0,4,1]],"ares_dns_write_header":[[72,7,72,35,0,0,4,1]],"ares_dns_rec_type_tostr":[[191,11,191,15,0,0,4,0],[192,5,192,25,0,0,4,1],[194,5,194,26,0,0,4,0],[196,5,196,29,0,0,4,0],[198,5,198,27,0,0,4,0],[200,5,200,27,0,0,4,0],[202,5,202,29,0,0,4,0],[204,5,204,26,0,0,4,0]],"ares_split_dns_name":[[343,33,343,77,0,0,4,0]],"ares_init_by_options":[[291,7,291,31,0,0,4,1],[295,7,295,35,0,0,4,1],[303,14,303,40,0,0,4,1],[314,7,314,31,0,0,4,1]],"ares_timeout_int":[[101,7,101,19,0,0,4,0]],"ares_qcache_calc_key":[[67,7,67,27,0,0,4,1]],"ares_nameoffset_create":[[51,7,51,30,0,0,4,0]],"ares_search_dnsrec":[[479,7,479,22,0,0,4,1],[479,26,479,40,0,0,4,1],[479,44,479,60,0,0,4,1]]},"library_calls":["ares_library_init","ares_init_options","ares_library_cleanup","ares_set_server_state_callback","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_set_type","ares_search_dnsrec","ares_timeout","ares_getsock","ares_process","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init_options","ares_set_server_state_callback","ares_dns_record_create","ares_dns_record_query_add","ares_dns_record_query_set_type","ares_search_dnsrec","ares_timeout","ares_getsock","ares_process","ares_dns_record_destroy","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


// Forward declaration of callback functions
static void  id_000000_server_state_callback(const char *name, ares_bool_t up, int flags, void *user_data);
static void  id_000000_search_callback(void *arg, ares_status_t status, size_t timeouts, const ares_dns_record_t *dnsrec);

// Structure to hold fuzzing context
struct fuzz_context {
    const uint8_t *data;
    size_t size;
    size_t offset;
    ares_channel_t *channel;
    bool initialized;
};

// Helper function to consume data from fuzzing input
static bool  id_000000_consume_bytes(fuzz_context *ctx, void *buf, size_t len) {
    if (ctx->offset + len > ctx->size) {
        return false;
    }
    memcpy(buf, ctx->data + ctx->offset, len);
    ctx->offset += len;
    return true;
}

// Helper function to consume a string from fuzzing input
static char * id_000000_consume_string(fuzz_context *ctx) {
    if (ctx->offset >= ctx->size) {
        return nullptr;
    }
    
    // Find null terminator or end of data
    size_t start = ctx->offset;
    size_t len = 0;
    while (ctx->offset + len < ctx->size && ctx->data[ctx->offset + len] != 0) {
        len++;
    }
    
    // Include null terminator if found
    if (ctx->offset + len < ctx->size) {
        len++; // Include null terminator
    }
    
    char *str = (char *)malloc(len + 1);
    if (!str) {
        return nullptr;
    }
    
    if (len > 0) {
        memcpy(str, ctx->data + ctx->offset, len);
        str[len] = '\0'; // Ensure null termination
    } else {
        str[0] = '\0';
    }
    
    ctx->offset += len;
    return str;
}

// Server state callback implementation
static void  id_000000_server_state_callback(const char *name, ares_bool_t up, int flags, void *user_data) {
    // This callback can be used for logging or debugging during fuzzing
    // For fuzzing purposes, we don't need to do anything substantial
    (void)name;
    (void)up;
    (void)flags;
    (void)user_data;
}

// Search callback implementation
static void  id_000000_search_callback(void *arg, ares_status_t status, size_t timeouts, const ares_dns_record_t *dnsrec) {
    fuzz_context *ctx = (fuzz_context *)arg;
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Get the opcode from the DNS record
        ares_dns_opcode_t opcode = ares_dns_record_get_opcode(dnsrec);
        (void)opcode; // Use opcode to avoid unused variable warning
        
        // Create an ares_addr structure from fuzzing data
        struct ares_addr addr;
        if (ctx->offset + sizeof(addr) <= ctx->size) {
            // Consume enough bytes for the address structure
            memcpy(&addr, ctx->data + ctx->offset, sizeof(addr));
            ctx->offset += sizeof(addr);
            
            // Convert address to PTR string
            char *ptr_str = ares_dns_addr_to_ptr(&addr);
            if (ptr_str) {
                // Use ares_inet_ntop to convert back (if it's an IP address)
                char dst[INET6_ADDRSTRLEN];
                const char *result = nullptr;
                
                if (addr.family == AF_INET) {
                    result = ares_inet_ntop(AF_INET, &addr.addr.addr4, dst, sizeof(dst));
                } else if (addr.family == AF_INET6) {
                    result = ares_inet_ntop(AF_INET6, &addr.addr.addr6, dst, sizeof(dst));
                }
                
                (void)result; // Use result to avoid unused variable warning
                ares_free_string(ptr_str);
            }
        }
    }
    
    // Clean up
    if (dnsrec) {
        ares_dns_record_destroy((ares_dns_record_t *)dnsrec);
    }
}

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize library if not already initialized
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    fuzz_context ctx;
    ctx.data = data;
    ctx.size = size;
    ctx.offset = 0;
    ctx.channel = nullptr;
    ctx.initialized = false;
    
    // Create ares channel
    struct ares_options options;
    int optmask = 0;
    memset(&options, 0, sizeof(options));
    
    if (ares_init_options(&ctx.channel, &options, optmask) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    ctx.initialized = true;
    
    // Set server state callback
    ares_set_server_state_callback(ctx.channel,  id_000000_server_state_callback, &ctx);
    
    // Create DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_record_create(&dnsrec, 1234, 0, ARES_OPCODE_QUERY, ARES_RCODE_NOERROR);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Consume domain name from fuzzing input
        char *domain_name =  id_000000_consume_string(&ctx);
        if (!domain_name) {
            domain_name = strdup("example.com");
        }
        
        // Add query to DNS record
        status = ares_dns_record_query_add(dnsrec, domain_name, ARES_REC_TYPE_A, ARES_CLASS_IN);
        
        if (status == ARES_SUCCESS) {
            // Set query type for the first query (index 0)
            // Consume query type from fuzzing input
            ares_dns_rec_type_t qtype = ARES_REC_TYPE_A;
            if (ctx.offset < ctx.size) {
                qtype = (ares_dns_rec_type_t)(data[ctx.offset] % 16); // Limit to valid range
                ctx.offset++;
            }
            
            status = ares_dns_record_query_set_type(dnsrec, 0, qtype);
            
            if (status == ARES_SUCCESS) {
                // Perform search with DNS record
                ares_search_dnsrec(ctx.channel, dnsrec,  id_000000_search_callback, &ctx);
                
                // Process events with timeout
                struct timeval maxtv, tv;
                maxtv.tv_sec = 0;
                maxtv.tv_usec = 10000; // 10ms timeout for fuzzing
                
                struct timeval *timeout = ares_timeout(ctx.channel, &maxtv, &tv);
                
                if (timeout) {
                    // Create file descriptors for processing
                    fd_set read_fds, write_fds;
                    FD_ZERO(&read_fds);
                    FD_ZERO(&write_fds);
                    
                    // Get sockets from channel
                    ares_socket_t socks[ARES_GETSOCK_MAXNUM];
                    int bitmask = ares_getsock(ctx.channel, socks, ARES_GETSOCK_MAXNUM);
                    
                    // Process available file descriptors
                    for (int i = 0; i < ARES_GETSOCK_MAXNUM; i++) {
                        if (ARES_GETSOCK_READABLE(bitmask, i)) {
                            FD_SET(socks[i], &read_fds);
                        }
                        if (ARES_GETSOCK_WRITABLE(bitmask, i)) {
                            FD_SET(socks[i], &write_fds);
                        }
                    }
                    
                    // Process with timeout
                    ares_process(ctx.channel, &read_fds, &write_fds);
                }
            }
        }
        
        free(domain_name);
        ares_dns_record_destroy(dnsrec);
    }
    
    // Clean up
    if (ctx.initialized) {
        ares_destroy(ctx.channel);
    }
    
    ares_library_cleanup();
    
    return 0;
}