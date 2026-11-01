// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init_mem at easy.c:213:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include curl.h first to define CURL_EXTERN and other macros */
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>

/* Custom memory callbacks for curl_global_init_mem */
static void *fuzz_malloc(size_t size) {
    return malloc(size);
}

static void fuzz_free(void *ptr) {
    free(ptr);
}

static void *fuzz_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char *fuzz_strdup(const char *ptr) {
    return strdup(ptr);
}

static void *fuzz_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;

    /* Initialize libcurl globally if not already done. 
       Using curl_global_init_mem to exercise custom memory paths. */
    if (curl_global_init_mem(CURL_GLOBAL_DEFAULT,
                             fuzz_malloc, fuzz_free,
                             fuzz_realloc, fuzz_strdup,
                             fuzz_calloc) != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle */
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Ensure we have some data to work with for options */
    if (Size == 0) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    /* Set some basic options using the fuzz input to diversify states */
    /* We use a subset of data to ensure null termination for string options */
    size_t url_len = (Size > 100) ? 100 : Size;
    char *url_buf = (char *)malloc(url_len + 1);
    if (!url_buf) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_buf, Data, url_len);
    url_buf[url_len] = '\0';

    /* Set URL */
    curl_easy_setopt(curl, CURLOPT_URL, url_buf);
    
    /* Set other options to trigger different code paths */
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    
    /* Prepare a dummy file if needed, though not strictly required for these APIs */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* 
     * Fuzzing Strategy:
     * 1. Call curl_easy_upkeep to test connection pool maintenance.
     * 2. Call curl_easy_perform to attempt a transfer (will likely fail due to invalid URL, but tests logic).
     * 3. Attempt WebSocket operations (curl_ws_start_frame, curl_ws_send) which require specific states.
     *    These will likely fail with CURLE_FAILED_INIT or similar if not in a valid WS state, which is expected.
     */

    /* Test curl_easy_upkeep */
    res = curl_easy_upkeep(curl);
    (void)res; /* Ignore return for fuzzing, just want to exercise the path */

    /* Test curl_easy_perform */
    res = curl_easy_perform(curl);
    (void)res;

    /* Test WebSocket functions */
    /* We need to extract flags and length from input if possible, otherwise use defaults */
    unsigned int flags = 0;
    curl_off_t frame_len = 0;
    size_t sent = 0;
    curl_off_t fragsize = 0;

    /* Safe extraction of flags (requires 4 bytes) */
    if (Size >= 4) {
        unsigned int temp_flags;
        memcpy(&temp_flags, Data, sizeof(unsigned int));
        flags = temp_flags;
        
        /* Safe extraction of frame_len (requires 8 bytes for curl_off_t, usually long long) */
        if (Size >= 12) {
            curl_off_t temp_frame_len;
            memcpy(&temp_frame_len, Data + 4, sizeof(curl_off_t));
            frame_len = temp_frame_len;
            
            curl_off_t temp_fragsize;
            memcpy(&temp_fragsize, Data + 8, sizeof(curl_off_t));
            fragsize = temp_fragsize;
        } else if (Size >= 12) {
            /* Fallback if we only have enough for frame_len but not fragsize */
            curl_off_t temp_frame_len;
            memcpy(&temp_frame_len, Data + 4, sizeof(curl_off_t));
            frame_len = temp_frame_len;
        }
    } else if (Size >= 2) {
        /* Fallback for small inputs to set flags partially if needed, though 4 bytes is standard */
        unsigned int temp_flags;
        memset(&temp_flags, 0, sizeof(unsigned int));
        memcpy(&temp_flags, Data, Size);
        flags = temp_flags;
    }

    /* Try to start a WebSocket frame */
    res = curl_ws_start_frame(curl, flags, frame_len);
    (void)res;

    /* Try to send WebSocket data */
    if (Size > 0) {
        res = curl_ws_send(curl, Data, Size, &sent, fragsize, flags);
        (void)res;
    }

    /* Cleanup */
    free(url_buf);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}