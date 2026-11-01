// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_lookupdev at pcap.c:1471:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_tstamp_type_val_to_name at pcap.c:3431:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int *dlt_list = NULL;
    int dlt_count;
    int dlt_val = 0;
    int tstamp_val = 0;
    const char *name_str;
    int name_to_val;
    int datalink;
    const char *desc;

    // Extract test parameters safely from fuzz input
    if (Size >= 4) {
        dlt_val = *(const int *)(Data);
    }
    if (Size >= 8) {
        tstamp_val = *(const int *)(Data + 4);
    }

    // 1. Test pcap_datalink_val_to_description_or_dlt
    desc = pcap_datalink_val_to_description_or_dlt(dlt_val);
    if (desc) {
        (void)strlen(desc);
    }

    // 2. Test pcap_datalink_val_to_name
    name_str = pcap_datalink_val_to_name(dlt_val);
    if (name_str) {
        (void)strlen(name_str);
    }

    // 3. Test pcap_tstamp_type_val_to_name
    const char *tstamp_name = pcap_tstamp_type_val_to_name(tstamp_val);
    if (tstamp_name) {
        (void)strlen(tstamp_name);
    }

    // 4. Test pcap_datalink_name_to_val
    if (Size > 4) {
        // Find a null terminator or use a substring
        size_t str_len = 0;
        while (str_len < Size - 4 && Data[4 + str_len] != '\0') {
            str_len++;
        }
        if (str_len > 0) {
            char *test_str = malloc(str_len + 1);
            if (test_str) {
                memcpy(test_str, Data + 4, str_len);
                test_str[str_len] = '\0';
                name_to_val = pcap_datalink_name_to_val(test_str);
                (void)name_to_val;
                free(test_str);
            }
        }
    }

    // 5. Test pcap_list_datalinks, pcap_datalink
    // Create a dummy file if we have data
    if (Size > 0) {
        write_dummy_file(Data, Size);
    }

    // Try to open the dummy file as a savefile
    handle = pcap_open_offline("./dummy_file", errbuf);
    
    if (handle) {
        // Test pcap_datalink
        datalink = pcap_datalink(handle);
        (void)datalink;

        // Test pcap_list_datalinks
        dlt_count = pcap_list_datalinks(handle, &dlt_list);
        if (dlt_count > 0 && dlt_list != NULL) {
            // Ensure we free the list as per API contract
            pcap_free_datalinks(dlt_list);
            dlt_list = NULL;
        }

        // Cleanup handle
        pcap_close(handle);
        handle = NULL;
    }

    // If savefile failed, try to open a live device (loopback or first available)
    if (!handle) {
        const char *device = pcap_lookupdev(errbuf);
        if (device) {
            handle = pcap_create(device, errbuf);
            if (handle) {
                // Try to activate
                if (pcap_activate(handle) == 0) {
                    // Test pcap_datalink
                    datalink = pcap_datalink(handle);
                    (void)datalink;

                    // Test pcap_list_datalinks
                    dlt_count = pcap_list_datalinks(handle, &dlt_list);
                    if (dlt_count > 0 && dlt_list != NULL) {
                        pcap_free_datalinks(dlt_list);
                        dlt_list = NULL;
                    }
                }
                pcap_close(handle);
                handle = NULL;
            }
        }
    }

    // Cleanup any remaining resources if needed
    if (dlt_list) {
        pcap_free_datalinks(dlt_list);
    }

    return 0;
}