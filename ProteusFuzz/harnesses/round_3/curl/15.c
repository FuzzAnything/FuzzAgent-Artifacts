// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/options.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare environment and invoke target functions

    // Fuzz curl_easy_option_next
    // Start with NULL, then iterate through options using input data to decide steps
    const struct curl_easyoption *prev = NULL;
    const struct curl_easyoption *opt = NULL;
    size_t idx = 0;
    
    // Iterate a limited number of times based on input to explore states
    int max_iterations = (Size > 0) ? (Data[0] % 100) + 1 : 1;
    for (int i = 0; i < max_iterations; i++) {
        opt = curl_easy_option_next(prev);
        if (opt == NULL) {
            break;
        }
        prev = opt;
        // Use the option data to potentially influence other fuzzing paths if needed
        (void)opt->id;
        (void)opt->type;
        (void)opt->name;
        (void)opt->flags;
    }

    // Fuzz curl_getenv
    // Construct a variable name from input data
    if (Size > 1) {
        size_t var_len = Size - 1;
        if (var_len > 256) var_len = 256;
        char *var_name = malloc(var_len + 1);
        if (var_name) {
            memcpy(var_name, Data, var_len);
            var_name[var_len] = '\0';
            
            char *env_val = curl_getenv(var_name);
            if (env_val) {
                // Use the value briefly
                (void)strlen(env_val);
                curl_free(env_val);
            }
            free(var_name);
        }
    }

    // Fuzz curl_version
    // This function has no arguments, just call it to exercise the code path
    char *version = curl_version();
    if (version) {
        (void)strlen(version);
    }

    // Fuzz curl_easy_option_by_name
    // Use input data as a potential option name
    if (Size > 0) {
        size_t name_len = Size;
        if (name_len > 128) name_len = 128;
        char *option_name = malloc(name_len + 1);
        if (option_name) {
            memcpy(option_name, Data, name_len);
            option_name[name_len] = '\0';
            
            const struct curl_easyoption *opt_by_name = curl_easy_option_by_name(option_name);
            if (opt_by_name) {
                (void)opt_by_name->id;
                (void)opt_by_name->type;
                (void)opt_by_name->name;
                (void)opt_by_name->flags;
            }
            free(option_name);
        }
    }

    // Fuzz curl_easy_option_by_id
    // Derive a CURLoption ID from input data
    if (Size > 0) {
        // Use a portion of input to generate an ID, wrapping around valid ranges if possible
        // Note: CURLoption is an enum, we cast input bytes to it
        unsigned long id_val = 0;
        if (Size >= sizeof(unsigned long)) {
            memcpy(&id_val, Data, sizeof(unsigned long));
        } else {
            memcpy(&id_val, Data, Size);
        }
        
        // Limit to a reasonable range to avoid undefined behavior with extreme values
        // Although the API should handle invalid IDs gracefully
        CURLoption id = (CURLoption)(id_val % 5000); // Arbitrary large limit
        
        const struct curl_easyoption *opt_by_id = curl_easy_option_by_id(id);
        if (opt_by_id) {
            (void)opt_by_id->id;
            (void)opt_by_id->type;
            (void)opt_by_id->name;
            (void)opt_by_id->flags;
        }
    }

    // 2. Cleanup
    // No dynamic allocations need explicit cleanup here except those handled above.
    // curl_getenv returns malloc'd memory which we curl_free'd.
    // curl_version returns static memory, no free needed.
    // curl_easy_option_* return static or internal pointers, no free needed.

    return 0;
}