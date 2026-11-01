// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_timeout at pcap.c:2562:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_set_buffer_size at pcap.c:2625:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_open at pcap.c:4949:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    pcap_t *handle_live = NULL;
    pcap_t *handle_open = NULL;
    int ret;

    // Ensure we have some data to work with for parameters
    if (Size == 0) {
        return 0;
    }

    // 1. pcap_statustostr
    (void)pcap_statustostr(PCAP_ERROR);

    // Create a handle for deferred activation
    handle = pcap_create("any", errbuf);
    if (handle == NULL) {
        // If creation fails, we can't test the subsequent deferred functions
        // but we can still test statustostr
        (void)pcap_statustostr(PCAP_ERROR);
        return 0;
    }

    // 2. pcap_set_timeout
    int timeout_val = (Size > 0) ? (int)Data[0] : 1000;
    ret = pcap_set_timeout(handle, timeout_val);
    (void)ret;

    // 3. pcap_statustostr
    (void)pcap_statustostr(ret);

    // 4. pcap_set_buffer_size
    int buffer_size_val = (Size > 1) ? (int)(Data[1] * 1024) : 1024 * 1024;
    if (buffer_size_val <= 0) buffer_size_val = 1024 * 1024;
    ret = pcap_set_buffer_size(handle, buffer_size_val);
    (void)ret;

    // 5. pcap_statustostr
    (void)pcap_statustostr(ret);

    // 6. pcap_activate
    ret = pcap_activate(handle);
    (void)ret;

    // 7. pcap_statustostr
    (void)pcap_statustostr(ret);

    // 8. pcap_geterr
    const char *err_msg = pcap_geterr(handle);
    (void)err_msg;

    // 9. pcap_statustostr
    (void)pcap_statustostr(PCAP_WARNING);

    // 10. pcap_geterr
    err_msg = pcap_geterr(handle);
    (void)err_msg;

    // 11. pcap_open
    // Construct a source string from input or use default
    const char *source = "any";
    if (Size > 10) {
        char src_buf[256];
        size_t copy_len = (Size > 255) ? 255 : Size;
        memcpy(src_buf, Data, copy_len);
        src_buf[copy_len] = '\0';
        source = src_buf;
    }
    
    int snaplen = (Size > 2) ? (int)Data[2] : 65535;
    int flags = (Size > 3) ? (int)Data[3] : 0;
    int read_timeout = (Size > 4) ? (int)Data[4] : 1000;
    
    // Create a dummy auth struct
    struct pcap_rmtauth auth = {0};
    
    handle_open = pcap_open(source, snaplen, flags, read_timeout, &auth, errbuf);
    if (handle_open != NULL) {
        // If pcap_open succeeds, we should close it
        pcap_close(handle_open);
    }

    // 12. pcap_open_live
    const char *device = "any";
    if (Size > 5) {
        char dev_buf[256];
        // Ensure we don't read past the end of Data
        size_t available = Size - 5;
        size_t copy_len = (available > 255) ? 255 : available;
        memcpy(dev_buf, Data + 5, copy_len);
        dev_buf[copy_len] = '\0';
        device = dev_buf;
    }
    
    int promisc = (Size > 6) ? (int)Data[6] : 0;
    int live_timeout = (Size > 7) ? (int)Data[7] : 1000;
    
    handle_live = pcap_open_live(device, snaplen, promisc, live_timeout, errbuf);
    if (handle_live != NULL) {
        // 13. pcap_close (for live handle)
        pcap_close(handle_live);
    }

    // Cleanup for the deferred handle if it was successfully activated or not
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}