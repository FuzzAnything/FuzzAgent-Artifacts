// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
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
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    
    // Write a minimal pcap global header (24 bytes)
    // Magic number (little endian)
    uint32_t magic = 0xa1b2c3d4;
    // Version major
    uint16_t version_major = 2;
    // Version minor
    uint16_t version_minor = 4;
    // Timezone offset
    int32_t tz = 0;
    // Timestamp accuracy
    uint32_t sigfigs = 0;
    // Snaplen
    uint32_t snaplen = 65535;
    // Network type (DLT_EN10MB)
    uint32_t network = 1;
    
    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&version_major, sizeof(version_major), 1, f);
    fwrite(&version_minor, sizeof(version_minor), 1, f);
    fwrite(&tz, sizeof(tz), 1, f);
    fwrite(&sigfigs, sizeof(sigfigs), 1, f);
    fwrite(&snaplen, sizeof(snaplen), 1, f);
    fwrite(&network, sizeof(network), 1, f);
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy savefile for pcap_open_offline
    const char *filename = "./dummy_file";
    create_dummy_savefile(filename);

    // 1. Test pcap_datalink_name_to_val with fuzzed string
    // Ensure null termination
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) return 0;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    int dlt_val = pcap_datalink_name_to_val(input_str);
    // dlt_val is int, no cleanup needed for this function
    free(input_str);

    // 2. Test pcap_open_offline to get a handle for other functions
    pcap_t *handle = pcap_open_offline(filename, NULL);
    if (!handle) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];

    // 3. Test pcap_activate (required for pcap_datalink, pcap_list_datalinks, pcap_set_datalink)
    int ret = pcap_activate(handle);
    if (ret != 0) {
        // If activation fails, we can't test functions requiring active handle
        pcap_close(handle);
        return 0;
    }

    // 4. Test pcap_datalink
    int current_dlt = pcap_datalink(handle);
    (void)current_dlt; // Use the return value to suppress warnings

    // 5. Test pcap_list_datalinks
    int *datalink_list = NULL;
    int count = pcap_list_datalinks(handle, &datalink_list);
    if (count > 0 && datalink_list != NULL) {
        // Try to set datalink to one of the listed ones
        for (int i = 0; i < count; i++) {
            if (datalink_list[i] != -1) {
                int set_ret = pcap_set_datalink(handle, datalink_list[i]);
                (void)set_ret;
                break; // Try one to avoid excessive loop in fuzzing
            }
        }
        // Cleanup datalink list
        pcap_free_datalinks(datalink_list);
    }

    // 6. Test pcap_set_datalink with a fuzzed value (derived from input)
    int fuzzed_dlt = (int)(Size % 1000) - 500; // Generate a range of values
    int set_ret = pcap_set_datalink(handle, fuzzed_dlt);
    (void)set_ret;

    // 7. Test pcap_free_tstamp_types
    // We need a valid list first. pcap_list_tstamp_types is not in the target list,
    // but we can try to free NULL or a dummy pointer to test robustness if the API allows.
    // The spec says passing NULL causes undefined behavior, so we avoid it unless we have a list.
    // Since we don't have pcap_list_tstamp_types in the target list, we skip this specific test
    // unless we can generate a list. However, to be safe and follow the "explore states" rule:
    // We will not call pcap_free_tstamp_types with NULL as per spec warning.
    // If we had pcap_list_tstamp_types, we would use it. Since we don't, we skip this specific call
    // to avoid undefined behavior, or we could try to call it with a dummy pointer if we want to crash (not recommended for stable fuzzing).
    // Let's assume we don't have a valid list from the target functions, so we skip pcap_free_tstamp_types.
    // However, to ensure we test the function if possible, we could try to allocate a dummy list?
    // No, the function expects a list returned by pcap_list_tstamp_types.
    // We will skip calling pcap_free_tstamp_types to avoid UB as per instructions.

    // 8. Cleanup
    pcap_close(handle);

    return 0;
}