// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper to create a cJSON object from fuzz input */
static cJSON *create_cjson_from_input(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateString("");
    }
    
    /* Ensure null termination for string creation */
    char *str = (char *)malloc(size + 1);
    if (!str) return NULL;
    
    memcpy(str, data, size);
    str[size] = '\0';
    
    cJSON *item = cJSON_CreateString(str);
    free(str);
    
    if (!item) {
        /* Fallback to creating a number if string creation fails */
        if (size >= sizeof(double)) {
            double val;
            memcpy(&val, data, sizeof(double));
            item = cJSON_CreateNumber(val);
        } else {
            item = cJSON_CreateNumber(0.0);
        }
    }
    
    return item;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a base cJSON object from input */
    cJSON *item = create_cjson_from_input(Data, Size);
    if (!item) {
        return 0;
    }

    /* 1. Test cJSON_PrintUnformatted */
    char *print_unformatted = cJSON_PrintUnformatted(item);
    if (print_unformatted) {
        free(print_unformatted);
    }

    /* 2. Test cJSON_Print */
    char *print_formatted = cJSON_Print(item);
    if (print_formatted) {
        free(print_formatted);
    }

    /* 3. Test cJSON_PrintBuffered */
    /* Try different prebuffer sizes */
    int prebuffer_sizes[] = {0, 10, 100, (int)Size, (int)Size * 2, -1};
    int num_sizes = sizeof(prebuffer_sizes) / sizeof(prebuffer_sizes[0]);
    
    for (int i = 0; i < num_sizes; i++) {
        int prebuffer = prebuffer_sizes[i];
        cJSON_bool fmt = (i % 2); /* Alternate between formatted and unformatted */
        
        char *print_buffered = cJSON_PrintBuffered(item, prebuffer, fmt);
        if (print_buffered) {
            free(print_buffered);
        }
    }

    /* 4. Test cJSON_PrintPreallocated */
    /* Allocate a buffer large enough to hold the output */
    size_t buffer_size = Size * 2 + 100;
    if (buffer_size > 1024 * 1024) buffer_size = 1024 * 1024; /* Cap size */
    
    char *preallocated_buffer = (char *)malloc(buffer_size);
    if (preallocated_buffer) {
        cJSON_bool success = cJSON_PrintPreallocated(item, preallocated_buffer, (int)buffer_size, 0);
        (void)success; /* Result handled internally */
        
        /* Test with formatted output */
        success = cJSON_PrintPreallocated(item, preallocated_buffer, (int)buffer_size, 1);
        (void)success;
        
        /* Test with small buffer to trigger failure */
        success = cJSON_PrintPreallocated(item, preallocated_buffer, 1, 0);
        (void)success;
        
        free(preallocated_buffer);
    }

    /* 5. Test cJSON_Minify */
    /* Create a copy of the formatted output to minify */
    char *formatted_copy = cJSON_Print(item);
    if (formatted_copy) {
        cJSON_Minify(formatted_copy);
        free(formatted_copy);
    }

    /* Cleanup cJSON object */
    cJSON_Delete(item);

    return 0;
}