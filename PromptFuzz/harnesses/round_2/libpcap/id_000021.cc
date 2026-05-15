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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["pcap_lib_version","pcap_fopen_offline","pcap_list_datalinks","pcap_lookupnet","pcap_free_datalinks","pcap_next","bpf_filter","pcap_dump_open","pcap_dump","pcap_sendpacket","pcap_dump_close","pcap_perror","pcap_close"],"critical_calls":["pcap_lib_version","pcap_fopen_offline","pcap_list_datalinks","pcap_lookupnet","pcap_free_datalinks","pcap_next","bpf_filter","pcap_dump_open","pcap_dump","pcap_sendpacket","pcap_dump_close","pcap_perror","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *data, size_t size) {
    if (size < sizeof(struct pcap_file_header)) {
        return 0;
    }

    // Use the input data as a pcap file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        return 0;
    }

    // Get libpcap version
    const char *version = pcap_lib_version();

    // Open pcap file from memory
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        fclose(in_file);
        return 0;
    }

    // List datalinks
    int *dlt_buf = nullptr;
    int dlt_count = pcap_list_datalinks(pcap, &dlt_buf);
    if (dlt_count > 0) {
        // Try to lookup network for "lo" (loopback interface)
        bpf_u_int32 netp = 0, maskp = 0;
        char errbuf2[PCAP_ERRBUF_SIZE];
        pcap_lookupnet("lo", &netp, &maskp, errbuf2);
        
        // Free the datalinks list
        if (dlt_buf) {
            pcap_free_datalinks(dlt_buf);
        }
    }

    // Create dummy BPF filter that accepts everything
    struct bpf_insn insns[] = {
        { 0x06, 0, 0, 0xFFFFFFFF }, // RET #0xFFFFFFFF
    };
    
    // Read first packet from pcap file
    struct pcap_pkthdr header;
    const u_char *packet = pcap_next(pcap, &header);
    
    // Apply BPF filter if we have a packet
    if (packet) {
        u_int filtered = bpf_filter(insns, packet, header.len, header.caplen);
        (void)filtered; // Use the result
    }
    
    // Create output pcap dumper
    pcap_dumper_t *dumper = pcap_dump_open(pcap, "output_file");
    if (dumper) {
        // Dump packet if we have one
        if (packet) {
            pcap_dump((u_char *)dumper, &header, packet);
        }
        
        // Try to send packet if we have one (using the same pcap handle)
        if (packet) {
            pcap_sendpacket(pcap, packet, header.caplen);
        }
        
        // Close dumper
        pcap_dump_close(dumper);
        
        // Remove the output file
        remove("output_file");
    }
    
    // Print any errors
    pcap_perror(pcap, "PCAP Error");
    
    // Cleanup
    pcap_close(pcap);
    fclose(in_file);
    
    return 0;
}