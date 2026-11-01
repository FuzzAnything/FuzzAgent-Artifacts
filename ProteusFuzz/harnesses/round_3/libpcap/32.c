// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
// pcap_datalink_val_to_description at pcap.c:3376:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    pcap_t *handle = NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    int dlt_val;
    const char *desc;
    const char *name;
    int ret;
    int i;

    if (Size == 0) {
        return 0;
    }

    // Fuzz pcap_datalink_val_to_description_or_dlt
    // Takes any int, returns a string.
    dlt_val = (int)Data[0];
    if (Size > 1) dlt_val |= (int)Data[1] << 8;
    if (Size > 2) dlt_val |= (int)Data[2] << 16;
    if (Size > 3) dlt_val |= (int)Data[3] << 24;
    
    desc = pcap_datalink_val_to_description_or_dlt(dlt_val);
    if (desc) {
        // Just consume the string to ensure no crash on usage
        (void)strlen(desc);
    }

    // Fuzz pcap_datalink_val_to_description
    desc = pcap_datalink_val_to_description(dlt_val);
    // Can be NULL, handle gracefully

    // Fuzz pcap_datalink_val_to_name
    name = pcap_datalink_val_to_name(dlt_val);
    // Can be NULL, handle gracefully

    // Fuzz pcap_datalink_name_to_val
    // Create a temporary string from input data to pass as name
    if (Size > 0) {
        char *name_buf = malloc(Size + 1);
        if (name_buf) {
            // Ensure it's a valid string by null-terminating
            memcpy(name_buf, Data, Size);
            name_buf[Size] = '\0';
            
            // Try to convert name to val
            ret = pcap_datalink_name_to_val(name_buf);
            // ret can be -1 on failure
            
            // Also try with case variations if possible (simple toggle)
            for (i = 0; i < Size; i++) {
                if (isalpha((unsigned char)name_buf[i])) {
                    name_buf[i] = islower((unsigned char)name_buf[i]) ? toupper((unsigned char)name_buf[i]) : tolower((unsigned char)name_buf[i]);
                }
            }
            ret = pcap_datalink_name_to_val(name_buf);
            free(name_buf);
        }
    }

    // Prepare for pcap_datalink and pcap_set_datalink
    // We need an activated pcap_t handle.
    // Try opening a savefile first (more deterministic for fuzzing)
    write_dummy_file(Data, Size);
    handle = pcap_open_offline("./dummy_file", errbuf);

    if (handle) {
        // Try to activate if not already active (pcap_open_offline usually activates on read, but let's be safe)
        // pcap_activate is internal in some versions, but pcap_datalink requires activation.
        // pcap_open_offline returns an activated handle usually, but let's try to read to ensure state.
        // If pcap_open_offline fails to activate properly, pcap_datalink might return PCAP_ERROR_NOT_ACTIVATED.
        
        // Fuzz pcap_datalink
        dlt_val = pcap_datalink(handle);
        if (dlt_val == PCAP_ERROR_NOT_ACTIVATED) {
            // Try to activate explicitly if possible, or just proceed to set_datalink which might fail gracefully
        } else {
            // Fuzz pcap_set_datalink with various DLT values
            // Use the first byte as the target DLT
            int target_dlt = (int)Data[0];
            if (Size > 1) target_dlt |= (int)Data[1] << 8;
            
            ret = pcap_set_datalink(handle, target_dlt);
            // ret is 0 on success, -1 on failure
        }

        // Cleanup
        pcap_close(handle);
        handle = NULL;
    }

    // Try opening as a live capture if offline failed or to test different code paths
    // Using "any" or a dummy interface might fail, but that's expected in fuzzing
    // We use the first part of data as a potential device name, but ensure it's safe
    if (!handle && Size > 0) {
        char device[256];
        size_t dev_len = Size < 255 ? Size : 255;
        memcpy(device, Data, dev_len);
        device[dev_len] = '\0';
        
        // Remove nulls in the middle to make it a valid string
        for (i = 0; i < dev_len; i++) {
            if (device[i] == '\0') device[i] = ' ';
        }

        handle = pcap_create(device, errbuf);
        if (handle) {
            // Try to activate
            ret = pcap_activate(handle);
            if (ret == 0) {
                // Handle is active, fuzz pcap_datalink
                dlt_val = pcap_datalink(handle);
                
                // Fuzz pcap_set_datalink
                int target_dlt = (int)Data[0];
                if (Size > 1) target_dlt |= (int)Data[1] << 8;
                ret = pcap_set_datalink(handle, target_dlt);
            }
            // Cleanup
            pcap_close(handle);
            handle = NULL;
        }
    }

    // Clean up dummy file
    remove("./dummy_file");

    return 0;
}