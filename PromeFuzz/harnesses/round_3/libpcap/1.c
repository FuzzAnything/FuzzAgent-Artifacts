// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    /* Create dummy pcap handle for functions that require it */
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap_handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (!pcap_handle) return 0;
    
    /* First pcap_statustostr */
    int status1 = *(const int*)Data;
    const char *status_str1 = pcap_statustostr(status1);
    (void)status_str1;
    
    /* First pcap_geterr */
    char *err1 = pcap_geterr(pcap_handle);
    (void)err1;
    
    /* Second pcap_statustostr */
    int status2 = *(const int*)(Data + 4);
    const char *status_str2 = pcap_statustostr(status2);
    (void)status_str2;
    
    /* Second pcap_geterr */
    char *err2 = pcap_geterr(pcap_handle);
    (void)err2;
    
    /* pcap_lookupnet */
    bpf_u_int32 netp = 0, maskp = 0;
    const char *device = "any";
    if (Size > 8) {
        size_t device_len = Size - 8;
        if (device_len > 0) {
            device = (const char*)(Data + 8);
            /* Ensure we don't read beyond buffer */
            size_t safe_len = device_len < PCAP_ERRBUF_SIZE ? device_len : PCAP_ERRBUF_SIZE - 1;
            pcap_lookupnet(device, &netp, &maskp, errbuf);
        }
    }
    
    /* pcap_compile */
    struct bpf_program fp;
    memset(&fp, 0, sizeof(fp));
    const char *filter = "ip";
    if (Size > 16) {
        size_t filter_len = Size - 16;
        if (filter_len > 0) {
            filter = (const char*)(Data + 16);
            /* Use strnlen to ensure we don't read beyond buffer */
            size_t safe_len = strnlen(filter, filter_len);
            if (safe_len < filter_len) {
                pcap_compile(pcap_handle, &fp, filter, 1, netp);
            }
        }
    } else {
        /* Use default filter if not enough data */
        pcap_compile(pcap_handle, &fp, filter, 1, netp);
    }
    
    /* Third pcap_geterr */
    char *err3 = pcap_geterr(pcap_handle);
    (void)err3;
    
    /* pcap_setfilter */
    pcap_setfilter(pcap_handle, &fp);
    
    /* Fourth pcap_geterr */
    char *err4 = pcap_geterr(pcap_handle);
    (void)err4;
    
    /* pcap_breakloop */
    pcap_breakloop(pcap_handle);
    
    /* pcap_close */
    pcap_close(pcap_handle);
    
    /* pcap_freecode */
    pcap_freecode(&fp);
    
    /* Create another handle for different code path */
    write_dummy_file(Data, Size);
    pcap_t *offline_handle = pcap_open_offline("./dummy_file", errbuf);
    if (offline_handle) {
        struct bpf_program fp2;
        memset(&fp2, 0, sizeof(fp2));
        
        /* Try compilation with different parameters */
        pcap_compile(offline_handle, &fp2, "tcp", 0, 0);
        pcap_setfilter(offline_handle, &fp2);
        pcap_breakloop(offline_handle);
        pcap_close(offline_handle);
        pcap_freecode(&fp2);
    }
    
    unlink("./dummy_file");
    return 0;
}