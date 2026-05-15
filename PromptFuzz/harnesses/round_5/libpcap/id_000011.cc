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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"pcap_statustostr":[[3732,2,3732,17,0,0,4,0]]},"library_calls":["pcap_findalldevs","pcap_lookupnet","pcap_freealldevs","pcap_fopen_offline","pcap_set_rfmon","pcap_minor_version","pcap_datalink","pcap_datalink_val_to_description_or_dlt","pcap_loop","pcap_stats","pcap_statustostr","pcap_close","pcap_freealldevs"],"critical_calls":["pcap_findalldevs","pcap_lookupnet","pcap_fopen_offline","pcap_set_rfmon","pcap_minor_version","pcap_datalink","pcap_datalink_val_to_description_or_dlt","pcap_loop","pcap_stats","pcap_statustostr","pcap_close","pcap_freealldevs"],"visited":1}
/**/


// Packet handler callback function
static void  id_000011_packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    // Simple packet handler that just counts packets
    int *packet_count = (int *)user_data;
    (*packet_count)++;
    
    // Demonstrate use of pcap_datalink_val_to_description_or_dlt
    // We'll get the datalink type from the pcap_t passed in a different way
    // (not directly available in this callback without extra context)
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
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
    int status = 0;
    const char *status_str = nullptr;
    const char *dlt_description = nullptr;
    
    // 1. Use pcap_findalldevs to get all network devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // Error finding devices, but continue with other operations
    }
    
    // 2. For each device found, try to use pcap_lookupnet
    if (alldevs != nullptr) {
        pcap_if_t *d = alldevs;
        while (d != nullptr) {
            bpf_u_int32 net = 0, mask = 0;
            // Try to get network address and netmask for this device
            if (pcap_lookupnet(d->name, &net, &mask, errbuf) == 0) {
                // Successfully got network info for this device
                break;  // Just use first successful device
            }
            d = d->next;
        }
    }
    
    // 3. Create a FILE* from the input data to simulate pcap file input
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        if (alldevs != nullptr) {
            pcap_freealldevs(alldevs);
        }
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Open the pcap file from memory
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (pcap_handle != nullptr) {
        // 5. Try to set monitor mode (although this doesn't make sense for offline files,
        //    we call it anyway as requested)
        pcap_set_rfmon(pcap_handle, 0);  // Disable monitor mode for offline file
        
        // 6. Get minor version of the pcap library
        int minor_ver = pcap_minor_version(pcap_handle);
        
        // 7. Demonstrate pcap_datalink_val_to_description_or_dlt
        int dlt = pcap_datalink(pcap_handle);
        dlt_description = pcap_datalink_val_to_description_or_dlt(dlt);
        
        // 8. Process packets from the pcap file
        int packet_count = 0;
        pcap_loop(pcap_handle, -1,  id_000011_packet_handler, (u_char *)&packet_count);
        
        // 9. Demonstrate pcap_statustostr
        status = pcap_stats(pcap_handle, nullptr);
        status_str = pcap_statustostr(status);
        
        // 10. Close the pcap handle
        pcap_close(pcap_handle);
    }
    
    // 11. Clean up
    assert_file_closed(&in_file);;
    
    // 12. Free allocated devices list
    if (alldevs != nullptr) {
        pcap_freealldevs(alldevs);
    }
    
    // Force use of variables to avoid unused variable warnings
    if (status_str != nullptr) {
        // Do nothing, just reference the variable
    }
    if (dlt_description != nullptr) {
        // Do nothing, just reference the variable
    }
    
    assert_file_closed(&in_file);
	return 0;
}