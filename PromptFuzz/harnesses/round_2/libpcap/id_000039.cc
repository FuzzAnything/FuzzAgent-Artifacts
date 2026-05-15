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
//<ID> 137
//<Prompt> []
/*<Combination>: [
*/
//<score> 34.666668, nr_unique_branch: 5
//<Quality> {"density":26,"unique_branches":{"setup_socket":[[2709,24,2709,43,0,0,4,0],[2713,7,2714,29,0,0,4,1]],"linux_get_stat":[[927,6,927,14,0,0,4,1],[932,6,932,22,0,0,4,1]],"pcap_activate_linux":[[1245,6,1245,25,0,0,4,0]]},"library_calls":["pcap_init","pcap_fopen_offline","pcap_fileno","pcap_list_datalinks","pcap_free_datalinks","pcap_close","pcap_dump_open","pcap_close","pcap_compile","pcap_next","pcap_offline_filter","pcap_dump","pcap_dump","pcap_create","pcap_set_snaplen","pcap_set_promisc","pcap_set_timeout","pcap_activate","pcap_inject","pcap_close","pcap_next","pcap_offline_filter","pcap_dump","pcap_dump_flush","pcap_freecode","pcap_dump_close","pcap_close"],"critical_calls":["pcap_init","pcap_fopen_offline","pcap_fileno","pcap_list_datalinks","pcap_free_datalinks","pcap_dump_open","pcap_compile","pcap_next","pcap_offline_filter","pcap_dump","pcap_create","pcap_set_snaplen","pcap_set_promisc","pcap_set_timeout","pcap_activate","pcap_inject","pcap_close","pcap_next","pcap_offline_filter","pcap_dump","pcap_dump_flush","pcap_freecode","pcap_dump_close","pcap_close"],"visited":2}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *data, size_t size) {
    if (size < 24) return 0; // Minimum pcap header size
    
    // Step 1: Initialize libpcap
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_init(PCAP_CHAR_ENC_LOCAL, errbuf) != 0) {
        return 0;
    }
    
    // Step 2: Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) return 0;
    
    // Step 3: Open pcap capture from file
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        fclose(in_file);
        return 0;
    }
    
    // Step 4: Get file descriptor (optional)
    int fd = pcap_fileno(pcap);
    // fd might be -1 for offline captures, but we still call the API
    
    // Step 5: List data links
    int *dlt_list = nullptr;
    int dlt_count = pcap_list_datalinks(pcap, &dlt_list);
    if (dlt_list) {
        pcap_free_datalinks(dlt_list);
    }
    
    // Step 6: Prepare output dump file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap);
        fclose(in_file);
        return 0;
    }
    
    pcap_dumper_t *dumper = pcap_dump_open(pcap, "output_file");
    if (!dumper) {
        pcap_close(pcap);
        fclose(in_file);
        fclose(out_file);
        return 0;
    }
    
    // Step 7: Compile BPF filter
    struct bpf_program filter;
    const char *filter_str = "ip or ip6";
    bpf_u_int32 netmask = 0xffffff00; // 255.255.255.0
    
    int compile_result = pcap_compile(pcap, &filter, filter_str, 1, netmask);
    
    // Step 8: Read and process packets
    struct pcap_pkthdr header;
    int packet_count = 0;
    const u_char *packet_data = nullptr;
    
    // Try to read first packet
    packet_data = pcap_next(pcap, &header);
    
    if (packet_data && header.caplen > 0 && header.caplen <= size) {
        // Apply filter if compilation was successful
        if (compile_result == 0) {
            int filter_result = pcap_offline_filter(&filter, &header, packet_data);
            if (filter_result != 0) {
                // Write filtered packet to dump file
                pcap_dump((u_char *)dumper, &header, packet_data);
            }
        } else {
            // Write packet without filtering
            pcap_dump((u_char *)dumper, &header, packet_data);
        }
        
        packet_count++;
        
        // Try to inject the packet (this requires a live capture, but we try anyway)
        // We'll create a dummy pcap for injection attempt
        pcap_t *inject_pcap = pcap_create("lo", errbuf);
        if (inject_pcap) {
            pcap_set_snaplen(inject_pcap, 65535);
            pcap_set_promisc(inject_pcap, 1);
            pcap_set_timeout(inject_pcap, 1000);
            
            if (pcap_activate(inject_pcap) == 0) {
                // Try to inject the packet we just read
                pcap_inject(inject_pcap, packet_data, 
                           std::min(header.caplen, static_cast<bpf_u_int32>(size)));
            }
            pcap_close(inject_pcap);
        }
    }
    
    // Step 9: Try to read more packets with second pcap_next call
    // (as required by instructions to call pcap_next twice)
    const u_char *packet_data2 = pcap_next(pcap, &header);
    if (packet_data2 && header.caplen > 0 && compile_result == 0) {
        // Apply filter
        int filter_result = pcap_offline_filter(&filter, &header, packet_data2);
        if (filter_result != 0) {
            pcap_dump((u_char *)dumper, &header, packet_data2);
        }
    }
    
    // Step 10: Flush and close resources
    pcap_dump_flush(dumper);
    
    // Free filter program if it was compiled
    if (compile_result == 0) {
        pcap_freecode(&filter);
    }
    
    // Close dump file
    pcap_dump_close(dumper);
    
    // Step 11: Close pcap and files
    pcap_close(pcap);
    fclose(in_file);
    fclose(out_file);
    
    // Step 12: Clean up output file
    remove("output_file");
    
    return 0;
}