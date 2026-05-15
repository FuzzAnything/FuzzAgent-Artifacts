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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["pcap_lookupdev","pcap_fopen_offline","pcap_bufsize","pcap_set_protocol_linux","pcap_get_required_select_timeout","pcap_file","pcap_datalink","pcap_datalink_val_to_name","pcap_close","pcap_dump_open_append","pcap_close","pcap_next","pcap_dump","pcap_statustostr","pcap_dump_close","pcap_close"],"critical_calls":["pcap_lookupdev","pcap_fopen_offline","pcap_bufsize","pcap_set_protocol_linux","pcap_get_required_select_timeout","pcap_file","pcap_datalink","pcap_datalink_val_to_name","pcap_dump_open_append","pcap_next","pcap_dump","pcap_statustostr","pcap_dump_close","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *data, size_t size) {
    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        return 0;
    }

    // 1. Lookup device (though not directly used)
    char errbuf[PCAP_ERRBUF_SIZE];
    char *device = pcap_lookupdev(errbuf);
    // Just call it, ignore result for fuzzing

    // 2. Open offline capture from input data
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        fclose(in_file);
        return 0;
    }

    // 3. Get buffer size
    int buf_size = pcap_bufsize(pcap_handle);

    // 4. Set Linux protocol (use a value from data if possible)
    int protocol = size > 0 ? data[0] : 0;
    pcap_set_protocol_linux(pcap_handle, protocol);

    // 5. Get required select timeout
    const struct timeval *timeout = pcap_get_required_select_timeout(pcap_handle);
    (void)timeout; // Use timeout to avoid unused variable warning

    // 6. Get FILE* from pcap handle
    FILE *pcap_file_handle = pcap_file(pcap_handle);
    (void)pcap_file_handle; // Use to avoid unused variable warning

    // 7. Get datalink name
    int dlt = pcap_datalink(pcap_handle);
    const char *dlt_name = pcap_datalink_val_to_name(dlt);
    (void)dlt_name; // Use to avoid unused variable warning

    // 8. Open dump file for appending
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap_handle);
        fclose(in_file);
        return 0;
    }
    pcap_dumper_t *dumper = pcap_dump_open_append(pcap_handle, "output_file");
    if (!dumper) {
        fclose(out_file);
        pcap_close(pcap_handle);
        fclose(in_file);
        return 0;
    }

    // 9. Process packets
    struct pcap_pkthdr header;
    const u_char *packet;
    while ((packet = pcap_next(pcap_handle, &header)) != nullptr) {
        // 10. Dump packet
        pcap_dump((u_char *)dumper, &header, packet);
    }

    // 11. Get status string (use a status code from data if possible)
    int status = size > 1 ? data[1] : 0;
    const char *status_str = pcap_statustostr(status);
    (void)status_str; // Use to avoid unused variable warning

    // 12. Cleanup
    pcap_dump_close(dumper);
    fclose(out_file);
    pcap_close(pcap_handle);
    fclose(in_file);

    // Remove output file to avoid accumulation
    remove("output_file");

    return 0;
}