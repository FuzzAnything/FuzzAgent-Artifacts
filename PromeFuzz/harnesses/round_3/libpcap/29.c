// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lib_version at pcap-linux.c:6223:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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

static void test_pcap_open_offline(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    write_dummy_file(data, size);
    pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
    if (pcap) {
        pcap_close(pcap);
    }
}

static void test_pcap_open_live(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    const char *device = "any";
    int snaplen = 65535;
    int promisc = 0;
    int timeout = 1000;
    
    if (size >= sizeof(int) * 3) {
        memcpy(&snaplen, data, sizeof(int));
        memcpy(&promisc, data + sizeof(int), sizeof(int));
        memcpy(&timeout, data + sizeof(int) * 2, sizeof(int));
    }
    
    pcap_t *pcap = pcap_open_live(device, snaplen, promisc, timeout, errbuf);
    if (pcap) {
        pcap_close(pcap);
    }
}

static void test_pcap_compile(const uint8_t *data, size_t size) {
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap) {
        return;
    }
    
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    
    char *filter = NULL;
    if (size > 0) {
        filter = (char *)malloc(size + 1);
        if (filter) {
            memcpy(filter, data, size);
            filter[size] = '\0';
            int optimize = 0;
            bpf_u_int32 netmask = 0;
            
            pcap_compile(pcap, &fp, filter, optimize, netmask);
            pcap_freecode(&fp);
            free(filter);
        }
    }
    
    pcap_close(pcap);
}

static void test_pcap_open(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char source[256] = "file://./dummy_file";
    int snaplen = 65535;
    int flags = 0;
    int read_timeout = 1000;
    
    write_dummy_file(data, size);
    
    if (size >= sizeof(int) * 3) {
        memcpy(&snaplen, data, sizeof(int));
        memcpy(&flags, data + sizeof(int), sizeof(int));
        memcpy(&read_timeout, data + sizeof(int) * 2, sizeof(int));
    }
    
    pcap_t *pcap = pcap_open(source, snaplen, flags, read_timeout, NULL, errbuf);
    if (pcap) {
        pcap_close(pcap);
    }
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    test_pcap_open_offline(Data, Size);
    
    test_pcap_open_live(Data, Size);
    
    const char *version = pcap_lib_version();
    (void)version;
    
    test_pcap_compile(Data, Size);
    
    test_pcap_open(Data, Size);
    
    int error_code = 0;
    if (Size >= sizeof(int)) {
        memcpy(&error_code, Data, sizeof(int));
    }
    const char *error_str = pcap_strerror(error_code);
    (void)error_str;
    
    unlink("./dummy_file");
    
    return 0;
}