// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
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

static void cleanup_devs(pcap_if_t *alldevs) {
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    pcap_t *handle = NULL;
    struct pcap_rmtauth auth = {0};
    char host[256], port[64], name[256];
    int type;

    // Ensure null termination for string operations
    char *source = (char *)malloc(Size + 1);
    if (!source) return 0;
    memcpy(source, Data, Size);
    source[Size] = '\0';

    // 1. pcap_findalldevs_ex
    // Test with various source strings and auth settings
    int ret = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        cleanup_devs(alldevs);
        alldevs = NULL;
    }

    // Test with NULL auth
    ret = pcap_findalldevs_ex(source, NULL, &alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        cleanup_devs(alldevs);
        alldevs = NULL;
    }

    // Test with "local://" prefix
    char local_source[Size + 10];
    if (Size + 10 < sizeof(local_source)) {
        snprintf(local_source, sizeof(local_source), "local://%s", source);
        ret = pcap_findalldevs_ex(local_source, NULL, &alldevs, errbuf);
        if (ret == 0 && alldevs != NULL) {
            cleanup_devs(alldevs);
            alldevs = NULL;
        }
    }

    // 2. pcap_create
    // Test with source string
    handle = pcap_create(source, errbuf);
    cleanup_pcap(handle);
    handle = NULL;

    // Test with NULL source (defaults to "any")
    handle = pcap_create(NULL, errbuf);
    cleanup_pcap(handle);
    handle = NULL;

    // Test with "any" explicitly
    handle = pcap_create("any", errbuf);
    cleanup_pcap(handle);
    handle = NULL;

    // 3. pcap_parsesrcstr
    memset(host, 0, sizeof(host));
    memset(port, 0, sizeof(port));
    memset(name, 0, sizeof(name));
    ret = pcap_parsesrcstr(source, &type, host, port, name, errbuf);
    // No cleanup needed for parsed strings

    // 4. pcap_lookupdev (Deprecated but tested for robustness)
    char *dev_name = pcap_lookupdev(errbuf);
    // pcap_lookupdev returns a static string, do not free
    (void)dev_name;

    // 5. pcap_open
    // Test with source, default snapshot, flags, timeout
    handle = pcap_open(source, 65535, 0, 1000, NULL, errbuf);
    cleanup_pcap(handle);
    handle = NULL;

    // Test with NULL source
    handle = pcap_open(NULL, 65535, 0, 1000, NULL, errbuf);
    cleanup_pcap(handle);
    handle = NULL;

    // Test with remote auth (dummy)
    auth.type = 0; // RPCAP_RMTAUTH_NULL
    auth.username = NULL;
    auth.password = NULL;
    handle = pcap_open(source, 65535, 0, 1000, &auth, errbuf);
    cleanup_pcap(handle);
    handle = NULL;

    // 6. pcap_findalldevs
    ret = pcap_findalldevs(&alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        cleanup_devs(alldevs);
        alldevs = NULL;
    }

    free(source);
    return 0;
}