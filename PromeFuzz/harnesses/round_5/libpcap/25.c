// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <pcap/namedb.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define PCAP_ERRBUF_SIZE 256
#define MAX_STRING_LEN 256

static void cleanup_addrinfo(struct addrinfo *ai) {
    if (ai != NULL) {
        freeaddrinfo(ai);
    }
}

static void cleanup_devices(pcap_if_t *alldevs) {
    if (alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

static void cleanup_pcap(pcap_t *pcap) {
    if (pcap != NULL) {
        pcap_close(pcap);
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Create dummy file for potential file operations */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Extract strings from fuzzer input */
    char input_str[MAX_STRING_LEN];
    size_t copy_len = Size < MAX_STRING_LEN - 1 ? Size : MAX_STRING_LEN - 1;
    memcpy(input_str, Data, copy_len);
    input_str[copy_len] = '\0';

    /* 1. Test pcap_nametoaddrinfo */
    struct addrinfo *ai = pcap_nametoaddrinfo(input_str);
    cleanup_addrinfo(ai);

    /* 2. Test pcap_parsesrcstr */
    int type;
    char host[MAX_STRING_LEN];
    char port[MAX_STRING_LEN];
    char name[MAX_STRING_LEN];
    char errbuf1[PCAP_ERRBUF_SIZE];
    pcap_parsesrcstr(input_str, &type, host, port, name, errbuf1);

    /* 3. Test pcap_findalldevs_ex */
    pcap_if_t *alldevs1 = NULL;
    char errbuf2[PCAP_ERRBUF_SIZE];
    pcap_findalldevs_ex(input_str, NULL, &alldevs1, errbuf2);
    cleanup_devices(alldevs1);

    /* 4. Test pcap_createsrcstr */
    char source[MAX_STRING_LEN];
    char errbuf3[PCAP_ERRBUF_SIZE];
    
    /* Try different source types */
    int source_types[] = {
        PCAP_SRC_IFLOCAL,
        PCAP_SRC_IFREMOTE,
        PCAP_SRC_FILE,
        PCAP_SRC_IFREMOTE
    };
    
    for (size_t i = 0; i < sizeof(source_types)/sizeof(source_types[0]); i++) {
        pcap_createsrcstr(source, source_types[i], 
                         input_str, "8888", "test", errbuf3);
    }

    /* 5. Test pcap_open */
    char errbuf4[PCAP_ERRBUF_SIZE];
    
    /* Try opening with different parameters */
    pcap_t *pcap_handle = pcap_open(input_str, 65535, PCAP_OPENFLAG_PROMISCUOUS, 
                                    1000, NULL, errbuf4);
    cleanup_pcap(pcap_handle);

    /* Try with dummy file */
    pcap_handle = pcap_open("file://./dummy_file", 65535, 0, 1000, NULL, errbuf4);
    cleanup_pcap(pcap_handle);

    /* Try with "any" device */
    pcap_handle = pcap_open("any", 65535, PCAP_OPENFLAG_PROMISCUOUS, 
                            1000, NULL, errbuf4);
    cleanup_pcap(pcap_handle);

    /* 6. Test pcap_findalldevs */
    pcap_if_t *alldevs2 = NULL;
    char errbuf5[PCAP_ERRBUF_SIZE];
    pcap_findalldevs(&alldevs2, errbuf5);
    cleanup_devices(alldevs2);

    /* Remove dummy file */
    remove("./dummy_file");

    return 0;
}