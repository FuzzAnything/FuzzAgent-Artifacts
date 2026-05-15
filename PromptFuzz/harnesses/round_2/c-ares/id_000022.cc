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
//<ID> 564
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 23
//<Quality> {"density":10,"unique_branches":{"ares_parse_sortlist":[[233,7,233,18,0,0,4,0],[261,9,261,31,0,0,4,0],[261,35,261,59,0,0,4,0],[261,35,261,59,0,0,4,1],[265,9,265,31,0,0,4,0],[281,7,281,29,0,0,4,0]],"parse_sort":[[125,7,125,29,0,0,4,0],[132,7,132,77,0,0,4,0],[138,7,138,29,0,0,4,0],[144,7,144,58,0,0,4,0],[160,9,161,10,0,0,4,0],[171,9,171,32,0,0,4,1],[177,11,177,38,0,0,4,1],[177,42,177,51,0,0,4,0],[188,11,188,62,0,0,4,0],[205,7,205,29,0,0,4,0]],"ip_natural_mask":[[78,7,78,31,0,0,4,0],[83,7,83,17,0,0,4,1],[87,7,87,17,0,0,4,0],[87,7,87,17,0,0,4,1]],"ares_set_sortlist":[[593,7,593,29,0,0,4,1],[593,33,593,41,0,0,4,1]],"ares_parse_aaaa_reply":[[90,7,90,23,0,0,4,1]]},"library_calls":["ares_library_init","ares_init","ares_library_cleanup","ares_destroy","ares_library_cleanup","ares_set_sortlist","ares_parse_aaaa_reply","ares_free_hostent","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_rec_type_fromstr","ares_dns_record_query_add","ares_dns_class_tostr","ares_dns_record_rr_cnt","ares_queue_wait_empty","ares_dns_record_destroy","ares_dns_parse","ares_dns_record_destroy","ares_cancel","ares_destroy","ares_library_cleanup"],"critical_calls":["ares_library_init","ares_init","ares_set_sortlist","ares_parse_aaaa_reply","ares_free_hostent","ares_dns_parse","ares_dns_record_get_rcode","ares_dns_rec_type_fromstr","ares_dns_record_query_add","ares_dns_class_tostr","ares_dns_record_rr_cnt","ares_queue_wait_empty","ares_dns_record_destroy","ares_dns_parse","ares_dns_record_destroy","ares_cancel","ares_destroy","ares_library_cleanup"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytesWithNullTerm(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize c-ares library
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    ares_channel_t *channel = nullptr;
    
    // Initialize channel
    if (ares_init(&channel) != ARES_SUCCESS) {
        ares_library_cleanup();
        return 0;
    }

    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        ares_destroy(channel);
        ares_library_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Read data for sortlist string (first 100 bytes or less)
    char sortstr[101] = {0};
    size_t read_size = fread(sortstr, 1, sizeof(sortstr) - 1, in_file);
    fseek(in_file, 0, SEEK_SET); // Reset file pointer

    // Set sortlist if we have data
    if (read_size > 0) {
        ares_set_sortlist(channel, sortstr);
    }

    // Parse AAAA reply from input data
    struct hostent *host = nullptr;
    struct ares_addr6ttl *addrttls = nullptr;
    int naddrttls = 0;
    
    // First, try to parse as AAAA reply
    unsigned char *buf = (unsigned char *)data;
    int alen = (int)(size > INT_MAX ? INT_MAX : size);
    
    int parse_result = ares_parse_aaaa_reply(buf, alen, &host, addrttls, &naddrttls);
    
    // Free hostent if allocated
    if (host) {
        ares_free_hostent(host);
    }

    // Now try to parse as DNS record using ares_dns_parse
    ares_dns_record_t *dnsrec = nullptr;
    ares_status_t status;
    
    // Parse DNS record (assuming ARES_DNS_PARSE_FLAG_RECORD is 0)
    status = ares_dns_parse(buf, size, 0, &dnsrec);
    
    if (status == ARES_SUCCESS && dnsrec) {
        // Get RCODE from DNS record
        ares_dns_rcode_t rcode = ares_dns_record_get_rcode(dnsrec);
        
        // Add a query to the DNS record (for completeness)
        ares_dns_rec_type_t qtype;
        const char *type_str = "AAAA";
        
        // Convert string to record type
        if (ares_dns_rec_type_fromstr(&qtype, type_str)) {
            // Add query with example domain
            ares_dns_record_query_add(dnsrec, "example.com", qtype, ARES_CLASS_IN);
        }
        
        // Get class as string
        const char *class_str = ares_dns_class_tostr(ARES_CLASS_IN);
        
        // Get count of resource records in answer section
        size_t rr_count = ares_dns_record_rr_cnt(dnsrec, ARES_SECTION_ANSWER);
        
        // Queue wait empty with short timeout
        ares_queue_wait_empty(channel, 10); // 10ms timeout
        
        // Destroy DNS record
        ares_dns_record_destroy(dnsrec);
    }

    // Try to parse again (second call as requested)
    status = ares_dns_parse(buf, size, 0, &dnsrec);
    if (status == ARES_SUCCESS && dnsrec) {
        ares_dns_record_destroy(dnsrec);
    }

    // Cancel any pending operations
    ares_cancel(channel);

    // Cleanup
    assert_file_closed(&in_file);;
    
    // Destroy channel
    ares_destroy(channel);
    
    // Library cleanup
    ares_library_cleanup();

    assert_file_closed(&in_file);
	return 0;
}