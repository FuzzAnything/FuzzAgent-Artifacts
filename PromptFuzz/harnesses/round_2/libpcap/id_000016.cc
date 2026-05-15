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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_major_version","pcap_is_swapped","pcap_list_tstamp_types","pcap_set_rfmon","pcap_get_selectable_fd","pcap_4_byte_aligned_int64_val","pcap_loop","pcap_stats","pcap_create","pcap_set_rfmon","pcap_get_selectable_fd","pcap_close","pcap_free_tstamp_types","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_major_version","pcap_is_swapped","pcap_list_tstamp_types","pcap_set_rfmon","pcap_get_selectable_fd","pcap_4_byte_aligned_int64_val","pcap_loop","pcap_stats","pcap_create","pcap_set_rfmon","pcap_get_selectable_fd","pcap_close","pcap_free_tstamp_types","pcap_close"],"visited":1}
/**/


// Include libpcap headers

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *data, size_t size) {
    if (size < sizeof(pcap_4_byte_aligned_int64) + 1) {
        return 0;  // Not enough data
    }

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = nullptr;
    struct pcap_stat stats;
    int *tstamp_types = nullptr;
    int count_tstamp_types = 0;
    
    // 1. Create a pcap handle for offline capture
    pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        fclose(in_file);
        return 0;
    }

    // 2. Get major version of libpcap used for this capture
    int major_ver = pcap_major_version(pcap_handle);

    // 3. Check if capture is swapped (endianness)
    int is_swapped = pcap_is_swapped(pcap_handle);

    // 4. List available timestamp types
    count_tstamp_types = pcap_list_tstamp_types(pcap_handle, &tstamp_types);
    
    // 5. Try to set monitor mode (will likely fail for offline capture, but we try)
    int rfmon_result = pcap_set_rfmon(pcap_handle, 1);

    // 6. Get selectable file descriptor (if available)
    int selectable_fd = pcap_get_selectable_fd(pcap_handle);

    // 7. Create a dummy pcap_4_byte_aligned_int64 from input data
    pcap_4_byte_aligned_int64 aligned_int64;
    size_t offset = size - sizeof(pcap_4_byte_aligned_int64);
    memcpy(&aligned_int64, data + offset, sizeof(pcap_4_byte_aligned_int64));
    
    // 8. Get the integer value from aligned structure
    int64_t int64_val = pcap_4_byte_aligned_int64_val(aligned_int64);

    // 9. Define a packet handler callback
    auto packet_handler = [](u_char *user_data, const struct pcap_pkthdr *header, 
                           const u_char *packet_data) {
        // Simple handler that does nothing but satisfies the API requirement
        return;
    };

    // 10. Process packets with pcap_loop (process only first 5 packets to limit execution)
    int loop_result = pcap_loop(pcap_handle, 5, packet_handler, nullptr);

    // 11. Get statistics from the capture
    int stats_result = pcap_stats(pcap_handle, &stats);

    // Create another pcap handle for live capture attempt
    pcap_t *live_pcap = pcap_create("any", errbuf);
    if (live_pcap) {
        // Try to set monitor mode on live capture
        pcap_set_rfmon(live_pcap, 0);
        
        // Get selectable fd for live capture
        pcap_get_selectable_fd(live_pcap);
        
        // Close the live capture handle
        pcap_close(live_pcap);
    }

    // Clean up resources
    if (tstamp_types) {
        pcap_free_tstamp_types(tstamp_types);
    }
    
    if (pcap_handle) {
        pcap_close(pcap_handle);
    }
    
    fclose(in_file);
    
    // Use the int64_val to prevent compiler optimization
    if (int64_val == 0xDEADBEEF) {
        // This should never happen, just to use the variable
        return 1;
    }

    return 0;
}