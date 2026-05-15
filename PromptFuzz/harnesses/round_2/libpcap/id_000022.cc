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
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["pcap_lib_version","pcap_fopen_offline","pcap_dump_open","pcap_perror","pcap_close","pcap_lookupnet","pcap_perror","pcap_list_datalinks","pcap_free_datalinks","pcap_next","bpf_filter","pcap_dump","pcap_create","pcap_activate","pcap_sendpacket","pcap_close","pcap_perror","pcap_dump_close","pcap_close"],"critical_calls":["pcap_lib_version","pcap_fopen_offline","pcap_dump_open","pcap_lookupnet","pcap_perror","pcap_list_datalinks","pcap_free_datalinks","pcap_next","bpf_filter","pcap_dump","pcap_create","pcap_activate","pcap_sendpacket","pcap_close","pcap_perror","pcap_dump_close","pcap_close"],"visited":1}
/**/


// Include necessary libpcap headers

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *data, size_t size) {
    // Create a FILE pointer from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        return 0;
    }

    // Get libpcap version
    const char *version = pcap_lib_version();
    (void)version; // Use version to avoid unused variable warning

    // Open the pcap file from memory
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        fclose(in_file);
        return 0;
    }

    // Open a dumper to write filtered packets
    pcap_dumper_t *dumper = pcap_dump_open(pcap_handle, "output_file");
    if (!dumper) {
        pcap_perror(pcap_handle, "Failed to open dumper");
        pcap_close(pcap_handle);
        fclose(in_file);
        return 0;
    }

    // Lookup network for loopback interface
    bpf_u_int32 netp, maskp;
    if (pcap_lookupnet("lo", &netp, &maskp, errbuf) == -1) {
        pcap_perror(pcap_handle, "Network lookup failed");
    }

    // Get list of datalinks
    int *datalinks;
    int count = pcap_list_datalinks(pcap_handle, &datalinks);
    if (count > 0) {
        pcap_free_datalinks(datalinks);
    }

    // Create a simple BPF filter program for IPv4 packets
    struct bpf_insn filter_program[] = {
        { 0x28, 0, 0, 0x0000000c }, // Load ethertype offset
        { 0x15, 0, 1, 0x00000800 }, // Compare with IPv4 (0x0800)
        { 0x06, 0, 0, 0x00000000 }, // Return 0 (drop)
        { 0x06, 0, 0, 0xffffffff }  // Return -1 (accept)
    };

    // Process packets from the input
    struct pcap_pkthdr header;
    const u_char *packet;
    int packet_count = 0;
    
    while ((packet = pcap_next(pcap_handle, &header)) != nullptr && packet_count < 100) {
        packet_count++;
        
        // Apply BPF filter to the packet
        u_int result = bpf_filter(filter_program, packet, header.len, header.caplen);
        
        if (result > 0) {
            // Write filtered packet to dumper
            pcap_dump((u_char *)dumper, &header, packet);
            
            // Send packet (using loopback device if we had one)
            // Note: pcap_sendpacket requires an active interface handle
            // We'll create a temporary handle for sending
            pcap_t *send_handle = pcap_create("lo", errbuf);
            if (send_handle) {
                if (pcap_activate(send_handle) == 0) {
                    // Send a minimal test packet if the input packet is too small
                    if (header.caplen >= 14) { // Ethernet header size
                        u_char test_packet[] = {
                            0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Destination MAC
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source MAC
                            0x08, 0x00,                         // EtherType: IPv4
                            // Minimal IPv4 header (20 bytes)
                            0x45, 0x00, 0x00, 0x14,
                            0x00, 0x00, 0x40, 0x00,
                            0x40, 0x01, 0x00, 0x00,
                            0x7f, 0x00, 0x00, 0x01,
                            0x7f, 0x00, 0x00, 0x01
                        };
                        
                        // Use first few bytes from input for the packet data if available
                        size_t copy_len = (size < sizeof(test_packet)) ? size : sizeof(test_packet);
                        if (copy_len > 14) { // Don't overwrite Ethernet header
                            memcpy(test_packet + 14, data, copy_len - 14);
                        }
                        
                        pcap_sendpacket(send_handle, test_packet, sizeof(test_packet));
                    }
                }
                pcap_close(send_handle);
            }
        }
    }

    // Report any errors
    if (packet_count == 0) {
        pcap_perror(pcap_handle, "No packets processed");
    }

    // Clean up all resources
    pcap_dump_close(dumper);
    pcap_close(pcap_handle);
    fclose(in_file);
    
    // Also close the output file if it was opened
    FILE *out_file = fopen("output_file", "rb");
    if (out_file) {
        fclose(out_file);
        remove("output_file");
    }

    return 0;
}