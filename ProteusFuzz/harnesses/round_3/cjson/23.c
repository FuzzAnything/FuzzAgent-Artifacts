// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_InitHooks at cJSON.c:209:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

static void *custom_malloc(size_t sz) {
    (void)sz;
    return NULL;
}

static void custom_free(void *ptr) {
    (void)ptr;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for parsing
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Test cJSON_Parse
    cJSON *root = cJSON_Parse(input_str);
    if (root) {
        // Test cJSON_PrintUnformatted
        char *unformatted = cJSON_PrintUnformatted(root);
        if (unformatted) {
            free(unformatted);
        }

        // Test cJSON_PrintBuffered
        char *buffered = cJSON_PrintBuffered(root, 256, 0);
        if (buffered) {
            free(buffered);
        }

        // Test cJSON_PrintBuffered with format
        buffered = cJSON_PrintBuffered(root, 256, 1);
        if (buffered) {
            free(buffered);
        }

        // Test cJSON_PrintPreallocated
        char prealloc_buf[1024];
        cJSON_PrintPreallocated(root, prealloc_buf, sizeof(prealloc_buf), 0);
        cJSON_PrintPreallocated(root, prealloc_buf, sizeof(prealloc_buf), 1);

        // Test cJSON_Minify on a copy of the unformatted string
        // We need to re-print to get a fresh string for minification test if we want to test minify logic
        // However, since unformatted is freed above, we create a new one for minify test
        char *unformatted_for_minify = cJSON_PrintUnformatted(root);
        if (unformatted_for_minify) {
            char *minify_buf = strdup(unformatted_for_minify);
            if (minify_buf) {
                cJSON_Minify(minify_buf);
                free(minify_buf);
            }
            free(unformatted_for_minify);
        }

        cJSON_Delete(root);
    }

    // Test cJSON_InitHooks with custom hooks
    cJSON_Hooks hooks;
    hooks.malloc_fn = malloc;
    hooks.free_fn = free;
    cJSON_InitHooks(&hooks);

    // Test cJSON_InitHooks with NULL to reset
    cJSON_InitHooks(NULL);

    // Test cJSON_InitHooks with partial hooks (should set realloc to NULL internally)
    cJSON_Hooks partial_hooks;
    partial_hooks.malloc_fn = custom_malloc;
    partial_hooks.free_fn = custom_free;
    cJSON_InitHooks(&partial_hooks);

    // Reset hooks again
    cJSON_InitHooks(NULL);

    // Test cJSON_Parse with invalid JSON (already done via input_str, but explicit check)
    cJSON *invalid_root = cJSON_Parse(input_str);
    if (invalid_root) {
        cJSON_Delete(invalid_root);
    }

    // Re-parse to test print functions with NULL buffer or small buffer
    root = cJSON_Parse(input_str);
    if (root) {
        // Test cJSON_PrintPreallocated with NULL buffer
        cJSON_PrintPreallocated(root, NULL, 100, 0);

        // Test cJSON_PrintPreallocated with small buffer
        char small_buf[10];
        cJSON_PrintPreallocated(root, small_buf, sizeof(small_buf), 0);

        cJSON_Delete(root);
    }

    // Test cJSON_Minify with NULL
    cJSON_Minify(NULL);

    // Test cJSON_Minify with valid string
    char minify_test[] = "  { \"key\" : \"value\" }  ";
    cJSON_Minify(minify_test);

    free(input_str);
    return 0;
}