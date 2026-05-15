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
//<ID> 331
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_set_tstamp_type","pcap_setdirection","pcap_datalink","pcap_snapshot","pcap_compile_nopcap","bpf_dump","pcap_findalldevs","pcap_freealldevs","pcap_freecode","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_set_tstamp_type","pcap_setdirection","pcap_datalink","pcap_snapshot","pcap_compile_nopcap","bpf_dump","pcap_findalldevs","pcap_freealldevs","pcap_freecode","pcap_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *data, size_t size) {
    if (size < 1) return 0;

    // 1. Create a FILE* for reading the input data (assumed to be pcap output)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) return 0;

    // 2. Open the pcap capture file for offline reading
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        fclose(in_file);
        return 0;
    }

    // 3. Try to set timestamp type (use first byte of data to choose type)
    int tstamp_type = (data[0] % 3) + 1; // Some valid types
    if (pcap_set_tstamp_type(pcap, tstamp_type) != 0) {
        // Continue even if setting fails
    }

    // 4. Try to set direction (use second byte for direction)
    pcap_direction_t direction = (data[1] % 2 == 0) ? PCAP_D_IN : PCAP_D_OUT;
    if (pcap_setdirection(pcap, direction) != 0) {
        // Continue even if setting fails
    }

    // 5. Get link type and snapshot length for pcap_compile_nopcap
    int linktype = pcap_datalink(pcap);
    int snaplen = pcap_snapshot(pcap);
    
    // 6. Create BPF program structure
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    
    // 7. Use pcap_compile_nopcap with a simple filter
    // Use a portion of data as filter string (avoid null bytes)
    char filter_str[256];
    size_t filter_len = (size < 255) ? size : 255;
    for (size_t i = 0; i < filter_len; i++) {
        filter_str[i] = (data[i] == 0) ? ' ' : data[i];
    }
    filter_str[filter_len] = '\0';
    
    int optimize = 1;
    bpf_u_int32 netmask = 0;
    if (pcap_compile_nopcap(linktype, snaplen, &fp, filter_str, optimize, netmask) == 0) {
        // 8. Dump the compiled BPF program
        bpf_dump(&fp, 1);  // 1 for verbose output
    }
    
    // 9. Find and free all network devices (use pcap_freealldevs)
    pcap_if_t *alldevs;
    if (pcap_findalldevs(&alldevs, errbuf) == 0) {
        pcap_freealldevs(alldevs);
    }
    
    // 10. Clean up resources
    pcap_freecode(&fp);
    pcap_close(pcap);
    fclose(in_file);
    
    return 0;
}