// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_lookupnet at pcap.c:1547:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
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

static void dummy_cleanup(pcap_t *handle) {
    if (handle) {
        pcap_close(handle);
    }
}

static pcap_t *create_dummy_handle(void) {
    pcap_t *handle = pcap_open_dead(DLT_EN10MB, 65535);
    return handle;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    pcap_t *handle = create_dummy_handle();
    if (!handle) return 0;

    pcap_close(handle);
    handle = NULL;

    char device_name[256];
    char errbuf[PCAP_ERRBUF_SIZE];
    bpf_u_int32 netp, maskp;
    
    size_t device_len = Size > 255 ? 255 : Size;
    memcpy(device_name, Data, device_len);
    device_name[device_len] = '\0';

    int lookup_result = pcap_lookupnet(device_name, &netp, &maskp, errbuf);

    char dlt_name_buf[256];
    size_t dlt_name_len = Size > 255 ? 255 : Size;
    if (dlt_name_len > 0) {
        memcpy(dlt_name_buf, Data, dlt_name_len);
        dlt_name_buf[dlt_name_len] = '\0';
    } else {
        dlt_name_buf[0] = '\0';
    }
    
    int dlt_val = pcap_datalink_name_to_val(dlt_name_buf);

    handle = create_dummy_handle();
    if (!handle) return 0;

    int set_result = pcap_set_datalink(handle, dlt_val);

    char *error_msg = pcap_geterr(handle);

    dummy_cleanup(handle);
    
    return 0;
}