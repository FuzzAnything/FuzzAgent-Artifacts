// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_assign at multi.c:3653:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/*
 * Static helper to initialize a dummy file if needed.
 */
static void init_dummy_file(void) {
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fprintf(fp, "dummy data for fuzzing\n");
        fclose(fp);
    }
}

/*
 * Helper function to read a byte from the input buffer
 */
static uint8_t read_byte(const uint8_t **ptr, size_t *remaining) {
    if (*remaining > 0) {
        uint8_t val = **ptr;
        (*ptr)++;
        (*remaining)--;
        return val;
    }
    return 0;
}

/*
 * Helper function to read an int32 from the input buffer
 */
static int32_t read_int32(const uint8_t **ptr, size_t *remaining) {
    if (*remaining >= 4) {
        int32_t val = 0;
        memcpy(&val, *ptr, 4);
        *ptr += 4;
        *remaining -= 4;
        return val;
    }
    return 0;
}

/*
 * Fuzz entry point
 */
int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Initialize dummy file */
    init_dummy_file();

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to add to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Extract data for fuzzing */
    const uint8_t *ptr = Data;
    size_t remaining = Size;

    /* Fuzz curl_multi_assign */
    if (remaining >= 8) {
        int sockfd = read_int32(&ptr, &remaining);
        void *sockp = (void *)(uintptr_t)read_int32(&ptr, &remaining);
        curl_multi_assign(multi_handle, sockfd, sockp);
    }

    /* Fuzz curl_multi_strerror */
    for (int i = 0; i < 10 && remaining > 0; i++) {
        CURLMcode error_code = (CURLMcode)read_byte(&ptr, &remaining);
        const char *str = curl_multi_strerror(error_code);
        (void)str; /* Suppress unused warning */
    }

    /* Fuzz curl_multi_setopt */
    if (remaining >= 4) {
        CURLMoption option = (CURLMoption)read_int32(&ptr, &remaining);
        
        if (option == CURLMOPT_PUSHFUNCTION) {
            /* Push function requires a callback and a userdata */
            /* We pass NULL to test error handling or skip if callback is complex */
            curl_multi_setopt(multi_handle, option, NULL);
        } else if (option == CURLMOPT_MAXCONNECTS) {
            long val = 0;
            if (remaining >= 4) {
                val = read_int32(&ptr, &remaining);
            }
            curl_multi_setopt(multi_handle, option, val);
        } else if (option == CURLMOPT_PIPELINING) {
            long val = 0;
            if (remaining >= 4) {
                val = read_int32(&ptr, &remaining);
            }
            curl_multi_setopt(multi_handle, option, val);
        } else {
            /* Default to passing a long value */
            long val = 0;
            if (remaining >= 4) {
                val = read_int32(&ptr, &remaining);
            }
            curl_multi_setopt(multi_handle, option, val);
        }
    }

    /* Fuzz curl_multi_wakeup */
    curl_multi_wakeup(multi_handle);

    /* Fuzz curl_multi_remove_handle */
    curl_multi_remove_handle(multi_handle, easy_handle);

    /* Cleanup easy handle */
    curl_easy_cleanup(easy_handle);

    /* Cleanup multi handle */
    curl_multi_cleanup(multi_handle);

    /* Cleanup global curl environment */
    curl_global_cleanup();

    return 0;
}