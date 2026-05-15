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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 2
//<Quality> {"density":7,"unique_branches":{"pcap_set_rfmon":[[2617,6,2617,32,0,0,4,1]],"pcap_minor_version":[[3544,6,3544,19,0,0,4,0]]},"library_calls":["pcap_findalldevs","pcap_lookupnet","pcap_create","pcap_set_rfmon","pcap_minor_version","pcap_close","pcap_fopen_offline","pcap_datalink","pcap_loop","pcap_minor_version","pcap_close","pcap_freealldevs","pcap_close"],"critical_calls":["pcap_findalldevs","pcap_lookupnet","pcap_create","pcap_set_rfmon","pcap_minor_version","pcap_close","pcap_fopen_offline","pcap_datalink","pcap_loop","pcap_minor_version","pcap_close","pcap_freealldevs","pcap_close"],"visited":0}
/**/


// Packet handler callback function for pcap_loop
static void  id_000012_packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Get the datalink type from user data (passed as integer pointer)
    int *dlt_ptr = (int *)user;
    int dlt = *dlt_ptr;
    
    // Use pcap_datalink_val_to_description_or_dlt to get description
    const char *desc = pcap_datalink_val_to_description_or_dlt(dlt);
    (void)desc; // Use variable to avoid unused warning
    
    // Use pcap_statustostr for status 0 (PCAP_STATUSTOSTR_UNKNOWN)
    const char *status_str = pcap_statustostr(0);
    (void)status_str; // Use variable to avoid unused warning
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;
    pcap_t *pcap_handle = nullptr;
    FILE *in_file = nullptr;
    
    // 1. Find all network devices using pcap_findalldevs
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // Error finding devices, but continue with other operations
    }
    
    // 2. If we found devices, process the first one
    if (alldevs != nullptr && alldevs->name != nullptr) {
        bpf_u_int32 netp = 0, maskp = 0;
        
        // 3. Look up network address and mask for the device
        if (pcap_lookupnet(alldevs->name, &netp, &maskp, errbuf) == -1) {
            netp = maskp = 0; // Use defaults if lookup fails
        }
        
        // 4. Create a pcap handle for the device
        pcap_handle = pcap_create(alldevs->name, errbuf);
        if (pcap_handle != nullptr) {
            // 5. Try to set monitor mode using pcap_set_rfmon
            // The second parameter uses the first byte of input data to decide
            int rfmon_mode = (data[0] & 0x01) ? 1 : 0;
            pcap_set_rfmon(pcap_handle, rfmon_mode);
            
            // 6. Get minor version using pcap_minor_version
            int minor_ver = pcap_minor_version(pcap_handle);
            (void)minor_ver; // Use variable to avoid unused warning
            
            // Don't activate the handle since we're not actually capturing
            pcap_close(pcap_handle);
            pcap_handle = nullptr;
        }
    }
    
    // 7. Process input data as a pcap file
    in_file = fmemopen((void *)data, size, "rb");
    if (in_file != nullptr) {
        // Open the pcap file from memory
        pcap_t *pcap_file = pcap_fopen_offline(in_file, errbuf);
        if (pcap_file != nullptr) {
            // Get datalink type
            int dlt = pcap_datalink(pcap_file);
            
            // 8. Use pcap_loop to process packets (process at most 10 packets)
            // Pass datalink type as user data to  id_000012_packet_handler
            pcap_loop(pcap_file, 10,  id_000012_packet_handler, (u_char *)&dlt);
            
            // 9. Get minor version for the file handle too
            int file_minor_ver = pcap_minor_version(pcap_file);
            (void)file_minor_ver; // Use variable to avoid unused warning
            
            pcap_close(pcap_file);
        }
        assert_file_closed(&in_file);;
    }
    
    // 10. Free all allocated resources
    if (alldevs != nullptr) {
        pcap_freealldevs(alldevs);
    }
    
    if (pcap_handle != nullptr) {
        pcap_close(pcap_handle);
    }
    
    assert_file_closed(&in_file);
	return 0;
}