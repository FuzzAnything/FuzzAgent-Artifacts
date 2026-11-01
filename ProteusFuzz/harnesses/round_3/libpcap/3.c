// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_strerror at pcap.c:3721:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_compile at gencode.c:1106:1 in pcap.h
// pcap_freecode at gencode.c:1291:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f == NULL) {
        return;
    }
    // Write a minimal pcap global header
    // magic number (little endian), version major, version minor, thiszone, sigfigs, snaplen, network
    uint8_t header[] = {
        0xa1, 0xb2, 0xc3, 0xd4, // magic
        0x02, 0x00, 0x00, 0x00, // version major (2.0)
        0x04, 0x00, 0x00, 0x00, // version minor (4.0)
        0x00, 0x00, 0x00, 0x00, // thiszone
        0x00, 0x00, 0x00, 0x00, // sigfigs
        0x00, 0x00, 0x00, 0x00, // snaplen (0)
        0x01, 0x00, 0x00, 0x00  // network (DLT_EN10MB)
    };
    fwrite(header, 1, sizeof(header), f);
    // Write a minimal packet header and some data
    uint8_t pkt_header[] = {
        0x00, 0x00, 0x00, 0x00, // ts_sec
        0x00, 0x00, 0x00, 0x00, // ts_usec
        0x04, 0x00, 0x00, 0x00, // incl_len
        0x04, 0x00, 0x00, 0x00  // orig_len
    };
    fwrite(pkt_header, 1, sizeof(pkt_header), f);
    uint8_t pkt_data[] = {0x00, 0x00, 0x00, 0x00};
    fwrite(pkt_data, 1, sizeof(pkt_data), f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. pcap_strerror
    // Use a value derived from the input to vary the error code
    int err_code = (int)(Data[0] % 100) - 50;
    const char *err_str = pcap_strerror(err_code);
    (void)err_str; // Suppress unused variable warning

    // Ensure dummy file exists for pcap_open_offline
    create_dummy_file();

    // 2. pcap_open_offline
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline("./dummy_file", errbuf);

    // 3. pcap_datalink_name_to_val
    // Use input data as a potential datalink name string
    // Ensure it is null-terminated for safety
    char dlt_name[256];
    size_t copy_len = (Size < 255) ? Size : 255;
    memcpy(dlt_name, Data, copy_len);
    dlt_name[copy_len] = '\0';
    
    int dlt_val = pcap_datalink_name_to_val(dlt_name);
    (void)dlt_val;

    // 4. pcap_open_dead
    // Use input data to determine linktype and snaplen
    // Check bounds before accessing Data[1]
    int linktype = (int)(Data[0] % 100);
    int snaplen = 64;
    if (Size > 1) {
        snaplen = (int)(Data[1] % 65536) + 64;
    }
    
    pcap_t *dead_handle = pcap_open_dead(linktype, snaplen);

    // 5. pcap_compile
    struct bpf_program fp;
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    int optimize = 0;
    if (Size > 2) {
        optimize = (Data[2] % 2);
    }
    
    // Prepare a filter string from input, ensuring it's null-terminated
    char filter_str[256];
    copy_len = (Size < 255) ? Size : 255;
    memcpy(filter_str, Data, copy_len);
    filter_str[copy_len] = '\0';

    int compile_result = -1;
    if (dead_handle != NULL) {
        compile_result = pcap_compile(dead_handle, &fp, filter_str, optimize, net);
        if (compile_result == 0) {
            pcap_freecode(&fp);
        }
    }
    (void)compile_result;

    // Cleanup
    if (dead_handle != NULL) {
        pcap_close(dead_handle);
    }
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}