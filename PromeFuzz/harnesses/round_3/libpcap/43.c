// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_datalink_ext at pcap.c:2946:1 in pcap.h
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

static void cleanup_handle(pcap_t *handle) {
    if (handle) {
        pcap_close(handle);
    }
}

static pcap_t *create_test_handle(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_t *handle = NULL;
    
    if (size > 0) {
        int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            write(fd, data, size > 1024 ? 1024 : size);
            close(fd);
            
            handle = pcap_open_offline("./dummy_file", errbuf);
            if (handle) {
                return handle;
            }
        }
    }
    
    handle = pcap_create("lo", errbuf);
    if (!handle) {
        handle = pcap_open_dead(DLT_EN10MB, 65535);
    }
    
    return handle;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    pcap_t *handle = create_test_handle(Data, Size);
    if (!handle) {
        return 0;
    }
    
    int scenario = Data[0] % 8;
    size_t data_index = 1;
    
    switch (scenario) {
        case 0: {
            if (Size > data_index + sizeof(int)) {
                int dlt = *(int*)(Data + data_index);
                pcap_set_datalink(handle, dlt);
                data_index += sizeof(int);
            }
            break;
        }
        
        case 1: {
            pcap_activate(handle);
            break;
        }
        
        case 2: {
            int activated = pcap_activate(handle);
            if (activated == 0) {
                pcap_snapshot(handle);
            }
            break;
        }
        
        case 3: {
            int activated = pcap_activate(handle);
            if (activated == 0) {
                int *dlt_buffer = NULL;
                pcap_list_datalinks(handle, &dlt_buffer);
                if (dlt_buffer) {
                    free(dlt_buffer);
                }
            }
            break;
        }
        
        case 4: {
            int activated = pcap_activate(handle);
            if (activated == 0) {
                pcap_datalink_ext(handle);
            }
            break;
        }
        
        case 5: {
            int activated = pcap_activate(handle);
            if (activated == 0) {
                pcap_bufsize(handle);
            }
            break;
        }
        
        case 6: {
            if (Size > data_index + sizeof(int)) {
                int dlt = *(int*)(Data + data_index);
                pcap_set_datalink(handle, dlt);
                data_index += sizeof(int);
            }
            pcap_activate(handle);
            pcap_snapshot(handle);
            break;
        }
        
        case 7: {
            pcap_activate(handle);
            int *dlt_buffer = NULL;
            int count = pcap_list_datalinks(handle, &dlt_buffer);
            if (count > 0 && dlt_buffer && Size > data_index) {
                int idx = Data[data_index] % (count > 0 ? count : 1);
                pcap_set_datalink(handle, dlt_buffer[idx]);
            }
            if (dlt_buffer) {
                free(dlt_buffer);
            }
            break;
        }
    }
    
    cleanup_handle(handle);
    
    if (access("./dummy_file", F_OK) == 0) {
        unlink("./dummy_file");
    }
    
    return 0;
}