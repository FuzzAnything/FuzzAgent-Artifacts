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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_close","pcap_dump_fopen","pcap_close","pcap_dump_file","pcap_4_byte_aligned_uint64_val","pcap_get_tstamp_precision","pcap_can_set_rfmon","pcap_setnonblock","pcap_minor_version","pcap_dispatch","pcap_next_ex","pcap_dump","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_dump_fopen","pcap_dump_file","pcap_4_byte_aligned_uint64_val","pcap_get_tstamp_precision","pcap_can_set_rfmon","pcap_setnonblock","pcap_minor_version","pcap_dispatch","pcap_next_ex","pcap_dump","pcap_dump_close","pcap_close"],"visited":0}
/**/


extern "C" {
}

// Handler function for pcap_dispatch
void id_000014_packet_handler(u_char *user, const struct pcap_pkthdr *hdr, const u_char *packet) {
    pcap_dumper_t *dumper = reinterpret_cast<pcap_dumper_t*>(user);
    
    // Dump packet to output file
    pcap_dump(reinterpret_cast<u_char*>(dumper), hdr, packet);
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *data, size_t size) {
    if (size < 24) return 0; // Minimum size for pcap file header
    
    // Create input FILE* from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) return 0;
    
    // Open pcap file for reading with default timestamp precision
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        fclose(in_file);
        return 0;
    }
    
    // Create output pcap file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap);
        fclose(in_file);
        return 0;
    }
    
    // Create pcap dumper for output file
    pcap_dumper_t *dumper = pcap_dump_fopen(pcap, out_file);
    if (!dumper) {
        fclose(out_file);
        pcap_close(pcap);
        fclose(in_file);
        return 0;
    }
    
    // Get FILE* from dumper using pcap_dump_file
    FILE *dump_file = pcap_dump_file(dumper);
    if (dump_file) {
        // Use pcap_4_byte_aligned_uint64_val with a dummy value
        pcap_4_byte_aligned_uint64 aligned_val;
        memset(&aligned_val, 0, sizeof(aligned_val));
        uint64_t val = pcap_4_byte_aligned_uint64_val(aligned_val);
        (void)val; // Suppress unused variable warning
    }
    
    // Get timestamp precision
    int tstamp_precision = pcap_get_tstamp_precision(pcap);
    (void)tstamp_precision; // Suppress unused variable warning
    
    // Check if we can set rfmon mode (even though we're reading from file)
    int can_rfmon = pcap_can_set_rfmon(pcap);
    (void)can_rfmon; // Suppress unused variable warning
    
    // Set non-blocking mode (won't affect file reading but we need to call it)
    char nonblock_err[PCAP_ERRBUF_SIZE] = {0};
    if (pcap_setnonblock(pcap, 1, nonblock_err) < 0) {
        // Non-blocking mode might not be supported for offline capture
    }
    
    // Get minor version
    int minor_ver = pcap_minor_version(pcap);
    (void)minor_ver; // Suppress unused variable warning
    
    // Process packets using pcap_dispatch with packet handler
    int packet_count = pcap_dispatch(pcap, 10, id_000014_packet_handler, reinterpret_cast<u_char*>(dumper));
    
    // Process remaining packets using pcap_next_ex
    struct pcap_pkthdr *pkthdr = nullptr;
    const u_char *pktdata = nullptr;
    int ret = 0;
    
    while ((ret = pcap_next_ex(pcap, &pkthdr, &pktdata)) == 1) {
        // Dump packet to output file
        pcap_dump(reinterpret_cast<u_char*>(dumper), pkthdr, pktdata);
    }
    
    // Cleanup
    pcap_dump_close(dumper);
    fclose(out_file);
    pcap_close(pcap);
    fclose(in_file);
    
    // Remove output file to avoid accumulating data
    remove("output_file");
    
    return 0;
}