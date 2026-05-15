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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"pcap_datalink_val_to_description_or_dlt":[[3457,13,3457,32,0,0,4,0]]},"library_calls":["pcap_fopen_offline","pcap_set_snaplen","pcap_datalink","pcap_datalink_val_to_description_or_dlt","pcap_datalink_val_to_name","pcap_lookupdev","pcap_set_protocol_linux","pcap_findalldevs","pcap_freealldevs","pcap_get_required_select_timeout","pcap_close","pcap_dump_open_append","pcap_dump_fopen","pcap_next","pcap_dump","pcap_dump","pcap_dump_ftell","pcap_dump_flush","pcap_dump_flush","pcap_dump_close","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_set_snaplen","pcap_datalink","pcap_datalink_val_to_description_or_dlt","pcap_datalink_val_to_name","pcap_lookupdev","pcap_set_protocol_linux","pcap_findalldevs","pcap_freealldevs","pcap_get_required_select_timeout","pcap_dump_open_append","pcap_dump_fopen","pcap_next","pcap_dump","pcap_dump","pcap_dump_ftell","pcap_dump_flush","pcap_dump_flush","pcap_dump_close","pcap_dump_close","pcap_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }

    // Create FILE pointer for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        return 0;
    }

    // 1. Open offline capture file
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap_handle = pcap_fopen_offline(in_file, errbuf);
    if (!pcap_handle) {
        fclose(in_file);
        return 0;
    }

    // 2. Set snapshot length
    int snaplen = 65535;
    if (size > 100) {
        snaplen = (data[0] % 1000) + 1;
    }
    pcap_set_snaplen(pcap_handle, snaplen);

    // 3. Get datalink type and convert to description
    int dlt = pcap_datalink(pcap_handle);
    const char *dlt_desc = pcap_datalink_val_to_description_or_dlt(dlt);
    const char *dlt_name = pcap_datalink_val_to_name(dlt);
    (void)dlt_desc; // Use variable to avoid unused warning
    (void)dlt_name; // Use variable to avoid unused warning

    // 4. Lookup device
    char *dev = pcap_lookupdev(errbuf);
    (void)dev; // Use variable

    // 5. Set protocol for Linux
    #ifdef __linux__
    int protocol = 0;
    if (size > 1) {
        protocol = data[1] % 256;
    }
    pcap_set_protocol_linux(pcap_handle, protocol);
    #endif

    // 6. Find all devices
    pcap_if_t *alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) == 0) {
        pcap_freealldevs(alldevs);
    }

    // 7. Get required select timeout
    const struct timeval *timeout = pcap_get_required_select_timeout(pcap_handle);
    (void)timeout; // Use variable

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap_handle);
        fclose(in_file);
        return 0;
    }

    // 8. Open dumpers
    pcap_dumper_t *dumper_append = pcap_dump_open_append(pcap_handle, "output_file");
    pcap_dumper_t *dumper_fopen = pcap_dump_fopen(pcap_handle, out_file);

    // 9. Process packets
    struct pcap_pkthdr header;
    const u_char *packet;
    int packet_count = 0;
    
    while ((packet = pcap_next(pcap_handle, &header)) != nullptr && packet_count < 100) {
        packet_count++;
        
        // 10. Dump packets to both dumpers
        if (dumper_append) {
            pcap_dump((u_char *)dumper_append, &header, packet);
        }
        if (dumper_fopen) {
            pcap_dump((u_char *)dumper_fopen, &header, packet);
        }
    }

    // 11. Get dump file position
    if (dumper_append) {
        long pos = pcap_dump_ftell(dumper_append);
        (void)pos; // Use variable
    }

    // 12. Flush dumpers
    if (dumper_append) {
        pcap_dump_flush(dumper_append);
    }
    if (dumper_fopen) {
        pcap_dump_flush(dumper_fopen);
    }

    // 13. Close dumpers
    if (dumper_append) {
        pcap_dump_close(dumper_append);
    }
    if (dumper_fopen) {
        pcap_dump_close(dumper_fopen);
    }

    // 14. Close pcap handle
    pcap_close(pcap_handle);

    // Close files
    fclose(in_file);
    fclose(out_file);

    // Clean up output file
    remove("output_file");

    return 0;
}