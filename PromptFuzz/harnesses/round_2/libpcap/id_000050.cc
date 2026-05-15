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
//<ID> 258
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"pcap_set_buffer_size":[[2691,6,2691,22,0,0,4,1]]},"library_calls":["pcap_init","pcap_lookupdev","pcap_create","pcap_set_buffer_size","pcap_set_datalink","pcap_list_tstamp_types","pcap_tstamp_type_val_to_name","pcap_activate","pcap_close","pcap_free_tstamp_types","pcap_sendpacket","pcap_bufsize","pcap_close","pcap_free_tstamp_types"],"critical_calls":["pcap_init","pcap_lookupdev","pcap_create","pcap_set_buffer_size","pcap_set_datalink","pcap_list_tstamp_types","pcap_tstamp_type_val_to_name","pcap_activate","pcap_sendpacket","pcap_bufsize","pcap_close","pcap_free_tstamp_types"],"visited":0}
/**/


extern "C" {
}

// Custom types from libpcap headers
typedef struct pcap pcap_t;
typedef unsigned char u_char;

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *data, size_t size) {
    // Early return for insufficient data
    if (size < 24) return 0;
    
    // Step 1: Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_init(PCAP_CHAR_ENC_LOCAL, errbuf) != 0) {
        return 0;
    }
    
    // Step 2: Look up a device
    char *device = pcap_lookupdev(errbuf);
    if (!device) {
        // Fallback to default device name if lookup fails
        device = (char *)"eth0";
    }
    
    // Step 3: Create a pcap handle for the device
    pcap_t *pcap_handle = pcap_create(device, errbuf);
    if (!pcap_handle) {
        return 0;
    }
    
    // Step 4: Set buffer size using first 4 bytes of input
    int buffer_size = 0;
    if (size >= 4) {
        buffer_size = *(reinterpret_cast<const int*>(data)) & 0xFFFF;  // Use lower 16 bits
        if (buffer_size < 0) buffer_size = 65536;  // Default if negative
        pcap_set_buffer_size(pcap_handle, buffer_size);
    }
    
    // Step 5: Set data link type using next 4 bytes
    if (size >= 8) {
        int dlt = *(reinterpret_cast<const int*>(data + 4)) & 0xFF;  // Use lower 8 bits
        pcap_set_datalink(pcap_handle, dlt);
    }
    
    // Step 6: List timestamp types
    int *tstamp_types = nullptr;
    int tstamp_count = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
    
    // Step 7: Convert timestamp type to name (if we have any types)
    const char *tstamp_name = nullptr;
    if (tstamp_count > 0 && tstamp_types) {
        tstamp_name = pcap_tstamp_type_val_to_name(tstamp_types[0]);
    }
    
    // Step 8: Activate the pcap handle
    if (pcap_activate(pcap_handle) != 0) {
        pcap_close(pcap_handle);
        pcap_free_tstamp_types(tstamp_types);
        return 0;
    }
    
    // Step 9: Prepare packet data from remaining input
    size_t packet_size = std::min(size - 8, (size_t)1514);  // Standard MTU
    if (packet_size > 0 && size > 8) {
        const u_char *packet_data = data + 8;
        
        // Step 10: Send the packet
        pcap_sendpacket(pcap_handle, packet_data, packet_size);
    }
    
    // Step 11: Get buffer size for verification
    int actual_buffer_size = pcap_bufsize(pcap_handle);
    
    // Step 12: Clean up resources
    pcap_close(pcap_handle);
    pcap_free_tstamp_types(tstamp_types);
    
    return 0;
}