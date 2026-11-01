// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_val_to_description at pcap.c:3376:1 in pcap.h
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
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
#include <fcntl.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static pcap_t *create_dummy_pcap(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    // Try to open a savefile first if we have enough data to simulate a pcap file
    // A minimal pcap file needs at least 24 bytes for the global header
    if (size >= 24) {
        write_dummy_file(data, size);
        handle = pcap_open_offline("./dummy_file", errbuf);
        if (handle) {
            // Activate the handle to allow pcap_datalink to work
            if (pcap_activate(handle) == 0) {
                return handle;
            }
            pcap_close(handle);
        }
    }

    // Fallback: Try to open a live capture on "any" or a common interface
    // This often fails in restricted environments, but is a valid state to test
    handle = pcap_create("any", errbuf);
    if (handle) {
        if (pcap_activate(handle) == 0) {
            return handle;
        }
        pcap_close(handle);
    }

    return NULL;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Extract potential DLT values from the input
    int dlt_val1 = 0;
    int dlt_val2 = 0;
    if (Size >= 4) {
        dlt_val1 = *(const int32_t *)(Data);
        if (Size >= 8) {
            dlt_val2 = *(const int32_t *)(Data + 4);
        }
    }

    // Fuzz pcap_datalink_val_to_description
    const char *desc1 = pcap_datalink_val_to_description(dlt_val1);
    (void)desc1; // Suppress unused warning, used for coverage

    // Fuzz pcap_datalink_val_to_description_or_dlt
    const char *desc_or_dlt = pcap_datalink_val_to_description_or_dlt(dlt_val1);
    if (desc_or_dlt) {
        // Just access to ensure no crash
        (void)desc_or_dlt[0];
    }

    // Fuzz pcap_datalink_val_to_name
    const char *name = pcap_datalink_val_to_name(dlt_val1);
    if (name) {
        (void)name[0];
    }

    // Fuzz pcap_datalink_name_to_val
    // We use a portion of the input as the name string, ensuring null termination
    if (Size > 0) {
        // Create a temporary buffer for the name
        char *name_buf = (char *)malloc(Size + 1);
        if (name_buf) {
            memcpy(name_buf, Data, Size);
            name_buf[Size] = '\0';
            
            int val = pcap_datalink_name_to_val(name_buf);
            (void)val; // Suppress unused warning

            // Also try with a substring to test different name lengths
            for (size_t i = 1; i < Size; i++) {
                name_buf[i] = '\0';
                pcap_datalink_name_to_val(name_buf);
            }
            free(name_buf);
        }
    }

    // Create a pcap handle to test pcap_datalink and pcap_set_datalink
    pcap_t *handle = create_dummy_pcap(Data, Size);

    if (handle) {
        // Fuzz pcap_datalink
        int current_dlt = pcap_datalink(handle);
        (void)current_dlt;

        // Fuzz pcap_set_datalink with various values
        // Try the extracted values
        if (pcap_set_datalink(handle, dlt_val1) == 0) {
            // Success, verify
            pcap_datalink(handle);
        }
        if (pcap_set_datalink(handle, dlt_val2) == 0) {
            pcap_datalink(handle);
        }

        // Try some common DLTs
        pcap_set_datalink(handle, DLT_EN10MB);
        pcap_set_datalink(handle, DLT_RAW);
        pcap_set_datalink(handle, DLT_NULL);
        
        // Try invalid DLTs
        pcap_set_datalink(handle, -1);
        pcap_set_datalink(handle, 0xFFFF);
        pcap_set_datalink(handle, 0xFFFFFFFF);

        pcap_close(handle);
    }

    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}