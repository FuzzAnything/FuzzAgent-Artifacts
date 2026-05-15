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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["pcap_lib_version","pcap_fopen_offline_with_tstamp_precision","pcap_set_promisc","pcap_set_tstamp_precision","pcap_major_version","pcap_bufsize","pcap_compile","pcap_next","pcap_offline_filter","pcap_create","pcap_activate","pcap_inject","pcap_close","pcap_freecode","pcap_close"],"critical_calls":["pcap_lib_version","pcap_fopen_offline_with_tstamp_precision","pcap_set_promisc","pcap_set_tstamp_precision","pcap_major_version","pcap_bufsize","pcap_compile","pcap_next","pcap_offline_filter","pcap_create","pcap_activate","pcap_inject","pcap_close","pcap_freecode","pcap_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get libpcap version
    const char *version = pcap_lib_version();
    (void)version; // Prevent unused variable warning
    
    // 2. Create FILE* from input data (libpcap output data)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Open offline pcap capture with timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_fopen_offline_with_tstamp_precision(in_file, 
                                                           PCAP_TSTAMP_PRECISION_MICRO, 
                                                           errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Set promiscuous mode (even though it's offline)
    pcap_set_promisc(pcap, 1);
    
    // 5. Set timestamp precision
    pcap_set_tstamp_precision(pcap, PCAP_TSTAMP_PRECISION_MICRO);
    
    // 6. Get major version of pcap file
    int major_ver = pcap_major_version(pcap);
    (void)major_ver; // Prevent unused variable warning
    
    // 7. Get buffer size
    int buf_size = pcap_bufsize(pcap);
    (void)buf_size; // Prevent unused variable warning
    
    // 8. Read packets and apply offline filter
    struct bpf_program filter;
    // Compile a simple filter (all packets)
    if (pcap_compile(pcap, &filter, "ip", 1, 0) == 0) {
        struct pcap_pkthdr header;
        const u_char *packet_data;
        
        // Try to read first packet
        packet_data = pcap_next(pcap, &header);
        if (packet_data) {
            // Apply offline filter to the packet
            int filter_result = pcap_offline_filter(&filter, &header, packet_data);
            (void)filter_result; // Prevent unused variable warning
            
            // 9. Try to inject the packet (even though we're offline)
            // Create a dummy pcap handle for injection attempt
            pcap_t *inject_pcap = pcap_create("lo", errbuf);
            if (inject_pcap) {
                pcap_activate(inject_pcap);
                // Try to inject the packet we read
                pcap_inject(inject_pcap, packet_data, header.caplen);
                pcap_close(inject_pcap);
            }
        }
        pcap_freecode(&filter);
    }
    
    // 10. Cleanup
    pcap_close(pcap);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}