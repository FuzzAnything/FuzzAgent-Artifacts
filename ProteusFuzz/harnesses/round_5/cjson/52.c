// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_CreateNull at cJSON.c:2461:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddStringToObject at cJSON.c:2210:22 in cJSON.h
// cJSON_AddNumberToObject at cJSON.c:2198:22 in cJSON.h
// cJSON_AddBoolToObject at cJSON.c:2186:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Helper function to create a simple cJSON object from raw data for testing */
static cJSON *create_test_object(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateNull();
    }

    /* Create a string from the input data, ensuring null termination */
    char *str = (char *)malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';

    cJSON *root = cJSON_CreateObject();
    if (!root) {
        free(str);
        return NULL;
    }

    cJSON_AddStringToObject(root, "input", str);
    free(str);

    /* Add a number based on first byte */
    cJSON_AddNumberToObject(root, "seed", (double)data[0]);

    /* Add a boolean based on second byte */
    cJSON_AddBoolToObject(root, "flag", (cJSON_bool)(size > 1 ? (data[1] & 1) : 0));

    return root;
}

/* Helper function to cleanup cJSON */
static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 1. Prepare environment */
    cJSON *item = create_test_object(Data, Size);
    if (!item) {
        return 0;
    }

    char *output = NULL;
    char *buffered_output = NULL;
    char *prealloc_buffer = NULL;
    int prealloc_size = 0;

    /* 2. Test cJSON_Version */
    const char *version = cJSON_Version();
    if (version) {
        /* Just access to ensure it doesn't crash, version is static */
        (void)version[0];
    }

    /* 3. Test cJSON_PrintUnformatted */
    output = cJSON_PrintUnformatted(item);
    if (output) {
        free(output);
        output = NULL;
    }

    /* 4. Test cJSON_PrintBuffered */
    /* Use a guess based on input size */
    int prebuffer_guess = Size * 2 + 100;
    if (prebuffer_guess < 0) prebuffer_guess = 100;
    
    buffered_output = cJSON_PrintBuffered(item, prebuffer_guess, 0);
    if (buffered_output) {
        free(buffered_output);
        buffered_output = NULL;
    }

    /* Test with formatted flag */
    buffered_output = cJSON_PrintBuffered(item, prebuffer_guess, 1);
    if (buffered_output) {
        free(buffered_output);
        buffered_output = NULL;
    }

    /* 5. Test cJSON_Print */
    output = cJSON_Print(item);
    if (output) {
        /* Test cJSON_Minify on the output string */
        /* cJSON_Minify requires a writable buffer, output is malloc'd so it's safe */
        cJSON_Minify(output);
        free(output);
        output = NULL;
    }

    /* 6. Test cJSON_PrintPreallocated */
    /* Estimate size needed. cJSON_PrintUnformatted gives a good hint if we had it, 
       but we can just use a generous buffer based on input size */
    prealloc_size = Size * 4 + 256;
    if (prealloc_size < 0) prealloc_size = 256;
    
    prealloc_buffer = (char *)malloc(prealloc_size);
    if (prealloc_buffer) {
        memset(prealloc_buffer, 0, prealloc_size);
        cJSON_bool res = cJSON_PrintPreallocated(item, prealloc_buffer, prealloc_size, 0);
        (void)res; /* Ignore result, just testing for crashes */
        
        /* Test with format flag */
        res = cJSON_PrintPreallocated(item, prealloc_buffer, prealloc_size, 1);
        (void)res;
        
        free(prealloc_buffer);
        prealloc_buffer = NULL;
    }

    /* 7. Test cJSON_Minify on a static-like string (created dynamically to be writable) */
    char *minify_test = NULL;
    if (output || buffered_output) {
        /* Re-generate a string to minify since we freed the previous ones */
        char *temp = cJSON_Print(item);
        if (temp) {
            minify_test = strdup(temp);
            free(temp);
        }
    }
    
    if (!minify_test && Size > 0) {
        /* Fallback: create a simple JSON string from input */
        minify_test = (char *)malloc(Size + 10);
        if (minify_test) {
            snprintf(minify_test, Size + 10, "{\"test\":\"%.*s\"}", (int)Size, Data);
        }
    }

    if (minify_test) {
        cJSON_Minify(minify_test);
        free(minify_test);
        minify_test = NULL;
    }

    /* 8. Cleanup */
    cleanup_cjson(item);

    return 0;
}