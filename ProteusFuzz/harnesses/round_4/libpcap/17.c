// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_setdirection at pcap.c:3819:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
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

static void dummy_file_setup(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write a minimal pcap header (24 bytes) to allow pcap_open_offline
        // Magic number: 0xa1b2c3d4 (little endian)
        uint32_t magic = 0xa1b2c3d4;
        // Version major: 2, minor: 4
        uint32_t version = 0x0402;
        // Zone, sigfigs, snaplen, network (DLT_EN10MB = 1)
        uint32_t zone = 0;
        uint32_t sigfigs = 0;
        uint32_t snaplen = 65535;
        uint32_t network = 1;

        fwrite(&magic, sizeof(uint32_t), 1, f);
        fwrite(&version, sizeof(uint32_t), 1, f);
        fwrite(&zone, sizeof(uint32_t), 1, f);
        fwrite(&sigfigs, sizeof(uint32_t), 1, f);
        fwrite(&snaplen, sizeof(uint32_t), 1, f);
        fwrite(&network, sizeof(uint32_t), 1, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle = NULL;
    int *datalink_list = NULL;
    int ret;

    // Ensure dummy file exists for pcap_open_offline
    dummy_file_setup();

    // Attempt to open the dummy file
    handle = pcap_open_offline("./dummy_file", errbuf);
    if (handle == NULL) {
        // If we can't open the file, try to create a live handle on "lo" or similar
        // This might fail on some systems, but it's a valid attempt for fuzzing
        handle = pcap_create("lo", errbuf);
        if (handle != NULL) {
            // Try to activate it
            ret = pcap_activate(handle);
            if (ret != 0) {
                pcap_close(handle);
                handle = NULL;
            }
        }
    }

    if (handle == NULL) {
        // If we have no handle, we can still test pcap_datalink_name_to_val
        if (Size >= 1) {
            // Create a null-terminated string from input
            char *str = (char *)malloc(Size + 1);
            if (str) {
                memcpy(str, Data, Size);
                str[Size] = '\0';
                pcap_datalink_name_to_val(str);
                free(str);
            }
        }
        return 0;
    }

    // Test pcap_datalink
    int dlt = pcap_datalink(handle);
    (void)dlt; // Suppress unused variable warning

    // Test pcap_list_datalinks
    ret = pcap_list_datalinks(handle, &datalink_list);
    if (ret > 0 && datalink_list != NULL) {
        // We have a list, try to set one of them
        for (int i = 0; i < ret; i++) {
            // Use modulo to pick a random DLT from the list based on input data
            int dlt_to_set = datalink_list[i];
            ret = pcap_set_datalink(handle, dlt_to_set);
            (void)ret;
        }
        // Free the list
        pcap_free_datalinks(datalink_list);
        datalink_list = NULL;
    }

    // Test pcap_setdirection with various values derived from input
    // pcap_direction_t is typically an enum: PCAP_D_IN, PCAP_D_OUT, PCAP_D_INOUT
    // Values are usually 1, 2, 3. Let's try a few variations.
    pcap_direction_t directions[] = { PCAP_D_IN, PCAP_D_OUT, PCAP_D_INOUT };
    for (int i = 0; i < 3; i++) {
        ret = pcap_setdirection(handle, directions[i]);
        (void)ret;
    }

    // Try to set direction with a value derived from input (might be invalid)
    if (Size > 0) {
        pcap_direction_t dir = (pcap_direction_t)(Data[0] % 4); // 0 to 3
        ret = pcap_setdirection(handle, dir);
        (void)ret;
    }

    // Cleanup
    if (datalink_list != NULL) {
        pcap_free_datalinks(datalink_list);
    }
    pcap_close(handle);

    return 0;
}