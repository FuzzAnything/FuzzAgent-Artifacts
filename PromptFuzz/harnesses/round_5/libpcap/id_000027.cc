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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["pcap_lib_version","pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_bufsize","pcap_major_version","pcap_set_tstamp_precision","pcap_create","pcap_set_promisc","pcap_setnonblock","pcap_activate","pcap_compile","pcap_next","pcap_offline_filter","pcap_inject","pcap_sendpacket","pcap_freecode","pcap_close","pcap_close"],"critical_calls":["pcap_lib_version","pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_bufsize","pcap_major_version","pcap_set_tstamp_precision","pcap_create","pcap_set_promisc","pcap_setnonblock","pcap_activate","pcap_compile","pcap_next","pcap_offline_filter","pcap_inject","pcap_sendpacket","pcap_freecode","pcap_close","pcap_close"],"visited":1}
/**/


// Include necessary libpcap headers

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Get libpcap version
    const char *version = pcap_lib_version();
    (void)version;  // Use version to avoid unused variable warning
    
    // Create a FILE* from input data for offline reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open offline capture with microsecond timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *offline_pcap = pcap_fopen_offline_with_tstamp_precision(
        in_file, PCAP_TSTAMP_PRECISION_MICRO, errbuf);
    
    if (!offline_pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get snapshot length from offline pcap
    int snapshot_len = pcap_snapshot(offline_pcap);
    
    // Get buffer size from offline pcap
    int bufsize = pcap_bufsize(offline_pcap);
    
    // Get major version from offline pcap
    int major_version = pcap_major_version(offline_pcap);
    
    // Set timestamp precision (though already set during fopen)
    pcap_set_tstamp_precision(offline_pcap, PCAP_TSTAMP_PRECISION_MICRO);
    
    // Create a live capture handle for injection
    pcap_t *live_pcap = pcap_create("lo", errbuf);  // Use loopback interface
    if (live_pcap) {
        // Set promiscuous mode
        pcap_set_promisc(live_pcap, 0);  // Non-promiscuous
        
        // Set non-blocking mode
        pcap_setnonblock(live_pcap, 1, errbuf);
        
        // Activate the live capture
        if (pcap_activate(live_pcap) == 0) {
            // Compile a BPF filter
            struct bpf_program filter;
            const char *filter_str = "ip";  // Simple IP filter
            bpf_u_int32 netmask = 0xffffff00;  // 255.255.255.0
            
            if (pcap_compile(offline_pcap, &filter, filter_str, 0, netmask) == 0) {
                // Read and process packets from offline capture
                struct pcap_pkthdr header;
                const u_char *packet;
                
                while ((packet = pcap_next(offline_pcap, &header)) != NULL) {
                    // Apply offline filter to packet
                    if (pcap_offline_filter(&filter, &header, packet)) {
                        // Try to inject packet
                        if (pcap_inject(live_pcap, packet, header.caplen) == -1) {
                            // Fall back to sendpacket
                            pcap_sendpacket(live_pcap, packet, header.caplen);
                        }
                    }
                }
                
                // Free compiled filter
                pcap_freecode(&filter);
            }
        }
    }
    
    // Clean up resources
    if (live_pcap) {
        pcap_close(live_pcap);
    }
    
    pcap_close(offline_pcap);
    // Note: pcap_fopen_offline_with_tstamp_precision closes the FILE*
    // So we don't need to assert_file_closed(&in_file); here
    
    assert_file_closed(&in_file);
	return 0;
}