// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void free_device_list(pcap_if_t *alldevs) {
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Ensure we have enough data to create a null-terminated string
    if (Size < 2) {
        return 0;
    }

    // Create a safe buffer for the input string
    char *safe_input = (char *)malloc(Size + 1);
    if (!safe_input) {
        return 0;
    }
    memcpy(safe_input, Data, Size);
    safe_input[Size] = '\0';

    // 1. Test pcap_create
    pcap_t *handle_create = pcap_create(safe_input, errbuf);
    if (handle_create != NULL) {
        // Cleanup for pcap_create
        pcap_close(handle_create);
    }

    // 2. Test pcap_findalldevs_ex
    pcap_if_t *alldevs = NULL;
    struct pcap_rmtauth auth = {0};
    // Try with the input as source, and also with NULL auth
    int ret_find = pcap_findalldevs_ex(safe_input, &auth, &alldevs, errbuf);
    if (ret_find == 0 && alldevs != NULL) {
        free_device_list(alldevs);
    }
    
    // Try with NULL auth
    ret_find = pcap_findalldevs_ex(safe_input, NULL, &alldevs, errbuf);
    if (ret_find == 0 && alldevs != NULL) {
        free_device_list(alldevs);
    }

    // 3. Test pcap_open
    // Try with various flags and timeouts derived from input
    int snaplen = (Size > 0) ? (Data[0] % 65536) : 65535;
    int flags = (Size > 1) ? (Data[1] % 8) : 0;
    int timeout = (Size > 2) ? (Data[2] % 10000) : 1000;
    
    pcap_t *handle_open = pcap_open(safe_input, snaplen, flags, timeout, &auth, errbuf);
    if (handle_open != NULL) {
        pcap_close(handle_open);
    }

    // 4. Test pcap_createsrcstr
    char source_buf[256];
    memset(source_buf, 0, sizeof(source_buf));
    int type = (Size > 3) ? (Data[3] % 4) : 0;
    int ret_create_src = pcap_createsrcstr(source_buf, type, safe_input, safe_input, safe_input, errbuf);
    (void)ret_create_src; // Ignore result, just exercising the function

    // 5. Test pcap_open_live
    int promisc = (Size > 4) ? (Data[4] % 2) : 0;
    pcap_t *handle_live = pcap_open_live(safe_input, snaplen, promisc, timeout, errbuf);
    if (handle_live != NULL) {
        pcap_close(handle_live);
    }

    // 6. Test pcap_parsesrcstr
    int parse_type;
    char parse_host[128];
    char parse_port[128];
    char parse_name[128];
    memset(parse_host, 0, sizeof(parse_host));
    memset(parse_port, 0, sizeof(parse_port));
    memset(parse_name, 0, sizeof(parse_name));
    
    int ret_parse = pcap_parsesrcstr(safe_input, &parse_type, parse_host, parse_port, parse_name, errbuf);
    (void)ret_parse; // Ignore result, just exercising the function

    free(safe_input);
    return 0;
}