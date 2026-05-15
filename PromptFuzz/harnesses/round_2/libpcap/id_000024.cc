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
//<ID> 43
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"gen_bsd_af_linktype_live":[[2963,2,2963,9,0,0,4,0]]},"library_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_set_tstamp_precision","pcap_tstamp_type_val_to_description","pcap_datalink","pcap_compile_nopcap","pcap_set_datalink","pcap_next","bpf_filter","pcap_breakloop","pcap_freecode","pcap_close"],"critical_calls":["pcap_fopen_offline_with_tstamp_precision","pcap_snapshot","pcap_set_tstamp_precision","pcap_tstamp_type_val_to_description","pcap_datalink","pcap_compile_nopcap","pcap_set_datalink","pcap_next","bpf_filter","pcap_breakloop","pcap_freecode","pcap_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *data, size_t size) {
    if (size < 24) return 0; // Minimum size for pcap file header
    
    // Step 1: Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) return 0;
    
    // Step 2: Open pcap file with microsecond timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap = pcap_fopen_offline_with_tstamp_precision(
        in_file, 
        PCAP_TSTAMP_PRECISION_MICRO, 
        errbuf
    );
    
    if (!pcap) {
        fclose(in_file);
        return 0;
    }
    
    // Step 3: Get snapshot length
    int snaplen = pcap_snapshot(pcap);
    
    // Step 4: Set nanosecond timestamp precision (if supported)
    // Note: This might fail for offline files, but we try it anyway
    pcap_set_tstamp_precision(pcap, PCAP_TSTAMP_PRECISION_NANO);
    
    // Step 5: Get description of timestamp type
    const char *tstamp_desc = pcap_tstamp_type_val_to_description(PCAP_TSTAMP_HOST);
    
    // Step 6: Compile BPF filter for offline processing
    struct bpf_program bpf;
    const char *filter_str = "ip or arp";
    bpf_u_int32 netmask = 0xffffff00; // 255.255.255.0
    
    // Get datalink type for compilation
    int dlt = pcap_datalink(pcap);
    
    // Compile filter using pcap_compile_nopcap
    int compile_result = pcap_compile_nopcap(
        snaplen,                    // snapshot length
        dlt,                        // data link type
        &bpf,                       // BPF program
        filter_str,                 // filter string
        1,                          // optimize
        netmask                     // netmask
    );
    
    // Step 7: Set datalink type (might fail for offline files)
    pcap_set_datalink(pcap, DLT_EN10MB);
    
    // Step 8: Read packets with breakloop capability
    int packet_count = 0;
    struct pcap_pkthdr header;
    const u_char *packet;
    
    // Set up a handler that will break after 10 packets
    while ((packet = pcap_next(pcap, &header)) != nullptr) {
        packet_count++;
        
        // Apply BPF filter if compilation was successful
        if (compile_result == 0) {
            if (bpf_filter(bpf.bf_insns, packet, header.len, header.caplen)) {
                // Packet matches filter
            }
        }
        
        // Break after 10 packets to avoid infinite loop
        if (packet_count >= 10) {
            pcap_breakloop(pcap);
            break;
        }
    }
    
    // Step 9: Cleanup resources
    if (compile_result == 0) {
        pcap_freecode(&bpf);
    }
    
    pcap_close(pcap);
    // Note: pcap_fopen_offline_with_tstamp_precision closes the FILE*
    // So we don't need to fclose(in_file) here
    
    // Test tstamp_desc usage to avoid unused variable warning
    if (tstamp_desc) {
        // Do nothing, just reference it
    }
    
    return 0;
}