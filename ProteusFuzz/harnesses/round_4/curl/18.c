// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_free at escape.c:189:6 in curl.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/urlapi.h>
#include <curl/multi.h>

/* Fuzz driver for curl URL and multi handle APIs */

static int fuzzer_test_url_api(const uint8_t *data, size_t size) {
    CURLU *handle = NULL;
    CURLUcode res;
    char *part = NULL;
    const char *strerror_msg;
    size_t offset = 0;
    int iterations = 10;

    /* Limit iterations based on input size to avoid infinite loops */
    if (size > 0) {
        iterations = (size / 10) + 1;
        if (iterations > 100) iterations = 100;
    }

    for (int i = 0; i < iterations; i++) {
        /* Create a new handle */
        handle = curl_url();
        if (!handle) {
            /* If we cannot create a handle, try to test error string conversion */
            strerror_msg = curl_url_strerror(CURLUE_OUT_OF_MEMORY);
            (void)strerror_msg; /* Use the result to avoid warnings */
            continue;
        }

        /* Attempt to set various parts if data is available */
        if (size > offset) {
            /* Try to set scheme */
            if (size - offset > 4) {
                res = curl_url_set(handle, CURLUPART_SCHEME, (const char *)(data + offset), 0);
                if (res != CURLUE_OK) {
                    strerror_msg = curl_url_strerror(res);
                    (void)strerror_msg;
                }
                offset += 4;
            }

            /* Try to set host */
            if (size - offset > 4) {
                res = curl_url_set(handle, CURLUPART_HOST, (const char *)(data + offset), 0);
                if (res != CURLUE_OK) {
                    strerror_msg = curl_url_strerror(res);
                    (void)strerror_msg;
                }
                offset += 4;
            }

            /* Try to set path */
            if (size - offset > 4) {
                res = curl_url_set(handle, CURLUPART_PATH, (const char *)(data + offset), 0);
                if (res != CURLUE_OK) {
                    strerror_msg = curl_url_strerror(res);
                    (void)strerror_msg;
                }
                offset += 4;
            }
            
            /* Try to set port */
            if (size - offset > 4) {
                res = curl_url_set(handle, CURLUPART_PORT, (const char *)(data + offset), 0);
                if (res != CURLUE_OK) {
                    strerror_msg = curl_url_strerror(res);
                    (void)strerror_msg;
                }
                offset += 4;
            }
        }

        /* Try to get various parts */
        res = curl_url_get(handle, CURLUPART_SCHEME, &part, 0);
        if (res == CURLUE_OK && part) {
            curl_free(part);
            part = NULL;
        } else {
            strerror_msg = curl_url_strerror(res);
            (void)strerror_msg;
        }

        res = curl_url_get(handle, CURLUPART_HOST, &part, 0);
        if (res == CURLUE_OK && part) {
            curl_free(part);
            part = NULL;
        } else {
            strerror_msg = curl_url_strerror(res);
            (void)strerror_msg;
        }

        res = curl_url_get(handle, CURLUPART_PATH, &part, 0);
        if (res == CURLUE_OK && part) {
            curl_free(part);
            part = NULL;
        } else {
            strerror_msg = curl_url_strerror(res);
            (void)strerror_msg;
        }

        /* Test with flags */
        res = curl_url_get(handle, CURLUPART_QUERY, &part, CURLU_DEFAULT_SCHEME);
        if (res == CURLUE_OK && part) {
            curl_free(part);
            part = NULL;
        } else {
            strerror_msg = curl_url_strerror(res);
            (void)strerror_msg;
        }

        /* Cleanup handle */
        curl_url_cleanup(handle);
        handle = NULL;
    }

    return 0;
}

static int fuzzer_test_multi_api(const uint8_t *data, size_t size) {
    CURLM *multi = NULL;
    CURL **handles = NULL;
    CURL *easy = NULL;
    int i;

    /* Create multi handle */
    multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    /* Create some easy handles and add them to multi */
    /* We create up to 5 handles to avoid excessive memory usage */
    int num_handles = 0;
    if (size > 0) {
        num_handles = (size % 5) + 1;
    }

    /* Allocate array to store handles for proper cleanup */
    CURL *handle_array[5]; 
    int count = 0;

    for (i = 0; i < num_handles; i++) {
        easy = curl_easy_init();
        if (easy) {
            curl_multi_add_handle(multi, easy);
            handle_array[count++] = easy;
        }
    }

    /* Get handles array */
    handles = curl_multi_get_handles(multi);
    if (handles) {
        /* Iterate through the array until NULL */
        for (i = 0; handles[i] != NULL; i++) {
            /* Just accessing to ensure validity */
            (void)handles[i];
        }
        /* Free the array */
        curl_free(handles);
        handles = NULL;
    }

    /* Cleanup easy handles using the stored array */
    for (i = 0; i < count; i++) {
        easy = handle_array[i];
        curl_multi_remove_handle(multi, easy);
        curl_easy_cleanup(easy);
    }

    curl_multi_cleanup(multi);

    return 0;
}

static int fuzzer_test_env_api(const uint8_t *data, size_t size) {
    char *env_var = NULL;
    const char *var_name = "TEST_FUZZ_VAR";

    /* Try to get environment variable */
    env_var = curl_getenv(var_name);
    if (env_var) {
        curl_free(env_var);
        env_var = NULL;
    }

    /* Try with a variable name derived from input if available */
    if (size > 0) {
        char var_buf[256];
        size_t len = (size < 255) ? size : 255;
        memcpy(var_buf, data, len);
        var_buf[len] = '\0';
        
        env_var = curl_getenv(var_buf);
        if (env_var) {
            curl_free(env_var);
            env_var = NULL;
        }
    }

    return 0;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    /* Test URL API */
    fuzzer_test_url_api(Data, Size);

    /* Test Multi API */
    fuzzer_test_multi_api(Data, Size);

    /* Test Environment API */
    fuzzer_test_env_api(Data, Size);

    /* Test error string conversion with various codes */
    const char *msg;
    msg = curl_url_strerror(CURLUE_OK);
    (void)msg;
    msg = curl_url_strerror(CURLUE_BAD_HANDLE);
    (void)msg;
    msg = curl_url_strerror(CURLUE_MALFORMED_INPUT);
    (void)msg;
    msg = curl_url_strerror(CURLUE_OUT_OF_MEMORY);
    (void)msg;
    msg = curl_url_strerror(CURLUE_URLDECODE);
    (void)msg;
    /* Removed CURLUE_NOT_BUILT_IN as it is not declared in the provided headers */

    return 0;
}