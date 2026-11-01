// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
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
#include <stdio.h>

static void fuzzer_test_pcap_lookupdev(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    // pcap_lookupdev takes an error buffer, but ignores it in many implementations.
    // We pass a valid buffer.
    pcap_lookupdev(errbuf);
}

static void fuzzer_test_pcap_open(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    // Create a safe source string from input, ensuring null termination
    char source[256];
    size_t len = (Size < sizeof(source) - 1) ? Size : sizeof(source) - 1;
    memcpy(source, Data, len);
    source[len] = '\0';
    
    // Fuzz snaplen, flags, timeout
    int snaplen = (len > 0) ? (int)Data[0] : 65535;
    int flags = (len > 1) ? (int)Data[1] : 0;
    int timeout = (len > 2) ? (int)Data[2] : 1000;
    
    // Auth structure
    struct pcap_rmtauth auth = {0};
    if (len > 3) {
        auth.type = (int)Data[3];
        // We don't set username/password to avoid complex memory management in fuzzing
        auth.username = NULL;
        auth.password = NULL;
    }

    pcap_t *handle = pcap_open(source, snaplen, flags, timeout, &auth, errbuf);
    if (handle != NULL) {
        pcap_close(handle);
    }
}

static void fuzzer_test_pcap_findalldevs_ex(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    char source[256];
    size_t len = (Size < sizeof(source) - 1) ? Size : sizeof(source) - 1;
    memcpy(source, Data, len);
    source[len] = '\0';

    struct pcap_rmtauth auth = {0};
    if (len > 0) {
        auth.type = (int)Data[0];
        auth.username = NULL;
        auth.password = NULL;
    }

    pcap_if_t *alldevs = NULL;
    int ret = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

static void fuzzer_test_pcap_findalldevs(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    // This function doesn't take input data for source, but we can still call it
    // to test its behavior. The input data is ignored here but ensures the fuzzer
    // calls this path.
    (void)Data;
    (void)Size;

    pcap_if_t *alldevs = NULL;
    int ret = pcap_findalldevs(&alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

static void fuzzer_test_pcap_parsesrcstr(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    char source[256];
    size_t len = (Size < sizeof(source) - 1) ? Size : sizeof(source) - 1;
    memcpy(source, Data, len);
    source[len] = '\0';

    int type;
    char host[128] = {0};
    char port[128] = {0};
    char name[128] = {0};

    pcap_parsesrcstr(source, &type, host, port, name, errbuf);
}

static void fuzzer_test_pcap_create(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    
    char device[256];
    size_t len = (Size < sizeof(device) - 1) ? Size : sizeof(device) - 1;
    memcpy(device, Data, len);
    device[len] = '\0';

    pcap_t *handle = pcap_create(device, errbuf);
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use the first byte to select which function to fuzz
    int choice = Data[0] % 6;

    switch (choice) {
        case 0:
            fuzzer_test_pcap_lookupdev(Data, Size);
            break;
        case 1:
            fuzzer_test_pcap_open(Data, Size);
            break;
        case 2:
            fuzzer_test_pcap_findalldevs_ex(Data, Size);
            break;
        case 3:
            fuzzer_test_pcap_findalldevs(Data, Size);
            break;
        case 4:
            fuzzer_test_pcap_parsesrcstr(Data, Size);
            break;
        case 5:
            fuzzer_test_pcap_create(Data, Size);
            break;
    }

    return 0;
}