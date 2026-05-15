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
//<ID> 14
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_set_buffer_size","pcap_strerror","pcap_set_immediate_mode","pcap_strerror","pcap_set_timeout","pcap_strerror","pcap_datalink_ext","pcap_close","pcap_dump_fopen","pcap_close","pcap_activate","pcap_strerror","pcap_geterr","pcap_loop","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_set_buffer_size","pcap_strerror","pcap_set_immediate_mode","pcap_strerror","pcap_set_timeout","pcap_strerror","pcap_datalink_ext","pcap_dump_fopen","pcap_activate","pcap_strerror","pcap_geterr","pcap_loop","pcap_dump_close","pcap_close"],"visited":0}
/**/


extern "C" {
}

// Custom packet handler for pcap_loop that calls pcap_dump
static void packet_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    // Use pcap_dump as required
    pcap_dump(user, h, bytes);
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }

    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        return 0;
    }

    // Step 1: Open pcap from memory stream
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        fclose(in_file);
        return 0;
    }

    // Step 2: Try to set buffer size (might fail for offline capture, but call it anyway)
    int buffer_size = 1024 * 1024; // 1MB
    if (pcap_set_buffer_size(pcap, buffer_size) != 0) {
        // Optional: Get error using pcap_strerror
        const char *err = pcap_strerror(1); // Using generic error code
        // Error can be ignored for fuzzing
    }

    // Step 3: Try to set immediate mode (might fail for offline capture)
    if (pcap_set_immediate_mode(pcap, 1) != 0) {
        const char *err = pcap_strerror(2);
    }

    // Step 4: Try to set timeout (might fail for offline capture)
    if (pcap_set_timeout(pcap, 1000) != 0) { // 1000ms timeout
        const char *err = pcap_strerror(3);
    }

    // Step 5: Get data link type
    int dlt = pcap_datalink_ext(pcap);
    // We don't use the result but the API is called

    // Step 6: Open output file for dumping
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

    // Step 7: Try to activate (will likely fail for offline capture, but call it)
    // Note: pcap_activate is meant for live captures, but we call it anyway
    if (pcap_activate(pcap) != 0) {
        const char *err = pcap_strerror(pcap_geterr(pcap) ? 4 : 0);
    }

    // Step 8: Process packets using pcap_loop which will call pcap_dump via packet_handler
    // We use dumper as the user data for packet_handler
    int packet_count = pcap_loop(pcap, -1, packet_handler, (u_char *)dumper);

    // Cleanup - free all resources
    pcap_dump_close(dumper);
    fclose(out_file);
    pcap_close(pcap);
    fclose(in_file);

    // Remove the output file created during fuzzing
    remove("output_file");

    return 0;
}