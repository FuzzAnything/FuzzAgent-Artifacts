// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_free_tstamp_types at pcap.c:534:1 in pcap.h
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

static pcap_t* create_test_pcap(const uint8_t *data, size_t size) {
    write_dummy_file(data, size);
    
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = pcap_open_offline("./dummy_file", errbuf);
    
    if (pcap == NULL) {
        pcap = pcap_open_dead(DLT_EN10MB, 65535);
    }
    
    return pcap;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Test pcap_datalink_val_to_name
    int dlt_val = Data[0] % 256;
    const char *name = pcap_datalink_val_to_name(dlt_val);
    
    // Test pcap_datalink_name_to_val
    if (Size > 1) {
        char name_buf[256];
        size_t name_len = Size > 255 ? 255 : Size - 1;
        memcpy(name_buf, Data + 1, name_len);
        name_buf[name_len] = '\0';
        int val = pcap_datalink_name_to_val(name_buf);
    }
    
    // Test pcap_list_datalinks and pcap_free_datalinks
    pcap_t *pcap = create_test_pcap(Data, Size);
    if (pcap != NULL) {
        int *dlt_list = NULL;
        int count = pcap_list_datalinks(pcap, &dlt_list);
        if (count > 0 && dlt_list != NULL) {
            pcap_free_datalinks(dlt_list);
        }
        
        // Test pcap_set_datalink
        if (Size > 1) {
            int set_dlt = Data[1] % 256;
            pcap_set_datalink(pcap, set_dlt);
        }
        
        pcap_close(pcap);
    }
    
    // Test pcap_free_tstamp_types
    if (Size > 2) {
        int *tstamp_types = (int*)malloc(sizeof(int) * (Size % 16 + 1));
        if (tstamp_types != NULL) {
            pcap_free_tstamp_types(tstamp_types);
        }
    }
    
    // Clean up
    unlink("./dummy_file");
    
    return 0;
}