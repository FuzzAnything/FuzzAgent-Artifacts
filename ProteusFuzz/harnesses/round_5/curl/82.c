// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_version_info at version.c:596:25 in curl.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl multi-handle options and version info.
 * Focuses on curl_multi_setopt, curl_multi_wakeup, and curl_version_info.
 * Note: Wcurl_easy_setopt_err_* are compile-time warnings, not runtime functions.
 */

/* Helper structure to manage input parsing state */
typedef struct {
    const uint8_t *ptr;
    size_t remaining;
} InputState;

static int read_int(InputState *state) {
    if (state->remaining < sizeof(int)) return 0;
    int val;
    memcpy(&val, state->ptr, sizeof(int));
    state->ptr += sizeof(int);
    state->remaining -= sizeof(int);
    return val;
}

static size_t read_size(InputState *state) {
    if (state->remaining < sizeof(size_t)) return 0;
    size_t val;
    memcpy(&val, state->ptr, sizeof(size_t));
    state->ptr += sizeof(size_t);
    state->remaining -= sizeof(size_t);
    return val;
}

static char *read_string(InputState *state) {
    if (state->remaining < sizeof(size_t)) return NULL;
    size_t len = read_size(state);
    if (len == 0 || len > 1024) return NULL; /* Limit length */
    if (state->remaining < len) return NULL;
    char *str = (char *)malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, state->ptr, len);
    str[len] = '\0';
    state->ptr += len;
    state->remaining -= len;
    return str;
}

/* Dummy callbacks for curl_multi_setopt */
static int dummy_push_callback(CURL *easy_handle,
                               struct curl_pushheaders *headers,
                               void *userptr) {
    (void)easy_handle;
    (void)headers;
    (void)userptr;
    return CURL_PUSH_DENY;
}

static int dummy_socket_callback(CURLM *multi_handle, curl_socket_t sockfd,
                                 int what, void *userptr, void *socketptr) {
    (void)multi_handle;
    (void)sockfd;
    (void)what;
    (void)userptr;
    (void)socketptr;
    return CURLM_OK;
}

static int dummy_timer_callback(CURLM *multi_handle, long timeout_ms, void *userptr) {
    (void)multi_handle;
    (void)timeout_ms;
    (void)userptr;
    return CURLM_OK;
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl globally once */
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* Initialize input state */
    InputState state;
    state.ptr = Data;
    state.remaining = Size;

    /* Test curl_multi_setopt with various options */
    int option = read_int(&state);
    
    /* Ensure option is within a safe range to avoid undefined behavior */
    if (option >= 0 && option <= 1000) {
        if (option == CURLMOPT_PUSHFUNCTION) {
            curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, dummy_push_callback);
        } else if (option == CURLMOPT_PIPELINING) {
            long pipelining = read_int(&state);
            curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);
        } else if (option == CURLMOPT_MAXCONNECTS) {
            long maxconnects = read_int(&state);
            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, maxconnects);
        } else if (option == CURLMOPT_MAX_HOST_CONNECTIONS) {
            long max_host_connections = read_int(&state);
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, max_host_connections);
        } else if (option == CURLMOPT_MAX_PIPELINE_LENGTH) {
            long max_pipeline_length = read_int(&state);
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_PIPELINE_LENGTH, max_pipeline_length);
        } else if (option == CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE) {
            long content_length_penalty_size = read_int(&state);
            curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, content_length_penalty_size);
        } else if (option == CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE) {
            long chunk_length_penalty_size = read_int(&state);
            curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, chunk_length_penalty_size);
        } else if (option == CURLMOPT_SOCKETFUNCTION) {
            curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
        } else if (option == CURLMOPT_SOCKETDATA) {
            void *socketdata = (void *)read_string(&state);
            curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, socketdata);
            if (socketdata) free(socketdata);
        } else if (option == CURLMOPT_TIMERFUNCTION) {
            curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
        } else if (option == CURLMOPT_TIMERDATA) {
            void *timerdata = (void *)read_string(&state);
            curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, timerdata);
            if (timerdata) free(timerdata);
        } else if (option == CURLMOPT_MAX_CONCURRENT_STREAMS) {
            long max_concurrent_streams = read_int(&state);
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_CONCURRENT_STREAMS, max_concurrent_streams);
        } else if (option == CURLMOPT_PUSHFUNCTION) {
             curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
        }
    }

    /* Test curl_multi_wakeup */
    if (Size > 0 && (Data[0] % 2 == 0)) {
        curl_multi_wakeup(multi_handle);
    }

    /* Test curl_version_info */
    if (Size > 0) {
        CURLversion version_type = (CURLversion)(Data[0] % 13);
        curl_version_info_data *info = curl_version_info(version_type);
        if (info) {
            /* Access some fields to ensure they are valid */
            (void)info->version;
            (void)info->version_num;
            (void)info->host;
            (void)info->features;
            (void)info->ssl_version;
            (void)info->libz_version;
            (void)info->ares;
            (void)info->libidn;
            (void)info->libssh_version;
            (void)info->brotli_version;
            (void)info->nghttp2_version;
            (void)info->quic_version;
            (void)info->cainfo;
            (void)info->capath;
            (void)info->zstd_version;
            (void)info->hyper_version;
            (void)info->gsasl_version;
            (void)info->rtmp_version;
        }
    }

    /* Cleanup */
    curl_multi_cleanup(multi_handle);

    return 0;
}