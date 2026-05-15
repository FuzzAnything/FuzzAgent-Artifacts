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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["pcap_fopen_offline","pcap_set_snaplen","pcap_lookupdev","pcap_set_protocol_linux","pcap_get_required_select_timeout","pcap_datalink","pcap_datalink_val_to_name","pcap_close","pcap_dump_open_append","pcap_dump_fopen","pcap_next","pcap_dump","pcap_dump_flush","pcap_dump_close","pcap_close"],"critical_calls":["pcap_fopen_offline","pcap_set_snaplen","pcap_lookupdev","pcap_set_protocol_linux","pcap_get_required_select_timeout","pcap_datalink","pcap_datalink_val_to_name","pcap_dump_open_append","pcap_dump_fopen","pcap_next","pcap_dump","pcap_dump_flush","pcap_dump_close","pcap_close"],"visited":1}
/**/


// libpcap headers
extern "C" {
    #include <pcap/pcap.h>
    #include <netinet/in.h>
    #ifdef __linux__
        #include <linux/if_ether.h>
    #endif
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }

    // 1. Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        return 0;
    }

    // 2. Open offline pcap capture from memory
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *pcap = pcap_fopen_offline(in_file, errbuf);
    if (!pcap) {
        fclose(in_file);
        return 0;
    }

    // 3. Set snapshot length (use value derived from input)
    int snaplen = (size > 0) ? (data[0] * 1024) : 65535;
    if (snaplen < 68) snaplen = 68;
    if (snaplen > 65535) snaplen = 65535;
    pcap_set_snaplen(pcap, snaplen);

    // 4. Look up a network device (ignored for offline mode)
    char *device = pcap_lookupdev(errbuf);
    (void)device; // May be NULL in offline context

    // 5. Set Linux protocol (only effective on Linux)
    #ifdef __linux__
        if (size > 1) {
            int protocol = (data[1] % 256) ? ETH_P_ALL : ETH_P_IP;
            pcap_set_protocol_linux(pcap, protocol);
        }
    #endif

    // 6. Get required select timeout (may be NULL for offline)
    const struct timeval *timeout = pcap_get_required_select_timeout(pcap);
    (void)timeout;

    // 7. Get datalink type and convert to name
    int dlt = pcap_datalink(pcap);
    const char *dlt_name = pcap_datalink_val_to_name(dlt);
    (void)dlt_name;

    // 8. Open pcap dumper in append mode
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        pcap_close(pcap);
        fclose(in_file);
        return 0;
    }
    pcap_dumper_t *dumper = pcap_dump_open_append(pcap, "output_file");
    if (!dumper) {
        // Fallback to fopen method if above fails
        dumper = pcap_dump_fopen(pcap, out_file);
    }

    // Process packets from input and dump them
    if (dumper) {
        struct pcap_pkthdr header;
        const u_char *packet;
        while ((packet = pcap_next(pcap, &header)) != nullptr) {
            pcap_dump((u_char *)dumper, &header, packet);
        }
        pcap_dump_flush(dumper);
        pcap_dump_close(dumper);
    }

    // Cleanup
    if (out_file) {
        fclose(out_file);
    }
    pcap_close(pcap);
    fclose(in_file);

    return 0;
}