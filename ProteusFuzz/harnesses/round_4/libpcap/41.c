// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

static void cleanup_pcap(pcap_t *handle) {
    if (handle) {
        pcap_close(handle);
    }
}

static void cleanup_devlist(pcap_if_t *devlist) {
    if (devlist) {
        pcap_freealldevs(devlist);
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    pcap_t *handle = NULL;
    const char *source_str = NULL;
    char *device_name = NULL;
    int ret;

    // Ensure minimum size to avoid trivial crashes
    if (Size < 2) {
        return 0;
    }

    // Create a dummy file for pcap_open with file source
    write_dummy_file(Data, Size);

    // Prepare a safe source string from input data, ensuring null termination
    // We limit the length to avoid huge allocations and ensure safety
    size_t safe_len = (Size > 255) ? 255 : Size;
    char *safe_source = malloc(safe_len + 1);
    if (!safe_source) return 0;
    memcpy(safe_source, Data, safe_len);
    safe_source[safe_len] = '\0';
    source_str = safe_source;

    // 1. Test pcap_findalldevs_ex
    // Try with local source, remote source simulation, and file source
    // We use the input data as the source string to fuzz parsing logic
    ret = pcap_findalldevs_ex(source_str, NULL, &alldevs, errbuf);
    if (ret == 0 && alldevs) {
        // Copy the name of the first device to a local buffer as recommended
        if (alldevs->name) {
            device_name = strdup(alldevs->name);
        }
        cleanup_devlist(alldevs);
    }

    // 2. Test pcap_findalldevs (local only)
    ret = pcap_findalldevs(&alldevs, errbuf);
    if (ret == 0 && alldevs) {
        if (alldevs->name) {
            device_name = strdup(alldevs->name);
        }
        cleanup_devlist(alldevs);
    }

    // 3. Test pcap_lookupdev (deprecated but still testable)
    char *lookup_dev = pcap_lookupdev(errbuf);
    if (lookup_dev) {
        // We don't own the memory returned by pcap_lookupdev, just use it
        if (device_name) free(device_name);
        device_name = strdup(lookup_dev);
    }

    // 4. Test pcap_create
    // Use device_name if available, otherwise use "any" or NULL
    const char *dev_arg = device_name ? device_name : (Size > 0 ? "any" : NULL);
    handle = pcap_create(dev_arg, errbuf);
    if (handle) {
        // Try to activate or just close immediately to test state
        // We don't necessarily activate to avoid needing network access
        cleanup_pcap(handle);
        handle = NULL;
    }

    // 5. Test pcap_open
    // Try with the source string, also try with the dummy file path
    struct pcap_rmtauth auth = {0};
    // Fuzz auth fields if data allows, but keep it simple for safety
    if (Size > 10) {
        auth.type = Data[0];
        // We don't allocate username/password dynamically from fuzz input to avoid complexity
        // Just pass NULL or static strings if needed, but for now NULL is safe
        auth.username = NULL;
        auth.password = NULL;
    }

    // Try opening with the fuzzed source string
    handle = pcap_open(source_str, 65535, 1, 1000, &auth, errbuf);
    if (handle) {
        cleanup_pcap(handle);
        handle = NULL;
    }

    // Try opening with the dummy file
    handle = pcap_open("./dummy_file", 65535, 1, 1000, NULL, errbuf);
    if (handle) {
        cleanup_pcap(handle);
        handle = NULL;
    }

    // 6. Test pcap_parsesrcstr
    char host[256] = {0};
    char port[256] = {0};
    char name[256] = {0};
    int type = 0;
    ret = pcap_parsesrcstr(source_str, &type, host, port, name, errbuf);
    // Ignore return value, just ensure it doesn't crash

    // Cleanup
    if (device_name) free(device_name);
    if (safe_source) free(safe_source);

    // Remove dummy file
    unlink("./dummy_file");

    return 0;
}