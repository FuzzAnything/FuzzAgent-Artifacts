// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static pcap_t *create_test_handle(int use_live) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    
    if (use_live) {
        handle = pcap_open_live("any", 65535, 0, 1000, errbuf);
    } else {
        handle = pcap_create("any", errbuf);
        if (handle) {
            pcap_activate(handle);
        }
    }
    return handle;
}

static void test_pcap_open(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char source[256];
    struct pcap_rmtauth auth;
    
    if (size > 255) size = 255;
    memcpy(source, data, size);
    source[size] = '\0';
    
    auth.type = 0;
    auth.username = NULL;
    auth.password = NULL;
    
    pcap_t *handle = pcap_open(source, 65535, 0, 1000, &auth, errbuf);
    if (handle) {
        pcap_close(handle);
    }
}

static void test_pcap_open_live(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char device[256];
    
    if (size > 255) size = 255;
    memcpy(device, data, size);
    device[size] = '\0';
    
    pcap_t *handle = pcap_open_live(device, 65535, 0, 1000, errbuf);
    if (handle) {
        pcap_close(handle);
    }
}

static void test_set_functions(pcap_t *handle, int value) {
    if (!handle) return;
    
    pcap_set_snaplen(handle, value);
    pcap_set_buffer_size(handle, value);
    pcap_set_timeout(handle, value);
}

int LLVMFuzzerTestOneInput_41(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    write_dummy_file(data, size);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    int int_val = *(int*)data;
    
    pcap_t *handle1 = create_test_handle(0);
    pcap_t *handle2 = create_test_handle(1);
    
    test_set_functions(handle1, int_val);
    test_set_functions(handle2, int_val);
    
    if (handle1) {
        struct pcap_samp *samp = pcap_setsampling(handle1);
        (void)samp;
        pcap_close(handle1);
    }
    
    if (handle2) {
        struct pcap_samp *samp = pcap_setsampling(handle2);
        (void)samp;
        pcap_close(handle2);
    }
    
    test_pcap_open(data, size);
    test_pcap_open_live(data, size);
    
    unlink("./dummy_file");
    return 0;
}