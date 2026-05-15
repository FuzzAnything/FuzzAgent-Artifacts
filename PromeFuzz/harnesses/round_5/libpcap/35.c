// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_findalldevs at pcap.c:672:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_parsesrcstr at pcap.c:2235:1 in pcap.h
// pcap_findalldevs_ex at pcap.c:4623:1 in pcap.h
// pcap_freealldevs at pcap.c:1414:1 in pcap.h
// pcap_createsrcstr at pcap.c:2121:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "pcap/pcap.h"

#define MAX_STRING_LEN 256
#define PCAP_ERRBUF_SIZE 256

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void fuzz_pcap_parsesrcstr(const uint8_t *data, size_t size) {
    char source[MAX_STRING_LEN];
    int type;
    char host[PCAP_ERRBUF_SIZE];
    char port[PCAP_ERRBUF_SIZE];
    char name[PCAP_ERRBUF_SIZE];
    char errbuf[PCAP_ERRBUF_SIZE];
    
    if (size > 0) {
        size_t copy_len = size < MAX_STRING_LEN - 1 ? size : MAX_STRING_LEN - 1;
        memcpy(source, data, copy_len);
        source[copy_len] = '\0';
        
        pcap_parsesrcstr(source, &type, host, port, name, errbuf);
    }
}

static void fuzz_pcap_findalldevs_ex(const uint8_t *data, size_t size) {
    char source[MAX_STRING_LEN];
    struct pcap_rmtauth auth;
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    if (size > 0) {
        size_t copy_len = size < MAX_STRING_LEN - 1 ? size : MAX_STRING_LEN - 1;
        memcpy(source, data, copy_len);
        source[copy_len] = '\0';
        
        auth.type = 0;
        auth.username = NULL;
        auth.password = NULL;
        
        int result = pcap_findalldevs_ex(source, &auth, &alldevs, errbuf);
        if (result == 0 && alldevs != NULL) {
            pcap_freealldevs(alldevs);
        }
    }
}

static void fuzz_pcap_createsrcstr(const uint8_t *data, size_t size) {
    char source[PCAP_ERRBUF_SIZE];
    int type = PCAP_SRC_IFREMOTE;
    char host[MAX_STRING_LEN];
    char port[MAX_STRING_LEN];
    char name[MAX_STRING_LEN];
    char errbuf[PCAP_ERRBUF_SIZE];
    
    if (size > 0) {
        size_t copy_len = size < MAX_STRING_LEN - 1 ? size : MAX_STRING_LEN - 1;
        memcpy(host, data, copy_len);
        host[copy_len] = '\0';
        
        port[0] = '\0';
        name[0] = '\0';
        
        pcap_createsrcstr(source, type, host, port, name, errbuf);
    }
}

static void fuzz_pcap_create(const uint8_t *data, size_t size) {
    char device[MAX_STRING_LEN];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle;
    
    if (size > 0) {
        size_t copy_len = size < MAX_STRING_LEN - 1 ? size : MAX_STRING_LEN - 1;
        memcpy(device, data, copy_len);
        device[copy_len] = '\0';
        
        pcap_handle = pcap_create(device, errbuf);
        if (pcap_handle != NULL) {
            pcap_close(pcap_handle);
        }
    }
}

static void fuzz_pcap_open(const uint8_t *data, size_t size) {
    char source[MAX_STRING_LEN];
    int snaplen = 65535;
    int flags = 0;
    int read_timeout = 1000;
    struct pcap_rmtauth auth;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle;
    
    if (size > 0) {
        size_t copy_len = size < MAX_STRING_LEN - 1 ? size : MAX_STRING_LEN - 1;
        memcpy(source, data, copy_len);
        source[copy_len] = '\0';
        
        auth.type = 0;
        auth.username = NULL;
        auth.password = NULL;
        
        pcap_handle = pcap_open(source, snaplen, flags, read_timeout, &auth, errbuf);
        if (pcap_handle != NULL) {
            pcap_close(pcap_handle);
        }
    }
}

static void fuzz_pcap_findalldevs(const uint8_t *data, size_t size) {
    pcap_if_t *alldevs = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    int result = pcap_findalldevs(&alldevs, errbuf);
    if (result == 0 && alldevs != NULL) {
        pcap_freealldevs(alldevs);
    }
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    fuzz_pcap_parsesrcstr(Data, Size);
    fuzz_pcap_findalldevs_ex(Data, Size);
    fuzz_pcap_createsrcstr(Data, Size);
    fuzz_pcap_create(Data, Size);
    fuzz_pcap_open(Data, Size);
    fuzz_pcap_findalldevs(Data, Size);
    
    unlink("./dummy_file");
    
    return 0;
}