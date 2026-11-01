// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_version at version.c:145:7 in curl.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/options.h>
#include <curl/mprintf.h>

static void fuzz_curl_maprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Ensure null termination for format string
    char *format = (char *)malloc(size + 1);
    if (!format) return;
    
    memcpy(format, data, size);
    format[size] = '\0';
    
    // Basic safety: replace any embedded nulls to prevent early termination issues
    // and ensure we have a valid format string start
    if (format[0] == '\0') {
        free(format);
        return;
    }
    
    // Fuzz with various argument counts
    char *result = NULL;
    
    // Case 1: No extra arguments
    result = curl_maprintf("%s", format);
    if (result) {
        curl_free(result);
    }
    
    // Case 2: With integer argument
    result = curl_maprintf("%s %d", format, 42);
    if (result) {
        curl_free(result);
    }
    
    // Case 3: With string argument
    result = curl_maprintf("%s %s", format, "test");
    if (result) {
        curl_free(result);
    }
    
    // Case 4: With hex argument
    result = curl_maprintf("%s %x", format, 0xDEADBEEF);
    if (result) {
        curl_free(result);
    }
    
    free(format);
}

static void fuzz_curl_easy_option_next(void) {
    const struct curl_easyoption *opt = NULL;
    int count = 0;
    
    // Iterate through all options
    while ((opt = curl_easy_option_next(opt)) != NULL) {
        count++;
        // Access fields to ensure they are valid
        (void)opt->name;
        (void)opt->id;
        (void)opt->type;
        (void)opt->flags;
        
        // Prevent infinite loops in case of corruption
        if (count > 10000) break;
    }
}

static void fuzz_curl_getenv(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Ensure null termination
    char *var_name = (char *)malloc(size + 1);
    if (!var_name) return;
    
    memcpy(var_name, data, size);
    var_name[size] = '\0';
    
    // Filter out invalid characters for environment variable names
    for (size_t i = 0; i < size; i++) {
        if (var_name[i] == '=' || var_name[i] == '\0' || var_name[i] < 32 || var_name[i] > 126) {
            var_name[i] = 'X';
        }
    }
    
    char *result = curl_getenv(var_name);
    if (result) {
        curl_free(result);
    }
    
    free(var_name);
}

static void fuzz_curl_easy_option_by_name(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char *name = (char *)malloc(size + 1);
    if (!name) return;
    
    memcpy(name, data, size);
    name[size] = '\0';
    
    const struct curl_easyoption *opt = curl_easy_option_by_name(name);
    if (opt) {
        (void)opt->name;
        (void)opt->id;
        (void)opt->type;
        (void)opt->flags;
    }
    
    free(name);
}

static void fuzz_curl_version(void) {
    char *version = curl_version();
    if (version) {
        // Just access it to ensure it's valid
        size_t len = strlen(version);
        (void)len;
    }
}

static void fuzz_curl_easy_option_by_id(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    // Read an integer from the data to use as an ID
    uint32_t id_val = 0;
    memcpy(&id_val, data, 4);
    
    // Use the value as a CURLoption ID
    const struct curl_easyoption *opt = curl_easy_option_by_id((CURLoption)id_val);
    if (opt) {
        (void)opt->name;
        (void)opt->id;
        (void)opt->type;
        (void)opt->flags;
    }
    
    // Also try with some common known IDs
    opt = curl_easy_option_by_id(CURLOPT_URL);
    if (opt) {
        (void)opt->name;
    }
    
    opt = curl_easy_option_by_id(CURLOPT_TIMEOUT);
    if (opt) {
        (void)opt->name;
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Split input into segments for different fuzzing targets
    // Ensure we don't go out of bounds
    size_t segment_size = Size / 6;
    if (segment_size < 1) segment_size = 1;
    
    // Calculate actual available size for each segment to avoid overflow
    size_t current_offset = 0;
    
    // Fuzz curl_maprintf
    fuzz_curl_maprintf(Data + current_offset, segment_size);
    current_offset += segment_size;
    
    // Fuzz curl_easy_option_next (no input needed)
    fuzz_curl_easy_option_next();
    
    // Fuzz curl_getenv
    if (current_offset + segment_size <= Size) {
        fuzz_curl_getenv(Data + current_offset, segment_size);
        current_offset += segment_size;
    }
    
    // Fuzz curl_easy_option_by_name
    if (current_offset + segment_size <= Size) {
        fuzz_curl_easy_option_by_name(Data + current_offset, segment_size);
        current_offset += segment_size;
    }
    
    // Fuzz curl_version (no input needed)
    fuzz_curl_version();
    
    // Fuzz curl_easy_option_by_id
    if (current_offset + segment_size <= Size) {
        fuzz_curl_easy_option_by_id(Data + current_offset, segment_size);
        current_offset += segment_size;
    }
    
    // Additional fuzzing with remaining data
    if (current_offset < Size) {
        size_t remaining_size = Size - current_offset;
        fuzz_curl_maprintf(Data + current_offset, remaining_size);
    }
    
    return 0;
}