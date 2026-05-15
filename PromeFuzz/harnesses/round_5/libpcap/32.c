// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define PCAP_ERRBUF_SIZE 256
#define MAX_STRING_LEN 256

static void init_random_string(char *buf, size_t buf_size, const uint8_t *data, size_t data_size, size_t *offset) {
    size_t i;
    size_t len = buf_size - 1;
    if (len > data_size - *offset) {
        len = data_size - *offset;
    }
    for (i = 0; i < len; i++) {
        buf[i] = data[*offset + i];
    }
    buf[len] = '\0';
    *offset += len;
}

static int get_random_int(const uint8_t *data, size_t data_size, size_t *offset) {
    int value = 0;
    if (*offset + sizeof(int) <= data_size) {
        memcpy(&value, &data[*offset], sizeof(int));
        *offset += sizeof(int);
    } else {
        if (*offset < data_size) {
            memcpy(&value, &data[*offset], data_size - *offset);
            *offset = data_size;
        }
    }
    return value;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    size_t offset = 0;
    char errbuf[PCAP_ERRBUF_SIZE];
    int type;
    char host[MAX_STRING_LEN];
    char port[MAX_STRING_LEN];
    char name[MAX_STRING_LEN];
    char source[MAX_STRING_LEN];
    struct pcap_rmtauth auth;
    pcap_if_t *alldevs = NULL;
    pcap_t *pcap_handle = NULL;
    int snaplen, flags, read_timeout;

    // Initialize strings from fuzzer input
    init_random_string(host, MAX_STRING_LEN, Data, Size, &offset);
    init_random_string(port, MAX_STRING_LEN, Data, Size, &offset);
    init_random_string(name, MAX_STRING_LEN, Data, Size, &offset);
    init_random_string(source, MAX_STRING_LEN, Data, Size, &offset);

    // Initialize integers from fuzzer input
    type = get_random_int(Data, Size, &offset);
    snaplen = get_random_int(Data, Size, &offset);
    flags = get_random_int(Data, Size, &offset);
    read_timeout = get_random_int(Data, Size, &offset);

    // Initialize auth structure
    auth.type = type;
    auth.username = host;  // Reuse host buffer for username
    auth.password = port;  // Reuse port buffer for password

    // Test pcap_parsesrcstr
    pcap_parsesrcstr(source, &type, host, port, name, errbuf);

    // Test pcap_createsrcstr
    pcap_createsrcstr(source, type, host, port, name, errbuf);

    // Test pcap_findalldevs_ex
    int ret = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // Test pcap_open
    pcap_handle = pcap_open(source, snaplen, flags, read_timeout, &auth, errbuf);
    if (pcap_handle != NULL) {
        pcap_close(pcap_handle);
        pcap_handle = NULL;
    }

    // Test pcap_findalldevs
    ret = pcap_findalldevs(&alldevs, errbuf);
    if (ret == 0 && alldevs != NULL) {
        pcap_freealldevs(alldevs);
        alldevs = NULL;
    }

    // Test pcap_open_live
    pcap_handle = pcap_open_live(source, snaplen, flags, read_timeout, errbuf);
    if (pcap_handle != NULL) {
        pcap_close(pcap_handle);
        pcap_handle = NULL;
    }

    return 0;
}