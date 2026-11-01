#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

/* Helper to safely cleanup cJSON structures */
static void safe_cleanup(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure we have a null-terminated string for input */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 
     * Strategy to hit uncovered lines:
     * 1. Line 441: Test NULL object, non-string type, and reference type.
     * 2. Line 446: Test NULL valuestring argument.
     * 3. Line 457: Test v1_len <= v2_len (shorter input) to reach overlap check.
     * 4. Line 469: Test v1_len > v2_len (longer input) to reach reallocation/free path.
     */

    /* --- Scenario 1: Hit Line 441 (NULL Object) --- */
    cJSON_SetValuestring(NULL, input_str);

    /* --- Scenario 2: Hit Line 441 (Non-String Type) --- */
    cJSON *num_obj = cJSON_CreateNumber(123);
    if (num_obj) {
        cJSON_SetValuestring(num_obj, input_str);
        cJSON_Delete(num_obj);
    }

    /* --- Scenario 3: Hit Line 441 (Reference Type) --- */
    /* Create a string reference which sets cJSON_IsReference flag */
    static char static_ref_str[] = "static_reference";
    cJSON *ref_obj = cJSON_CreateStringReference(static_ref_str);
    if (ref_obj) {
        cJSON_SetValuestring(ref_obj, input_str);
        cJSON_Delete(ref_obj);
    }

    /* --- Scenario 4: Hit Line 446 (NULL valuestring argument) --- */
    cJSON *str_obj = cJSON_CreateString("initial_value");
    if (str_obj) {
        /* Pass NULL as the second argument */
        cJSON_SetValuestring(str_obj, NULL);
        /* Cleanup will happen later */
    }

    /* --- Scenario 5: Hit Line 457 (Overlap check when v1_len <= v2_len) --- */
    /* Create an object with a LONG string so that input_str (potentially short) is <= */
    /* We construct a long string manually to ensure v2_len is large */
    char long_buf[512];
    memset(long_buf, 'A', sizeof(long_buf) - 1);
    long_buf[sizeof(long_buf) - 1] = '\0';
    
    cJSON *long_str_obj = cJSON_CreateString(long_buf);
    if (long_str_obj) {
        /* input_str might be short, triggering v1_len <= v2_len branch */
        cJSON_SetValuestring(long_str_obj, input_str);
        cJSON_Delete(long_str_obj);
    }

    /* --- Scenario 6: Hit Line 469 (Reallocation path when v1_len > v2_len) --- */
    /* Create an object with a SHORT string */
    cJSON *short_str_obj = cJSON_CreateString("a");
    if (short_str_obj) {
        /* If input_str is longer than "a", this triggers the reallocation path */
        /* This hits line 464 (cJSON_strdup), 465 (check copy), and 469 (check old valuestring) */
        cJSON_SetValuestring(short_str_obj, input_str);
        cJSON_Delete(short_str_obj);
    }

    /* Cleanup the main string object created in Scenario 4 */
    if (str_obj) {
        cJSON_Delete(str_obj);
    }

    free(input_str);

    return 0;
}