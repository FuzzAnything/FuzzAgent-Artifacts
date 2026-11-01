// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_tstamp_type_val_to_name at pcap.c:3431:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_live at pcap.c:2749:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
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
#include <ctype.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int *dlt_list = NULL;
    int count;

    // Convert input data to int for DLT value testing
    int dlt_val = 0;
    if (Size >= 4) {
        memcpy(&dlt_val, Data, 4);
    } else if (Size > 0) {
        dlt_val = Data[0];
    }

    // Test pcap_datalink_val_to_description_or_dlt
    const char *desc = pcap_datalink_val_to_description_or_dlt(dlt_val);
    if (desc) {
        // Just use the string to ensure it's valid
        (void)strlen(desc);
    }

    // Test pcap_datalink_val_to_name
    const char *name = pcap_datalink_val_to_name(dlt_val);
    if (name) {
        (void)strlen(name);
    }

    // Test pcap_tstamp_type_val_to_name with derived value
    int tstamp_val = (Size > 1) ? Data[1] : 0;
    const char *tstamp_name = pcap_tstamp_type_val_to_name(tstamp_val);
    if (tstamp_name) {
        (void)strlen(tstamp_name);
    }

    // Test pcap_datalink_name_to_val with input data as string
    // Ensure null termination
    char *input_str = NULL;
    if (Size > 0) {
        input_str = malloc(Size + 1);
        if (input_str) {
            memcpy(input_str, Data, Size);
            input_str[Size] = '\0';
            int name_to_val = pcap_datalink_name_to_val(input_str);
            // name_to_val is int, no cleanup needed
            free(input_str);
        }
    }

    // Try to open a pcap handle for pcap_list_datalinks and pcap_datalink
    // First try live capture (might fail, which is fine)
    handle = pcap_open_live("lo", 65536, 1, 1000, errbuf);
    
    if (!handle) {
        // Try opening the dummy file if live capture failed
        write_dummy_file(Data, Size);
        handle = pcap_open_offline("./dummy_file", errbuf);
    }

    if (handle) {
        // Activate the handle if needed (pcap_activate is implicit in some open functions, 
        // but pcap_datalink requires activation)
        // For pcap_open_live and pcap_open_offline, the handle is usually activated.
        // However, pcap_datalink checks the 'activated' flag.
        // If pcap_open_offline returns a handle, it is typically activated.
        
        // Test pcap_datalink
        int linktype = pcap_datalink(handle);
        // linktype is int, no cleanup needed

        // Test pcap_list_datalinks
        count = pcap_list_datalinks(handle, &dlt_list);
        if (count > 0 && dlt_list) {
            // Iterate to exercise the list
            for (int i = 0; i < count; i++) {
                int dlt = dlt_list[i];
                // Reuse conversion functions with list values
                (void)pcap_datalink_val_to_description_or_dlt(dlt);
                (void)pcap_datalink_val_to_name(dlt);
            }
            pcap_free_datalinks(dlt_list);
            dlt_list = NULL;
        }

        pcap_close(handle);
        handle = NULL;
    }

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}