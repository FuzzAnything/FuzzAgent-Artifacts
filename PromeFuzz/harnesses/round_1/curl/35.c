// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_version_info at version.c:596:25 in curl.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize with fuzzer-derived flags
    long flags = (long)Data[0];
    CURLcode init_res = curl_global_init(flags);
    if (init_res != CURLE_OK) {
        return 0;
    }

    // Fuzz curl_global_trace with config string from input
    const char *config = (const char *)Data;
    size_t config_len = Size;
    if (config_len > 100) {
        config_len = 100;
    }
    char *trace_config = (char *)malloc(config_len + 1);
    if (trace_config) {
        memcpy(trace_config, config, config_len);
        trace_config[config_len] = '\0';
        (void)curl_global_trace(trace_config);
        free(trace_config);
    }

    // Get version info - safe to call multiple times
    (void)curl_version();
    
    // Fuzz curl_version_info with version stamp derived from input
    CURLversion stamp = (CURLversion)(Data[0] % (CURLVERSION_LAST + 1));
    const curl_version_info_data *info = curl_version_info(stamp);
    (void)info;

    // Fuzz curl_getdate with date string from input
    size_t date_len = Size;
    if (date_len > 50) {
        date_len = 50;
    }
    char *date_str = (char *)malloc(date_len + 1);
    if (date_str) {
        memcpy(date_str, Data, date_len);
        date_str[date_len] = '\0';
        time_t result = curl_getdate(date_str, NULL);
        (void)result;
        free(date_str);
    }

    // Create dummy file for any potential file operations
    write_dummy_file(Data, Size > 1024 ? 1024 : Size);

    // Clean up
    curl_global_cleanup();
    remove_dummy_file();

    return 0;
}