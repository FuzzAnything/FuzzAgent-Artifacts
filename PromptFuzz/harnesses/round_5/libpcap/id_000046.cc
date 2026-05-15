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
//<ID> 246
//<Prompt> []
/*<Combination>: [
*/
//<score> 68, nr_unique_branch: 3
//<Quality> {"density":17,"unique_branches":{"gen_ncode":[[7476,2,7476,13,0,0,4,1]],"bpf_image":[[214,2,214,28,0,0,4,0]],"gen_loadx_iphdrlen":[[2668,6,2668,15,0,0,4,0]]},"library_calls":["pcap_lookupdev","pcap_lookupnet","pcap_fopen_offline","pcap_is_swapped","pcap_set_tstamp_type","pcap_getnonblock","pcap_datalink_name_to_val","pcap_compile","bpf_dump","pcap_freecode","pcap_close","pcap_dump_open_append","pcap_close","pcap_next","pcap_dump","pcap_dump_flush","pcap_dump_close","pcap_close"],"critical_calls":["pcap_lookupdev","pcap_lookupnet","pcap_fopen_offline","pcap_is_swapped","pcap_set_tstamp_type","pcap_getnonblock","pcap_datalink_name_to_val","pcap_compile","bpf_dump","pcap_freecode","pcap_dump_open_append","pcap_next","pcap_dump","pcap_dump_flush","pcap_dump_close","pcap_close"],"visited":0}
/**/


// Include libpcap headers

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small
    if (size < 24) {
	return 0;
	}
	

    // Open input data as a FILE stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Event: Process a pcap capture file from memory, apply BPF filter,
    // and append filtered packets to an output pcap file

    // 1. Get default capture device (optional)
    char errbuf[PCAP_ERRBUF_SIZE];
    char *dev = pcap_lookupdev(errbuf);
    
    // 2. Lookup network for the device (optional)
    bpf_u_int32 net, mask;
    if (dev) {
        pcap_lookupnet(dev, &net, &mask, errbuf);
    }

    // 3. Open offline pcap capture from memory file
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Check if capture file is byte-swapped
    int swapped = pcap_is_swapped(pcap);

    // 5. Set timestamp type (try host type)
    pcap_set_tstamp_type(pcap, PCAP_TSTAMP_HOST);

    // 6. Get non-blocking mode
    char nonblock_err[PCAP_ERRBUF_SIZE];
    pcap_getnonblock(pcap, nonblock_err);

    // 7. Convert data link type name to value
    int dlt_val = pcap_datalink_name_to_val("EN10MB");

    // 8. Compile BPF filter program
    struct bpf_program fp;
    const char *filter_str = "tcp port 80";
    bpf_u_int32 netmask = 0xffffff00;
    
    if (pcap_compile(pcap, &fp, filter_str, 0, netmask) == 0) {
        // 9. Dump BPF program (for debugging)
        bpf_dump(&fp, 1);
        
        // 10. Free compiled BPF program
        pcap_freecode(&fp);
    }

    // 11. Open output pcap file for appending
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    pcap_dumper_t *dumper = pcap_dump_open_append(pcap, "output_file");
    if (!dumper) {
        assert_file_closed(&out_file);;
        pcap_close(pcap);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // 12. Process packets from input
    struct pcap_pkthdr header;
    const u_char *packet;
    int packet_count = 0;
    
    while ((packet = pcap_next(pcap, &header)) != nullptr && packet_count < 10) {
        // 13. Write packet to output file
        pcap_dump((u_char *)dumper, &header, packet);
        packet_count++;
    }

    // 14. Flush output
    pcap_dump_flush(dumper);

    // 15. Close dumper
    pcap_dump_close(dumper);

    // Cleanup
    pcap_close(pcap);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}