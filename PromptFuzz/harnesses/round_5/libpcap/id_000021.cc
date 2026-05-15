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
//<ID> 75
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 3
//<Quality> {"density":14,"unique_branches":{"pcapint_offline_read":[[657,7,657,20,0,0,4,0],[658,8,658,14,0,0,4,0],[658,8,658,14,0,0,4,1]]},"library_calls":["pcap_fopen_offline","pcap_set_tstamp_precision","pcap_set_tstamp_precision","pcap_set_timeout","pcap_list_datalinks","pcap_free_datalinks","pcap_file","pcap_compile","bpf_dump","pcap_freecode","pcap_dump_fopen","pcap_loop","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_set_tstamp_precision","pcap_set_tstamp_precision","pcap_set_timeout","pcap_list_datalinks","pcap_free_datalinks","pcap_file","pcap_compile","bpf_dump","pcap_freecode","pcap_dump_fopen","pcap_loop","pcap_dump_close","pcap_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libpcap APIs to process pcap data from memory:

*/


extern "C" {
}

// Packet handler callback for pcap_loop
static void  id_000021_packet_handler(u_char *user, const struct pcap_pkthdr *hdr, const u_char *data) {
    // Dump packet info (simplified)
    static int packet_count = 0;
    packet_count++;
    
    // Break loop after 10 packets
    if (packet_count >= 10) {
        pcap_t *pcap = reinterpret_cast<pcap_t*>(user);
        pcap_breakloop(pcap);
    }
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 24) {
	return 0;
	}
	 // Minimum pcap header size
    
    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open pcap file from memory
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Set timestamp precision (try both micro and nano)
    pcap_set_tstamp_precision(pcap, PCAP_TSTAMP_PRECISION_MICRO);
    pcap_set_tstamp_precision(pcap, PCAP_TSTAMP_PRECISION_NANO);
    
    // 2. Set timeout (though for offline capture, this may not matter)
    pcap_set_timeout(pcap, 1000); // 1 second timeout
    
    // 3. List available data link types
    int *dlt_list = nullptr;
    int dlt_count = pcap_list_datalinks(pcap, &dlt_list);
    if (dlt_list) {
        pcap_free_datalinks(dlt_list);
    }
    
    // 4. Get the FILE* associated with the pcap handle
    FILE *pcap_file_ptr = pcap_file(pcap);
    (void)pcap_file_ptr; // Use to avoid unused variable warning
    
    // 5. Create BPF program and dump it
    struct bpf_program bpf_prog;
    const char *filter_str = "ip"; // Simple IP filter
    bpf_u_int32 netmask = 0xffffff00; // Example netmask
    
    // Try to compile filter (may fail on invalid input)
    if (pcap_compile(pcap, &bpf_prog, filter_str, 1, netmask) == 0) {
        // Dump BPF program
        bpf_dump(&bpf_prog, 1);
        pcap_freecode(&bpf_prog);
    }
    
    // 6. Create output file for dumping packets
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        pcap_dumper_t *dumper = pcap_dump_fopen(pcap, out_file);
        if (dumper) {
            // 7. Process packets with pcap_loop
            // Pass pcap handle as user data to break loop
            pcap_loop(pcap, 0,  id_000021_packet_handler, reinterpret_cast<u_char*>(pcap));
            
            // 8. Close the dumper
            pcap_dump_close(dumper);
        }
        assert_file_closed(&out_file);;
        remove("output_file"); // Clean up
    }
    
    // 9. Clean up resources
    pcap_close(pcap);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}