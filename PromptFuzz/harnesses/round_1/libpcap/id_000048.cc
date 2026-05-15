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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":18,"unique_branches":{"pcap_list_datalinks":[[3020,6,3020,23,0,0,4,1],[3037,7,3037,26,0,0,4,1]]},"library_calls":["pcap_init","pcap_fopen_offline","pcap_is_swapped","pcap_snapshot","pcap_bufsize","pcap_loop","pcap_findalldevs","pcap_create","pcap_set_timeout","pcap_set_immediate_mode","pcap_activate","pcap_list_tstamp_types","pcap_free_tstamp_types","pcap_list_datalinks","pcap_free_datalinks","pcap_close","pcap_freealldevs","pcap_close"],"critical_calls":["pcap_init","pcap_fopen_offline","pcap_is_swapped","pcap_snapshot","pcap_bufsize","pcap_loop","pcap_findalldevs","pcap_create","pcap_set_timeout","pcap_set_immediate_mode","pcap_activate","pcap_list_tstamp_types","pcap_free_tstamp_types","pcap_list_datalinks","pcap_free_datalinks","pcap_close","pcap_freealldevs","pcap_close"],"visited":1}
/**/


// Packet handler callback for pcap_loop
static void  id_000048_packet_handler(u_char *user, const struct pcap_pkthdr *hdr, const u_char *data) {
    // Simple packet counter - increment integer pointed by user
    (*(int*)user)++;
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (!data || size < 24) {
	return 0;
	}
	
    
    // Step 1: Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_init(PCAP_CHAR_ENC_UTF_8, errbuf) != 0) {
        // Initialization failed, but continue anyway
    }
    
    // Step 2: Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 3: Open offline pcap handle
    pcap_t *offline_handle = pcap_fopen_offline(in_file, errbuf);
    if (!offline_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 4: Check properties of offline handle
    int swapped = pcap_is_swapped(offline_handle);
    int snaplen = pcap_snapshot(offline_handle);
    int bufsize = pcap_bufsize(offline_handle);
    
    // Step 5: Process packets from offline capture
    int packet_count = 0;
    pcap_loop(offline_handle, -1,  id_000048_packet_handler, (u_char*)&packet_count);
    
    // Step 6: Find network devices
    pcap_if_t *all_devices = nullptr;
    if (pcap_findalldevs(&all_devices, errbuf) == 0 && all_devices) {
        // Step 7: Create a live capture handle using first device
        pcap_t *live_handle = pcap_create(all_devices->name, errbuf);
        if (live_handle) {
            // Step 8: Set capture options
            pcap_set_timeout(live_handle, 100);  // 100ms timeout
            pcap_set_immediate_mode(live_handle, 1);
            
            // Step 9: Activate the handle
            if (pcap_activate(live_handle) == 0) {
                // Step 10: List timestamp types
                int *tstamp_types = nullptr;
                int tstamp_count = pcap_list_tstamp_types(live_handle, &tstamp_types);
                if (tstamp_types) {
                    // Free timestamp types array
                    pcap_free_tstamp_types(tstamp_types);
                }
                
                // Step 11: List data link types
                int *dlt_list = nullptr;
                int dlt_count = pcap_list_datalinks(live_handle, &dlt_list);
                if (dlt_list) {
                    pcap_free_datalinks(dlt_list);
                }
            }
            pcap_close(live_handle);
        }
        
        // Step 12: Free device list
        pcap_freealldevs(all_devices);
    }
    
    // Cleanup
    pcap_close(offline_handle);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}