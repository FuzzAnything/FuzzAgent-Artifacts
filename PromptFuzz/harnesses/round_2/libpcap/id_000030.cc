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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["pcap_init","pcap_fopen_offline","pcap_lookupnet","pcap_snapshot","pcap_datalink","pcap_compile_nopcap","pcap_minor_version","pcap_setdirection","pcap_setfilter","pcap_loop","pcap_freecode","bpf_filter","bpf_validate","pcap_tstamp_type_val_to_name","pcap_close"],"critical_calls":["pcap_init","pcap_fopen_offline","pcap_lookupnet","pcap_snapshot","pcap_datalink","pcap_compile_nopcap","pcap_minor_version","pcap_setdirection","pcap_setfilter","pcap_loop","pcap_freecode","bpf_filter","bpf_validate","pcap_tstamp_type_val_to_name","pcap_close"],"visited":0}
/**/


extern "C" {
    #include "pcap/pcap.h"
}

// Custom packet handler callback
static void packet_handler(u_char *user_data, const struct pcap_pkthdr *header, const u_char *packet) {
    // Simple processing: count packets
    int *packet_count = (int *)user_data;
    (*packet_count)++;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *data, size_t size) {
    if (size < 100) return 0;  // Need minimum data for meaningful processing
    
    // 1. Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_init(PCAP_CHAR_ENC_LOCAL, errbuf) < 0) {
        return 0;
    }
    
    // 2. Create FILE* from input data for reading pcap data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) return 0;
    
    // 3. Open pcap capture from file
    pcap_t *handle = pcap_fopen_offline(in_file, errbuf);
    if (!handle) {
        fclose(in_file);
        return 0;
    }
    
    // 4. Get network information (simulate for a dummy interface)
    bpf_u_int32 netp = 0, maskp = 0;
    const char *dummy_device = "eth0";
    int lookup_result = pcap_lookupnet(dummy_device, &netp, &maskp, errbuf);
    // Note: This may fail since "eth0" might not exist, but we still called the API
    
    // 5. Compile a BPF filter program without active capture
    struct bpf_program filter_prog;
    const char *filter_str = "ip";  // Simple IP filter
    bpf_u_int32 netmask = 0xFFFFFF00;  // 255.255.255.0
    
    // Use compile_nopcap with dummy parameters
    int snap_len = pcap_snapshot(handle);
    int linktype = pcap_datalink(handle);
    int compile_result = pcap_compile_nopcap(snap_len, linktype, &filter_prog, 
                                           filter_str, 1, netmask);
    
    // 6. Get library minor version
    int minor_ver = pcap_minor_version(handle);
    
    // 7. Set capture direction (if supported)
    int dir_result = pcap_setdirection(handle, PCAP_D_IN);
    
    // 8. Process packets using pcap_loop with BPF filtering
    int packet_count = 0;
    int loop_result = 0;
    
    if (compile_result == 0) {
        // Set compiled filter
        if (pcap_setfilter(handle, &filter_prog) == 0) {
            // Process up to 10 packets
            loop_result = pcap_loop(handle, 10, packet_handler, (u_char *)&packet_count);
        }
        pcap_freecode(&filter_prog);
    }
    
    // 9. Apply BPF filter directly on sample packet data
    // Create a simple BPF program structure
    struct bpf_insn insns[] = {
        { 0x06, 0, 0, 0xFFFFFFFF },  // RET #-1 (pass all)
    };
    struct bpf_program direct_prog;
    direct_prog.bf_len = 1;
    direct_prog.bf_insns = insns;
    
    // Test filter on a sample packet (using middle portion of input data)
    if (size > 100) {
        u_int filter_result = bpf_filter(insns, data + 50, 
                                         (size - 50 > 65535) ? 65535 : size - 50, 
                                         size - 50);
        // Validate BPF program
        int valid = bpf_validate(insns, 1);
    }
    
    // 10. Get timestamp type name
    const char *ts_name = pcap_tstamp_type_val_to_name(PCAP_TSTAMP_HOST);
    
    // Cleanup
    pcap_close(handle);
    fclose(in_file);
    
    return 0;
}