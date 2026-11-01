// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_tstamp_type_name_to_val at pcap.c:3419:1 in pcap.h
// pcap_tstamp_type_val_to_name at pcap.c:3431:1 in pcap.h
// pcap_tstamp_type_val_to_description at pcap.c:3443:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
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
#include <errno.h>

// Helper to create a dummy pcap file for testing
static int create_dummy_pcap_file(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return -1;
    }

    // Write a minimal pcap global header
    // Magic number (little endian)
    uint32_t magic = 0xa1b2c3d4;
    fwrite(&magic, sizeof(uint32_t), 1, fp);
    // Version major
    uint16_t ver_major = 2;
    fwrite(&ver_major, sizeof(uint16_t), 1, fp);
    // Version minor
    uint16_t ver_minor = 4;
    fwrite(&ver_minor, sizeof(uint16_t), 1, fp);
    // Timezone offset (0)
    int32_t tz = 0;
    fwrite(&tz, sizeof(int32_t), 1, fp);
    // Sigfigs (0)
    int32_t sigfigs = 0;
    fwrite(&sigfigs, sizeof(int32_t), 1, fp);
    // Snaplen (65535)
    uint32_t snaplen = 65535;
    fwrite(&snaplen, sizeof(uint32_t), 1, fp);
    // Linktype (Ethernet)
    uint32_t linktype = 1;
    fwrite(&linktype, sizeof(uint32_t), 1, fp);

    fclose(fp);
    return 0;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Test pcap_tstamp_type_name_to_val
    // Use input data as a string (ensure null termination)
    size_t max_len = Size < 256 ? Size : 256;
    char name_buf[257];
    memcpy(name_buf, Data, max_len);
    name_buf[max_len] = '\0';

    int type_val = pcap_tstamp_type_name_to_val(name_buf);
    if (type_val != PCAP_ERROR) {
        // 2. Test pcap_tstamp_type_val_to_name
        const char *name = pcap_tstamp_type_val_to_name(type_val);
        (void)name; // Use result to avoid unused warning

        // 3. Test pcap_tstamp_type_val_to_description
        const char *desc = pcap_tstamp_type_val_to_description(type_val);
        (void)desc; // Use result to avoid unused warning
    }

    // 4. Test pcap_free_tstamp_types with NULL (should be avoided, but we test safety)
    // The spec says passing NULL causes undefined behavior, so we skip direct NULL call
    // unless we have a valid list. We can't easily generate a valid list without pcap_list_tstamp_types
    // which is not in the target list, so we skip this specific test or rely on internal safety.
    // To be safe, we don't call it with NULL.

    // 5. Test pcap_set_tstamp_type and pcap_get_tstamp_precision
    // We need a pcap_t handle. We can try to open a dummy file or a live device.
    // Opening a live device might fail on some environments, so we try a file first.
    
    const char *dummy_file = "./dummy_file";
    create_dummy_pcap_file(dummy_file);

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(dummy_file, errbuf);

    if (handle) {
        // Try to set timestamp type before activation (pcap_open_offline usually activates immediately)
        // If activated, pcap_set_tstamp_type should return PCAP_ERROR_ACTIVATED
        int ret = pcap_set_tstamp_type(handle, PCAP_TSTAMP_HOST);
        (void)ret;

        // Try with other potential types (fuzzing the type value)
        int fuzz_type = (int)(Data[0] % 10) - 5; // Generate small range of types
        ret = pcap_set_tstamp_type(handle, fuzz_type);
        (void)ret;

        // Get timestamp precision
        int precision = pcap_get_tstamp_precision(handle);
        (void)precision;

        pcap_close(handle);
    } else {
        // If file open fails, try to open a live capture on "lo" or similar
        // This might fail on restricted environments, which is fine for fuzzing
        handle = pcap_create("lo", errbuf);
        if (handle) {
            // Set timestamp type before activation
            int fuzz_type = (int)(Data[0] % 10) - 5;
            int ret = pcap_set_tstamp_type(handle, fuzz_type);
            (void)ret;

            // Try to activate
            ret = pcap_activate(handle);
            if (ret == 0) {
                // Now it's active, setting type should fail
                ret = pcap_set_tstamp_type(handle, PCAP_TSTAMP_HOST);
                (void)ret;

                int precision = pcap_get_tstamp_precision(handle);
                (void)precision;
            }
            pcap_close(handle);
        }
    }

    // Cleanup dummy file
    remove(dummy_file);

    return 0;
}