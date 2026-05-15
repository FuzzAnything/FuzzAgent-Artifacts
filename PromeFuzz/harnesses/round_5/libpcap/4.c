// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
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

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    write_dummy_file(Data, Size);

    char errbuf[PCAP_ERRBUF_SIZE] = {0};
    char errbuf2[PCAP_ERRBUF_SIZE] = {0};
    struct pcap_rmtauth auth = {0};
    const char *source = "./dummy_file";
    
    int status1 = (int)(Data[0] % 256) - 128;
    const char *str1 = pcap_statustostr(status1);
    (void)str1;

    pcap_t *pcap1 = pcap_create("any", errbuf);
    if (pcap1) {
        int timeout = (int)(Data[0] % 10000);
        int ret1 = pcap_set_timeout(pcap1, timeout);
        (void)ret1;

        int status2 = (int)(Data[0] % 256) - 128;
        const char *str2 = pcap_statustostr(status2);
        (void)str2;

        int buffer_size = (int)(Data[0] % 65536);
        int ret2 = pcap_set_buffer_size(pcap1, buffer_size);
        (void)ret2;

        int status3 = (int)(Data[0] % 256) - 128;
        const char *str3 = pcap_statustostr(status3);
        (void)str3;

        int ret3 = pcap_activate(pcap1);
        (void)ret3;

        int status4 = (int)(Data[0] % 256) - 128;
        const char *str4 = pcap_statustostr(status4);
        (void)str4;

        char *err1 = pcap_geterr(pcap1);
        (void)err1;

        int status5 = (int)(Data[0] % 256) - 128;
        const char *str5 = pcap_statustostr(status5);
        (void)str5;

        char *err2 = pcap_geterr(pcap1);
        (void)err2;

        pcap_close(pcap1);
    }

    int snaplen = (int)(Data[0] % 65536);
    int flags = (int)(Data[0] % 256);
    int read_timeout = (int)(Data[0] % 10000);
    
    pcap_t *pcap2 = pcap_open(source, snaplen, flags, read_timeout, &auth, errbuf);
    if (pcap2) {
        pcap_close(pcap2);
    }

    const char *device = "eth0";
    int promisc = (int)(Data[0] % 2);
    int timeout2 = (int)(Data[0] % 10000);
    
    pcap_t *pcap3 = pcap_open_live(device, snaplen, promisc, timeout2, errbuf2);
    if (pcap3) {
        pcap_close(pcap3);
    }

    unlink("./dummy_file");
    return 0;
}