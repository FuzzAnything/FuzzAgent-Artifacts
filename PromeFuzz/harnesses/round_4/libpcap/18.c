// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_snapshot at pcap.c:3455:1 in pcap.h
// pcap_is_swapped at pcap.c:3463:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void test_functions_on_handle(pcap_t *handle, int is_active, const uint8_t *data, size_t size) {
    int ret;
    
    /* Use fuzz data to influence parameter values */
    int param1 = 0;
    int param2 = 0;
    if (size >= 1) param1 = data[0] % 256;
    if (size >= 2) param2 = data[1] % 256;
    
    /* Test pcap_set_immediate_mode - must be called before activation */
    if (!is_active) {
        pcap_set_immediate_mode(handle, param1 & 1);
    }
    
    /* Test pcap_set_snaplen - must be called before activation */
    if (!is_active) {
        pcap_set_snaplen(handle, param1 * 100);
    }
    
    /* Test pcap_activate if not already active */
    if (!is_active) {
        ret = pcap_activate(handle);
        is_active = (ret == 0);
    }
    
    /* Test functions that require active handle */
    if (is_active) {
        pcap_snapshot(handle);
        pcap_is_swapped(handle);
        pcap_bufsize(handle);
    }
}

static int create_dummy_pcap_file(const uint8_t *data, size_t size) {
    /* Create a minimal pcap file header (24 bytes) */
    struct pcap_file_header header;
    header.magic = 0xa1b2c3d4;  /* TCPDUMP_MAGIC */
    header.version_major = 2;
    header.version_minor = 4;
    header.thiszone = 0;
    header.sigfigs = 0;
    header.snaplen = 65535;
    header.linktype = 1;  /* DLT_EN10MB */
    
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    write(fd, &header, sizeof(header));
    
    /* Add a packet header if we have enough data */
    if (size >= 16) {
        struct pcap_pkthdr pkt_header;
        pkt_header.ts.tv_sec = 0;
        pkt_header.ts.tv_usec = 0;
        pkt_header.caplen = size < 65535 ? size : 65535;
        pkt_header.len = size;
        
        write(fd, &pkt_header, sizeof(pkt_header));
        write(fd, data, pkt_header.caplen);
    }
    
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    
    /* Test 1: Create a live capture handle */
    handle = pcap_create("any", errbuf);
    if (handle != NULL) {
        test_functions_on_handle(handle, 0, Data, Size);
        pcap_close(handle);
    }
    
    /* Test 2: Create a dead handle for compiling filters */
    handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (handle != NULL) {
        test_functions_on_handle(handle, 1, Data, Size);
        pcap_close(handle);
    }
    
    /* Test 3: Open a dummy pcap file */
    if (create_dummy_pcap_file(Data, Size) == 0) {
        handle = pcap_open_offline("./dummy_file", errbuf);
        if (handle != NULL) {
            /* File handles are considered active */
            test_functions_on_handle(handle, 1, Data, Size);
            pcap_close(handle);
        }
        unlink("./dummy_file");
    }
    
    return 0;
}