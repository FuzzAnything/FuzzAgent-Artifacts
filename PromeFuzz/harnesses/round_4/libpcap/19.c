// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_bufsize at pcap.c:3487:1 in pcap.h
// pcap_compile at gencode.c:1105:1 in pcap.h
// pcap_setfilter at pcap.c:3807:1 in pcap.h
// pcap_freecode at gencode.c:1290:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_set_snaplen at pcap.c:2535:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <pcap/pcap.h>

static void dummy_handler(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    (void)user;
    (void)h;
    (void)bytes;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = NULL;
    int fd = -1;
    ssize_t written;
    
    if (Size < 1) {
        return 0;
    }
    
    const uint8_t *current = Data;
    size_t remaining = Size;
    
    int use_file = (*current) & 1;
    current++;
    remaining--;
    
    if (use_file && remaining > 0) {
        fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            written = write(fd, current, remaining > 1024 ? 1024 : remaining);
            close(fd);
        }
        
        pcap = pcap_open_offline("./dummy_file", errbuf);
        if (pcap == NULL) {
            pcap = pcap_open_dead(DLT_EN10MB, 65535);
        }
        unlink("./dummy_file");
    } else {
        pcap = pcap_open_dead(DLT_EN10MB, 65535);
    }
    
    if (pcap == NULL) {
        return 0;
    }
    
    pcap_close(pcap);
    
    pcap = pcap_create("lo", errbuf);
    if (pcap == NULL) {
        pcap = pcap_create("any", errbuf);
    }
    if (pcap == NULL) {
        return 0;
    }
    
    if (remaining > 0) {
        int param1 = (int)(*current % 256);
        current++;
        remaining--;
        
        pcap_set_immediate_mode(pcap, param1 & 1);
    }
    
    if (remaining > 0) {
        int param2 = (int)(*current % 256);
        current++;
        remaining--;
        
        pcap_set_promisc(pcap, param2 & 1);
    }
    
    if (remaining > 0) {
        int param3 = (int)(*current % 256);
        current++;
        remaining--;
        
        pcap_set_timeout(pcap, param3);
    }
    
    if (remaining > 0) {
        int param4 = (int)(*current % 65536);
        current++;
        remaining--;
        if (remaining > 0) {
            param4 = (param4 << 8) | (*current % 256);
            current++;
            remaining--;
        }
        
        pcap_set_snaplen(pcap, param4);
    }
    
    int activate_ret = pcap_activate(pcap);
    
    if (activate_ret == 0) {
        int bufsize = pcap_bufsize(pcap);
        (void)bufsize;
        
        struct bpf_program fp;
        const char filter_str[] = "ip";
        if (pcap_compile(pcap, &fp, filter_str, 0, PCAP_NETMASK_UNKNOWN) == 0) {
            pcap_setfilter(pcap, &fp);
            pcap_freecode(&fp);
        }
        
        if (remaining > 0) {
            int loop_cnt = (*current % 10) + 1;
            current++;
            remaining--;
            
            pcap_loop(pcap, loop_cnt, dummy_handler, NULL);
        }
        
        pcap_breakloop(pcap);
    }
    
    pcap_close(pcap);
    
    return 0;
}