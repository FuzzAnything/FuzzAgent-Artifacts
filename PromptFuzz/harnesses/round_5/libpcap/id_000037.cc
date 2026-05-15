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
//<ID> 201
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_minor_version","pcap_datalink","pcap_datalink_val_to_description_or_dlt","pcap_set_datalink","pcap_set_rfmon","pcap_tstamp_type_val_to_description","pcap_findalldevs","pcap_lookupnet","pcap_freealldevs","pcap_compile","pcap_freecode","pcap_loop","pcap_stats","pcap_statustostr","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_minor_version","pcap_datalink","pcap_datalink_val_to_description_or_dlt","pcap_set_datalink","pcap_set_rfmon","pcap_tstamp_type_val_to_description","pcap_findalldevs","pcap_lookupnet","pcap_freealldevs","pcap_compile","pcap_freecode","pcap_loop","pcap_stats","pcap_statustostr","pcap_close"],"visited":0}
/**/


extern "C" {
}

// Dummy handler for pcap_loop
static void  id_000037_pcap_handler_callback(u_char *user, const struct pcap_pkthdr *h, const u_char *data) {
    // Do nothing - just consume the packet
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 24) {  // Minimum pcap header size
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Open offline capture file
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Get pcap version
    int minor_ver = pcap_minor_version(pcap);
    
    // 3. Get datalink type and its description
    int dlt = pcap_datalink(pcap);
    const char *dlt_desc = pcap_datalink_val_to_description_or_dlt(dlt);
    
    // 4. Try to set datalink (may fail for offline capture)
    pcap_set_datalink(pcap, dlt);
    
    // 5. Try to set monitor mode (will fail for offline but call it anyway)
    pcap_set_rfmon(pcap, 0);
    
    // 6. Get timestamp type description
    const char *ts_desc = pcap_tstamp_type_val_to_description(0);
    
    // 7. Find all devices (just to call the API)
    pcap_if_t *alldevs;
    if (pcap_findalldevs(&alldevs, errbuf) == 0) {
        // 8. Lookup network for first device if available
        if (alldevs) {
            bpf_u_int32 net, mask;
            pcap_lookupnet(alldevs->name, &net, &mask, errbuf);
        }
        // 9. Free device list
        pcap_freealldevs(alldevs);
    }
    
    // 10. Compile a simple filter
    struct bpf_program fp;
    const char *filter_str = "ip";
    if (pcap_compile(pcap, &fp, filter_str, 0, PCAP_NETMASK_UNKNOWN) == 0) {
        pcap_freecode(&fp);
    }
    
    // 11. Process packets with pcap_loop
    pcap_loop(pcap, 3,  id_000037_pcap_handler_callback, nullptr);
    
    // 12. Get statistics
    struct pcap_stat stats;
    if (pcap_stats(pcap, &stats) == 0) {
        // 13. Convert status to string
        const char *status_str = pcap_statustostr(0);
    }
    
    // 14. Clean up
    pcap_close(pcap);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}