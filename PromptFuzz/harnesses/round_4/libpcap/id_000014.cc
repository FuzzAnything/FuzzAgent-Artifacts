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
#include <pcap/bluetooth.h>
}
extern "C" {
#include <pcap/can_socketcan.h>
}
extern "C" {
#include <pcap/compiler-tests.h>
}
extern "C" {
#include <pcap/dlt.h>
}
extern "C" {
#include <pcap/funcattrs.h>
}
extern "C" {
#include <pcap/ipnet.h>
}
extern "C" {
#include <pcap/nflog.h>
}
extern "C" {
#include <pcap/pcap-inttypes.h>
}
extern "C" {
#include <pcap/pcap.h>
}
extern "C" {
#include <pcap/sll.h>
}
extern "C" {
#include <pcap/socket.h>
}
extern "C" {
#include <pcap/usb.h>
}
extern "C" {
#include <pcap/vlan.h>
}
//<ID> 395
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 7
//<Quality> {"density":18,"unique_branches":{"pcap_lookupdev":[[1498,6,1498,21,0,0,4,1],[1504,6,1504,46,0,0,4,1],[1507,6,1507,21,0,0,4,1],[1507,25,1507,60,0,0,4,1]],"gen_bsd_af_linktype_offline":[[2993,2,2993,21,0,0,4,0]],"pcap_tstamp_type_val_to_description":[[3510,14,3510,49,0,0,4,0],[3511,7,3511,49,0,0,4,0]]},"library_calls":["pcap_fopen_offline","pcap_set_tstamp_precision","pcap_tstamp_type_val_to_description","pcap_list_datalinks","pcap_set_datalink","pcap_free_datalinks","pcap_lookupdev","pcap_dump_open","pcap_close","pcap_compile","pcap_next","pcap_offline_filter","pcap_dump","pcap_freecode","pcap_inject","pcap_stats","pcap_statustostr","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_set_tstamp_precision","pcap_tstamp_type_val_to_description","pcap_list_datalinks","pcap_set_datalink","pcap_free_datalinks","pcap_lookupdev","pcap_dump_open","pcap_compile","pcap_next","pcap_offline_filter","pcap_dump","pcap_freecode","pcap_inject","pcap_stats","pcap_statustostr","pcap_dump_close","pcap_close"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified libpcap APIs to process a pcap file from input data, filter packets, and write them to an output file:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<42) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
	//fuzzer shim end}




    if (size < sizeof(struct pcap_pkthdr)) {
        return 0;
    }

    // 1. Create input FILE* from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Open pcap file for reading with default timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Set timestamp precision (microsecond or nanosecond)
    int precision = PCAP_TSTAMP_PRECISION_MICRO;
    if (size > 100) {
        precision = PCAP_TSTAMP_PRECISION_NANO;
    }
    pcap_set_tstamp_precision(pcap, fuzz_int32_t_1);

    // 4. Get timestamp type description
    const char *ts_desc = pcap_tstamp_type_val_to_description(fuzz_int32_t_2);
    if (ts_desc) {
        // Reference ts_desc to avoid unused variable warning
        (void)ts_desc;
    }

    // 5. Get list of datalinks and free it
    int *dlt_list = nullptr;
    int dlt_count = pcap_list_datalinks(pcap, &dlt_list);
    if (dlt_count > 0 && dlt_list) {
        // Try to set datalink to first available type
        if (dlt_count > 1) {
            pcap_set_datalink(pcap, dlt_list[0]);
        }
        pcap_free_datalinks(dlt_list);
    }

    // 6. Lookup default network device
    char *dev = pcap_lookupdev(errbuf);
    if (dev) {
        // Reference dev to avoid unused variable warning
        (void)dev;
    }

    // 7. Open dumper for output
    pcap_dumper_t *dumper = pcap_dump_open(pcap, "output_file");
    if (!dumper) {
        pcap_close(pcap);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // 8. Create a simple filter program (capture all packets)
    struct bpf_program filter;
    const char *filter_str = "ip or arp or tcp or udp";
    bpf_u_int32 netmask = 0xffffff00; // 255.255.255.0
    
    if (pcap_compile(pcap, &filter, fuzz_str_3, fuzz_int32_t_4, fuzz_uint32_t_5) == 0) {
        // 9. Process packets with offline filter
        struct pcap_pkthdr header;
        const u_char *packet;
        
        while ((packet = pcap_next(pcap, &header)) != nullptr) {
            // Apply filter to each packet
            if (pcap_offline_filter(&filter, &header, packet)) {
                // Write filtered packet to output
                pcap_dump((u_char *)dumper, &header, packet);
            }
        }
        
        pcap_freecode(&filter);
    }

    // 10. Try to inject a packet (simulate sending)
    if (size > sizeof(struct pcap_pkthdr)) {
        // Use first part of data as packet to inject
        size_t inject_size = size > 1500 ? 1500 : size;
        pcap_inject(pcap, data, inject_size);
    }

    // 11. Convert status to string for error reporting
    int status = pcap_stats(pcap, nullptr);
    if (status != 0) {
        const char *status_str = pcap_statustostr(status);
        if (status_str) {
            // Reference status_str to avoid unused variable warning
            (void)status_str;
        }
    }

    // 12. Clean up resources
    pcap_dump_close(dumper);
    pcap_close(pcap);
    assert_file_closed(&in_file);;

    // 13. Remove output file if created
    remove("output_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
}