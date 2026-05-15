// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_set_protocol_linux at pcap-linux.c:6201:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_inject at pcap.c:4155:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <pcap/pcap.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static int try_activate_and_inject(pcap_t *handle, const uint8_t *packet_data, size_t packet_size) {
    int ret = pcap_activate(handle);
    if (ret == 0) {
        if (packet_size > 0 && packet_size <= INT_MAX) {
            pcap_inject(handle, packet_data, packet_size);
        }
        return 1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    
    write_dummy_file(Data, Size);
    
    int protocol = Data[0];
    int buffer_size = (Data[0] << 8) | (Data[1 % Size]);
    int promisc = Data[0] & 1;
    int snaplen = (Data[1 % Size] << 8) | (Data[2 % Size]);
    int timeout = (Data[3 % Size] << 8) | (Data[4 % Size]);
    
    const char *device = "lo";
    if (Size > 5 && Data[5 % Size] < 10) {
        device = "any";
    }
    
    pcap_t *handle1 = pcap_open_live(device, snaplen, promisc, timeout, errbuf);
    if (handle1) {
        pcap_set_protocol_linux(handle1, protocol);
        pcap_set_buffer_size(handle1, buffer_size);
        pcap_set_promisc(handle1, promisc);
        
        size_t packet_size = Size > 100 ? 100 : Size;
        if (packet_size > 0) {
            pcap_inject(handle1, Data, packet_size);
        }
        
        pcap_close(handle1);
    }
    
    pcap_t *handle2 = pcap_create(device, errbuf);
    if (handle2) {
        pcap_set_protocol_linux(handle2, protocol);
        pcap_set_buffer_size(handle2, buffer_size);
        pcap_set_promisc(handle2, promisc);
        
        if (try_activate_and_inject(handle2, Data, Size % 1500)) {
            pcap_close(handle2);
        } else {
            pcap_close(handle2);
        }
    }
    
    pcap_t *handle3 = pcap_open_offline("./dummy_file", errbuf);
    if (handle3) {
        pcap_set_protocol_linux(handle3, protocol);
        pcap_set_buffer_size(handle3, buffer_size);
        pcap_set_promisc(handle3, promisc);
        
        int activate_ret = pcap_activate(handle3);
        if (activate_ret == 0) {
            if (Size > 10) {
                pcap_inject(handle3, Data, Size % 100);
            }
        }
        pcap_close(handle3);
    }
    
    unlink("./dummy_file");
    return 0;
}