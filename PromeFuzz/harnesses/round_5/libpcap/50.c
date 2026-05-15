// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include "pcap/namedb.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void cleanup_addrinfo(struct addrinfo *ai) {
    if (ai) {
        freeaddrinfo(ai);
    }
}

static void cleanup_devices(pcap_if_t *alldevs) {
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }
}

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap) {
        pcap_close(pcap);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create null-terminated strings from fuzzer input
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) return 0;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Test pcap_nametoaddrinfo
    struct addrinfo *ai = pcap_nametoaddrinfo(input_str);
    cleanup_addrinfo(ai);

    // 2. Test pcap_parsesrcstr
    int type;
    char host[256], port[256], name[256], errbuf1[PCAP_ERRBUF_SIZE];
    pcap_parsesrcstr(input_str, &type, host, port, name, errbuf1);

    // 3. Test pcap_findalldevs_ex with NULL auth
    pcap_if_t *alldevs1 = NULL;
    char errbuf2[PCAP_ERRBUF_SIZE];
    pcap_findalldevs_ex(input_str, NULL, &alldevs1, errbuf2);
    cleanup_devices(alldevs1);

    // 4. Test pcap_createsrcstr
    char source[256];
    char errbuf3[PCAP_ERRBUF_SIZE];
    // Use different source types
    int types_to_test[] = {PCAP_SRC_IFREMOTE, PCAP_SRC_IFLOCAL, PCAP_SRC_FILE};
    for (int i = 0; i < 3; i++) {
        pcap_createsrcstr(source, types_to_test[i], input_str, 
                         (Size > 10) ? "80" : NULL, 
                         (Size > 20) ? "eth0" : NULL, 
                         errbuf3);
    }

    // 5. Test pcap_open with different flags and timeouts
    char errbuf4[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = NULL;
    
    // Try with different combinations of flags
    int flags_list[] = {0, PCAP_OPENFLAG_PROMISCUOUS, PCAP_OPENFLAG_NOCAPTURE_LOCAL};
    for (int i = 0; i < 3; i++) {
        pcap_handle = pcap_open(input_str, 65535, flags_list[i], 1000, NULL, errbuf4);
        cleanup_pcap(pcap_handle);
        pcap_handle = NULL;
    }

    // 6. Test pcap_findalldevs
    pcap_if_t *alldevs2 = NULL;
    char errbuf5[PCAP_ERRBUF_SIZE];
    pcap_findalldevs(&alldevs2, errbuf5);
    cleanup_devices(alldevs2);

    // Additional test: Create a dummy file and test with file source
    if (Size > 0) {
        int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            // Write pcap file header (simplified)
            struct pcap_file_header pfh;
            pfh.magic = 0xa1b2c3d4;
            pfh.version_major = 2;
            pfh.version_minor = 4;
            pfh.thiszone = 0;
            pfh.sigfigs = 0;
            pfh.snaplen = 65535;
            pfh.linktype = 1; // DLT_EN10MB
            
            write(fd, &pfh, sizeof(pfh));
            close(fd);
            
            // Test with file source
            char file_source[300];
            snprintf(file_source, sizeof(file_source), "file://./dummy_file");
            pcap_handle = pcap_open(file_source, 65535, 0, 1000, NULL, errbuf4);
            cleanup_pcap(pcap_handle);
            
            unlink("./dummy_file");
        }
    }

    free(input_str);
    return 0;
}