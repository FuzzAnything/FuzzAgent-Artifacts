// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_ssls_import at easy.c:1334:10 in curl.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/websockets.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Dummy callback implementations to satisfy type checks if needed */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return size * nmemb;
}

static int dummy_debug_cb(CURL *curl, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)curl; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

static int dummy_sockopt_cb(CURL *curl, curl_socket_t fd, curlsocktype purpose, void *userptr) {
    (void)curl; (void)fd; (void)purpose; (void)userptr;
    return 0;
}

static int dummy_progress_cb(CURL *curl, double dltotal, double dlnow, double ultotal, double ulnow, void *userptr) {
    (void)curl; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow; (void)userptr;
    return 0;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Initialize buffers from input data */
    char recv_buf[4096];
    char send_buf[4096];
    size_t recv_len = 0;
    size_t sent_len = 0;
    size_t n = 0;
    struct curl_ws_frame *meta = NULL;
    curl_off_t fragsize = 0;
    unsigned int flags = 0;

    /* Parse input data to drive fuzzing logic */
    size_t idx = 0;
    size_t max_idx = Size;

    if (idx < max_idx) {
        fragsize = Data[idx++];
    }
    if (idx < max_idx) {
        flags = Data[idx++];
    }

    /* Prepare dummy file if needed (though not strictly used in these APIs directly) */
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    /* Setup curl options to simulate a connection state for testing */
    /* We use a dummy URL, but the APIs we test require a connected socket or CONNECT_ONLY */
    /* Since we cannot actually connect in a fuzzer, we rely on the API's internal validation */
    curl_easy_setopt(curl, CURLOPT_URL, "ws://localhost:8080/ws");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1L); /* Fail fast if network is involved */
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    
    /* Set some callbacks to exercise type checking macros indirectly */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
    curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, dummy_sockopt_cb);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);

    /* Try to perform (will likely fail due to no server, but sets up state or errors) */
    (void)curl_easy_perform(curl);

    /* Fuzz curl_ws_recv */
    if (idx < max_idx) {
        size_t recv_buflen = Data[idx++];
        if (recv_buflen > sizeof(recv_buf)) recv_buflen = sizeof(recv_buf);
        memset(recv_buf, 0, sizeof(recv_buf));
        curl_ws_recv(curl, recv_buf, recv_buflen, &recv_len, &meta);
    }

    /* Fuzz curl_ws_send */
    if (idx < max_idx) {
        size_t send_buflen = Data[idx++];
        if (send_buflen > sizeof(send_buf)) send_buflen = sizeof(send_buf);
        /* Fill send buffer with fuzz data */
        size_t copy_len = (Size < send_buflen) ? Size : send_buflen;
        memcpy(send_buf, Data, copy_len);
        curl_ws_send(curl, send_buf, send_buflen, &sent_len, fragsize, flags);
    }

    /* Fuzz curl_easy_send */
    if (idx < max_idx) {
        size_t easy_send_buflen = Data[idx++];
        if (easy_send_buflen > sizeof(send_buf)) easy_send_buflen = sizeof(send_buf);
        curl_easy_send(curl, send_buf, easy_send_buflen, &n);
    }

    /* Fuzz curl_easy_ssls_import */
    /* Requires session_key, shmac, sdata */
    if (idx < max_idx) {
        const char *session_key = (const char *)(Data + idx);
        idx += 16; /* Skip 16 bytes for key simulation */
        if (idx > Size) idx = Size;
        
        const unsigned char *shmac = (const unsigned char *)(Data + idx);
        size_t shmac_len = (idx + 32 <= Size) ? 32 : (Size - idx);
        idx += shmac_len;
        
        const unsigned char *sdata = (const unsigned char *)(Data + idx);
        size_t sdata_len = (idx + 100 <= Size) ? 100 : (Size - idx);
        
        curl_easy_ssls_import(curl, session_key, shmac, shmac_len, sdata, sdata_len);
    }

    /* Fuzz curl_easy_recv */
    if (idx < max_idx) {
        size_t easy_recv_buflen = Data[idx++];
        if (easy_recv_buflen > sizeof(recv_buf)) easy_recv_buflen = sizeof(recv_buf);
        memset(recv_buf, 0, sizeof(recv_buf));
        curl_easy_recv(curl, recv_buf, easy_recv_buflen, &n);
    }

    /* Cleanup */
    if (meta) {
        /* curl_ws_frame is a struct, usually not dynamically allocated by curl_ws_recv in a way we free */
        /* But if we allocated it elsewhere, we would free it here. */
    }
    
    curl_easy_cleanup(curl);
    return 0;
}