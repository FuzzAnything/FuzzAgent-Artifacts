// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    pcap_t *handle = NULL;
    const char *source = (Size > 0) ? (const char *)Data : NULL;
    
    // Ensure null termination for string operations if Data is used as string
    char *safe_source = NULL;
    if (Size > 0) {
        safe_source = malloc(Size + 1);
        if (!safe_source) return 0;
        memcpy(safe_source, Data, Size);
        safe_source[Size] = '\0';
    }

    // 1. pcap_lookupdev (Deprecated but tested for robustness)
    memset(errbuf, 0, sizeof(errbuf));
    char *dev = pcap_lookupdev(errbuf);
    // No cleanup needed for dev, it's static or malloc'd by libpcap depending on impl, 
    // but typically caller doesn't free. We just ignore the result for fuzzing stability.
    
    // 2. pcap_findalldevs
    memset(errbuf, 0, sizeof(errbuf));
    if (pcap_findalldevs(&alldevs, errbuf) == 0) {
        // Cleanup
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // 3. pcap_findalldevs_ex
    memset(errbuf, 0, sizeof(errbuf));
    // Use NULL auth for local discovery
    if (pcap_findalldevs_ex(safe_source ? safe_source : NULL, NULL, &alldevs, errbuf) == 0) {
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // 4. pcap_parsesrcstr
    int type;
    char host[256], port[256], name[256];
    memset(&type, 0, sizeof(type));
    memset(host, 0, sizeof(host));
    memset(port, 0, sizeof(port));
    memset(name, 0, sizeof(name));
    memset(errbuf, 0, sizeof(errbuf));
    pcap_parsesrcstr(safe_source ? safe_source : NULL, &type, host, port, name, errbuf);

    // 5. pcap_create
    memset(errbuf, 0, sizeof(errbuf));
    handle = pcap_create(safe_source ? safe_source : NULL, errbuf);
    if (handle) {
        pcap_close(handle);
        handle = NULL;
    }

    // 6. pcap_open
    memset(errbuf, 0, sizeof(errbuf));
    // Create a dummy file if source looks like a file path or just to test file opening
    if (Size > 0) {
        write_dummy_file(Data, Size);
    }
    
    // Try opening the dummy file
    handle = pcap_open("./dummy_file", 65535, 0, 1000, NULL, errbuf);
    if (handle) {
        pcap_close(handle);
        handle = NULL;
    }

    // Try opening with the fuzzed source string (might fail, which is expected)
    handle = pcap_open(safe_source ? safe_source : NULL, 65535, 0, 1000, NULL, errbuf);
    if (handle) {
        pcap_close(handle);
        handle = NULL;
    }

    // Cleanup
    if (safe_source) {
        free(safe_source);
    }
    
    // Remove dummy file
    remove("./dummy_file");

    return 0;
}