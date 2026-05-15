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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 2
//<Quality> {"density":19,"unique_branches":{"pcap_tstamp_type_val_to_description":[[3510,14,3510,49,0,0,4,1],[3511,7,3511,49,0,0,4,1]]},"library_calls":["pcap_lib_version","pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_major_version","pcap_create","pcap_create","pcap_set_promisc","pcap_activate","pcap_close","pcap_bufsize","pcap_compile","pcap_next","pcap_offline_filter","pcap_tstamp_type_val_to_description","pcap_datalink","pcap_sendpacket","bpf_filter","pcap_next_ex","pcap_freecode","pcap_close","pcap_close"],"critical_calls":["pcap_lib_version","pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_major_version","pcap_create","pcap_create","pcap_set_promisc","pcap_activate","pcap_close","pcap_compile","pcap_next","pcap_offline_filter","pcap_tstamp_type_val_to_description","pcap_datalink","pcap_sendpacket","bpf_filter","pcap_next_ex","pcap_freecode","pcap_close","pcap_close"],"visited":0}
/**/


extern "C" {
}

// Custom event: Read packets from an offline pcap file, filter them using BPF,
// send filtered packets through a live capture interface, and log metadata

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create a FILE* from input data (libpcap output data)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get libpcap version
    const char *version = pcap_lib_version();
    (void)version; // Use version to avoid unused variable warning
    
    // 3. Open offline capture with timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *offline_pcap = pcap_fopen_offline_with_tstamp_precision(
        in_file, PCAP_TSTAMP_PRECISION_MICRO, errbuf);
    if (!offline_pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get snapshot length and major version from offline pcap
    int snapshot_len = pcap_snapshot(offline_pcap);
    int major_version = pcap_major_version(offline_pcap);
    (void)snapshot_len;
    (void)major_version;
    
    // 5. Create a live capture device for sending packets
    pcap_t *live_pcap = pcap_create("any", errbuf);
    if (!live_pcap) {
        // Fallback to loopback if "any" fails
        live_pcap = pcap_create("lo", errbuf);
    }
    
    if (live_pcap) {
        // 6. Set promiscuous mode and activate
        pcap_set_promisc(live_pcap, 1);
        if (pcap_activate(live_pcap) < 0) {
            pcap_close(live_pcap);
            live_pcap = nullptr;
        } else {
            // 7. Get buffer size
            int bufsize = pcap_bufsize(live_pcap);
            (void)bufsize;
        }
    }
    
    // 8. Compile BPF filter
    struct bpf_program filter;
    const char *filter_str = "ip or arp";
    bpf_u_int32 netmask = 0xffffff00;
    
    if (pcap_compile(offline_pcap, &filter, filter_str, 1, netmask) == 0) {
        // 9. Read and process packets from offline capture
        struct pcap_pkthdr header;
        const u_char *packet;
        
        // Use pcap_next for first packet
        packet = pcap_next(offline_pcap, &header);
        
        // Use pcap_next_ex for subsequent packets
        struct pcap_pkthdr *header_ptr;
        const u_char *packet_ptr;
        int ret;
        
        while (true) {
            if (packet) {
                // 10. Apply offline filter
                if (pcap_offline_filter(&filter, &header, packet)) {
                    // 11. Get timestamp description
                    const char *ts_desc = pcap_tstamp_type_val_to_description(
                        pcap_datalink(offline_pcap));
                    (void)ts_desc;
                    
                    // 12. Send packet through live interface if available
                    if (live_pcap) {
                        pcap_sendpacket(live_pcap, packet, header.caplen);
                    }
                    
                    // 13. Apply BPF filter directly
                    u_int filter_result = bpf_filter(
                        filter.bf_insns, 
                        packet, 
                        header.len, 
                        header.caplen);
                    (void)filter_result;
                }
            }
            
            // Get next packet using pcap_next_ex
            ret = pcap_next_ex(offline_pcap, &header_ptr, &packet_ptr);
            if (ret <= 0) break;
            
            packet = packet_ptr;
            memcpy(&header, header_ptr, sizeof(header));
        }
        
        // 14. Free compiled filter
        pcap_freecode(&filter);
    }
    
    // 15. Cleanup
    if (live_pcap) {
        pcap_close(live_pcap);
    }
    pcap_close(offline_pcap);
    
    // Note: in_file is closed by pcap_close(offline_pcap) 
    // since pcap_fopen_offline_with_tstamp_precision takes ownership
    
    assert_file_closed(&in_file);
	return 0;
}