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
//<ID> 63
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 6
//<Quality> {"density":16,"unique_branches":{"pcap_set_tstamp_type":[[2637,6,2637,32,0,0,4,1],[2644,6,2644,21,0,0,4,1],[2651,6,2651,31,0,0,4,0],[2652,7,2652,38,0,0,4,0]],"setup_socket":[[2652,7,2652,24,0,0,4,0]],"pcap_activate":[[2790,7,2790,27,0,0,4,1]]},"library_calls":["pcap_fopen_offline","pcap_major_version","pcap_is_swapped","pcap_list_tstamp_types","pcap_findalldevs","pcap_create","pcap_set_rfmon","pcap_set_tstamp_type","pcap_set_promisc","pcap_activate","pcap_get_selectable_fd","pcap_4_byte_aligned_int64_val","pcap_loop","pcap_stats","pcap_create","pcap_close","pcap_close","pcap_close","pcap_free_tstamp_types","pcap_freealldevs"],"critical_calls":["pcap_fopen_offline","pcap_major_version","pcap_is_swapped","pcap_list_tstamp_types","pcap_findalldevs","pcap_create","pcap_set_rfmon","pcap_set_tstamp_type","pcap_set_promisc","pcap_activate","pcap_get_selectable_fd","pcap_4_byte_aligned_int64_val","pcap_loop","pcap_stats","pcap_create","pcap_close","pcap_close","pcap_close","pcap_free_tstamp_types","pcap_freealldevs"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libpcap APIs:

*/


// libpcap headers
extern "C" {
}

// Custom packet handler callback
void id_000028_packet_handler(u_char *user, const struct pcap_pkthdr *header, const u_char *packet) {
    // Do nothing in fuzzing - just count packets
    int *packet_count = (int *)user;
    (*packet_count)++;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *data, size_t size) {
    if (size < 24) return 0;  // Minimum pcap file header size
    
    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) return 0;
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        fclose(in_file);
        return 0;
    }
    
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap_handle = nullptr;
    pcap_t *live_handle = nullptr;
    pcap_if_t *all_devices = nullptr;
    int *tstamp_types = nullptr;
    int tstamp_type_count = 0;
    struct pcap_stat stats = {0};
    int ret = 0;
    
    // 1. Open offline pcap file from memory stream
    pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        fclose(in_file);
        fclose(out_file);
        return 0;
    }
    
    // 2. Get pcap major version
    int major_ver = pcap_major_version(pcap_handle);
    (void)major_ver;  // Use variable to avoid unused warning
    
    // 3. Check if pcap file is byte-swapped
    int swapped = pcap_is_swapped(pcap_handle);
    (void)swapped;
    
    // 4. List timestamp types
    tstamp_type_count = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
    
    // 5. Find all available devices
    ret = pcap_findalldevs(&all_devices, errbuf);
    
    // 6-7. If we found devices, try to create live capture handle
    if (ret == 0 && all_devices && all_devices->name) {
        // Create live capture handle
        live_handle = pcap_create(all_devices->name, errbuf);
        if (live_handle) {
            // 6. Try to set monitor mode (may fail if not supported)
            pcap_set_rfmon(live_handle, 1);
            
            // 7. Try to set timestamp type (use first from list if available)
            if (tstamp_type_count > 0) {
                pcap_set_tstamp_type(live_handle, tstamp_types[0]);
            }
            
            // 8. Set promiscuous mode
            pcap_set_promisc(live_handle, 1);
            
            // Try to activate the live handle (may fail in fuzzing context)
            ret = pcap_activate(live_handle);
        }
    }
    
    // 9. Get selectable file descriptor for offline handle
    int selectable_fd = pcap_get_selectable_fd(pcap_handle);
    (void)selectable_fd;
    
    // 10. Convert 4-byte aligned int64 (dummy value for demonstration)
    pcap_4_byte_aligned_int64 aligned_val;
    memset(&aligned_val, 0, sizeof(aligned_val));
    if (size >= sizeof(aligned_val.halves)) {
        memcpy(&aligned_val.halves, data, sizeof(aligned_val.halves));
    }
    int64_t int64_val = pcap_4_byte_aligned_int64_val(aligned_val);
    (void)int64_val;
    
    // 11. Process packets with pcap_loop (limited to 10 packets for fuzzing)
    int packet_count = 0;
    pcap_loop(pcap_handle, 10, id_000028_packet_handler, (u_char *)&packet_count);
    
    // 12. Get statistics
    ret = pcap_stats(pcap_handle, &stats);
    (void)ret;
    
    // 13. Create another capture handle (for API coverage)
    pcap_t *another_handle = pcap_create("any", errbuf);
    if (another_handle) {
        // Don't activate it to avoid issues
        pcap_close(another_handle);
    }
    
    // 14. Close pcap handles
    pcap_close(pcap_handle);
    if (live_handle) {
        pcap_close(live_handle);
    }
    
    // 15. Free timestamp types array
    if (tstamp_types) {
        pcap_free_tstamp_types(tstamp_types);
    }
    
    // Free devices list
    if (all_devices) {
        pcap_freealldevs(all_devices);
    }
    
    // Close files
    fclose(in_file);
    fclose(out_file);
    
    return 0;
}