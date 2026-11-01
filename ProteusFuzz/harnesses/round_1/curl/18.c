// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_easy_option_by_name at easygetopt.c:53:31 in options.h
// curl_easy_option_by_id at easygetopt.c:59:31 in options.h
// curl_easy_option_next at easygetopt.c:66:1 in options.h
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

/* Helper to safely free curl memory */
static void safe_free(void *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 1. Prepare environment and call curl_version */
    char *version = curl_version();
    if (version) {
        /* Just use the pointer to ensure it's valid, no need to free as it's static */
        (void)version;
    }

    /* 2. Fuzz curl_getenv with input data as variable name */
    /* Ensure null termination for the variable name string */
    char *env_var = (char *)malloc(Size + 1);
    if (!env_var) return 0;
    memcpy(env_var, Data, Size);
    env_var[Size] = '\0';

    char *env_result = curl_getenv(env_var);
    safe_free(env_result);
    free(env_var);

    /* 3. Fuzz curl_easy_option_by_name */
    /* Use input data as option name */
    char *opt_name = (char *)malloc(Size + 1);
    if (!opt_name) return 0;
    memcpy(opt_name, Data, Size);
    opt_name[Size] = '\0';

    const struct curl_easyoption *opt_by_name = curl_easy_option_by_name(opt_name);
    if (opt_by_name) {
        /* Access fields to ensure validity */
        (void)opt_by_name->name;
        (void)opt_by_name->id;
        (void)opt_by_name->type;
        (void)opt_by_name->flags;
    }
    free(opt_name);

    /* 4. Fuzz curl_easy_option_by_id */
    /* Extract an integer from input to use as ID */
    CURLoption id = 0;
    if (Size >= sizeof(CURLoption)) {
        memcpy(&id, Data, sizeof(CURLoption));
    } else if (Size > 0) {
        id = (CURLoption)Data[0];
    }

    const struct curl_easyoption *opt_by_id = curl_easy_option_by_id(id);
    if (opt_by_id) {
        (void)opt_by_id->name;
        (void)opt_by_id->id;
        (void)opt_by_id->type;
        (void)opt_by_id->flags;
    }

    /* 5. Fuzz curl_easy_option_next by iterating */
    const struct curl_easyoption *prev = NULL;
    int iteration_count = 0;
    while ((prev = curl_easy_option_next(prev)) != NULL) {
        (void)prev->name;
        (void)prev->id;
        (void)prev->type;
        (void)prev->flags;
        iteration_count++;
        /* Prevent infinite loops in case of corruption, though unlikely for this API */
        if (iteration_count > 10000) break;
    }

    /* 6. Trigger type-checking macros (compile-time, but we can invoke logic if needed) */
    /* The macros like Wcurl_easy_setopt_err_hstswrite_cb are compile-time warnings.
     * We cannot "fuzz" them at runtime directly as they are not functions.
     * However, we can ensure the headers are included and the environment is set up.
     * The fuzzing of the actual setopt behavior would require a CURL handle and real options,
     * which is outside the scope of these specific introspection functions.
     * We rely on the compiler to handle the warnings if we were to write code that triggers them.
     */

    return 0;
}