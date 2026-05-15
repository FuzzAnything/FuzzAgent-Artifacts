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
//<ID> 563
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 8
//<Quality> {"density":7,"unique_branches":{"parse_sort":[[149,7,149,63,0,0,4,0],[160,9,161,10,0,0,4,1],[167,9,167,31,0,0,4,1],[171,9,171,32,0,0,4,0],[174,11,174,19,0,0,4,1],[174,23,174,33,0,0,4,1],[177,11,177,38,0,0,4,0],[177,42,177,51,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_set_sortlist","ares_parse_aaaa_reply","ares_free_hostent","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_query_add","ares_dns_rec_type_fromstr","ares_dns_record_rr_cnt","ares_queue_wait_empty","ares_dns_class_tostr","ares_dns_record_destroy","ares_init","ares_reinit","ares_destroy","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_sortlist","ares_parse_aaaa_reply","ares_free_hostent","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_record_query_add","ares_dns_rec_type_fromstr","ares_dns_record_rr_cnt","ares_queue_wait_empty","ares_dns_class_tostr","ares_dns_record_destroy","ares_init","ares_reinit","ares_destroy","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize the c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }
    
    // 2. Create a channel
    ares_channel_t *channel = nullptr;
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }
    
    // 3. Set sortlist (using a simple default value)
    ares_set_sortlist(channel, "1.2.3.4/24 2.3.4.5");
    
    // 4. Parse the input as both AAAA reply and generic DNS record
    struct hostent *host = nullptr;
    struct ares_addr6ttl addrttls[10];
    int naddrttls = 0;
    
    // Try parsing as AAAA reply
    ares_parse_aaaa_reply(data, static_cast<int>(size), &host, addrttls, &naddrttls);
    if (host) {
        ares_free_hostent(host);
        host = nullptr;
    }
    
    // 5. Parse as generic DNS record
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status = ares_dns_parse(data, size, 0, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // 6. Get the rcode from DNS record
        ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
        (void)rcode; // Use it to avoid unused variable warning
        
        // 7. Add a query to the DNS record (modify it in memory)
        ares_dns_record_query_add(dnsrec, "example.com", ARES_REC_TYPE_AAAA, ARES_CLASS_IN);
        
        // 8. Convert string to record type
        ares_dns_rec_type_t qtype;
        ares_dns_rec_type_fromstr(&qtype, "AAAA");
        
        // 9. Get count of records in answer section
        size_t rr_cnt = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        (void)rr_cnt;
        
        // 10. Wait for any pending operations (with minimal timeout)
        ares_queue_wait_empty(channel, 0);
        
        // 11. Convert class to string for demonstration
        const char *class_str = ares_dns_class_tostr(ARES_CLASS_IN);
        (void)class_str;
        
        ares_dns_record_destroy(dnsrec);
    }
    
    // 12. Try to reinitialize the channel (this is tricky with existing channel)
    // Create a new channel for reinit demonstration
    ares_channel_t *channel2 = nullptr;
    if (ares_init(&channel2) == ARES_SUCCESS) {
        ares_status_t reinit_status = ares_reinit(channel2);
        (void)reinit_status;
        ares_destroy(channel2);
    }
    
    // 13. Cancel any pending operations
    ares_cancel(channel);
    
    // 14. Clean up
    ares_destroy(channel);
    ares_library_cleanup();
    
    return 0;
}