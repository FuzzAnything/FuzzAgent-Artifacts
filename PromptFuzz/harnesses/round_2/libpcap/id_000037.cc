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
//<ID> 124
//<Prompt> []
/*<Combination>: [
*/
//<score> 16.5, nr_unique_branch: 2
//<Quality> {"density":11,"unique_branches":{"pcap_check_header":[[355,7,355,38,0,0,4,0]],"pcap_next_packet":[[564,2,564,15,0,0,4,0]]},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_set_tstamp_precision","pcap_minor_version","pcap_tstamp_type_val_to_description","pcap_set_datalink","pcap_snapshot","pcap_compile","pcap_next","bpf_filter","pcap_breakloop","pcap_freecode","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_set_tstamp_precision","pcap_minor_version","pcap_tstamp_type_val_to_description","pcap_set_datalink","pcap_snapshot","pcap_compile","pcap_next","bpf_filter","pcap_breakloop","pcap_freecode","pcap_close"],"visited":1}
/**/


// Include libpcap headers

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *data, size_t size) {
    // Early exit if input is too small to be meaningful
    if (size < sizeof(struct pcap_file_header)) {
        return 0;
    }

    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        return 0;
    }

    // Open pcap file for reading with nanosecond timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap = pcap_fopen_offline_with_tstamp_precision(in_file, 
                                                           PCAP_TSTAMP_PRECISION_NANO, 
                                                           errbuf);
    if (!pcap) {
        fclose(in_file);
        return 0;
    }

    // Try to set timestamp precision (may fail for offline captures)
    pcap_set_tstamp_precision(pcap, PCAP_TSTAMP_PRECISION_MICRO);

    // Get pcap minor version
    int minor_ver = pcap_minor_version(pcap);
    (void)minor_ver; // Use variable to avoid unused warning

    // Get timestamp type description
    const char *ts_desc = pcap_tstamp_type_val_to_description(PCAP_TSTAMP_HOST);
    (void)ts_desc;

    // Try to set datalink type (may fail for offline captures)
    pcap_set_datalink(pcap, DLT_EN10MB);

    // Get snapshot length
    int snaplen = pcap_snapshot(pcap);
    (void)snaplen;

    // Compile BPF filter
    struct bpf_program fp;
    const char *filter_str = "ip";
    bpf_u_int32 netmask = 0xffffff00;
    if (pcap_compile(pcap, &fp, filter_str, 1, netmask) == 0) {
        // Process packets
        struct pcap_pkthdr header;
        const u_char *packet;
        int packet_count = 0;
        const int max_packets = 100; // Limit for fuzzing
        
        while ((packet = pcap_next(pcap, &header)) != nullptr && 
               packet_count++ < max_packets) {
            
            // Apply BPF filter to packet
            u_int result = bpf_filter(fp.bf_insns, packet, header.len, header.caplen);
            (void)result;
            
            // Break loop conditionally
            if (packet_count > 50) {
                pcap_breakloop(pcap);
                break;
            }
        }
        
        // Free compiled filter
        pcap_freecode(&fp);
    }

    // Cleanup
    pcap_close(pcap);
    fclose(in_file); // pcap_close should close the FILE*, but we'll close it explicitly
    
    return 0;
}