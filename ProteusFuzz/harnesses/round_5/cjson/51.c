// This fuzz driver is generated for library cjson, aiming to fuzz the following functions:
// cJSON_Parse at cJSON.c:1227:23 in cJSON.h
// cJSON_Delete at cJSON.c:253:20 in cJSON.h
// cJSON_Version at cJSON.c:124:27 in cJSON.h
// cJSON_PrintUnformatted at cJSON.c:1312:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_PrintBuffered at cJSON.c:1317:22 in cJSON.h
// cJSON_Print at cJSON.c:1307:22 in cJSON.h
// cJSON_Minify at cJSON.c:2924:20 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
// cJSON_PrintPreallocated at cJSON.c:1348:26 in cJSON.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

static cJSON *create_cjson_from_fuzz_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }

    // Ensure null termination for parsing
    char *json_str = (char *)malloc(size + 1);
    if (!json_str) {
        return NULL;
    }
    memcpy(json_str, data, size);
    json_str[size] = '\0';

    cJSON *item = cJSON_Parse(json_str);
    free(json_str);
    return item;
}

static void cleanup_cjson(cJSON *item) {
    if (item) {
        cJSON_Delete(item);
    }
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Test cJSON_Version (no input dependency, just call it)
    const char *version = cJSON_Version();
    if (version) {
        // Just to ensure it's valid and not null
        (void)strlen(version);
    }

    // Create a cJSON object from fuzz input
    cJSON *item = create_cjson_from_fuzz_data(Data, Size);

    // 2. Test cJSON_PrintUnformatted
    if (item) {
        char *unformatted = cJSON_PrintUnformatted(item);
        if (unformatted) {
            free(unformatted);
        }
    }

    // 3. Test cJSON_PrintBuffered
    if (item) {
        int prebuffer = Size > 1000 ? 1000 : Size;
        char *buffered = cJSON_PrintBuffered(item, prebuffer, 0); // unformatted
        if (buffered) {
            free(buffered);
        }
        buffered = cJSON_PrintBuffered(item, prebuffer, 1); // formatted
        if (buffered) {
            free(buffered);
        }
    }

    // 4. Test cJSON_Print
    if (item) {
        char *formatted = cJSON_Print(item);
        if (formatted) {
            free(formatted);
        }
    }

    // 5. Test cJSON_Minify
    // We need a writable buffer with JSON content
    if (Size > 0) {
        char *minify_buf = (char *)malloc(Size + 1);
        if (minify_buf) {
            memcpy(minify_buf, Data, Size);
            minify_buf[Size] = '\0';
            cJSON_Minify(minify_buf);
            free(minify_buf);
        }
    }

    // 6. Test cJSON_PrintPreallocated
    if (item) {
        char buffer[4096];
        int len = sizeof(buffer);
        cJSON_bool res1 = cJSON_PrintPreallocated(item, buffer, len, 0);
        (void)res1; // Suppress unused warning
        cJSON_bool res2 = cJSON_PrintPreallocated(item, buffer, len, 1);
        (void)res2; // Suppress unused warning
    }

    // Cleanup
    cleanup_cjson(item);

    return 0;
}