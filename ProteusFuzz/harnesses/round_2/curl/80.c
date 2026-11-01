// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>
#include <stdarg.h>

/* Dummy callback implementations to satisfy type checks */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return size * nmemb;
}

static int dummy_conv_callback(char *ptr, size_t len, int in, void *userdata) {
    (void)ptr; (void)len; (void)in; (void)userdata;
    return 0;
}

static int dummy_seek_callback(void *inclientdata, curl_off_t offset, int origin) {
    (void)inclientdata; (void)offset; (void)origin;
    return 0;
}

/* Helper to safely parse integers from fuzz input */
static long get_long_from_input(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(long) > Size) {
        return 0;
    }
    long val;
    memcpy(&val, Data + *offset, sizeof(long));
    *offset += sizeof(long);
    return val;
}

/* Helper to safely parse size_t from fuzz input */
static size_t get_size_t_from_input(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(size_t) > Size) {
        return 0;
    }
    size_t val;
    memcpy(&val, Data + *offset, sizeof(size_t));
    *offset += sizeof(size_t);
    return val;
}

/* Helper to get a null-terminated string from fuzz input */
static char* get_string_from_input(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset >= Size) return NULL;
    size_t len = 0;
    while (*offset + len < Size && Data[*offset + len] != '\0') {
        len++;
    }
    if (*offset + len >= Size) return NULL;
    
    char *str = malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, Data + *offset, len);
    str[len] = '\0';
    *offset += len + 1;
    return str;
}

/* Helper varargs function to fuzz curl_mvsnprintf */
static void fuzz_mvsnprintf_varargs(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[256];
    curl_mvsnprintf(buffer, 256, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize curl */
    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    char error_buffer[CURL_ERROR_SIZE];
    memset(error_buffer, 0, sizeof(error_buffer));

    size_t offset = 0;

    /* Fuzz curl_strnequal */
    if (offset + 1 < Size) {
        int mode = Data[offset++];
        char *s1 = NULL, *s2 = NULL;
        size_t n = 0;

        if (mode & 1) s1 = get_string_from_input(Data, Size, &offset);
        if (mode & 2) s2 = get_string_from_input(Data, Size, &offset);
        
        if (offset + sizeof(size_t) <= Size) {
            memcpy(&n, Data + offset, sizeof(size_t));
            offset += sizeof(size_t);
        }

        if (s1 && s2) {
            curl_strnequal(s1, s2, n);
        } else if (s1) {
            curl_strnequal(s1, NULL, n);
            curl_strnequal(NULL, s1, n);
        } else if (s2) {
            curl_strnequal(NULL, s2, n);
            curl_strnequal(s2, NULL, n);
        } else {
            curl_strnequal(NULL, NULL, n);
        }

        free(s1);
        free(s2);
    }

    /* Fuzz curl_mvsnprintf via helper */
    if (offset + 1 < Size) {
        int mode = Data[offset++];
        const char *format = "%s";
        
        char *fmt_str = get_string_from_input(Data, Size, &offset);
        if (fmt_str) {
            format = fmt_str;
        }

        int dummy_int = 0;
        if (offset + sizeof(int) <= Size) {
            memcpy(&dummy_int, Data + offset, sizeof(int));
            offset += sizeof(int);
        }

        if (fmt_str && strlen(fmt_str) > 0) {
            /* Try to match format to arguments if possible, otherwise use safe defaults */
            if (strstr(fmt_str, "%d")) {
                fuzz_mvsnprintf_varargs(fmt_str, "fuzz", dummy_int);
            } else {
                fuzz_mvsnprintf_varargs(fmt_str, "fuzz");
            }
        } else {
            fuzz_mvsnprintf_varargs("%s", "fuzz");
        }
        
        free((void*)fmt_str);
    }

    /* Fuzz curl_easy_setopt with various option types */
    if (offset + 1 < Size) {
        int opt_mode = Data[offset++];

        /* CURLOPT_URL (string) */
        if (opt_mode & 1) {
            char *url = get_string_from_input(Data, Size, &offset);
            if (url) {
                curl_easy_setopt(curl, CURLOPT_URL, url);
                free(url);
            }
        }

        /* CURLOPT_WRITEFUNCTION (callback) */
        if (opt_mode & 2) {
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        }

        /* CURLOPT_READFUNCTION (callback) */
        if (opt_mode & 4) {
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
        }

        /* CURLOPT_CONV_TO_NETWORK_FUNCTION (conv callback) */
        if (opt_mode & 8) {
            curl_easy_setopt(curl, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_callback);
        }

        /* CURLOPT_CONV_FROM_NETWORK_FUNCTION (conv callback) */
        if (opt_mode & 16) {
            curl_easy_setopt(curl, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_callback);
        }

        /* CURLOPT_CONV_FROM_UTF8_FUNCTION (conv callback) */
        if (opt_mode & 32) {
            curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_callback);
        }

        /* CURLOPT_SEEKFUNCTION (seek callback) */
        if (opt_mode & 64) {
            curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_callback);
        }

        /* CURLOPT_ERRORBUFFER (char buffer) */
        if (opt_mode & 128) {
            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);
        }

        /* CURLOPT_POSTFIELDS (void* or char*) */
        if (opt_mode & 256) {
            char *post = get_string_from_input(Data, Size, &offset);
            if (post) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post);
                free(post);
            }
        }

        /* CURLOPT_POSTFIELDSIZE (long) */
        if (opt_mode & 512) {
            long size = get_long_from_input(Data, Size, &offset);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, size);
        }

        /* CURLOPT_TIMEOUT (long) */
        if (opt_mode & 1024) {
            long timeout = get_long_from_input(Data, Size, &offset);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        }

        /* CURLOPT_FOLLOWLOCATION (long) */
        if (opt_mode & 2048) {
            long follow = get_long_from_input(Data, Size, &offset);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, follow);
        }
    }

    /* Fuzz curl_easy_getinfo */
    if (offset + 1 < Size) {
        int info_mode = Data[offset++];
        char *str_info;
        long long_info;
        double double_info;
        struct curl_slist *slist_info;
        struct curl_tlssessioninfo tls_info;
        struct curl_certinfo cert_info;
        curl_socket_t socket_info;
        curl_off_t off_t_info;

        memset(&tls_info, 0, sizeof(tls_info));
        memset(&cert_info, 0, sizeof(cert_info));
        socket_info = 0;
        off_t_info = 0;

        if (info_mode & 1) {
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &str_info);
        }
        if (info_mode & 2) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &long_info);
        }
        if (info_mode & 4) {
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &double_info);
        }
        if (info_mode & 8) {
            curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &slist_info);
            if (slist_info) curl_slist_free_all(slist_info);
        }
        if (info_mode & 16) {
            curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_info);
        }
        if (info_mode & 32) {
            curl_easy_getinfo(curl, CURLINFO_CERTINFO, &cert_info);
        }
        if (info_mode & 64) {
            curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &socket_info);
        }
        if (info_mode & 128) {
            curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &off_t_info);
        }
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    return 0;
}