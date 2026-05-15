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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 5
//<Quality> {"density":19,"unique_branches":{"pcap_set_tstamp_precision":[[2706,6,2706,32,0,0,4,1],[2713,6,2713,26,0,0,4,1],[2723,6,2723,36,0,0,4,1],[2733,15,2733,44,0,0,4,0],[2734,8,2734,62,0,0,4,0]]},"library_calls":["pcap_lib_version","pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_major_version","pcap_bufsize","pcap_set_tstamp_precision","pcap_setnonblock","pcap_compile","pcap_next","pcap_offline_filter","pcap_freecode","pcap_create","pcap_set_promisc","pcap_setnonblock","pcap_set_tstamp_precision","pcap_activate","pcap_inject","pcap_sendpacket","pcap_close","pcap_close"],"critical_calls":["pcap_lib_version","pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_major_version","pcap_bufsize","pcap_set_tstamp_precision","pcap_setnonblock","pcap_compile","pcap_next","pcap_offline_filter","pcap_freecode","pcap_create","pcap_set_promisc","pcap_setnonblock","pcap_set_tstamp_precision","pcap_activate","pcap_inject","pcap_sendpacket","pcap_close","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get libpcap version
    const char *version = pcap_lib_version();
    (void)version; // Suppress unused variable warning
    
    // 2. Create a FILE* from input data for reading pcap data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Open pcap file from memory with timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *offline_handle = pcap_fopen_offline_with_tstamp_precision(
        in_file, PCAP_TSTAMP_PRECISION_MICRO, errbuf);
    
    if (!offline_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get snapshot length
    int snapshot_len = pcap_snapshot(offline_handle);
    (void)snapshot_len;
    
    // 5. Get major version
    int major_version = pcap_major_version(offline_handle);
    (void)major_version;
    
    // 6. Get buffer size
    int bufsize = pcap_bufsize(offline_handle);
    (void)bufsize;
    
    // 7. Set timestamp precision (though already set during open)
    pcap_set_tstamp_precision(offline_handle, PCAP_TSTAMP_PRECISION_MICRO);
    
    // 8. Set non-blocking mode for offline handle
    pcap_setnonblock(offline_handle, 1, errbuf);
    
    // 9. Compile a BPF filter
    struct bpf_program filter;
    const char *filter_str = "ip or arp or tcp or udp";
    bpf_u_int32 netmask = 0xffffff00;
    
    if (pcap_compile(offline_handle, &filter, filter_str, 1, netmask) == 0) {
        // 10. Read and process packets from offline capture
        struct pcap_pkthdr header;
        const u_char *packet;
        
        while ((packet = pcap_next(offline_handle, &header)) != nullptr) {
            // 11. Apply offline filter to each packet
            if (pcap_offline_filter(&filter, &header, packet)) {
                // Packet passed the filter
                // We'll use this packet for injection later
            }
        }
        
        // 12. Free compiled filter
        pcap_freecode(&filter);
    }
    
    // 13. Create a live capture handle for packet injection
    pcap_t *live_handle = pcap_create("lo", errbuf); // Use loopback interface
    if (live_handle) {
        // 14. Set promiscuous mode
        pcap_set_promisc(live_handle, 0); // Not promiscuous
        
        // 15. Set non-blocking mode
        pcap_setnonblock(live_handle, 1, errbuf);
        
        // 16. Set timestamp precision
        pcap_set_tstamp_precision(live_handle, PCAP_TSTAMP_PRECISION_MICRO);
        
        // 17. Activate the live handle
        if (pcap_activate(live_handle) == 0) {
            // 18. Try to inject a test packet
            const char test_packet[] = {
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Destination MAC
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source MAC
                0x08, 0x00 // IP Ethertype
            };
            
            // 19. Inject packet
            pcap_inject(live_handle, test_packet, sizeof(test_packet));
            
            // 20. Send packet (similar to inject but with different semantics)
            pcap_sendpacket(live_handle, (const u_char *)test_packet, sizeof(test_packet));
        }
        
        // 21. Close live handle
        pcap_close(live_handle);
    }
    
    // 22. Close offline handle
    pcap_close(offline_handle);
    
    // Note: in_file is closed by pcap_close(offline_handle) when using pcap_fopen_offline
    
    assert_file_closed(&in_file);
	return 0;
}