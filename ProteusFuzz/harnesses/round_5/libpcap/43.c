// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
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

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

static void cleanup_devs(pcap_if_t *alldevs) {
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Ensure the input data is null-terminated for string operations
    char *source_str = (char *)malloc(Size + 1);
    if (!source_str) {
        return 0;
    }
    memcpy(source_str, Data, Size);
    source_str[Size] = '\0';

    // 1. Test pcap_parsesrcstr
    int type = 0;
    char host[256] = {0};
    char port[256] = {0};
    char name[256] = {0};
    pcap_parsesrcstr(source_str, &type, host, port, name, errbuf);

    // 2. Test pcap_createsrcstr
    char constructed_source[512] = {0};
    pcap_createsrcstr(constructed_source, type, host, port, name, errbuf);

    // 3. Test pcap_findalldevs_ex
    pcap_if_t *alldevs = NULL;
    struct pcap_rmtauth auth = {0};
    // Try with the original source and the constructed source
    pcap_findalldevs_ex(source_str, &auth, &alldevs, errbuf);
    if (alldevs) {
        cleanup_devs(alldevs);
        alldevs = NULL;
    }
    pcap_findalldevs_ex(constructed_source, &auth, &alldevs, errbuf);
    if (alldevs) {
        cleanup_devs(alldevs);
        alldevs = NULL;
    }

    // 4. Test pcap_open_live
    pcap_t *handle_live = pcap_open_live(source_str, 65535, 1, 1000, errbuf);
    cleanup_pcap(handle_live);

    handle_live = pcap_open_live(constructed_source, 65535, 0, 1000, errbuf);
    cleanup_pcap(handle_live);

    // 5. Test pcap_create
    pcap_t *handle_create = pcap_create(source_str, errbuf);
    if (handle_create) {
        // Try to activate if possible, but we just want to test the creation path
        // pcap_activate is not in the target list, so we just close it.
        cleanup_pcap(handle_create);
    }

    handle_create = pcap_create(constructed_source, errbuf);
    cleanup_pcap(handle_create);

    // 6. Test pcap_open
    // We need to provide a valid source string. We try both original and constructed.
    // Note: pcap_open might fail for invalid sources, which is expected in fuzzing.
    pcap_t *handle_open = pcap_open(source_str, 65535, 0, 1000, &auth, errbuf);
    cleanup_pcap(handle_open);

    handle_open = pcap_open(constructed_source, 65535, 1, 1000, &auth, errbuf);
    cleanup_pcap(handle_open);

    // Test with NULL source (defaults to "any")
    handle_open = pcap_open(NULL, 65535, 0, 1000, &auth, errbuf);
    cleanup_pcap(handle_open);

    handle_live = pcap_open_live(NULL, 65535, 1, 1000, errbuf);
    cleanup_pcap(handle_live);

    handle_create = pcap_create(NULL, errbuf);
    cleanup_pcap(handle_create);

    free(source_str);
    return 0;
}