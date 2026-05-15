// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_datalink_val_to_description at pcap.c:3376:1 in pcap.h
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
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

static void test_datalink_functions(int dlt_val) {
    const char *desc1 = pcap_datalink_val_to_description(dlt_val);
    const char *desc2 = pcap_datalink_val_to_description_or_dlt(dlt_val);
    const char *name = pcap_datalink_val_to_name(dlt_val);
    
    if (desc1) {
        volatile char c = desc1[0];
        (void)c;
    }
    if (desc2) {
        volatile char c = desc2[0];
        (void)c;
    }
    if (name) {
        volatile char c = name[0];
        (void)c;
    }
}

static void test_name_to_val(const char *name) {
    int val = pcap_datalink_name_to_val(name);
    volatile int v = val;
    (void)v;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    int dlt_val = 0;
    if (Size >= sizeof(int)) {
        memcpy(&dlt_val, Data, sizeof(int));
    } else {
        memcpy(&dlt_val, Data, Size);
    }
    
    test_datalink_functions(dlt_val);
    
    char *name_buf = malloc(Size + 1);
    if (name_buf) {
        memcpy(name_buf, Data, Size);
        name_buf[Size] = '\0';
        test_name_to_val(name_buf);
        free(name_buf);
    }
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
    if (pcap) {
        int *dlt_list = NULL;
        int count = pcap_list_datalinks(pcap, &dlt_list);
        if (count > 0 && dlt_list) {
            pcap_free_datalinks(dlt_list);
        }
        pcap_close(pcap);
    }
    
    unlink("./dummy_file");
    return 0;
}