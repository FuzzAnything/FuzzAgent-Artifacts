// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_set_immediate_mode at pcap.c:2616:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_loop at pcap.c:2899:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
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

static void dummy_callback(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes) {
    /* Do nothing - just a dummy callback */
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap = NULL;
    int status;
    const char *status_str;
    char *error_msg;
    
    /* Create a dummy pcap handle for testing */
    pcap = pcap_create("lo", errbuf);
    if (pcap == NULL) {
        /* If we can't create a handle, try with a different approach */
        pcap = pcap_open_dead(DLT_EN10MB, 65535);
        if (pcap == NULL) {
            return 0;
        }
    }
    
    /* First call: pcap_set_immediate_mode */
    if (Size > 0) {
        int immediate_mode = Data[0] & 1;
        status = pcap_set_immediate_mode(pcap, immediate_mode);
        /* Second call: pcap_statustostr */
        status_str = pcap_statustostr(status);
        (void)status_str; /* Use result to avoid compiler warning */
    }
    
    /* Third call: pcap_set_timeout */
    if (Size > 1) {
        int timeout = Data[1] % 1000; /* Limit timeout to reasonable range */
        status = pcap_set_timeout(pcap, timeout);
        /* Fourth call: pcap_statustostr */
        status_str = pcap_statustostr(status);
        (void)status_str; /* Use result to avoid compiler warning */
    }
    
    /* Fifth call: pcap_activate */
    status = pcap_activate(pcap);
    /* Sixth call: pcap_statustostr */
    status_str = pcap_statustostr(status);
    (void)status_str; /* Use result to avoid compiler warning */
    
    /* Seventh call: pcap_geterr */
    error_msg = pcap_geterr(pcap);
    (void)error_msg; /* Use result to avoid compiler warning */
    
    /* If activation succeeded, try to do a dummy loop to explore more states */
    if (status == 0) {
        /* Create a dummy file with fuzz data for potential offline reading */
        int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            if (Size > 0) {
                write(fd, Data, Size > 1024 ? 1024 : Size);
            }
            close(fd);
            
            /* Try to open the dummy file as offline capture */
            pcap_t *offline_pcap = pcap_open_offline("./dummy_file", errbuf);
            if (offline_pcap != NULL) {
                /* Try a dummy loop */
                pcap_loop(offline_pcap, 1, dummy_callback, NULL);
                pcap_close(offline_pcap);
            }
            
            /* Clean up the dummy file */
            unlink("./dummy_file");
        }
    }
    
    /* Clean up */
    if (pcap != NULL) {
        pcap_close(pcap);
    }
    
    return 0;
}