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
//<ID> 229
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_major_version","pcap_minor_version","pcap_setdirection","pcap_lookupdev","pcap_create","pcap_can_set_rfmon","pcap_set_rfmon","pcap_close","pcap_loop","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_major_version","pcap_minor_version","pcap_setdirection","pcap_lookupdev","pcap_create","pcap_can_set_rfmon","pcap_set_rfmon","pcap_close","pcap_loop","pcap_close"],"visited":0}
/**/


// Dummy callback function for pcap_loop
static void  id_000051_packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Do nothing - just consume the packet
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 24) {  // Minimum size for pcap file header
        return 0;
    }

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);

    // Open the pcap file from memory
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // Get major/minor version (APIs 4 & 1)
    int major_version = pcap_major_version(pcap_handle);
    int minor_version = pcap_minor_version(pcap_handle);
    
    // Set direction (API 3)
    pcap_setdirection(pcap_handle, PCAP_D_INOUT);

    // Look up a device (API 5) - just for demonstration
    char *device = pcap_lookupdev(errbuf);
    
    // For the rfmon API (API 0), we need a live capture handle
    // Create one using the found device (or default)
    pcap_t *live_handle = NULL;
    if (device) {
        live_handle = pcap_create(device, errbuf);
        if (live_handle) {
            // Check if we can set monitor mode
            if (pcap_can_set_rfmon(live_handle)) {
                pcap_set_rfmon(live_handle, 1);  // Try to enable monitor mode
            }
            // Clean up the live handle
            pcap_close(live_handle);
        }
    }

    // Process the pcap file (API 2) - loop through all packets
    pcap_loop(pcap_handle, 0,  id_000051_packet_handler, NULL);

    // Cleanup
    pcap_close(pcap_handle);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	return 0;
}