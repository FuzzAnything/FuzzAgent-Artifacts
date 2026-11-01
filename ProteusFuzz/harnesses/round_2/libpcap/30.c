// This fuzz driver is generated for library libpcap, aiming to fuzz the following functions:
// pcap_get_tstamp_precision at pcap.c:2689:1 in pcap.h
// pcap_list_tstamp_types at pcap.c:492:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_set_tstamp_type at pcap.c:2571:1 in pcap.h
// pcap_free_tstamp_types at pcap.c:534:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
// pcap_close at pcap.c:4177:1 in pcap.h
// pcap_open_dead_with_tstamp_precision at pcap.c:4477:1 in pcap.h
// pcap_set_tstamp_precision at pcap.c:2640:1 in pcap.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*
 * Fuzz driver for libpcap timestamp-related APIs.
 * Targets:
 *  - pcap_set_tstamp_precision
 *  - pcap_get_tstamp_precision
 *  - pcap_list_tstamp_types
 *  - pcap_open_dead_with_tstamp_precision
 *  - pcap_free_tstamp_types
 *  - pcap_set_tstamp_type
 */

/*
 * Helper to extract an integer from the fuzzer input.
 * Returns a value in the range [min_val, max_val] if possible,
 * otherwise returns min_val.
 */
static int extract_int(const uint8_t *data, size_t size, size_t *offset, int min_val, int max_val) {
    if (*offset + 4 > size) {
        return min_val;
    }
    int32_t val;
    memcpy(&val, data + *offset, sizeof(int32_t));
    *offset += 4;
    
    if (val < min_val) return min_val;
    if (val > max_val) return max_val;
    return (int)val;
}

/*
 * Helper to extract a linktype from the fuzzer input.
 * Common linktypes are in the range 0-2000, but we allow a wider range for fuzzing.
 */
static int extract_linktype(const uint8_t *data, size_t size, size_t *offset) {
    return extract_int(data, size, offset, 0, 65535);
}

/*
 * Helper to extract a timestamp precision value.
 * Valid values are typically PCAP_TSTAMP_PRECISION_MICRO (0) and 
 * PCAP_TSTAMP_PRECISION_NANO (1). We fuzz with a wider range to test error handling.
 */
static int extract_tstamp_precision(const uint8_t *data, size_t size, size_t *offset) {
    return extract_int(data, size, offset, -100, 100);
}

/*
 * Helper to extract a timestamp type value.
 * Valid values include PCAP_TSTAMP_HOST, PCAP_TSTAMP_HOST_PRECISE, etc.
 */
static int extract_tstamp_type(const uint8_t *data, size_t size, size_t *offset) {
    return extract_int(data, size, offset, -100, 100);
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    size_t offset = 0;
    int linktype = extract_linktype(Data, Size, &offset);
    int tstamp_precision = extract_tstamp_precision(Data, Size, &offset);
    int tstamp_type = extract_tstamp_type(Data, Size, &offset);
    
    /* 
     * Create a dead handle using pcap_open_dead_with_tstamp_precision.
     * This is safe for fuzzing as it doesn't require a live network interface.
     */
    pcap_t *handle = pcap_open_dead_with_tstamp_precision(linktype, tstamp_precision, 65535);
    
    if (handle == NULL) {
        /* If creation fails, we can't test other APIs on this handle.
         * This is acceptable behavior for fuzzing.
         */
        return 0;
    }

    /*
     * Test pcap_set_tstamp_precision
     * Note: This must be called before activation. Since pcap_open_dead_with_tstamp_precision
     * creates a handle that is not activated, we can call this.
     */
    int new_precision = extract_tstamp_precision(Data, Size, &offset);
    (void)pcap_set_tstamp_precision(handle, new_precision);

    /*
     * Test pcap_get_tstamp_precision
     */
    int current_precision = pcap_get_tstamp_precision(handle);
    (void)current_precision;

    /*
     * Test pcap_list_tstamp_types
     * This allocates an array that must be freed.
     */
    int *tstamp_types = NULL;
    int count = pcap_list_tstamp_types(handle, &tstamp_types);
    
    if (count > 0 && tstamp_types != NULL) {
        /*
         * Test pcap_set_tstamp_type with the first type in the list
         * and also with the fuzzed type.
         */
        if (count > 0) {
            (void)pcap_set_tstamp_type(handle, tstamp_types[0]);
        }
        (void)pcap_set_tstamp_type(handle, tstamp_type);
        
        /*
         * Free the allocated array
         */
        pcap_free_tstamp_types(tstamp_types);
    } else if (count < 0) {
        /* Error case, e.g., malloc failure or no types supported */
        /* Nothing to free */
    }

    /*
     * Attempt to call pcap_set_tstamp_precision again after potential activation
     * (though pcap_open_dead doesn't activate, we simulate the call to test robustness)
     */
    (void)pcap_set_tstamp_precision(handle, new_precision);

    /*
     * Cleanup: Close the handle
     */
    pcap_close(handle);

    return 0;
}