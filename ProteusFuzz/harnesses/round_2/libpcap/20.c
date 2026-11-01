// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_set_promisc at pcap.c:2544:1 in pcap.h
// pcap_setsampling at pcap.c:5067:1 in pcap.h
// pcap_breakloop at pcap.c:2932:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void cleanup_pcap(pcap_t *handle) {
    if (handle != NULL) {
        pcap_close(handle);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;

    // Test pcap_create
    // Use a portion of input as device name, ensuring null termination
    const char *device_name = NULL;
    if (Size > 0) {
        size_t dev_len = (Size < 100) ? Size : 100;
        char *dev_buf = malloc(dev_len + 1);
        if (dev_buf != NULL) {
            memcpy(dev_buf, Data, dev_len);
            dev_buf[dev_len] = '\0';
            device_name = dev_buf;
        }
    }

    handle = pcap_create(device_name, errbuf);
    if (device_name != NULL) {
        free((void *)device_name);
    }

    if (handle != NULL) {
        // Test pcap_set_buffer_size
        if (Size >= 2) {
            int buf_size = (int)(Data[0] | (Data[1] << 8));
            if (buf_size > 0) {
                pcap_set_buffer_size(handle, buf_size);
            }
        }

        // Test pcap_set_promisc
        if (Size >= 3) {
            int promisc = (Data[2] % 2);
            pcap_set_promisc(handle, promisc);
        }

        // Test pcap_setsampling (may return NULL locally)
        struct pcap_samp *samp = pcap_setsampling(handle);
        if (samp != NULL && Size >= 4) {
            samp->method = Data[3] % 5;
            if (Size >= 5) {
                samp->value = (int)(Data[4] | (Data[5] << 8));
            }
        }

        // Test pcap_breakloop
        // This function is safe to call even if not in a loop
        pcap_breakloop(handle);

        // Cleanup before testing pcap_open
        pcap_close(handle);
        handle = NULL;
    }

    // Test pcap_open
    // Construct a source string from input
    if (Size > 0) {
        size_t src_len = (Size < 100) ? Size : 100;
        char *src_buf = malloc(src_len + 1);
        if (src_buf != NULL) {
            memcpy(src_buf, Data, src_len);
            src_buf[src_len] = '\0';

            int snaplen = 65535;
            if (Size >= 2) {
                snaplen = (int)(Data[0] | (Data[1] << 8));
                if (snaplen <= 0) snaplen = 65535;
            }

            int flags = 0;
            if (Size >= 3) {
                flags = (Data[2] % 4);
            }

            int read_timeout = 1000;
            if (Size >= 5) {
                read_timeout = (int)(Data[3] | (Data[4] << 8));
                if (read_timeout < 0) read_timeout = 1000;
            }

            struct pcap_rmtauth auth = {0};
            if (Size >= 6) {
                auth.type = Data[5] % 3;
            }

            handle = pcap_open(src_buf, snaplen, flags, read_timeout, &auth, errbuf);
            free(src_buf);
        }
    }

    // Final cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}