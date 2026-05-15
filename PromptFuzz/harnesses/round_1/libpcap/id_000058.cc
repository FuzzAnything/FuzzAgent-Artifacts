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
//<ID> 313
//<Prompt> []
/*<Combination>: [
*/
//<score> 75, nr_unique_branch: 8
//<Quality> {"density":15,"unique_branches":{"pcap_check_header":[[337,2,337,34,0,0,4,1],[354,2,354,33,0,0,4,0],[355,7,355,38,0,0,4,0],[355,7,355,38,0,0,4,1]],"pcap_next_packet":[[564,2,564,15,0,0,4,0]],"pcap_ng_check_header":[[894,2,894,34,0,0,4,1],[898,2,898,33,0,0,4,0]],"sf_write_header":[[788,14,788,67,0,0,4,0]]},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_bufsize","pcap_list_tstamp_types","pcap_statustostr","pcap_free_tstamp_types","pcap_getnonblock","pcap_compile","pcap_dump_fopen","pcap_next","pcap_offline_filter","pcap_dump","pcap_dump_flush","pcap_dump_close","pcap_freecode","pcap_freealldevs","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_bufsize","pcap_list_tstamp_types","pcap_statustostr","pcap_free_tstamp_types","pcap_getnonblock","pcap_compile","pcap_dump_fopen","pcap_next","pcap_offline_filter","pcap_dump","pcap_dump_flush","pcap_dump_close","pcap_freecode","pcap_freealldevs","pcap_close"],"visited":0}
/*Here's a fuzzer that tests libpcap's ability to read PCAP files with different timestamp precisions, apply BPF filters, and analyze packets:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 24) {
	return 0;
	}
	  // Minimum size for basic PCAP header

    // 1. Create input FILE* from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Open PCAP file with nanosecond timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap = pcap_fopen_offline_with_tstamp_precision(in_file, 
                                                           PCAP_TSTAMP_PRECISION_NANO, 
                                                           errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get buffer size
    int buf_size = pcap_bufsize(pcap);
    (void)buf_size; // Mark as used

    // 4. List timestamp types
    int *tstamp_types = nullptr;
    int tstamp_count = pcap_list_tstamp_types(pcap, &tstamp_types);
    if (tstamp_count > 0 && tstamp_types) {
        // Convert a status to string (use first timestamp type as example)
        const char *status_str = pcap_statustostr(tstamp_types[0]);
        (void)status_str; // Mark as used
        
        // Free timestamp types list
        pcap_free_tstamp_types(tstamp_types);
    }

    // 5. Try to get non-blocking mode status
    char nonblock_err[PCAP_ERRBUF_SIZE] = {0};
    int nonblock_ret = pcap_getnonblock(pcap, nonblock_err);
    (void)nonblock_ret; // Mark as used

    // 6. Create a simple BPF filter that matches all packets
    struct bpf_program bpf_filter;
    const char *filter_str = "ip or arp or tcp or udp or icmp";
    
    if (pcap_compile(pcap, &bpf_filter, filter_str, 1, PCAP_NETMASK_UNKNOWN) == 0) {
        // Create output file for dumping filtered packets
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            pcap_dumper_t *dumper = pcap_dump_fopen(pcap, out_file);
            
            if (dumper) {
                // Read and process packets
                struct pcap_pkthdr header;
                const u_char *packet_data;
                
                while ((packet_data = pcap_next(pcap, &header)) != nullptr) {
                    // Apply offline filter to each packet
                    int filter_result = pcap_offline_filter(&bpf_filter, &header, packet_data);
                    
                    if (filter_result != 0) {
                        // Packet passed the filter - dump it
                        pcap_dump((u_char *)dumper, &header, packet_data);
                    }
                }
                
                // Flush the dumper
                pcap_dump_flush(dumper);
                pcap_dump_close(dumper);
            }
            
            assert_file_closed(&out_file);;
        }
        
        // Free BPF program
        pcap_freecode(&bpf_filter);
    }

    // 7. Call pcap_freealldevs (even though we didn't use pcap_findalldevs)
    // This is safe to call with NULL
    pcap_freealldevs(nullptr);

    // 8. Cleanup
    pcap_close(pcap);
    assert_file_closed(&in_file);;
    
    // Remove output file if created
    remove("output_file");

    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}