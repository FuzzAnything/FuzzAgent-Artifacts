// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_setnonblock at pcap.c:3599:1 in pcap.h
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

static int parse_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return 0;
    return data[(*offset)++];
}

static const char* parse_string(const uint8_t *data, size_t size, size_t *offset, char *buf, size_t buf_size) {
    if (*offset >= size) return "eth0";
    
    size_t i = 0;
    while (*offset < size && i < buf_size - 1 && data[*offset] != 0) {
        buf[i++] = data[(*offset)++];
    }
    buf[i] = '\0';
    if (*offset < size && data[*offset] == 0) (*offset)++;
    
    return buf;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    size_t offset = 0;
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    char device_buf[256];
    
    const char *device = parse_string(Data, Size, &offset, device_buf, sizeof(device_buf));
    
    int test_case = parse_int(Data, Size, &offset) % 3;
    
    if (test_case == 0) {
        pcap_t *pcap = pcap_create(device, errbuf);
        if (pcap == NULL) return 0;
        
        int immediate_mode = parse_int(Data, Size, &offset) & 1;
        pcap_set_immediate_mode(pcap, immediate_mode);
        
        int snaplen = parse_int(Data, Size, &offset) * 100 + 1;
        pcap_set_snaplen(pcap, snaplen);
        
        pcap_activate(pcap);
        
        pcap_direction_t direction = parse_int(Data, Size, &offset) % 3;
        pcap_setdirection(pcap, direction);
        
        int nonblock = parse_int(Data, Size, &offset) & 1;
        pcap_setnonblock(pcap, nonblock, errbuf);
        
        pcap_close(pcap);
    } else if (test_case == 1) {
        int snaplen = parse_int(Data, Size, &offset) * 100 + 1;
        int promisc = parse_int(Data, Size, &offset) & 1;
        int timeout = parse_int(Data, Size, &offset) * 100;
        
        pcap_t *pcap = pcap_open_live(device, snaplen, promisc, timeout, errbuf);
        if (pcap == NULL) return 0;
        
        pcap_direction_t direction = parse_int(Data, Size, &offset) % 3;
        pcap_setdirection(pcap, direction);
        
        int nonblock = parse_int(Data, Size, &offset) & 1;
        pcap_setnonblock(pcap, nonblock, errbuf);
        
        pcap_close(pcap);
    } else {
        const char *filename = "./dummy_file";
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            if (Size > 0) {
                write(fd, Data, Size > 1024 ? 1024 : Size);
            }
            close(fd);
            
            pcap_t *pcap = pcap_open_offline(filename, errbuf);
            if (pcap != NULL) {
                pcap_direction_t direction = parse_int(Data, Size, &offset) % 3;
                pcap_setdirection(pcap, direction);
                
                int nonblock = parse_int(Data, Size, &offset) & 1;
                pcap_setnonblock(pcap, nonblock, errbuf);
                
                pcap_close(pcap);
            }
            unlink(filename);
        }
    }
    
    return 0;
}