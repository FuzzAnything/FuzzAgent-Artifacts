// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
// pcap_datalink_val_to_description at pcap.c:3376:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int dlt_val;
    const char *name_str;
    const char *desc_str;
    const char *desc_or_dlt_str;
    int name_to_val_result;

    if (Size < 4) {
        return 0;
    }

    // Extract a potential DLT value from the input
    dlt_val = (int)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));

    // Test pcap_datalink_val_to_name
    name_str = pcap_datalink_val_to_name(dlt_val);
    (void)name_str; // Use the result to avoid unused variable warning

    // Test pcap_datalink_val_to_description_or_dlt
    desc_or_dlt_str = pcap_datalink_val_to_description_or_dlt(dlt_val);
    (void)desc_or_dlt_str;

    // Test pcap_datalink_val_to_description
    desc_str = pcap_datalink_val_to_description(dlt_val);
    (void)desc_str;

    // Test pcap_datalink_name_to_val with a substring from input as a name
    // We create a null-terminated string from the input data to pass as a name
    char name_buf[256];
    size_t name_len = Size < 255 ? Size : 255;
    memcpy(name_buf, Data, name_len);
    name_buf[name_len] = '\0';
    name_to_val_result = pcap_datalink_name_to_val(name_buf);
    (void)name_to_val_result;

    // Attempt to open a handle to test pcap_datalink and pcap_set_datalink
    // We try opening a savefile first if we have enough data, otherwise try live capture on a dummy device
    if (Size > 100) {
        write_dummy_file(Data, Size);
        handle = pcap_open_offline("./dummy_file", errbuf);
    }

    if (handle == NULL) {
        // Fallback to trying to open a live capture on a dummy device name
        // This will likely fail, but we need to test the API flow
        handle = pcap_create("dummy_device", errbuf);
        if (handle != NULL) {
            // Try to activate it
            if (pcap_activate(handle) == 0) {
                // Handle is active, test pcap_datalink
                int current_dlt = pcap_datalink(handle);
                (void)current_dlt;

                // Try to set a new datalink
                int set_result = pcap_set_datalink(handle, dlt_val);
                (void)set_result;

                // Verify the change
                int new_dlt = pcap_datalink(handle);
                (void)new_dlt;
            }
        }
    }

    if (handle != NULL) {
        // Test pcap_datalink on an inactive handle (if it wasn't activated)
        // This should return PCAP_ERROR_NOT_ACTIVATED
        int inactive_dlt = pcap_datalink(handle);
        (void)inactive_dlt;

        // Test pcap_set_datalink on an inactive handle
        int set_inactive_result = pcap_set_datalink(handle, dlt_val);
        (void)set_inactive_result;

        pcap_close(handle);
    }

    return 0;
}