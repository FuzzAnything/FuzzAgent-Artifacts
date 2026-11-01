// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    pcap_if_t *alldevs1 = NULL;
    pcap_if_t *alldevs2 = NULL;
    pcap_if_t *alldevs3 = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_rmtauth auth;
    bpf_u_int32 net, mask;
    int ret;

    // Initialize auth structure
    memset(&auth, 0, sizeof(auth));
    auth.type = 0; // RPCAP_RMTAUTH_NULL
    auth.username = NULL;
    auth.password = NULL;

    // Create dummy file if data is provided
    if (Size > 0) {
        create_dummy_file(Data, Size);
    }

    // 1. First call to pcap_findalldevs_ex with "any" source
    ret = pcap_findalldevs_ex("any", &auth, &alldevs1, errbuf);
    if (ret == 0 && alldevs1 != NULL) {
        // Try pcap_lookupnet on the first device if available
        if (alldevs1->name != NULL) {
            ret = pcap_lookupnet(alldevs1->name, &net, &mask, errbuf);
            (void)ret; // Ignore result for fuzzing
        }
    }

    // 2. Second call to pcap_findalldevs_ex with "local" source
    ret = pcap_findalldevs_ex("local", &auth, &alldevs2, errbuf);
    if (ret == 0 && alldevs2 != NULL) {
        // Try pcap_lookupnet on the first device if available
        if (alldevs2->name != NULL) {
            ret = pcap_lookupnet(alldevs2->name, &net, &mask, errbuf);
            (void)ret;
        }
    }

    // 3. Call to pcap_findalldevs
    ret = pcap_findalldevs(&alldevs3, errbuf);
    if (ret == 0 && alldevs3 != NULL) {
        // Try pcap_lookupnet on the first device if available
        if (alldevs3->name != NULL) {
            ret = pcap_lookupnet(alldevs3->name, &net, &mask, errbuf);
            (void)ret;
        }
    }

    // 4. Cleanup: Free all allocated device lists
    if (alldevs1 != NULL) {
        pcap_freealldevs(alldevs1);
    }
    if (alldevs2 != NULL) {
        pcap_freealldevs(alldevs2);
    }
    if (alldevs3 != NULL) {
        pcap_freealldevs(alldevs3);
    }

    // Remove dummy file
    remove("./dummy_file");

    return 0;
}