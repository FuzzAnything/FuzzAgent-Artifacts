// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_val_to_description at pcap.c:3376:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_tstamp_type_val_to_description at pcap.c:3443:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/*
 * Fuzz driver for libpcap API functions.
 * Targets:
 * 1. pcap_datalink_val_to_description
 * 2. pcap_datalink_name_to_val
 * 3. pcap_tstamp_type_val_to_description
 * 4. pcap_datalink_val_to_name
 * 5. pcap_datalink
 * 6. pcap_set_datalink
 */

/* Helper to write dummy data to a file for pcap_open_offline */
static int create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return -1;
    }
    /* Write a minimal pcap header if size is large enough, otherwise just raw data */
    if (size >= 24) {
        /* Minimal valid pcap header (magic, version, etc.) */
        uint32_t magic = 0xa1b2c3d4;
        uint16_t version_major = 2;
        uint16_t version_minor = 4;
        int32_t thiszone = 0;
        uint32_t sigfigs = 0;
        int32_t snaplen = 65535;
        int32_t network = 1; /* Ethernet */
        
        fwrite(&magic, sizeof(magic), 1, f);
        fwrite(&version_major, sizeof(version_major), 1, f);
        fwrite(&version_minor, sizeof(version_minor), 1, f);
        fwrite(&thiszone, sizeof(thiszone), 1, f);
        fwrite(&sigfigs, sizeof(sigfigs), 1, f);
        fwrite(&snaplen, sizeof(snaplen), 1, f);
        fwrite(&network, sizeof(network), 1, f);
        
        /* Write remaining data */
        if (size > 24) {
            fwrite(data, 1, size - 24, f);
        }
    } else {
        /* Just write the input data as is, might fail to open but that's okay for fuzzing */
        fwrite(data, 1, size, f);
    }
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * Strategy:
     * Use the input data to drive different API calls.
     * We will partition the input to test different scenarios.
     */

    /* Test 1: pcap_datalink_val_to_description */
    /* Use first integer from data as dlt value */
    if (Size >= 4) {
        int dlt_val = *(const int32_t *)(Data);
        const char *desc = pcap_datalink_val_to_description(dlt_val);
        (void)desc; /* Suppress unused warning, just exercising the function */
    }

    /* Test 2: pcap_datalink_name_to_val */
    /* Use a substring from data as the name string */
    if (Size > 0) {
        /* Ensure null termination */
        char name_buf[256];
        size_t len = (Size < 255) ? Size : 255;
        memcpy(name_buf, Data, len);
        name_buf[len] = '\0';
        
        int dlt_from_name = pcap_datalink_name_to_val(name_buf);
        (void)dlt_from_name;
    }

    /* Test 3: pcap_tstamp_type_val_to_description */
    /* Use second integer from data if available */
    if (Size >= 8) {
        int tstamp_val = *(const int32_t *)(Data + 4);
        const char *tstamp_desc = pcap_tstamp_type_val_to_description(tstamp_val);
        (void)tstamp_desc;
    }

    /* Test 4: pcap_datalink_val_to_name */
    /* Use third integer from data if available */
    if (Size >= 12) {
        int dlt_val2 = *(const int32_t *)(Data + 8);
        const char *dlt_name = pcap_datalink_val_to_name(dlt_val2);
        (void)dlt_name;
    }

    /* Test 5 & 6: pcap_datalink and pcap_set_datalink */
    /* These require a valid pcap_t handle. We will try to create one from a dummy file. */
    const char *dummy_file = "./dummy_file";
    
    /* Create a dummy file with the input data */
    if (create_dummy_file(dummy_file, Data, Size) == 0) {
        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *handle = pcap_open_offline(dummy_file, errbuf);
        
        if (handle != NULL) {
            /* The handle is open. pcap_datalink and pcap_set_datalink require activation.
             * pcap_open_offline automatically activates the handle.
             */
            
            /* Test pcap_datalink */
            int current_dlt = pcap_datalink(handle);
            (void)current_dlt;
            
            /* Test pcap_set_datalink with various values derived from input */
            if (Size >= 4) {
                int new_dlt = *(const int32_t *)(Data);
                int ret = pcap_set_datalink(handle, new_dlt);
                (void)ret;
            }
            
            /* Test pcap_set_datalink with a fixed known value (e.g., DLT_EN10MB) */
            int ret_fixed = pcap_set_datalink(handle, 1); /* DLT_EN10MB is usually 1 */
            (void)ret_fixed;

            pcap_close(handle);
        }
        /* If handle is NULL, pcap_open_offline failed, which is a valid state to explore */
    }
    
    /* Cleanup dummy file */
    remove(dummy_file);

    return 0;
}