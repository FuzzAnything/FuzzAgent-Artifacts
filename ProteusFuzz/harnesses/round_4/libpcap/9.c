// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_geterr at pcap.c:3549:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_statustostr at pcap.c:3654:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    pcap_t *handle = NULL;
    int *datalinks = NULL;
    int dlt_count;
    int dlt_val;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Ensure we have enough data to derive a DLT value
    if (Size < 1) {
        return 0;
    }

    // Derive a DLT value from the input data
    dlt_val = (int)Data[0];

    // Initialize error buffer
    memset(errbuf, 0, sizeof(errbuf));

    // Create a minimal valid pcap file to ensure pcap_open_offline succeeds
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Minimal pcap header (24 bytes)
        uint32_t magic = 0xa1b2c3d4;
        uint16_t ver_major = 2;
        uint16_t ver_minor = 4;
        int32_t tz = 0;
        uint32_t sigfigs = 0;
        uint32_t snaplen = 65535;
        uint32_t network = 1; // Ethernet

        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&ver_major, sizeof(ver_major), 1, f);
        fwrite(&ver_minor, sizeof(ver_minor), 1, f);
        fwrite(&tz, sizeof(tz), 1, f);
        fwrite(&sigfigs, sizeof(sigfigs), 1, f);
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fwrite(&network, sizeof(network), 1, f);
        fclose(f);
    }

    // Attempt to open the file
    handle = pcap_open_offline("./dummy_file", errbuf);

    // 1. Invoke pcap_geterr
    if (handle != NULL) {
        char *err_msg = pcap_geterr(handle);
        (void)err_msg;
    }

    // 2. Invoke pcap_list_datalinks
    if (handle != NULL) {
        dlt_count = pcap_list_datalinks(handle, &datalinks);
        
        // 3. Invoke pcap_statustostr
        // Use the count or a derived value as status
        const char *status_str = pcap_statustostr(dlt_count);
        (void)status_str;

        // 4. Invoke pcap_geterr again
        char *err_msg2 = pcap_geterr(handle);
        (void)err_msg2;

        // 5. Invoke pcap_datalink_val_to_name
        // Use the derived dlt_val from input
        const char *dlt_name = pcap_datalink_val_to_name(dlt_val);
        (void)dlt_name;

        // Also try with values from the datalinks list if available
        if (datalinks != NULL && dlt_count > 0) {
            for (int i = 0; i < dlt_count; i++) {
                pcap_datalink_val_to_name(datalinks[i]);
            }
        }

        // 6. Invoke pcap_free_datalinks
        pcap_free_datalinks(datalinks);
        datalinks = NULL;
    } else {
        // Fallback path if handle is NULL (though we tried to create a valid file)
        // Invoke functions that don't require a handle
        pcap_statustostr(0);
        pcap_statustostr(-1);
        pcap_datalink_val_to_name(dlt_val);
        pcap_datalink_val_to_name(0);
        pcap_free_datalinks(NULL);
    }

    // Cleanup
    if (handle != NULL) {
        pcap_close(handle);
    }

    return 0;
}