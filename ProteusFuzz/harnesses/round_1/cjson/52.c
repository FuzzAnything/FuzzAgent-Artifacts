#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"

/* Custom allocator functions for testing hooks */
static void *custom_malloc(size_t sz) {
    return malloc(sz);
}

static void custom_free(void *ptr) {
    free(ptr);
}

/* Helper to reset hooks to default state before each test to ensure isolation */
static void reset_hooks_to_default(void) {
    cJSON_InitHooks(NULL);
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Use the first byte to determine the test scenario */
    uint8_t scenario = Data[0] % 5;

    /* Reset hooks before each test to ensure a clean state */
    reset_hooks_to_default();

    cJSON_Hooks hooks = {0};

    switch (scenario) {
        case 0:
            /* Scenario 0: Pass NULL */
            /* Covers: Line 211 (if hooks == NULL), Line 234 (condition true after reset) */
            cJSON_InitHooks(NULL);
            break;

        case 1:
            /* Scenario 1: malloc_fn set, free_fn NULL */
            /* Covers: Line 221 (if malloc_fn != NULL) */
            /* Line 234 will be false because allocate != malloc */
            hooks.malloc_fn = custom_malloc;
            hooks.free_fn = NULL;
            cJSON_InitHooks(&hooks);
            break;

        case 2:
            /* Scenario 2: malloc_fn NULL, free_fn set */
            /* Covers: Line 227 (if free_fn != NULL) */
            /* Line 234 will be false because deallocate != free */
            hooks.malloc_fn = NULL;
            hooks.free_fn = custom_free;
            cJSON_InitHooks(&hooks);
            break;

        case 3:
            /* Scenario 3: Both set */
            /* Covers: Line 221, Line 227 */
            /* Line 234 will be false because both are custom */
            hooks.malloc_fn = custom_malloc;
            hooks.free_fn = custom_free;
            cJSON_InitHooks(&hooks);
            break;

        case 4:
            /* Scenario 4: Both NULL */
            /* Covers: Line 221 (false), Line 227 (false), Line 234 (true) */
            /* This specifically targets the path where defaults are preserved */
            hooks.malloc_fn = NULL;
            hooks.free_fn = NULL;
            cJSON_InitHooks(&hooks);
            break;
    }

    return 0;
}