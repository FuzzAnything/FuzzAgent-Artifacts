// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
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

#define MAX_STRING_LEN 256
#define PCAP_BUF_SIZE 1024

static void fuzz_pcap_open(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char source[MAX_STRING_LEN];
    struct pcap_rmtauth auth;
    
    // Prepare source string from fuzz data
    size_t source_len = size > MAX_STRING_LEN - 1 ? MAX_STRING_LEN - 1 : size;
    memcpy(source, data, source_len);
    source[source_len] = '\0';
    
    // Extract parameters from data
    int snaplen = 0;
    int flags = 0;
    int read_timeout = 0;
    
    if (size >= source_len + sizeof(int)) {
        memcpy(&snaplen, data + source_len, sizeof(int));
    }
    if (size >= source_len + 2 * sizeof(int)) {
        memcpy(&flags, data + source_len + sizeof(int), sizeof(int));
    }
    if (size >= source_len + 3 * sizeof(int)) {
        memcpy(&read_timeout, data + source_len + 2 * sizeof(int), sizeof(int));
    }
    
    // Set up auth structure
    auth.type = 0;
    auth.username = NULL;
    auth.password = NULL;
    
    // Call the function
    pcap_t *pcap = pcap_open(source, snaplen, flags, read_timeout, &auth, errbuf);
    if (pcap) {
        pcap_close(pcap);
    }
}

static void fuzz_pcap_parsesrcstr(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char source[MAX_STRING_LEN];
    int type;
    char host[PCAP_BUF_SIZE];
    char port[PCAP_BUF_SIZE];
    char name[PCAP_BUF_SIZE];
    
    // Prepare source string from fuzz data
    size_t source_len = size > MAX_STRING_LEN - 1 ? MAX_STRING_LEN - 1 : size;
    memcpy(source, data, source_len);
    source[source_len] = '\0';
    
    // Call the function
    int result = pcap_parsesrcstr(source, &type, host, port, name, errbuf);
    (void)result; // Use result to avoid unused variable warning
}

static void fuzz_pcap_findalldevs_ex(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char source[MAX_STRING_LEN];
    pcap_if_t *alldevs = NULL;
    struct pcap_rmtauth auth;
    
    // Prepare source string from fuzz data
    size_t source_len = size > MAX_STRING_LEN - 1 ? MAX_STRING_LEN - 1 : size;
    memcpy(source, data, source_len);
    source[source_len] = '\0';
    
    // Set up auth structure
    auth.type = 0;
    auth.username = NULL;
    auth.password = NULL;
    
    // Call the function
    int result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    if (result == 0 && alldevs) {
        pcap_freealldevs(alldevs);
    }
}

static void fuzz_pcap_createsrcstr(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char source[PCAP_BUF_SIZE];
    int type = 0;
    char host[MAX_STRING_LEN];
    char port[MAX_STRING_LEN];
    char name[MAX_STRING_LEN];
    
    // Extract type from data
    if (size >= sizeof(int)) {
        memcpy(&type, data, sizeof(int));
    }
    
    // Prepare host, port, name strings
    size_t offset = sizeof(int);
    size_t host_len = 0, port_len = 0, name_len = 0;
    
    if (offset < size) {
        host_len = (size - offset) > MAX_STRING_LEN - 1 ? MAX_STRING_LEN - 1 : size - offset;
        memcpy(host, data + offset, host_len);
        host[host_len] = '\0';
        offset += host_len;
    } else {
        host[0] = '\0';
    }
    
    if (offset < size) {
        port_len = (size - offset) > MAX_STRING_LEN - 1 ? MAX_STRING_LEN - 1 : size - offset;
        memcpy(port, data + offset, port_len);
        port[port_len] = '\0';
        offset += port_len;
    } else {
        port[0] = '\0';
    }
    
    if (offset < size) {
        name_len = (size - offset) > MAX_STRING_LEN - 1 ? MAX_STRING_LEN - 1 : size - offset;
        memcpy(name, data + offset, name_len);
        name[name_len] = '\0';
    } else {
        name[0] = '\0';
    }
    
    // Call the function
    int result = pcap_createsrcstr(source, type, host, port, name, errbuf);
    (void)result; // Use result to avoid unused variable warning
}

static void fuzz_pcap_open_live(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char device[MAX_STRING_LEN];
    
    // Prepare device string from fuzz data
    size_t device_len = size > MAX_STRING_LEN - 1 ? MAX_STRING_LEN - 1 : size;
    memcpy(device, data, device_len);
    device[device_len] = '\0';
    
    // Extract parameters from data
    int snaplen = 0;
    int promisc = 0;
    int to_ms = 0;
    
    if (size >= device_len + sizeof(int)) {
        memcpy(&snaplen, data + device_len, sizeof(int));
    }
    if (size >= device_len + 2 * sizeof(int)) {
        memcpy(&promisc, data + device_len + sizeof(int), sizeof(int));
    }
    if (size >= device_len + 3 * sizeof(int)) {
        memcpy(&to_ms, data + device_len + 2 * sizeof(int), sizeof(int));
    }
    
    // Call the function
    pcap_t *pcap = pcap_open_live(device, snaplen, promisc, to_ms, errbuf);
    if (pcap) {
        pcap_close(pcap);
    }
}

static void fuzz_pcap_findalldevs(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = NULL;
    
    // Call the function (doesn't use input data directly)
    int result = pcap_findalldevs(&alldevs, errbuf);
    if (result == 0 && alldevs) {
        pcap_freealldevs(alldevs);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Use first byte to select which function to test
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (selector % 6) {
        case 0:
            fuzz_pcap_open(fuzz_data, fuzz_size);
            break;
        case 1:
            fuzz_pcap_parsesrcstr(fuzz_data, fuzz_size);
            break;
        case 2:
            fuzz_pcap_findalldevs_ex(fuzz_data, fuzz_size);
            break;
        case 3:
            fuzz_pcap_createsrcstr(fuzz_data, fuzz_size);
            break;
        case 4:
            fuzz_pcap_open_live(fuzz_data, fuzz_size);
            break;
        case 5:
            fuzz_pcap_findalldevs(fuzz_data, fuzz_size);
            break;
    }
    
    return 0;
}