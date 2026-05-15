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
//<ID> 211
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["pcap_lib_version","pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_bufsize","pcap_major_version","pcap_create","pcap_set_promisc","pcap_activate","pcap_compile","pcap_next_ex","pcap_offline_filter","pcap_sendpacket","pcap_next","bpf_filter","pcap_tstamp_type_val_to_description","pcap_freecode","pcap_close","pcap_close","pcap_close"],"critical_calls":["pcap_lib_version","pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_bufsize","pcap_major_version","pcap_create","pcap_set_promisc","pcap_activate","pcap_compile","pcap_next_ex","pcap_offline_filter","pcap_sendpacket","pcap_next","bpf_filter","pcap_tstamp_type_val_to_description","pcap_freecode","pcap_close","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 24) {
	return 0;
	}
	 // Minimum size for pcap header
    
    // 1. Get libpcap version
    const char *version = pcap_lib_version();
    
    // 2. Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Open pcap file for offline reading with timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *offline_pcap = pcap_fopen_offline_with_tstamp_precision(in_file, PCAP_TSTAMP_PRECISION_MICRO, errbuf);
    if (!offline_pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get pcap properties
    int snapshot_len = pcap_snapshot(offline_pcap);
    int bufsize = pcap_bufsize(offline_pcap);
    int major_version = pcap_major_version(offline_pcap);
    
    // 5. Create a live capture pcap handle
    pcap_t *live_pcap = pcap_create("any", errbuf);
    if (live_pcap) {
        // 6. Set promiscuous mode
        pcap_set_promisc(live_pcap, 1);
        
        // 7. Activate the live pcap
        if (pcap_activate(live_pcap) == 0) {
            // 8. Compile a BPF filter
            struct bpf_program fp;
            const char *filter_str = "ip";
            bpf_u_int32 netmask = 0xffffff00;
            
            if (pcap_compile(offline_pcap, &fp, filter_str, 0, netmask) == 0) {
                // 9. Read packets from offline pcap using pcap_next_ex
                struct pcap_pkthdr *header;
                const u_char *packet_data;
                int packet_count = 0;
                
                while (pcap_next_ex(offline_pcap, &header, &packet_data) == 1 && packet_count < 10) {
                    packet_count++;
                    
                    // 10. Apply offline filter
                    if (pcap_offline_filter(&fp, header, packet_data)) {
                        // 11. Send packet using live pcap
                        pcap_sendpacket(live_pcap, packet_data, header->caplen);
                    }
                    
                    // 12. Also read with pcap_next
                    struct pcap_pkthdr header2;
                    const u_char *packet_data2 = pcap_next(offline_pcap, &header2);
                    if (packet_data2) {
                        // 13. Apply BPF filter directly
                        if (bpf_filter(fp.bf_insns, packet_data2, header2.caplen, header2.len)) {
                            // Packet passed filter
                        }
                    }
                }
                
                // 14. Get timestamp type description
                const char *ts_desc = pcap_tstamp_type_val_to_description(PCAP_TSTAMP_HOST);
                
                // 15. Free the compiled filter
                pcap_freecode(&fp);
            }
            
            // 16. Close live pcap
            pcap_close(live_pcap);
        } else {
            pcap_close(live_pcap);
        }
    }
    
    // 17. Close offline pcap (this also closes the FILE*)
    pcap_close(offline_pcap);
    
    assert_file_closed(&in_file);
	return 0;
}