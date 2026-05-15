// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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

#define ERRBUFSIZE 256
#define MAX_SOURCE_LEN 1024

static void fuzz_pcap_findalldevs_ex(const uint8_t *data, size_t size) {
    char errbuf[ERRBUFSIZE];
    pcap_if_t *alldevs = NULL;
    struct pcap_rmtauth auth;
    
    // Extract source string from fuzzer data
    size_t source_len = size > MAX_SOURCE_LEN ? MAX_SOURCE_LEN : size;
    char *source = malloc(source_len + 1);
    if (!source) return;
    
    memcpy(source, data, source_len);
    source[source_len] = '\0';
    
    // Initialize auth structure with fuzzer data
    auth.type = size > 0 ? data[0] % 3 : 0;
    
    // Extract username and password from fuzzer data
    size_t username_len = size > 1 ? data[1] % 64 : 0;
    size_t password_len = size > 2 ? data[2] % 64 : 0;
    
    if (username_len > 0 && size > 3) {
        auth.username = malloc(username_len + 1);
        if (auth.username) {
            size_t copy_len = username_len < size - 3 ? username_len : size - 3;
            memcpy(auth.username, data + 3, copy_len);
            auth.username[copy_len] = '\0';
        }
    } else {
        auth.username = NULL;
    }
    
    if (password_len > 0 && size > 3 + username_len) {
        auth.password = malloc(password_len + 1);
        if (auth.password) {
            size_t copy_len = password_len < size - 3 - username_len ? password_len : size - 3 - username_len;
            memcpy(auth.password, data + 3 + username_len, copy_len);
            auth.password[copy_len] = '\0';
        }
    } else {
        auth.password = NULL;
    }
    
    // Call the target function
    int result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
    
    // Cleanup
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }
    free(source);
    free(auth.username);
    free(auth.password);
}

static void fuzz_pcap_open(const uint8_t *data, size_t size) {
    char errbuf[ERRBUFSIZE];
    
    // Extract source string from fuzzer data
    size_t source_len = size > MAX_SOURCE_LEN ? MAX_SOURCE_LEN : size;
    char *source = malloc(source_len + 1);
    if (!source) return;
    
    memcpy(source, data, source_len);
    source[source_len] = '\0';
    
    // Extract integer parameters from fuzzer data
    int snaplen = size > 0 ? (int)data[0] * 256 + (size > 1 ? data[1] : 0) : 0;
    int flags = size > 2 ? (int)data[2] : 0;
    int read_timeout = size > 3 ? (int)data[3] * 100 : 0;
    
    struct pcap_rmtauth auth;
    auth.type = size > 4 ? data[4] % 3 : 0;
    
    // Extract username and password from fuzzer data
    size_t username_len = size > 5 ? data[5] % 64 : 0;
    size_t password_len = size > 6 ? data[6] % 64 : 0;
    
    if (username_len > 0 && size > 7) {
        auth.username = malloc(username_len + 1);
        if (auth.username) {
            size_t copy_len = username_len < size - 7 ? username_len : size - 7;
            memcpy(auth.username, data + 7, copy_len);
            auth.username[copy_len] = '\0';
        }
    } else {
        auth.username = NULL;
    }
    
    if (password_len > 0 && size > 7 + username_len) {
        auth.password = malloc(password_len + 1);
        if (auth.password) {
            size_t copy_len = password_len < size - 7 - username_len ? password_len : size - 7 - username_len;
            memcpy(auth.password, data + 7 + username_len, copy_len);
            auth.password[copy_len] = '\0';
        }
    } else {
        auth.password = NULL;
    }
    
    // Call the target function
    pcap_t *pcap = pcap_open(source, snaplen, flags, read_timeout, &auth, errbuf);
    
    // Cleanup
    if (pcap) {
        pcap_close(pcap);
    }
    free(source);
    free(auth.username);
    free(auth.password);
}

static void fuzz_pcap_findalldevs(const uint8_t *data, size_t size) {
    char errbuf[ERRBUFSIZE];
    pcap_if_t *alldevs = NULL;
    
    // Call the target function
    int result = pcap_findalldevs(&alldevs, errbuf);
    
    // Cleanup
    if (alldevs) {
        pcap_freealldevs(alldevs);
    }
}

static void fuzz_pcap_lookupdev(const uint8_t *data, size_t size) {
    char errbuf[ERRBUFSIZE];
    
    // Call the target function
    char *device = pcap_lookupdev(errbuf);
    
    // No cleanup needed for pcap_lookupdev
    (void)device;
}

static void fuzz_pcap_create(const uint8_t *data, size_t size) {
    char errbuf[ERRBUFSIZE];
    
    // Extract device string from fuzzer data
    size_t device_len = size > MAX_SOURCE_LEN ? MAX_SOURCE_LEN : size;
    char *device = malloc(device_len + 1);
    if (!device) return;
    
    memcpy(device, data, device_len);
    device[device_len] = '\0';
    
    // Call the target function
    pcap_t *pcap = pcap_create(device, errbuf);
    
    // Cleanup - only close if pcap_create succeeded
    if (pcap) {
        pcap_close(pcap);
    }
    free(device);
}

static void fuzz_pcap_open_live(const uint8_t *data, size_t size) {
    char errbuf[ERRBUFSIZE];
    
    // Extract device string from fuzzer data
    size_t device_len = size > MAX_SOURCE_LEN ? MAX_SOURCE_LEN : size;
    char *device = malloc(device_len + 1);
    if (!device) return;
    
    memcpy(device, data, device_len);
    device[device_len] = '\0';
    
    // Extract integer parameters from fuzzer data
    int snaplen = size > 0 ? (int)data[0] * 256 + (size > 1 ? data[1] : 0) : 0;
    int promisc = size > 2 ? data[2] & 1 : 0;
    int to_ms = size > 3 ? (int)data[3] * 100 : 0;
    
    // Call the target function
    pcap_t *pcap = pcap_open_live(device, snaplen, promisc, to_ms, errbuf);
    
    // Cleanup
    if (pcap) {
        pcap_close(pcap);
    }
    free(device);
}

int LLVMFuzzerTestOneInput_30(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    // Use first byte to determine which function to fuzz
    uint8_t function_selector = data[0] % 6;
    
    // Adjust data pointer and size for the remaining data
    const uint8_t *fuzz_data = size > 1 ? data + 1 : data;
    size_t fuzz_size = size > 1 ? size - 1 : 1;
    
    switch (function_selector) {
        case 0:
            fuzz_pcap_findalldevs_ex(fuzz_data, fuzz_size);
            break;
        case 1:
            fuzz_pcap_open(fuzz_data, fuzz_size);
            break;
        case 2:
            fuzz_pcap_findalldevs(fuzz_data, fuzz_size);
            break;
        case 3:
            fuzz_pcap_lookupdev(fuzz_data, fuzz_size);
            break;
        case 4:
            fuzz_pcap_create(fuzz_data, fuzz_size);
            break;
        case 5:
            fuzz_pcap_open_live(fuzz_data, fuzz_size);
            break;
    }
    
    return 0;
}