// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_ParseWithOpts at cJSON.c:1131:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void cleanup_strings(char **strings, int count) {
    for (int i = 0; i < count; i++) {
        if (strings[i]) {
            free(strings[i]);
            strings[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string for parsing
    char *json_str = (char *)malloc(Size + 1);
    if (!json_str) {
        return 0;
    }
    memcpy(json_str, Data, Size);
    json_str[Size] = '\0';
    
    // Parse the JSON
    const char *error_ptr = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_str, &error_ptr, 0);
    
    if (parsed) {
        // Store pointers to allocated strings for cleanup
        char *allocated_strings[4] = {NULL, NULL, NULL, NULL};
        int string_index = 0;
        
        // Test cJSON_PrintBuffered with various parameters
        int prebuffer = (Size % 1024) + 1; // Vary prebuffer size
        cJSON_bool fmt = Size % 2; // Alternate between formatted and unformatted
        allocated_strings[string_index++] = cJSON_PrintBuffered(parsed, prebuffer, fmt);
        
        // Test with different prebuffer sizes
        allocated_strings[string_index++] = cJSON_PrintBuffered(parsed, 0, 1);
        allocated_strings[string_index++] = cJSON_PrintBuffered(parsed, 1024, 0);
        
        // Test cJSON_Print
        allocated_strings[string_index++] = cJSON_Print(parsed);
        
        // Test cJSON_PrintUnformatted
        char *unformatted = cJSON_PrintUnformatted(parsed);
        if (unformatted) {
            // Test cJSON_Minify on the unformatted string
            cJSON_Minify(unformatted);
            free(unformatted);
        }
        
        // Test cJSON_Minify on a writable copy of the original input
        char *minify_input = (char *)malloc(Size + 1);
        if (minify_input) {
            memcpy(minify_input, json_str, Size);
            minify_input[Size] = '\0';
            cJSON_Minify(minify_input);
            free(minify_input);
        }
        
        // Test cJSON_Minify with NULL input (should do nothing)
        cJSON_Minify(NULL);
        
        // Clean up allocated strings
        cleanup_strings(allocated_strings, 4);
        
        // Delete the parsed JSON
        cJSON_Delete(parsed);
    }
    
    // Test with require_null_terminated = 1
    cJSON *parsed2 = cJSON_ParseWithOpts(json_str, NULL, 1);
    if (parsed2) {
        cJSON_Delete(parsed2);
    }
    
    // Test parsing with a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        // Read the file and parse it
        fp = fopen("./dummy_file", "rb");
        if (fp) {
            fseek(fp, 0, SEEK_END);
            long file_size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            
            if (file_size > 0) {
                char *file_content = (char *)malloc(file_size + 1);
                if (file_content) {
                    fread(file_content, 1, file_size, fp);
                    file_content[file_size] = '\0';
                    
                    cJSON *file_parsed = cJSON_ParseWithOpts(file_content, NULL, 0);
                    if (file_parsed) {
                        char *printed = cJSON_Print(file_parsed);
                        if (printed) {
                            free(printed);
                        }
                        cJSON_Delete(file_parsed);
                    }
                    free(file_content);
                }
            }
            fclose(fp);
        }
    }
    
    free(json_str);
    return 0;
}