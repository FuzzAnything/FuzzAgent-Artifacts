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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_minor_version","pcap_can_set_rfmon","pcap_setnonblock","pcap_get_tstamp_precision","pcap_close","pcap_dump_fopen","pcap_close","pcap_dump_file","pcap_4_byte_aligned_uint64_val","pcap_dump","pcap_dump","pcap_dispatch","pcap_next_ex","pcap_dump","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_minor_version","pcap_can_set_rfmon","pcap_setnonblock","pcap_get_tstamp_precision","pcap_dump_fopen","pcap_dump_file","pcap_4_byte_aligned_uint64_val","pcap_dump","pcap_dump","pcap_dispatch","pcap_next_ex","pcap_dump","pcap_dump_close","pcap_close"],"visited":0}
/**/


// libpcap headers

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *data, size_t size) {
    // Return early if there's not enough data for basic operations
    if (size < sizeof(pcap_4_byte_aligned_uint64) + 1) {
        return 0;
    }

    // Create input FILE* from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        return 0;
    }

    // Open pcap from the input file
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        fclose(in_file);
        return 0;
    }

    // Get minor version (API requirement)
    int minor_ver = pcap_minor_version(pcap);

    // Check if we can set rfmon (API requirement)
    int can_rfmon = pcap_can_set_rfmon(pcap);

    // Set non-blocking mode (API requirement)
    int set_nonblock = pcap_setnonblock(pcap, 1, errbuf);

    // Get timestamp precision (API requirement)
    int ts_precision = pcap_get_tstamp_precision(pcap);

    // Create output file for dumping
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap);
        fclose(in_file);
        return 0;
    }

    // Create dumper for output
    pcap_dumper_t *dumper = pcap_dump_fopen(pcap, out_file);
    if (!dumper) {
        fclose(out_file);
        pcap_close(pcap);
        fclose(in_file);
        return 0;
    }

    // Get dumper's FILE* (API requirement)
    FILE *dump_file_ptr = pcap_dump_file(dumper);

    // Convert 4-byte aligned uint64 value from input data (API requirement)
    pcap_4_byte_aligned_uint64 aligned_val;
    memcpy(&aligned_val, data, sizeof(pcap_4_byte_aligned_uint64));
    uint64_t uint64_val = pcap_4_byte_aligned_uint64_val(aligned_val);

    // Define packet handler callback
    auto packet_handler = [](u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
        pcap_dumper_t *dmp = (pcap_dumper_t *)user;
        pcap_dump((u_char *)dmp, h, bytes);
    };

    // Use pcap_dispatch to process packets (API requirement)
    int dispatch_count = pcap_dispatch(pcap, 5, packet_handler, (u_char *)dumper);

    // Use pcap_next_ex to get next packet (API requirement)
    struct pcap_pkthdr *pkt_header = nullptr;
    const u_char *pkt_data = nullptr;
    int next_ex_result = pcap_next_ex(pcap, &pkt_header, &pkt_data);
    
    // If we got a packet, dump it
    if (next_ex_result == 1 && pkt_header && pkt_data) {
        pcap_dump((u_char *)dumper, pkt_header, pkt_data);
    }

    // Cleanup resources
    pcap_dump_close(dumper);
    fclose(out_file);
    pcap_close(pcap);
    fclose(in_file);

    // Use the variables to avoid compiler warnings
    (void)minor_ver;
    (void)can_rfmon;
    (void)set_nonblock;
    (void)ts_precision;
    (void)dump_file_ptr;
    (void)uint64_val;
    (void)dispatch_count;
    (void)next_ex_result;

    return 0;
}