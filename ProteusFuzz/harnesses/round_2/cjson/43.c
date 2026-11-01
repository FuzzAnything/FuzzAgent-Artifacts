// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateString at cJSON.c:2531:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_CreateObject at cJSON.c:2609:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
// cJSON_CreateArray at cJSON.c:2598:23 in cJSON.h
// cJSON_CreateNumber at cJSON.c:2505:23 in cJSON.h
// cJSON_AddItemToArray at cJSON.c:2061:26 in cJSON.h
// cJSON_AddItemToObject at cJSON.c:2119:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>

/* Helper to create a cJSON object from raw bytes safely */
static cJSON *create_cjson_from_input_safe(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }

    /* 
       cJSON_CreateString expects a null-terminated string. 
       Fuzz input may contain embedded nulls or no null terminator.
       To avoid heap-buffer-overflow in strlen inside cJSON_CreateString,
       we must ensure the string passed is null-terminated and within bounds.
       
       Strategy: 
       1. If the input already contains a null byte, truncate there.
       2. If not, create a temporary buffer with a null terminator.
    */
    
    const char *start = (const char *)data;
    size_t len = size;
    
    /* Find the first null byte */
    const char *null_pos = memchr(data, 0, size);
    if (null_pos != NULL) {
        len = null_pos - start;
    }

    if (len == 0) {
        /* Empty string or only nulls */
        return cJSON_CreateString("");
    }

    /* 
       If the input doesn't have a null terminator, we need to create a safe copy.
       However, cJSON_CreateString will call strlen. If we pass a pointer to 
       non-null-terminated memory, strlen will read past the buffer.
       So we MUST ensure the string is null-terminated before passing to cJSON_CreateString.
    */
    if (null_pos == NULL) {
        /* No null terminator found in the input buffer */
        char *safe_str = (char *)malloc(len + 1);
        if (!safe_str) return NULL;
        memcpy(safe_str, data, len);
        safe_str[len] = '\0';
        
        cJSON *item = cJSON_CreateString(safe_str);
        free(safe_str);
        return item;
    } else {
        /* Null terminator found, safe to pass directly as it's null-terminated */
        return cJSON_CreateString(start);
    }
}

/* Helper to create a complex JSON structure from input data to increase state diversity */
static cJSON *create_complex_cjson_from_input(const uint8_t *data, size_t size) {
    if (size == 0) {
        return cJSON_CreateObject();
    }

    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        return NULL;
    }

    /* Add the raw input as a string value safely */
    cJSON *str_item = create_cjson_from_input_safe(data, size);
    if (str_item) {
        cJSON_AddItemToObject(root, "input_data", str_item);
    }

    /* Add a number based on the first byte */
    cJSON *num_item = cJSON_CreateNumber(data[0]);
    if (num_item) {
        cJSON_AddItemToObject(root, "first_byte", num_item);
    }

    /* Add an array with some elements */
    cJSON *arr = cJSON_CreateArray();
    if (arr) {
        for (size_t i = 0; i < size && i < 10; i++) {
            cJSON *arr_item = cJSON_CreateNumber(data[i]);
            if (arr_item) {
                cJSON_AddItemToArray(arr, arr_item);
            }
        }
        cJSON_AddItemToObject(root, "byte_array", arr);
    }

    return root;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        /* Test with NULL input for functions that accept it */
        char *out = cJSON_Print(NULL);
        if (out) free(out);
        
        out = cJSON_PrintUnformatted(NULL);
        if (out) free(out);
        
        char buf[1024];
        cJSON_PrintPreallocated(NULL, buf, 1024, 1);
        
        cJSON_Minify(NULL);
        
        out = cJSON_PrintBuffered(NULL, 1024, 1);
        if (out) free(out);
        
        /* cJSON_Version does not take input, but we can call it */
        const char *ver = cJSON_Version();
        (void)ver; /* Suppress unused warning */
        
        return 0;
    }

    /* Create a cJSON object from the input data safely */
    cJSON *item = create_complex_cjson_from_input(Data, Size);
    if (item == NULL) {
        return 0;
    }

    /* 1. Test cJSON_Print */
    char *formatted = cJSON_Print(item);
    if (formatted) {
        free(formatted);
    }

    /* 2. Test cJSON_PrintUnformatted */
    char *unformatted = cJSON_PrintUnformatted(item);
    if (unformatted) {
        free(unformatted);
    }

    /* 3. Test cJSON_PrintPreallocated */
    char buffer[4096];
    cJSON_bool success = cJSON_PrintPreallocated(item, buffer, sizeof(buffer), 1);
    (void)success; /* Check success/failure internally */
    
    /* Test with format=0 */
    success = cJSON_PrintPreallocated(item, buffer, sizeof(buffer), 0);
    (void)success;

    /* 4. Test cJSON_Minify */
    /* We need a modifiable string. Let's use the formatted output from above if we had it, 
       or create a copy of a known string. Since we freed 'formatted', let's create a new one. */
    char *minify_target = cJSON_Print(item);
    if (minify_target) {
        /* Make a copy to avoid modifying the original if we wanted to reuse, 
           but cJSON_Minify modifies in-place, so we just minify the allocated string. */
        cJSON_Minify(minify_target);
        free(minify_target);
    }

    /* 5. Test cJSON_PrintBuffered */
    char *buffered = cJSON_PrintBuffered(item, 1024, 1);
    if (buffered) {
        free(buffered);
    }
    
    buffered = cJSON_PrintBuffered(item, 1024, 0);
    if (buffered) {
        free(buffered);
    }
    
    /* Test with a very small prebuffer to trigger reallocations */
    buffered = cJSON_PrintBuffered(item, 10, 1);
    if (buffered) {
        free(buffered);
    }

    /* 6. Test cJSON_Version */
    const char *version = cJSON_Version();
    (void)version;

    /* Cleanup */
    cJSON_Delete(item);

    return 0;
}