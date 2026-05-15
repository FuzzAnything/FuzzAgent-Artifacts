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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 1
//<Quality> {"density":27,"unique_branches":{"pcap_activate_linux":[[1227,7,1227,26,0,0,4,0]]},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_bufsize","pcap_list_datalinks","pcap_set_datalink","pcap_free_datalinks","pcap_list_tstamp_types","pcap_set_tstamp_type","pcap_free_tstamp_types","pcap_set_tstamp_precision","pcap_compile","bpf_dump","pcap_freecode","pcap_next","pcap_next_ex","pcap_minor_version","pcap_findalldevs","pcap_create","pcap_set_snaplen","pcap_set_promisc","pcap_set_timeout","pcap_activate","pcap_sendpacket","pcap_inject","pcap_close","pcap_freealldevs","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_bufsize","pcap_list_datalinks","pcap_set_datalink","pcap_free_datalinks","pcap_list_tstamp_types","pcap_set_tstamp_type","pcap_free_tstamp_types","pcap_set_tstamp_precision","pcap_compile","bpf_dump","pcap_freecode","pcap_next","pcap_next_ex","pcap_minor_version","pcap_findalldevs","pcap_create","pcap_set_snaplen","pcap_set_promisc","pcap_set_timeout","pcap_activate","pcap_sendpacket","pcap_inject","pcap_close","pcap_freealldevs","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<37) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    if (size < 24) {
	return 0;
	}
	  // Minimum size for pcap header

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Open pcap file from memory with timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_offline = pcap_fopen_offline_with_tstamp_precision(in_file, 
                                                                    fuzz_uint32_t_1, 
                                                                    errbuf);
    if (!pcap_offline) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get snapshot length and buffer size
    int snapshot = pcap_snapshot(pcap_offline);
    int bufsize = pcap_bufsize(pcap_offline);

    // List and set data link types
    int *dlt_list = nullptr;
    int dlt_count = pcap_list_datalinks(pcap_offline, &dlt_list);
    if (dlt_count > 0) {
        // Try to set to first available DLT
        pcap_set_datalink(pcap_offline, dlt_list[0]);
        pcap_free_datalinks(dlt_list);
    }

    // List and set timestamp types
    int *tstamp_types = nullptr;
    int tstamp_count = pcap_list_tstamp_types(pcap_offline, &tstamp_types);
    if (tstamp_count > 0) {
        // Try to set to first available timestamp type
        pcap_set_tstamp_type(pcap_offline, tstamp_types[0]);
        pcap_free_tstamp_types(tstamp_types);
    }

    // Set timestamp precision
    pcap_set_tstamp_precision(pcap_offline, fuzz_int32_t_2);

    // Compile BPF filter
    struct bpf_program bpf_prog;
    const char *filter_str = "ip";
    if (pcap_compile(pcap_offline, &bpf_prog, fuzz_str_3, fuzz_int32_t_4, fuzz_uint32_t_5) == 0) {
        bpf_dump(&bpf_prog, fuzz_int32_t_6);
        pcap_freecode(&bpf_prog);
    }

    // Read packets from pcap file
    struct pcap_pkthdr header;
    const u_char *packet_data;
    
    // Read first packet with pcap_next
    packet_data = pcap_next(pcap_offline, &header);
    
    // Read remaining packets with pcap_next_ex
    struct pcap_pkthdr *header_ptr;
    const u_char **packet_data_ptr = &packet_data;
    while (pcap_next_ex(pcap_offline, &header_ptr, packet_data_ptr) == 1) {
        // Packet available
    }

    // Get minor version
    int minor_ver = pcap_minor_version(pcap_offline);

    // Find all devices
    pcap_if_t *alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) == 0) {
        if (alldevs) {
            // Create a live pcap handle using first device
            pcap_t *pcap_live = pcap_create(alldevs->name, errbuf);
            if (pcap_live) {
                // Set various parameters
                pcap_set_snaplen(pcap_live, fuzz_int32_t_7);
                pcap_set_promisc(pcap_live, fuzz_int32_t_8);
                pcap_set_timeout(pcap_live, fuzz_int32_t_9);
                
                // Activate the live capture
                if (pcap_activate(pcap_live) == 0) {
                    // Try to send a packet if we have packet data
                    if (size > 0) {
                        // Use a small portion of input data as packet
                        size_t send_size = size > 1514 ? 1514 : size;
                        pcap_sendpacket(pcap_live, data, static_cast<int>(send_size));
                        
                        // Also try inject
                        pcap_inject(pcap_live, data, send_size);
                    }
                }
                pcap_close(pcap_live);
            }
        }
        pcap_freealldevs(alldevs);
    }

    // Cleanup
    pcap_close(pcap_offline);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}