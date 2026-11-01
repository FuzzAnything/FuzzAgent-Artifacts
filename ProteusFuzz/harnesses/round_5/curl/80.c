// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Static helper to create a dummy file if needed */
static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content for fuzzing\n");
        fclose(f);
    }
}

/* Static callback for push function to avoid external linkage issues */
static int dummy_push_callback(CURL *parent, CURL *easy_handle,
                               size_t num_headers, struct curl_slist *headers,
                               void *userp) {
    (void)parent;
    (void)easy_handle;
    (void)num_headers;
    (void)headers;
    (void)userp;
    return 0; /* Accept all pushes */
}

/* Static callback for timer function */
static int dummy_timer_callback(CURLM *multi_handle, long timeout_ms, void *userp) {
    (void)multi_handle;
    (void)timeout_ms;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    /* Ensure we have enough data to avoid buffer overflows */
    if (Size < 12) {
        return 0;
    }

    /* Initialize libcurl if not already done */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a dummy file for potential file-based options */
    create_dummy_file();

    /* Initialize multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Initialize an easy handle to add to multi */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Use input data to drive fuzzing logic safely */
    int option_idx = Data[0] % 10;
    int long_val = (int)(Data[1] << 8 | Data[2]);
    long long_long_val = (long)(Data[3] << 24 | Data[4] << 16 | Data[5] << 8 | Data[6]);
    curl_off_t off_val = (curl_off_t)(Data[7] << 8 | Data[8]);
    void *cb_data = (void *)(uintptr_t)(Data[9] << 8 | Data[10]);

    /* Fuzz curl_multi_setopt with various options */
    CURLMcode res;

    /* Set PUSHFUNCTION if option_idx suggests it */
    if (option_idx == 0) {
        res = curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, dummy_push_callback);
        if (res == CURLM_OK) {
            res = curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, cb_data);
        }
    }
    /* Set MAXCONNECTS */
    else if (option_idx == 1) {
        res = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, long_val);
    }
    /* Set PIPELINING */
    else if (option_idx == 2) {
        res = curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, long_val);
    }
    /* Set TIMERFUNCTION */
    else if (option_idx == 3) {
        res = curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
    }
    /* Set TIMERDATA */
    else if (option_idx == 4) {
        res = curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, cb_data);
    }
    /* Set SOCKETFUNCTION (requires socket callback, simplified here) */
    else if (option_idx == 5) {
        /* We cannot easily fuzz a complex socket callback without more setup, skip or use NULL */
        res = curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, NULL);
    }
    /* Set SOCKETDATA */
    else if (option_idx == 6) {
        res = curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, cb_data);
    }
    /* Set MAX_HOST_CONNECTIONS */
    else if (option_idx == 7) {
        res = curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, long_val);
    }
    /* Set CONTENT_LENGTH_PENALTY_SIZE */
    else if (option_idx == 8) {
        res = curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, off_val);
    }
    /* Set CHUNK_LENGTH_PENALTY_SIZE */
    else if (option_idx == 9) {
        res = curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, off_val);
    }

    /* Fuzz curl_multi_strerror with various error codes */
    int error_code_idx = Data[11] % 20;
    CURLMcode error_code = (CURLMcode)error_code_idx;
    const char *error_str = curl_multi_strerror(error_code);
    if (error_str) {
        /* Ensure the string is valid by checking length or content if needed */
        (void)strlen(error_str);
    }

    /* Fuzz curl_multi_remove_handle */
    /* First, add the easy handle to the multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) == CURLM_OK) {
        /* Try to remove it */
        res = curl_multi_remove_handle(multi_handle, easy_handle);
        /* Try to remove it again (should return CURLM_OK or handle already removed) */
        res = curl_multi_remove_handle(multi_handle, easy_handle);
        /* Try to remove with invalid handle */
        res = curl_multi_remove_handle(multi_handle, NULL);
        /* Try to remove with invalid multi handle */
        res = curl_multi_remove_handle(NULL, easy_handle);
    }

    /* Fuzz curl_multi_wakeup */
    res = curl_multi_wakeup(multi_handle);
    /* Try with invalid handle */
    res = curl_multi_wakeup(NULL);

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}