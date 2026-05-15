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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["pcap_lib_version","pcap_fopen_offline","pcap_list_datalinks","pcap_free_datalinks","pcap_lookupnet","pcap_close","pcap_dump_fopen","pcap_close","pcap_compile","bpf_filter","pcap_freecode","pcap_next","pcap_dump","pcap_sendpacket","pcap_perror","pcap_dump_close","pcap_close"],"critical_calls":["pcap_lib_version","pcap_fopen_offline","pcap_list_datalinks","pcap_free_datalinks","pcap_lookupnet","pcap_dump_fopen","pcap_compile","bpf_filter","pcap_freecode","pcap_next","pcap_dump","pcap_sendpacket","pcap_perror","pcap_dump_close","pcap_close"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }

    // 1. Create a FILE* from input data to simulate libpcap output data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        return 0;
    }

    // 2. Print libpcap version (required API)
    const char *version = pcap_lib_version();

    // 3. Open pcap file for reading
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        fclose(in_file);
        return 0;
    }

    // 4. Get data link types (required API)
    int *dlt_list = nullptr;
    int dlt_count = pcap_list_datalinks(pcap, &dlt_list);
    if (dlt_count > 0 && dlt_list) {
        pcap_free_datalinks(dlt_list);
    }

    // 5. Create dummy network lookup (required API)
    bpf_u_int32 net = 0, mask = 0;
    char dummy_device[] = "dummy0";
    if (pcap_lookupnet(dummy_device, &net, &mask, errbuf) == -1) {
        // This is expected for dummy device - continue anyway
    }

    // 6. Open output file for dumping
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap);
        fclose(in_file);
        return 0;
    }
    
    pcap_dumper_t *dumper = pcap_dump_fopen(pcap, out_file);
    if (!dumper) {
        fclose(out_file);
        pcap_close(pcap);
        fclose(in_file);
        return 0;
    }

    // 7. Create a simple BPF filter (required API)
    struct bpf_program fp;
    const char *filter_str = "ip";
    bpf_u_int32 netp = net; // Use network from lookup
    if (pcap_compile(pcap, &fp, filter_str, 1, netp) == -1) {
        // Filter compilation failed - continue without filter
    } else {
        // Create a dummy packet to test bpf_filter (required API)
        struct bpf_insn *insns = fp.bf_insns;
        int insn_count = fp.bf_len;
        
        // Create dummy packet data (minimal Ethernet+IP header)
        uint8_t dummy_packet[60] = {0};
        if (size >= 60) {
            memcpy(dummy_packet, data, 60 > size ? size : 60);
        }
        
        // Apply BPF filter
        u_int result = bpf_filter(insns, dummy_packet, 60, 60);
        (void)result; // Result not used in fuzzing context
        
        pcap_freecode(&fp);
    }

    // 8. Read and process packets
    struct pcap_pkthdr header;
    const u_char *packet;
    
    while ((packet = pcap_next(pcap, &header)) != nullptr) {
        // Dump packet to file
        pcap_dump((u_char *)dumper, &header, packet);
        
        // Try to send packet back (required API)
        // Note: In real usage, this would need a live capture handle
        // For fuzzing, we use the same pcap handle which may fail
        int send_result = pcap_sendpacket(pcap, packet, header.caplen);
        (void)send_result; // Result not used in fuzzing context
        
        // Print any errors using pcap_perror (required API)
        if (send_result == -1) {
            pcap_perror(pcap, "Sending packet failed");
        }
    }

    // 9. Clean up resources
    pcap_dump_close(dumper);
    fclose(out_file);
    pcap_close(pcap);
    fclose(in_file);

    return 0;
}