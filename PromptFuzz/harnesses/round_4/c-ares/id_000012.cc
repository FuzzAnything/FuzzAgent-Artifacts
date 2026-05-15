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
//<ID> 188
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":4,"unique_branches":{},"library_calls":["ares_parse_txt_reply","ares_init_options","ares_timeout","ares_dns_rec_type_fromstr","ares_dns_opt_get_name","ares_destroy","ares_destroy_options","ares_free_data"],"critical_calls":["ares_parse_txt_reply","ares_init_options","ares_timeout","ares_dns_rec_type_fromstr","ares_dns_opt_get_name","ares_destroy","ares_destroy_options","ares_free_data"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint16_t, fuzz_uint16_t_3, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // 1. Parse TXT reply from the input data
    struct ares_txt_reply *txt_out = nullptr;
    int parse_result = ares_parse_txt_reply(data, static_cast<int>(size), &txt_out);
    
    // 2. Initialize c-ares channel with options
    ares_channel_t *channel = nullptr;
    struct ares_options options;
    int optmask = 0;
    
    memset(&options, 0, sizeof(options));
    options.timeout = 5000; // 5 second timeout
    options.tries = 2;
    optmask = ARES_OPT_TIMEOUTMS | ARES_OPT_TRIES;
    
    int init_result = ares_init_options(&channel, &options, fuzz_int32_t_1);
    
    if (init_result == ARES_SUCCESS) {
        // 3. Get timeout for the channel
        struct timeval maxtv;
        struct timeval tv;
        maxtv.tv_sec = 10;
        maxtv.tv_usec = 0;
        
        struct timeval *timeout = ares_timeout(channel, &maxtv, &tv);
        
        // 4. Convert string to DNS record type
        ares_dns_rec_type_t qtype;
        ares_bool_t conv_result = ares_dns_rec_type_fromstr(&qtype, fuzz_str_2);
        
        // 5. Get option name for DNS resource record
        // Using ARES_RR_OPT_OPTION as the key (value 0) and option 8 (EDNS0_OPTION_NSID)
        const char *opt_name = ares_dns_opt_get_name(static_cast<ares_dns_rr_key_t>(0), fuzz_uint16_t_3);
        
        // Clean up the channel
        ares_destroy(channel);
    }
    
    // 6. Destroy options structure
    ares_destroy_options(&options);
    
    // 7. Free TXT reply if it was allocated
    if (txt_out) {
        ares_free_data(txt_out);
    }
    
    return 0;
}