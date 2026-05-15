// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_datalink_val_to_description at pcap.c:3376:1 in pcap.h
// pcap_datalink_val_to_description_or_dlt at pcap.c:3388:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap/pcap.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void fuzz_datalink_functions(int dlt_val, const char *name) {
    // Test pcap_datalink_val_to_description
    const char *desc = pcap_datalink_val_to_description(dlt_val);
    (void)desc; // Use result to avoid compiler warnings
    
    // Test pcap_datalink_val_to_description_or_dlt
    const char *desc_or_dlt = pcap_datalink_val_to_description_or_dlt(dlt_val);
    (void)desc_or_dlt;
    
    // Test pcap_datalink_val_to_name
    const char *name_result = pcap_datalink_val_to_name(dlt_val);
    (void)name_result;
    
    // Test pcap_datalink_name_to_val
    if (name != NULL && name[0] != '\0') {
        int val = pcap_datalink_name_to_val(name);
        (void)val;
    }
}

static void fuzz_list_datalinks(void) {
    // Create a dummy pcap handle using pcap_open_dead
    pcap_t *pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap == NULL) {
        return;
    }
    
    // Try to activate the handle (not strictly needed for pcap_open_dead but good practice)
    int activate_result = pcap_activate(pcap);
    (void)activate_result;
    
    // Test pcap_list_datalinks
    int *dlt_list = NULL;
    int count = pcap_list_datalinks(pcap, &dlt_list);
    
    // If we got a list, free it
    if (count > 0 && dlt_list != NULL) {
        pcap_free_datalinks(dlt_list);
    }
    
    // Clean up
    pcap_close(pcap);
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Extract DLT value from first few bytes
    int dlt_val = 0;
    if (Size >= sizeof(int)) {
        memcpy(&dlt_val, Data, sizeof(int));
    } else {
        dlt_val = Data[0];
    }
    
    // Create a null-terminated string from the remaining data for name testing
    char *name_buf = NULL;
    if (Size > sizeof(int)) {
        size_t name_len = Size - sizeof(int);
        name_buf = (char *)malloc(name_len + 1);
        if (name_buf != NULL) {
            memcpy(name_buf, Data + sizeof(int), name_len);
            name_buf[name_len] = '\0';
        }
    } else if (Size > 0) {
        // If we don't have enough bytes for int, use all bytes for name
        name_buf = (char *)malloc(Size + 1);
        if (name_buf != NULL) {
            memcpy(name_buf, Data, Size);
            name_buf[Size] = '\0';
        }
    }
    
    // Test datalink conversion functions
    fuzz_datalink_functions(dlt_val, name_buf);
    
    // Test list datalinks function (with some probability to avoid excessive calls)
    if ((dlt_val & 0x3) == 0) { // 25% probability
        fuzz_list_datalinks();
    }
    
    // Clean up
    if (name_buf != NULL) {
        free(name_buf);
    }
    
    return 0;
}