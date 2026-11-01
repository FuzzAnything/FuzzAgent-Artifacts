// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_open_offline at savefile.c:388:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink_val_to_description at pcap.c:3376:1 in pcap.h
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink at pcap.c:2938:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f != NULL) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

static pcap_t *create_test_handle(const uint8_t *Data, size_t Size) {
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Try to open a loopback device or a dummy file
    // First attempt: loopback (lo)
    handle = pcap_create("lo", errbuf);
    if (handle == NULL) {
        // Second attempt: open the dummy file if it exists and has data
        if (Size > 0) {
            write_dummy_file(Data, Size);
            handle = pcap_open_offline("./dummy_file", errbuf);
        } else {
            return NULL;
        }
    }

    if (handle != NULL) {
        // Try to activate the handle
        if (pcap_activate(handle) != 0) {
            pcap_close(handle);
            handle = NULL;
        }
    }

    return handle;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for string operations
    char *input_str = malloc(Size + 1);
    if (input_str == NULL) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Fuzz pcap_datalink_name_to_val
    // This function takes a string and returns an int.
    int dlt_val = pcap_datalink_name_to_val(input_str);
    (void)dlt_val; // Suppress unused variable warning

    // 2. Fuzz pcap_datalink_val_to_description
    // Convert the result from above, or fuzz with raw data interpreted as int
    int fuzz_dlt = 0;
    if (Size >= sizeof(int)) {
        memcpy(&fuzz_dlt, Data, sizeof(int));
    }
    const char *desc = pcap_datalink_val_to_description(fuzz_dlt);
    (void)desc;

    // 3. Fuzz pcap_datalink_val_to_description_or_dlt
    const char *desc_or_dlt = pcap_datalink_val_to_description_or_dlt(fuzz_dlt);
    (void)desc_or_dlt;

    // 4. Fuzz pcap_datalink_val_to_name
    const char *name = pcap_datalink_val_to_name(fuzz_dlt);
    (void)name;

    // 5. Fuzz pcap_datalink and pcap_set_datalink
    // These require a valid, activated pcap_t handle.
    pcap_t *handle = create_test_handle(Data, Size);
    
    if (handle != NULL) {
        // Test pcap_datalink
        int current_dlt = pcap_datalink(handle);
        (void)current_dlt;

        // Test pcap_set_datalink
        // Try setting to the fuzzed value
        int set_result = pcap_set_datalink(handle, fuzz_dlt);
        (void)set_result;

        // Try setting to the value obtained from name_to_val
        int set_result2 = pcap_set_datalink(handle, dlt_val);
        (void)set_result2;

        // Cleanup handle
        pcap_close(handle);
    }

    free(input_str);
    return 0;
}