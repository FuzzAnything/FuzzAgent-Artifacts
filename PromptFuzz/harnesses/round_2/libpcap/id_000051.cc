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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"pcap_tstamp_type_val_to_description":[[3510,14,3510,49,0,0,4,1]]},"library_calls":["pcap_fopen_offline","pcap_minor_version","pcap_snapshot","pcap_set_tstamp_precision","pcap_tstamp_type_val_to_description","pcap_set_datalink","pcap_dump_fopen","pcap_setnonblock","pcap_compile","pcap_setfilter","pcap_compile","pcap_setfilter","pcap_next","pcap_dump","pcap_breakloop","pcap_dump_flush","pcap_dump_close","pcap_freecode","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_minor_version","pcap_snapshot","pcap_set_tstamp_precision","pcap_tstamp_type_val_to_description","pcap_set_datalink","pcap_dump_fopen","pcap_setnonblock","pcap_compile","pcap_setfilter","pcap_next","pcap_dump","pcap_breakloop","pcap_dump_flush","pcap_dump_close","pcap_freecode","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    pcap_t *pcap = nullptr;
    pcap_dumper_t *dumper = nullptr;
    struct bpf_program filter_program;
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    
    // 1. Create input file from data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) return 0;
    
    // 2. Open pcap file for reading
    pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        fclose(in_file);
        return 0;
    }
    
    // 3. Get minor version
    int minor_ver = pcap_minor_version(pcap);
    
    // 4. Get snapshot length
    int snap_len = pcap_snapshot(pcap);
    
    // 5. Set timestamp precision (try both micro and nano)
    int tstamp_precision = (data[0] % 2) ? PCAP_TSTAMP_PRECISION_MICRO : PCAP_TSTAMP_PRECISION_NANO;
    pcap_set_tstamp_precision(pcap, tstamp_precision);
    
    // 6. Get timestamp type description (use first 4 bytes for type value)
    int tstamp_type = 0;
    if (size >= 4) {
        memcpy(&tstamp_type, data, sizeof(int));
        tstamp_type = tstamp_type % 10; // Keep it in valid range
    }
    const char *tstamp_desc = pcap_tstamp_type_val_to_description(tstamp_type);
    
    // 7. Set datalink type (cycle through valid types based on input)
    int dlt = DLT_EN10MB; // Default Ethernet
    if (size >= 8) {
        memcpy(&dlt, data + 4, sizeof(int));
        dlt = abs(dlt) % 256; // Keep in reasonable range
        pcap_set_datalink(pcap, dlt);
    }
    
    // 8. Open output file for dumping
    out_file = fopen("output_file", "wb");
    if (out_file) {
        dumper = pcap_dump_fopen(pcap, out_file);
    }
    
    // 9. Set non-blocking mode (toggle based on input)
    int nonblock = (size >= 12 && (data[11] % 2)) ? 1 : 0;
    pcap_setnonblock(pcap, nonblock, errbuf);
    
    // 10. Compile filter (use data to construct filter string)
    memset(&filter_program, 0, sizeof(filter_program));
    const char *filter_str = "ip";
    bpf_u_int32 netmask = 0xffffff00;
    
    if (size >= 16) {
        // Create filter string from data if we have enough bytes
        size_t filter_len = size < 100 ? size : 100;
        char *dynamic_filter = (char*)malloc(filter_len + 1);
        if (dynamic_filter) {
            for (size_t i = 0; i < filter_len; i++) {
                dynamic_filter[i] = (data[i % size] % 95) + 32; // Printable ASCII
            }
            dynamic_filter[filter_len] = '\0';
            
            // Try to compile with dynamic filter
            if (pcap_compile(pcap, &filter_program, dynamic_filter, 0, netmask) == 0) {
                // Set the filter if compilation succeeded
                pcap_setfilter(pcap, &filter_program);
            }
            free(dynamic_filter);
        }
    } else {
        // Use default filter
        pcap_compile(pcap, &filter_program, filter_str, 0, netmask);
        pcap_setfilter(pcap, &filter_program);
    }
    
    // 11. Read and process packets
    struct pcap_pkthdr header;
    int packet_count = 0;
    const u_char *packet;
    
    while ((packet = pcap_next(pcap, &header)) != nullptr && packet_count < 100) {
        packet_count++;
        
        // Dump packet if dumper is available
        if (dumper) {
            pcap_dump((u_char*)dumper, &header, packet);
        }
        
        // Break loop after certain condition (based on input data)
        if (size >= 20 && packet_count > (data[19] % 10)) {
            pcap_breakloop(pcap);
            break;
        }
    }
    
    // 12. Flush dumper
    if (dumper) {
        pcap_dump_flush(dumper);
    }
    
    // 13. Cleanup
    if (dumper) {
        pcap_dump_close(dumper);
    }
    
    pcap_freecode(&filter_program);
    pcap_close(pcap);
    
    if (out_file) {
        fclose(out_file);
    }
    
    if (in_file) {
        fclose(in_file);
    }
    
    return 0;
}