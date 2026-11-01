// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_tstamp_type_name_to_val at pcap.c:3419:1 in pcap.h
// pcap_tstamp_type_val_to_description at pcap.c:3443:1 in pcap.h
// pcap_tstamp_type_val_to_name at pcap.c:3431:1 in pcap.h
// pcap_free_tstamp_types at pcap.c:534:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Fuzz pcap_tstamp_type_name_to_val
    // Data is treated as a string for this function
    // We ensure it's null-terminated by creating a buffer
    char *name_buf = (char *)malloc(Size + 1);
    if (name_buf) {
        memcpy(name_buf, Data, Size);
        name_buf[Size] = '\0';
        int val = pcap_tstamp_type_name_to_val(name_buf);
        (void)val; // Use return value to suppress warnings
        free(name_buf);
    }

    // Fuzz pcap_tstamp_type_val_to_description
    // Extract an integer from the input data
    if (Size >= sizeof(int)) {
        int type_val = 0;
        memcpy(&type_val, Data, sizeof(int));
        const char *desc = pcap_tstamp_type_val_to_description(type_val);
        (void)desc;
    }

    // Fuzz pcap_tstamp_type_val_to_name
    if (Size >= sizeof(int)) {
        int type_val = 0;
        memcpy(&type_val, Data, sizeof(int));
        const char *name = pcap_tstamp_type_val_to_name(type_val);
        (void)name;
    }

    // Fuzz pcap_free_tstamp_types
    // We need to allocate a list that mimics what pcap_list_tstamp_types returns
    // Since we can't easily call pcap_list_tstamp_types without a valid handle,
    // we simulate the allocation. The function expects an array of ints.
    // We allocate a small array to test the free logic.
    int *tstamp_list = NULL;
    if (Size > 0) {
        size_t count = (Size % 10) + 1; // 1 to 10 elements
        tstamp_list = (int *)malloc(count * sizeof(int));
        if (tstamp_list) {
            for (size_t i = 0; i < count; i++) {
                tstamp_list[i] = (int)Data[i % Size];
            }
            pcap_free_tstamp_types(tstamp_list);
            tstamp_list = NULL; // Prevent double free
        }
    }

    // Fuzz pcap_set_tstamp_type and pcap_get_tstamp_precision
    // These require a valid pcap_t handle. We try to create one using a dummy file.
    // We write a minimal pcap header to a dummy file to allow pcap_open_offline to succeed.
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        // Write a minimal pcap header (24 bytes)
        // Magic number, Version major, Version minor, Zone, Sigfigs, Snaplen, Network
        uint32_t pcap_header[7];
        pcap_header[0] = 0xa1b2c3d4;
        pcap_header[1] = 2;
        pcap_header[2] = 4;
        pcap_header[3] = 0;
        pcap_header[4] = 0;
        pcap_header[5] = 65535;
        pcap_header[6] = 1;
        fwrite(pcap_header, sizeof(uint32_t), 7, f);
        fclose(f);

        char errbuf[PCAP_ERRBUF_SIZE];
        pcap_t *handle = pcap_open_offline(dummy_file, errbuf);
        if (handle) {
            // Try to set timestamp type
            if (Size >= sizeof(int)) {
                int tstamp_type = 0;
                memcpy(&tstamp_type, Data, sizeof(int));
                int ret_set = pcap_set_tstamp_type(handle, tstamp_type);
                (void)ret_set;
            }

            // Try to get timestamp precision
            int precision = pcap_get_tstamp_precision(handle);
            (void)precision;

            pcap_close(handle);
        }
        
        // Clean up dummy file
        remove(dummy_file);
    }

    return 0;
}