#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <ares.h>
#include <ares_dns_record.h>

/* 
 * Fuzz driver for ares_dns_rr_key_datatype
 * Target: /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/sources/c-ares/src/lib/record/ares_dns_mapping.c
 * Function: ares_dns_rr_key_datatype
 * Uncovered Lines: 453-454, 457, 460-471, 474-479, 482-489, 492-506, 509-515, 518, 521, 524-526, 529-531
 */

/* Helper to safely read an integer from the buffer to use as a key */
static int read_key_from_input(const uint8_t *data, size_t size, ares_dns_rr_key_t *key_out) {
    if (size < sizeof(int32_t)) {
        return 0;
    }
    
    /* Read 4 bytes as a signed integer to cover the enum range */
    int32_t val;
    memcpy(&val, data, sizeof(int32_t));
    
    /* Cast to the enum type */
    *key_out = (ares_dns_rr_key_t)val;
    return 1;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *data, size_t size) {
    /* Initialize c-ares library to ensure valid global state */
    if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
        return 0;
    }

    if (size == 0) {
        ares_library_cleanup();
        return 0;
    }

    /* 
     * Strategy: 
     * The target function is a switch statement on an enum. 
     * To cover all cases, we need to pass every possible enum value.
     * We will parse the input buffer as a stream of integers (keys) and call the function.
     * This allows the fuzzer to mutate the integer values to hit specific enum constants.
     */
    
    size_t offset = 0;
    while (offset + sizeof(int32_t) <= size) {
        ares_dns_rr_key_t key;
        
        if (!read_key_from_input(data + offset, size - offset, &key)) {
            break;
        }

        /* Invoke the target function */
        ares_dns_datatype_t result = ares_dns_rr_key_datatype(key);
        
        /* 
         * The result is an enum. We don't need to do anything with it, 
         * but using it prevents compiler warnings about unused variables 
         * and ensures the call is optimized out only if truly unused.
         */
        (void)result;

        /* Advance by 4 bytes to try the next potential key */
        offset += sizeof(int32_t);
    }

    /* 
     * Also try single byte values to ensure we hit small enum values efficiently
     * and potentially trigger edge cases if the enum has small constants.
     */
    for (size_t i = 0; i < size && i < 256; i++) {
        ares_dns_rr_key_t key = (ares_dns_rr_key_t)data[i];
        ares_dns_datatype_t result = ares_dns_rr_key_datatype(key);
        (void)result;
    }

    /* Cleanup c-ares library */
    ares_library_cleanup();

    return 0;
}