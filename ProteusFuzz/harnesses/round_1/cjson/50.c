#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <cJSON.h>

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

static char *create_writable_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    char *str = (char *)malloc(size + 1);
    if (!str) {
        return NULL;
    }
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from fuzz input
    char *json_str = create_writable_string(Data, Size);
    if (!json_str) {
        return 0;
    }

    // Parse JSON
    cJSON *item = cJSON_Parse(json_str);
    free(json_str);

    // If parsing failed, try to construct a large string manually to test boundary conditions
    // We want to test the 'ensure' function with large 'needed' values to hit lines 506, 523
    if (!item) {
        // Try to create a large JSON string manually if parsing failed
        // This is a heuristic to trigger large 'needed' in ensure
        size_t large_size = Size > 1000000 ? Size : 1000000;
        // Ensure we don't overflow size_t or cause OOM immediately
        if (large_size > (size_t)(INT_MAX / 2)) {
            large_size = (size_t)(INT_MAX / 2) + 100;
        }
        
        char *large_buf = (char *)malloc(large_size + 100);
        if (large_buf) {
            // Construct a valid JSON string: "{\"key\":\"VALUE...\"}"
            // We need a string value that is very long
            large_buf[0] = '{';
            large_buf[1] = '"';
            large_buf[2] = 'k';
            large_buf[3] = 'e';
            large_buf[4] = 'y';
            large_buf[5] = '"';
            large_buf[6] = ':';
            large_buf[7] = '"';
            
            // Fill with data
            size_t fill_len = large_size - 20;
            if (fill_len > 0) {
                memset(large_buf + 8, 'A', fill_len);
            }
            
            // Close the string and object
            size_t end_idx = 8 + fill_len;
            large_buf[end_idx] = '"';
            large_buf[end_idx + 1] = '}';
            large_buf[end_idx + 2] = '\0';
            
            item = cJSON_Parse(large_buf);
            free(large_buf);
        }
    }

    if (!item) {
        // Even if item is NULL, we can test the APIs with NULL if they handle it gracefully
        // But ensure is called inside print_value which checks item != NULL.
        // So we need a valid item.
        // If we still don't have an item, create a simple one.
        item = cJSON_CreateString("test");
    }

    // Test 1: cJSON_Print (uses global_hooks, allows reallocation)
    // This path hits: ensure -> check realloc -> realloc (Line 540, 543)
    char *print_result = cJSON_Print(item);
    if (print_result) {
        free(print_result);
    }

    // Test 2: cJSON_PrintUnformatted
    char *print_unformatted = cJSON_PrintUnformatted(item);
    if (print_unformatted) {
        free(print_unformatted);
    }

    // Test 3: cJSON_PrintBuffered
    // This path also uses global_hooks
    int prebuffer = (Size > 1000) ? 1000 : (int)Size;
    char *print_buffered = cJSON_PrintBuffered(item, prebuffer, 1);
    if (print_buffered) {
        free(print_buffered);
    }
    print_buffered = cJSON_PrintBuffered(item, prebuffer, 0);
    if (print_buffered) {
        free(print_buffered);
    }

    // Test 4: cJSON_PrintPreallocated with a SMALL buffer
    // This sets noalloc = true. If buffer is too small, ensure returns NULL at line 519.
    // This helps cover the noalloc path.
    char small_buffer[10];
    cJSON_bool prealloc_result = cJSON_PrintPreallocated(item, small_buffer, sizeof(small_buffer), 1);
    (void)prealloc_result;
    
    // Test with format = 0
    prealloc_result = cJSON_PrintPreallocated(item, small_buffer, sizeof(small_buffer), 0);
    (void)prealloc_result;

    // Test 5: Try to trigger INT_MAX checks by using a very large prebuffer in PrintBuffered?
    // No, PrintBuffered allocates.
    // The 'needed' calculation in ensure depends on the content.
    // If we have a huge string in 'item', 'needed' will be huge.
    // We tried to construct that above.

    // Cleanup
    cleanup_cjson(item);

    return 0;
}