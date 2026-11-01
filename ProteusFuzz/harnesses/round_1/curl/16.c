// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_version at version.c:145:7 in curl.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/options.h>
#include <curl/typecheck-gcc.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/*
 * Fuzz driver for curl API functions:
 * - curl_version
 * - curl_easy_option_next
 * - curl_getenv
 * - curl_easy_option_by_name
 * - curl_easy_option_by_id
 *
 * Note: Wcurl_multi_setopt_err_cb_data and related macros are compile-time
 * checks and do not require runtime invocation for fuzzing.
 */

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    /* 1. Prepare environment */
    if (Size == 0) {
        return 0;
    }

    /* Ensure null-termination for string operations */
    char *input_str = NULL;
    if (Size > 0) {
        input_str = malloc(Size + 1);
        if (!input_str) {
            return 0;
        }
        memcpy(input_str, Data, Size);
        input_str[Size] = '\0';
    }

    /* 2. Invoke target functions */

    /* curl_version: No arguments, returns static string */
    const char *version = curl_version();
    (void)version; /* Use return value to avoid unused warning */

    /* curl_easy_option_next: Iterate through all options */
    const struct curl_easyoption *opt = NULL;
    while ((opt = curl_easy_option_next(opt)) != NULL) {
        /* Explore option metadata */
        (void)opt->name;
        (void)opt->id;
        (void)opt->type;
        (void)opt->flags;
    }

    /* curl_easy_option_by_name: Use input data as option name */
    if (input_str) {
        const struct curl_easyoption *named_opt = curl_easy_option_by_name(input_str);
        if (named_opt) {
            (void)named_opt->name;
            (void)named_opt->id;
            (void)named_opt->type;
            (void)named_opt->flags;
        }
    }

    /* curl_easy_option_by_id: Use input data as option ID */
    if (Size > 0) {
        CURLoption id = (CURLoption)Data[0];
        const struct curl_easyoption *id_opt = curl_easy_option_by_id(id);
        if (id_opt) {
            (void)id_opt->name;
            (void)id_opt->id;
            (void)id_opt->type;
            (void)id_opt->flags;
        }
    }

    /* curl_getenv: Use input data as environment variable name */
    if (input_str) {
        char *env_val = curl_getenv(input_str);
        if (env_val) {
            /* Use the value to explore state */
            (void)strlen(env_val);
            curl_free(env_val);
        }
    }

    /* 3. Cleanup */
    if (input_str) {
        free(input_str);
    }

    return 0;
}