// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
// pcap_tstamp_type_val_to_name at pcap.c:3431:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
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

static void create_dummy_savefile(const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return;

    // Write a minimal pcap global header (24 bytes)
    // Magic number (little endian)
    uint32_t magic = 0xa1b2c3d4;
    // Version major
    uint16_t ver_major = 2;
    // Version minor
    uint16_t ver_minor = 4;
    // Timezone
    int32_t tz = 0;
    // Sigfigs
    uint32_t sigfigs = 0;
    // Snaplen
    uint32_t snaplen = 65535;
    // Network (DLT_EN10MB)
    uint32_t network = 1;

    fwrite(&magic, sizeof(magic), 1, fp);
    fwrite(&ver_major, sizeof(ver_major), 1, fp);
    fwrite(&ver_minor, sizeof(ver_minor), 1, fp);
    fwrite(&tz, sizeof(tz), 1, fp);
    fwrite(&sigfigs, sizeof(sigfigs), 1, fp);
    fwrite(&snaplen, sizeof(snaplen), 1, fp);
    fwrite(&network, sizeof(network), 1, fp);

    fclose(fp);
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Test pcap_datalink_name_to_val
    // Ensure the input is null-terminated for string operations
    char *name_buf = (char *)malloc(Size + 1);
    if (!name_buf) return 0;
    memcpy(name_buf, Data, Size);
    name_buf[Size] = '\0';

    int dlt_val = pcap_datalink_name_to_val(name_buf);
    (void)dlt_val; // Use result to avoid unused warnings

    // 2. Test pcap_datalink_val_to_name
    // Use the extracted dlt_val or a fuzzed integer if Size allows
    int fuzzed_dlt = -1;
    if (Size >= 4) {
        fuzzed_dlt = *(const int *)Data;
    }
    const char *dlt_name = pcap_datalink_val_to_name(fuzzed_dlt);
    (void)dlt_name;

    // 3. Test pcap_datalink_val_to_description_or_dlt
    const char *dlt_desc = pcap_datalink_val_to_description_or_dlt(fuzzed_dlt);
    (void)dlt_desc;

    // 4. Test pcap_tstamp_type_val_to_name
    int tstamp_type = -1;
    if (Size >= 8) {
        tstamp_type = *(const int *)(Data + 4);
    }
    const char *tstamp_name = pcap_tstamp_type_val_to_name(tstamp_type);
    (void)tstamp_name;

    // 5. Test pcap_list_datalinks and pcap_set_datalink
    // These require a pcap_t handle. We create a dummy savefile to open.
    const char *dummy_file = "./dummy_file";
    create_dummy_savefile(dummy_file);

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(dummy_file, errbuf);

    if (handle) {
        // Try to list datalinks
        int *dlt_list = NULL;
        int count = pcap_list_datalinks(handle, &dlt_list);
        if (count > 0 && dlt_list != NULL) {
            // Try to set datalink using values from the list or fuzzed input
            int target_dlt = fuzzed_dlt;
            if (count > 0 && fuzzed_dlt < 0) {
                target_dlt = dlt_list[0];
            }
            pcap_set_datalink(handle, target_dlt);
            free(dlt_list);
        }

        pcap_close(handle);
    }

    // Cleanup
    free(name_buf);
    return 0;
}