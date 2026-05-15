// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_create at pcap.c:2242:1 in pcap.h
// pcap_set_datalink at pcap.c:3004:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_datalink_val_to_name at pcap.c:3364:1 in pcap.h
// pcap_datalink_name_to_val at pcap.c:3352:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_list_datalinks at pcap.c:2954:1 in pcap.h
// pcap_free_datalinks at pcap.c:2998:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead at pcap.c:4539:1 in pcap.h
// pcap_activate at pcap.c:2695:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_list_tstamp_types at pcap.c:492:1 in pcap.h
// pcap_free_tstamp_types at pcap.c:534:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pcap/pcap.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void test_pcap_datalink_val_to_name(const uint8_t *data, size_t size) {
    int dlt_val;
    if (size >= sizeof(int)) {
        memcpy(&dlt_val, data, sizeof(int));
        const char *name = pcap_datalink_val_to_name(dlt_val);
        (void)name; // Use result to avoid unused variable warning
    }
}

static void test_pcap_datalink_name_to_val(const uint8_t *data, size_t size) {
    if (size > 0) {
        // Create null-terminated string from fuzzer data
        char *name = malloc(size + 1);
        if (name) {
            memcpy(name, data, size);
            name[size] = '\0';
            int val = pcap_datalink_name_to_val(name);
            (void)val; // Use result
            free(name);
        }
    }
}

static void test_pcap_list_and_free_datalinks(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap;
    
    // Create a dead pcap handle for testing
    pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap == NULL) {
        return;
    }
    
    // Activate the handle (dead handle should already be activated, but be safe)
    if (pcap_activate(pcap) < 0) {
        pcap_close(pcap);
        return;
    }
    
    int *dlt_buffer = NULL;
    int count = pcap_list_datalinks(pcap, &dlt_buffer);
    
    if (count > 0 && dlt_buffer != NULL) {
        // Use fuzzer data to potentially modify the buffer before freeing
        if (size > 0 && count > 0) {
            size_t idx = data[0] % count;
            dlt_buffer[idx] = dlt_buffer[idx]; // No-op, but accesses buffer
        }
        pcap_free_datalinks(dlt_buffer);
    }
    
    pcap_close(pcap);
}

static void test_pcap_free_tstamp_types(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap;
    
    // Create a dead pcap handle for testing
    pcap = pcap_open_dead(DLT_EN10MB, 65535);
    if (pcap == NULL) {
        return;
    }
    
    // Activate the handle
    if (pcap_activate(pcap) < 0) {
        pcap_close(pcap);
        return;
    }
    
    int *tstamp_types = NULL;
    int count = pcap_list_tstamp_types(pcap, &tstamp_types);
    
    if (count > 0 && tstamp_types != NULL) {
        // Use fuzzer data to potentially modify the buffer before freeing
        if (size > 0 && count > 0) {
            size_t idx = data[0] % count;
            tstamp_types[idx] = tstamp_types[idx]; // No-op, but accesses buffer
        }
        pcap_free_tstamp_types(tstamp_types);
    }
    
    pcap_close(pcap);
}

static void test_pcap_set_datalink(const uint8_t *data, size_t size) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *pcap;
    
    // Create a non-activated pcap handle
    pcap = pcap_create("lo", errbuf);
    if (pcap == NULL) {
        // Try with dummy interface name
        pcap = pcap_create("any", errbuf);
        if (pcap == NULL) {
            return;
        }
    }
    
    // Use fuzzer data to determine DLT value
    int dlt_val = DLT_EN10MB; // Default
    if (size >= sizeof(int)) {
        memcpy(&dlt_val, data, sizeof(int));
    }
    
    // Try to set datalink type
    int result = pcap_set_datalink(pcap, dlt_val);
    (void)result; // Use result
    
    // Clean up without activating
    pcap_close(pcap);
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Use first byte to select which function(s) to test
    uint8_t selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size > 1 ? Size - 1 : 0;
    
    // Test all functions in different combinations based on selector
    if (selector & 0x01) {
        test_pcap_datalink_val_to_name(remaining_data, remaining_size);
    }
    
    if (selector & 0x02) {
        test_pcap_datalink_name_to_val(remaining_data, remaining_size);
    }
    
    if (selector & 0x04) {
        test_pcap_list_and_free_datalinks(remaining_data, remaining_size);
    }
    
    if (selector & 0x08) {
        test_pcap_free_tstamp_types(remaining_data, remaining_size);
    }
    
    if (selector & 0x10) {
        test_pcap_set_datalink(remaining_data, remaining_size);
    }
    
    // Also test some combinations of functions
    if ((selector & 0x20) && remaining_size > 0) {
        // Test datalink value/name conversion round trip
        int dlt_val;
        if (remaining_size >= sizeof(int)) {
            memcpy(&dlt_val, remaining_data, sizeof(int));
            const char *name = pcap_datalink_val_to_name(dlt_val);
            if (name != NULL) {
                int converted_val = pcap_datalink_name_to_val(name);
                (void)converted_val; // Use result
            }
        }
    }
    
    return 0;
}