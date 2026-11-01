// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_perror at pcap.c:3543:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void cleanup_pcap_if(pcap_if_t *alldevs) {
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Create a buffer for source strings and parsing outputs
    char source_buf[256];
    char host_buf[64];
    char port_buf[32];
    char name_buf[64];
    int type = 0;
    pcap_if_t *alldevs = NULL;
    pcap_t *handle = NULL;
    pcap_t *handle_create = NULL;
    struct pcap_rmtauth auth = {0};

    // Ensure null termination for string operations
    size_t copy_len = Size < sizeof(source_buf) - 1 ? Size : sizeof(source_buf) - 1;
    memcpy(source_buf, Data, copy_len);
    source_buf[copy_len] = '\0';

    // 1. Test pcap_parsesrcstr
    memset(host_buf, 0, sizeof(host_buf));
    memset(port_buf, 0, sizeof(port_buf));
    memset(name_buf, 0, sizeof(name_buf));
    int parse_ret = pcap_parsesrcstr(source_buf, &type, host_buf, port_buf, name_buf, errbuf);
    (void)parse_ret;

    // 2. Test pcap_createsrcstr based on parsed or fuzzed type
    int create_type = (type > 0 && type < 10) ? type : 0;
    const char *host = (strlen(host_buf) > 0) ? host_buf : NULL;
    const char *port = (strlen(port_buf) > 0) ? port_buf : NULL;
    const char *name = (strlen(name_buf) > 0) ? name_buf : NULL;
    
    char constructed_source[256];
    memset(constructed_source, 0, sizeof(constructed_source));
    int create_ret = pcap_createsrcstr(constructed_source, create_type, host, port, name, errbuf);
    (void)create_ret;

    // 3. Test pcap_findalldevs_ex with original source
    int find_ret = pcap_findalldevs_ex(source_buf, &auth, &alldevs, errbuf);
    if (find_ret == 0 && alldevs != NULL) {
        cleanup_pcap_if(alldevs);
        alldevs = NULL;
    }

    // 4. Test pcap_findalldevs_ex with constructed source
    if (strlen(constructed_source) > 0) {
        find_ret = pcap_findalldevs_ex(constructed_source, &auth, &alldevs, errbuf);
        if (find_ret == 0 && alldevs != NULL) {
            cleanup_pcap_if(alldevs);
            alldevs = NULL;
        }
    }

    // 5. Test pcap_open with original source
    handle = pcap_open(source_buf, 65535, 0, 1000, &auth, errbuf);
    if (handle != NULL) {
        // Trigger pcap_perror with a valid handle
        pcap_perror(handle, "pcap_open_success");
        cleanup_pcap(handle);
        handle = NULL;
    } else {
        // pcap_perror requires a valid pcap_t pointer to access errbuf.
        // We cannot pass NULL. Instead, we just ignore the error printing
        // or create a dummy handle if needed, but standard practice is
        // to only call pcap_perror on a valid handle.
        // Since we can't safely call it with NULL, we skip the error print here.
        // The error message is already in errbuf if needed for other checks.
    }

    // 6. Test pcap_open with constructed source
    if (strlen(constructed_source) > 0) {
        handle = pcap_open(constructed_source, 65535, 0, 1000, &auth, errbuf);
        if (handle != NULL) {
            pcap_perror(handle, "pcap_open_constructed_success");
            cleanup_pcap(handle);
            handle = NULL;
        }
    }

    // 7. Test pcap_create with original source
    handle_create = pcap_create(source_buf, errbuf);
    if (handle_create != NULL) {
        pcap_perror(handle_create, "pcap_create_success");
        cleanup_pcap(handle_create);
        handle_create = NULL;
    }

    // 8. Test pcap_create with constructed source
    if (strlen(constructed_source) > 0) {
        handle_create = pcap_create(constructed_source, errbuf);
        if (handle_create != NULL) {
            pcap_perror(handle_create, "pcap_create_constructed_success");
            cleanup_pcap(handle_create);
            handle_create = NULL;
        }
    }

    return 0;
}