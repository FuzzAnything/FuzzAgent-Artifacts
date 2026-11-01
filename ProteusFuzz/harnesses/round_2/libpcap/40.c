// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
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

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Create a safe source string from input, ensuring null termination
    size_t max_len = Size < PCAP_ERRBUF_SIZE - 1 ? Size : PCAP_ERRBUF_SIZE - 1;
    char source[PCAP_ERRBUF_SIZE];
    memcpy(source, Data, max_len);
    source[max_len] = '\0';

    // Test pcap_findalldevs_ex
    pcap_if_t *alldevs = NULL;
    struct pcap_rmtauth auth = {0};
    auth.type = 0;
    auth.username = NULL;
    auth.password = NULL;

    int ret = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        // Successfully found devices, iterate briefly to exercise code paths
        pcap_if_t *dev = alldevs;
        int count = 0;
        while (dev != NULL && count < 10) {
            dev = dev->next;
            count++;
        }
        cleanup_pcap_if(alldevs);
    }

    // Test pcap_create
    pcap_t *handle = pcap_create(source, errbuf);
    if (handle != NULL) {
        cleanup_pcap(handle);
    }

    // Test pcap_createsrcstr
    char src_buf[PCAP_ERRBUF_SIZE];
    memset(src_buf, 0, sizeof(src_buf));
    int type = (Data[0] % 4); // 0-3
    const char *host = (Size > 10) ? (const char *)(Data + 10) : "localhost";
    const char *port = (Size > 20) ? (const char *)(Data + 20) : "2002";
    const char *name = (Size > 30) ? (const char *)(Data + 30) : "any";

    // Ensure strings are null-terminated for safety in this context
    char host_buf[64], port_buf[64], name_buf[64];
    memset(host_buf, 0, sizeof(host_buf));
    memset(port_buf, 0, sizeof(port_buf));
    memset(name_buf, 0, sizeof(name_buf));
    
    // Safe copy with truncation
    size_t h_len = strlen(host);
    if (h_len > 63) h_len = 63;
    memcpy(host_buf, host, h_len);
    
    size_t p_len = strlen(port);
    if (p_len > 63) p_len = 63;
    memcpy(port_buf, port, p_len);
    
    size_t n_len = strlen(name);
    if (n_len > 63) n_len = 63;
    memcpy(name_buf, name, n_len);

    ret = pcap_createsrcstr(src_buf, type, host_buf, port_buf, name_buf, errbuf);
    if (ret == 0 && strlen(src_buf) > 0) {
        // Use the generated source string for further testing
        memset(errbuf, 0, sizeof(errbuf));
        handle = pcap_open(src_buf, 65535, 1, 1000, &auth, errbuf);
        if (handle != NULL) {
            cleanup_pcap(handle);
        }
    }

    // Test pcap_parsesrcstr
    int parsed_type;
    char parsed_host[64], parsed_port[64], parsed_name[64];
    memset(&parsed_type, 0, sizeof(parsed_type));
    memset(parsed_host, 0, sizeof(parsed_host));
    memset(parsed_port, 0, sizeof(parsed_port));
    memset(parsed_name, 0, sizeof(parsed_name));

    ret = pcap_parsesrcstr(source, &parsed_type, parsed_host, parsed_port, parsed_name, errbuf);
    // No cleanup needed for parsed strings, just return

    // Test pcap_open_live
    memset(errbuf, 0, sizeof(errbuf));
    handle = pcap_open_live(source, 65535, 1, 1000, errbuf);
    if (handle != NULL) {
        cleanup_pcap(handle);
    }

    return 0;
}