// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
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

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

static void cleanup_devs(pcap_if_t *devs) {
    if (devs != NULL) {
        pcap_freealldevs(devs);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Create a null-terminated string from the input data for device/source names
    char *device_name = malloc(Size + 1);
    if (device_name == NULL) {
        return 0;
    }
    memcpy(device_name, Data, Size);
    device_name[Size] = '\0';

    // Test 1: pcap_create
    pcap_t *handle = pcap_create(device_name, errbuf);
    if (handle != NULL) {
        // Do not activate, just close to test the deferred activation path
        pcap_close(handle);
    }

    // Test 2: pcap_findalldevs_ex
    pcap_if_t *alldevs = NULL;
    struct pcap_rmtauth auth = {0};
    // Use a subset of data for source to avoid potential issues with very long strings
    size_t source_len = Size > 100 ? 100 : Size;
    char source[101];
    memcpy(source, Data, source_len);
    source[source_len] = '\0';

    int ret = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        // Copy name of first device if exists
        if (alldevs->name != NULL) {
            char *dev_name_copy = strdup(alldevs->name);
            if (dev_name_copy != NULL) {
                free(dev_name_copy);
            }
        }
        cleanup_devs(alldevs);
    }

    // Test 3: pcap_lookupdev (deprecated but still tested)
    char *lookup_dev = pcap_lookupdev(errbuf);
    if (lookup_dev != NULL) {
        // Just check it's not null, we don't own the memory
        // The function returns a static buffer or similar, do not free
    }

    // Test 4: pcap_findalldevs
    pcap_if_t *alldevs_local = NULL;
    ret = pcap_findalldevs(&alldevs_local, errbuf);
    if (ret == 0 && alldevs_local != NULL) {
        // Copy name of first device
        if (alldevs_local->name != NULL) {
            char *dev_name_copy = strdup(alldevs_local->name);
            if (dev_name_copy != NULL) {
                free(dev_name_copy);
            }
        }
        cleanup_devs(alldevs_local);
    }

    // Test 5: pcap_open
    // Use the same source string
    pcap_t *handle_open = pcap_open(source, 65535, 1, 1000, &auth, errbuf);
    cleanup_pcap(handle_open);

    // Test 6: pcap_parsesrcstr
    int type;
    char host[256];
    char port[256];
    char name[256];
    memset(host, 0, sizeof(host));
    memset(port, 0, sizeof(port));
    memset(name, 0, sizeof(name));

    ret = pcap_parsesrcstr(source, &type, host, port, name, errbuf);
    // No cleanup needed for parsed strings as they are local buffers

    free(device_name);
    return 0;
}