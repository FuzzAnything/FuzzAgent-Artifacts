// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void create_dummy_file(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal pcap header
    uint32_t magic = 0xa1b2c3d4;
    fwrite(&magic, sizeof(uint32_t), 1, f);
    uint16_t major = 2;
    fwrite(&major, sizeof(uint16_t), 1, f);
    uint16_t minor = 4;
    fwrite(&minor, sizeof(uint16_t), 1, f);
    int32_t tz = 0;
    fwrite(&tz, sizeof(int32_t), 1, f);
    uint32_t sigfigs = 0;
    fwrite(&sigfigs, sizeof(uint32_t), 1, f);
    uint32_t snaplen = 65535;
    fwrite(&snaplen, sizeof(uint32_t), 1, f);
    uint32_t linktype = 1; // Ethernet
    fwrite(&linktype, sizeof(uint32_t), 1, f);
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a dummy file for pcap_open_offline
    create_dummy_file("./dummy_file");

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int *datalinks = NULL;

    // Try to open the dummy file
    handle = pcap_open_offline("./dummy_file", errbuf);
    
    // If handle is NULL, we cannot safely proceed with the required sequence
    if (!handle) {
        return 0;
    }

    // 1. Invoke pcap_geterr
    char *err_msg = pcap_geterr(handle);
    (void)err_msg;

    // 2. Invoke pcap_list_datalinks
    int dlt_count = pcap_list_datalinks(handle, &datalinks);
    (void)dlt_count;

    // 3. Invoke pcap_statustostr
    int status_code = (int)(Data[0] & 0xFF);
    const char *status_str = pcap_statustostr(status_code);
    (void)status_str;

    // 4. Invoke pcap_geterr again
    err_msg = pcap_geterr(handle);
    (void)err_msg;

    // 5. Invoke pcap_datalink_val_to_name
    int dlt_val = (int)(Data[0] & 0xFF);
    if (Size > 1) {
        dlt_val = (int)(Data[1] & 0xFF) * 10 + (int)(Data[0] & 0xFF);
    }
    const char *dlt_name = pcap_datalink_val_to_name(dlt_val);
    (void)dlt_name;

    // 6. Invoke pcap_free_datalinks
    if (datalinks) {
        pcap_free_datalinks(datalinks);
        datalinks = NULL;
    }

    // Cleanup
    if (handle) {
        pcap_close(handle);
    }

    return 0;
}