// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
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
#include <arpa/inet.h>

static void safe_free_devs(pcap_if_t *devs) {
    if (devs != NULL) {
        pcap_freealldevs(devs);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *devs1 = NULL;
    pcap_if_t *devs2 = NULL;
    pcap_if_t *devs3 = NULL;
    bpf_u_int32 net, mask;
    struct pcap_rmtauth auth = {0};

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // Step 1: First call to pcap_findalldevs_ex
    // Use a simple source string derived from input or a default
    const char *source1 = "pcap://";
    if (Size > 0 && Data[0] == 'r') {
        source1 = "rpcap://";
    }
    auth.type = 0; // RPCAP_RMTAUTH_NULL
    auth.username = NULL;
    auth.password = NULL;

    int ret1 = pcap_findalldevs_ex(source1, &auth, &devs1, errbuf);
    if (ret1 == 0) {
        // Success, devs1 is populated
    }

    // Step 2: Second call to pcap_findalldevs_ex with different parameters
    const char *source2 = "local://";
    if (Size > 1 && Data[1] == 'f') {
        source2 = "file:///dev/null";
    }
    // Reuse auth or set to NULL
    int ret2 = pcap_findalldevs_ex(source2, NULL, &devs2, errbuf);
    if (ret2 == 0) {
        // Success, devs2 is populated
    }

    // Step 3: Call pcap_findalldevs
    int ret3 = pcap_findalldevs(&devs3, errbuf);
    if (ret3 == 0) {
        // Success, devs3 is populated
    }

    // Step 4: Call pcap_lookupnet
    // We need a device name. Try to get one from the lists if available.
    const char *dev_name = "any";
    
    // Try to extract a device name from devs1, devs2, or devs3 if they exist
    pcap_if_t *current = devs1;
    if (current == NULL) current = devs2;
    if (current == NULL) current = devs3;
    
    if (current != NULL && current->name != NULL) {
        // Use the first available device name, but ensure it's safe
        // We limit the length to avoid buffer overflows in hypothetical scenarios
        // though pcap_lookupnet takes a const char*
        dev_name = current->name;
    } else if (Size > 0) {
        // Fallback: create a temporary string from input if no device found
        // This is just to test the function with arbitrary input
        // Note: This might fail, which is expected in fuzzing
        static char temp_dev[256];
        size_t copy_len = (Size < 255) ? Size : 255;
        memcpy(temp_dev, Data, copy_len);
        temp_dev[copy_len] = '\0';
        dev_name = temp_dev;
    }

    int ret4 = pcap_lookupnet(dev_name, &net, &mask, errbuf);
    // Ignore return value for fuzzing purposes, just ensure we don't crash

    // Step 5: Cleanup
    safe_free_devs(devs1);
    safe_free_devs(devs2);
    safe_free_devs(devs3);

    return 0;
}