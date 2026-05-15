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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"pcap_fopen_offline_with_tstamp_precision":[[498,7,498,17,0,0,4,0]]},"library_calls":["pcap_fopen_offline","pcap_get_tstamp_precision","pcap_can_set_rfmon","pcap_setnonblock","pcap_minor_version","pcap_dump_fopen","pcap_dump_file","pcap_dispatch","pcap_dump_close","pcap_close","pcap_fopen_offline","pcap_next_ex","pcap_4_byte_aligned_uint64_val","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_get_tstamp_precision","pcap_can_set_rfmon","pcap_setnonblock","pcap_minor_version","pcap_dump_fopen","pcap_dump_file","pcap_dispatch","pcap_dump_close","pcap_close","pcap_fopen_offline","pcap_next_ex","pcap_4_byte_aligned_uint64_val","pcap_close"],"visited":1}
/**/


// Include necessary libpcap headers

// Helper function to handle packet data in pcap_dispatch
void id_000013_packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Minimal packet processing for fuzzing
    (void)user;
    (void)h;
    (void)bytes;
    // Could process packet here, but for fuzzing we just consume the data
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }

    // Create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        return 0;
    }

    // Create output file for dumping
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        fclose(in_file);
        return 0;
    }

    // Open pcap from the memory file
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        fclose(in_file);
        fclose(out_file);
        return 0;
    }

    // 1. Get timestamp precision
    int tstamp_precision = pcap_get_tstamp_precision(pcap_handle);
    (void)tstamp_precision; // Mark as used

    // 2. Check if can set RFMON (monitor mode) - for file-based captures, typically returns 0
    int can_rfmon = pcap_can_set_rfmon(pcap_handle);
    (void)can_rfmon; // Mark as used

    // 3. Set non-blocking mode (even though file is already non-blocking)
    char nonblock_err[PCAP_ERRBUF_SIZE];
    int set_nonblock_result = pcap_setnonblock(pcap_handle, 1, nonblock_err);
    (void)set_nonblock_result; // Mark as used

    // 4. Get minor version
    int minor_ver = pcap_minor_version(pcap_handle);
    (void)minor_ver; // Mark as used

    // Create a dumper for writing packets
    pcap_dumper_t *dumper = pcap_dump_fopen(pcap_handle, out_file);
    if (dumper) {
        // 5. Get FILE* from dumper
        FILE *dump_file = pcap_dump_file(dumper);
        (void)dump_file; // Mark as used

        // 6. Process packets using pcap_dispatch with our packet handler
        int dispatch_result = pcap_dispatch(pcap_handle, -1, id_000013_packet_handler, (u_char *)dumper);
        (void)dispatch_result; // Mark as used

        // Clean up dumper
        pcap_dump_close(dumper);
    }

    // Reset file position to beginning for second reading method
    fseek(in_file, 0, SEEK_SET);
    pcap_close(pcap_handle);
    pcap_handle = pcap_fopen_offline(in_file, errbuf);
    
    if (pcap_handle) {
        // 7. Alternative packet reading with pcap_next_ex
        struct pcap_pkthdr *pkthdr = nullptr;
        const u_char *pkt_data = nullptr;
        
        while (true) {
            int next_result = pcap_next_ex(pcap_handle, &pkthdr, &pkt_data);
            if (next_result <= 0) {
                break;
            }
            
            // 8. Demonstrate pcap_4_byte_aligned_uint64_val usage
            // Create a dummy aligned uint64 from packet length (if available)
            if (pkthdr && pkt_data) {
                pcap_4_byte_aligned_uint64 aligned_val;
                // Use packet length as an example value
                aligned_val.halves[0] = (uint32_t)pkthdr->len;
                aligned_val.halves[1] = (uint32_t)(pkthdr->len >> 32);
                
                uint64_t real_val = pcap_4_byte_aligned_uint64_val(aligned_val);
                (void)real_val; // Mark as used
            }
        }
    }

    // Cleanup
    if (pcap_handle) {
        pcap_close(pcap_handle);
    }
    fclose(in_file);
    fclose(out_file);
    
    // Remove output file created during fuzzing
    remove("output_file");

    return 0;
}