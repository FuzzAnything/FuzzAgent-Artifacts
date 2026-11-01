// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
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
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
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

/* Dummy callback to satisfy type requirements if needed */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 3) {
        return 0;
    }

    /* Initialize libcurl global environment */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* Create a CURLM handle for multi interface testing */
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }

    /* Create a CURL handle to add to the multi handle */
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }

    /* Prepare a dummy file for options that might require file paths */
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fprintf(fp, "dummy content");
        fclose(fp);
    }

    /* Extract values from fuzz input to drive diverse option setting */
    long long_val = (long)Data[0];
    curl_off_t off_val = (curl_off_t)Data[1];
    int option_idx = Data[2] % 10; /* Use modulo to keep within reasonable range for testing */
    
    /* Fuzz curl_multi_setopt with various option types */
    /* Test CURLMOPT_MAXCONNECTS (long) */
    if (option_idx == 0) {
        curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, long_val);
    }
    /* Test CURLMOPT_MAX_HOST_CONNECTIONS (long) */
    else if (option_idx == 1) {
        curl_multi_setopt(multi, CURLMOPT_MAX_HOST_CONNECTIONS, long_val);
    }
    /* Test CURLMOPT_PIPELINING (long) */
    else if (option_idx == 2) {
        curl_multi_setopt(multi, CURLMOPT_PIPELINING, long_val);
    }
    /* Test CURLMOPT_PUSHFUNCTION (callback) - using NULL or dummy */
    else if (option_idx == 3) {
        curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, NULL);
    }
    /* Test CURLMOPT_SOCKETFUNCTION (callback) */
    else if (option_idx == 4) {
        curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION, NULL);
    }
    /* Test CURLMOPT_TIMERFUNCTION (callback) */
    else if (option_idx == 5) {
        curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, NULL);
    }
    /* Test CURLMOPT_MAX_PIPELINE_LENGTH (long) */
    else if (option_idx == 6) {
        curl_multi_setopt(multi, CURLMOPT_MAX_PIPELINE_LENGTH, long_val);
    }
    /* Test CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE (curl_off_t) */
    else if (option_idx == 7) {
        curl_multi_setopt(multi, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, off_val);
    }
    /* Test CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE (curl_off_t) */
    else if (option_idx == 8) {
        curl_multi_setopt(multi, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, off_val);
    }
    /* Test CURLMOPT_MAX_CONCURRENT_STREAMS (long) */
    else if (option_idx == 9) {
        curl_multi_setopt(multi, CURLMOPT_MAX_CONCURRENT_STREAMS, long_val);
    }

    /* Fuzz curl_easy_setopt with diverse options using the easy handle */
    /* Test CURLOPT_URL (string) */
    if (Size > 10) {
        char *url_buf = malloc(Size + 1);
        if (url_buf) {
            memcpy(url_buf, Data, Size);
            url_buf[Size] = '\0';
            curl_easy_setopt(easy, CURLOPT_URL, url_buf);
            free(url_buf);
        }
    }

    /* Test CURLOPT_ERRORBUFFER (char buffer) */
    char error_buffer[CURL_ERROR_SIZE];
    curl_easy_setopt(easy, CURLOPT_ERRORBUFFER, error_buffer);

    /* Test CURLOPT_READFUNCTION (callback) */
    curl_easy_setopt(easy, CURLOPT_READFUNCTION, dummy_read_callback);

    /* Test CURLOPT_WRITEFUNCTION (callback) */
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, dummy_read_callback);

    /* Test CURLOPT_TIMEOUT (long) */
    curl_easy_setopt(easy, CURLOPT_TIMEOUT, long_val);

    /* Test CURLOPT_MAXREDIRS (long) */
    curl_easy_setopt(easy, CURLOPT_MAXREDIRS, long_val);

    /* Test CURLOPT_INFILESIZE_LARGE (curl_off_t) */
    curl_easy_setopt(easy, CURLOPT_INFILESIZE_LARGE, off_val);

    /* Add the easy handle to the multi handle */
    curl_multi_add_handle(multi, easy);

    /* Fuzz curl_multi_wakeup */
    curl_multi_wakeup(multi);

    /* Cleanup */
    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_global_cleanup();

    /* Remove dummy file */
    remove("./dummy_file");

    return 0;
}