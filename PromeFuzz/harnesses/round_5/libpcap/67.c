// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_set_rfmon at pcap.c:2553:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_can_set_rfmon at pcap.c:466:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
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

static void fuzz_pcap_functions(pcap_t *pcap, const uint8_t *data, size_t size) {
    if (size < 24) return;
    
    int param1 = *(int*)data;
    int param2 = *(int*)(data + 4);
    int param3 = *(int*)(data + 8);
    int param4 = *(int*)(data + 12);
    int param5 = *(int*)(data + 16);
    int param6 = *(int*)(data + 20);
    
    pcap_set_protocol_linux(pcap, param1);
    pcap_set_rfmon(pcap, param2);
    pcap_set_snaplen(pcap, param3);
    pcap_can_set_rfmon(pcap);
    pcap_set_buffer_size(pcap, param5);
    pcap_set_promisc(pcap, param6);
}

static pcap_t* create_pcap_handle_from_data(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = NULL;
    
    if (size > 0) {
        int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            write(fd, data, size > 1024 ? 1024 : size);
            close(fd);
            
            pcap = pcap_open_offline("./dummy_file", errbuf);
            if (pcap) {
                return pcap;
            }
        }
    }
    
    pcap = pcap_create("any", errbuf);
    return pcap;
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    pcap_t *pcap = create_pcap_handle_from_data(Data, Size);
    if (!pcap) return 0;
    
    fuzz_pcap_functions(pcap, Data, Size);
    
    pcap_close(pcap);
    unlink("./dummy_file");
    
    return 0;
}