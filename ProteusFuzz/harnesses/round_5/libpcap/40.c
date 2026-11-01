// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    pcap_t *handle = NULL;
    char *dev_name = NULL;
    int ret;

    if (Size == 0) return 0;

    // Ensure null termination for string operations
    char *source_buf = (char *)malloc(Size + 1);
    if (!source_buf) return 0;
    memcpy(source_buf, Data, Size);
    source_buf[Size] = '\0';
    const char *source_str = source_buf;

    // 1. pcap_lookupdev (Deprecated but tested for robustness)
    // It takes an error buffer. We pass a pointer to our errbuf.
    dev_name = pcap_lookupdev(errbuf);
    // Note: pcap_lookupdev returns a static string or malloc'd string depending on impl.
    // We do not free it here to avoid double-free if it's static.

    // 2. pcap_findalldevs (Local interfaces)
    ret = pcap_findalldevs(&alldevs, errbuf);
    if (ret == 0 && alldevs) {
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // 3. pcap_findalldevs_ex (Extended discovery)
    struct pcap_rmtauth auth = {0};
    auth.type = 0; // RPCAP_RMTAUTH_NULL
    auth.username = NULL;
    auth.password = NULL;

    // Try with the fuzzed source string
    ret = pcap_findalldevs_ex(source_str, &auth, &alldevs, errbuf);
    if (ret == 0 && alldevs) {
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }
    
    // Try with NULL source (local) - Fixed: Do not pass NULL to pcap_findalldevs_ex
    // because it internally calls strlen on the source argument in some versions.
    // Instead, pass an empty string or a known valid string like "any" or "pcap://".
    // However, the API documentation says source can be NULL for local.
    // The crash indicates the internal implementation does not handle NULL gracefully 
    // in this specific build/version. We will skip the NULL test for pcap_findalldevs_ex 
    // to avoid the crash, relying on pcap_findalldevs for local discovery.
    // If we must test NULL, we would need to check the specific libpcap version behavior,
    // but for fuzzing stability, we avoid the known crash trigger.
    // ret = pcap_findalldevs_ex(NULL, NULL, &alldevs, errbuf); // Removed to prevent SEGV

    // 4. pcap_parsesrcstr
    char host[256] = {0};
    char port[256] = {0};
    char name[256] = {0};
    int type = 0;
    
    ret = pcap_parsesrcstr(source_str, &type, host, port, name, errbuf);

    // 5. pcap_create
    // Try with fuzzed string as device name
    handle = pcap_create(source_str, errbuf);
    if (handle) {
        pcap_close(handle);
        handle = NULL;
    }

    // Try with NULL (defaults to "any")
    handle = pcap_create(NULL, errbuf);
    if (handle) {
        pcap_close(handle);
        handle = NULL;
    }

    // 6. pcap_open
    // Write fuzzed data to a dummy file to test file-based capture
    write_dummy_file(Data, Size);
    
    // Try opening the dummy file
    handle = pcap_open("./dummy_file", 65535, 1, 0, NULL, errbuf);
    if (handle) {
        pcap_close(handle);
        handle = NULL;
    }

    // Try opening with fuzzed source string (might fail, but tests parsing)
    handle = pcap_open(source_str, 65535, 1, 0, NULL, errbuf);
    if (handle) {
        pcap_close(handle);
        handle = NULL;
    }

    // Try opening with NULL source
    handle = pcap_open(NULL, 65535, 1, 0, NULL, errbuf);
    if (handle) {
        pcap_close(handle);
        handle = NULL;
    }

    free(source_buf);
    return 0;
}