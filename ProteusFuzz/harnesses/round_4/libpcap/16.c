// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
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

static void create_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f) {
        fprintf(f, "dummy pcap data for testing\n");
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    pcap_if_t *alldevs = NULL;
    pcap_if_t *alldevs2 = NULL;
    pcap_if_t *alldevs3 = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_rmtauth auth;
    bpf_u_int32 net, mask;
    char *device_name_copy = NULL;
    const char *source;

    // Initialize auth structure
    auth.type = 0;
    auth.username = NULL;
    auth.password = NULL;

    // Ensure error buffer is clean
    memset(errbuf, 0, sizeof(errbuf));

    // Decide source based on input data to explore different states
    if (Size > 0 && Data[0] % 3 == 0) {
        // Try local source
        source = "local://";
    } else if (Size > 0 && Data[0] % 3 == 1) {
        // Try remote source (might fail, which is expected for fuzzing)
        source = "remote://127.0.0.1";
    } else {
        // Try file source
        create_dummy_file("./dummy_file");
        source = "file://./dummy_file";
    }

    // 1. First call to pcap_findalldevs_ex
    int ret1 = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    if (ret1 == 0 && alldevs) {
        // Copy name if exists to avoid use-after-free
        if (alldevs->name) {
            device_name_copy = strdup(alldevs->name);
        }
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // 2. Second call to pcap_findalldevs_ex with potentially different source
    if (Size > 0 && Data[0] % 2 == 0) {
        source = "local://";
    } else {
        source = "remote://192.168.1.1";
    }
    int ret2 = pcap_findalldevs_ex(source, NULL, &alldevs2, errbuf);
    if (ret2 == 0 && alldevs2) {
        if (alldevs2->name) {
            free(device_name_copy);
            device_name_copy = strdup(alldevs2->name);
        }
        pcap_freealldevs(alldevs2);
        alldevs2 = NULL;
    }

    // 3. Call to pcap_findalldevs
    int ret3 = pcap_findalldevs(&alldevs3, errbuf);
    if (ret3 == 0 && alldevs3) {
        if (alldevs3->name) {
            free(device_name_copy);
            device_name_copy = strdup(alldevs3->name);
        }
        pcap_freealldevs(alldevs3);
        alldevs3 = NULL;
    }

    // 4. Call to pcap_lookupnet
    // Use the copied device name if available, otherwise try "any" or a dummy name
    const char *lookup_name = NULL;
    if (device_name_copy) {
        lookup_name = device_name_copy;
    } else {
        // Fallback to "any" or a generic name
        lookup_name = "any";
    }

    int ret4 = pcap_lookupnet(lookup_name, &net, &mask, errbuf);
    // Return value is ignored for fuzzing purposes, we just want to trigger the code path

    // Cleanup
    free(device_name_copy);
    device_name_copy = NULL;

    // 5. Final cleanup if any lists were not freed (should not happen in this flow, but for safety)
    if (alldevs) pcap_freealldevs(alldevs);
    if (alldevs2) pcap_freealldevs(alldevs2);
    if (alldevs3) pcap_freealldevs(alldevs3);

    return 0;
}