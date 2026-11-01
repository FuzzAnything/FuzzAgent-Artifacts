// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
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
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Fuzz driver for curl_easy_setopt and related type-checking macros.
 * The target APIs listed (Wcurl_easy_setopt_err_*) are compile-time warning macros,
 * not runtime functions. This driver focuses on fuzzing curl_easy_setopt with diverse
 * inputs to exercise the underlying logic and type-checking paths.
 */

/* Dummy callback implementations to satisfy type requirements */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

static int dummy_seek_cb(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return CURL_SEEKFUNC_OK;
}

static int dummy_progress_cb(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp; (void)dltotal; (void)dlnow; (void)ultotal; (void)ulnow;
    return 0;
}

static int dummy_debug_cb(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle; (void)type; (void)data; (void)size; (void)userptr;
    return 0;
}

static int dummy_conv_cb(char *in, size_t inlen, char *out, size_t outlen, size_t *outlenset) {
    (void)in; (void)inlen; (void)out; (void)outlen; (void)outlenset;
    return 0;
}

static int dummy_trailer_cb(CURL *curl, char *key, size_t keylen, char *val, size_t vallen, void *userp) {
    (void)curl; (void)key; (void)keylen; (void)val; (void)vallen; (void)userp;
    return 0;
}

static int dummy_resolver_start_cb(CURL *curl, void *resolver_start_data, void *userp) {
    (void)curl; (void)resolver_start_data; (void)userp;
    return 0;
}

/* Helper to create a null-terminated string from fuzz input */
static char *create_safe_string(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    char *str = (char *)malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    /* Prepare a safe string for URL option */
    char *url_str = create_safe_string(Data, Size);
    if (!url_str) {
        curl_easy_cleanup(curl);
        return 0;
    }

    /* Fuzz various options with diverse inputs */
    
    /* 1. Test CURLOPT_URL with fuzzed string */
    curl_easy_setopt(curl, CURLOPT_URL, url_str);

    /* 2. Test long options with fuzzed data interpreted as long */
    long fuzz_long = 0;
    if (Size >= sizeof(long)) {
        memcpy(&fuzz_long, Data, sizeof(long));
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, fuzz_long);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, fuzz_long);
    }

    /* 3. Test callback options */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, dummy_seek_cb);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, dummy_progress_cb);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, dummy_debug_cb);
    curl_easy_setopt(curl, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_cb);
    curl_easy_setopt(curl, CURLOPT_TRAILERFUNCTION, dummy_trailer_cb);
    curl_easy_setopt(curl, CURLOPT_RESOLVER_START_FUNCTION, dummy_resolver_start_cb);

    /* 4. Test data pointers for callbacks */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_SEEKDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_DEBUGDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_TRAILERDATA, (void *)Data);
    curl_easy_setopt(curl, CURLOPT_RESOLVER_START_DATA, (void *)Data);

    /* 5. Test string options with fuzzed string */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, url_str);
    curl_easy_setopt(curl, CURLOPT_REFERER, url_str);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, url_str);

    /* 6. Test slist options */
    struct curl_slist *slist = NULL;
    slist = curl_slist_append(slist, url_str);
    if (slist) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_HTTP200ALIASES, slist);
        curl_slist_free_all(slist);
    }

    /* 7. Test POST fields */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, url_str);
    curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, url_str);

    /* 8. Test error buffer */
    char error_buffer[CURL_ERROR_SIZE];
    memset(error_buffer, 0, CURL_ERROR_SIZE);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    /* 9. Test off_t options */
    curl_off_t fuzz_off = 0;
    if (Size >= sizeof(curl_off_t)) {
        memcpy(&fuzz_off, Data, sizeof(curl_off_t));
        curl_easy_setopt(curl, CURLOPT_RESUME_FROM, fuzz_off);
    }

    /* 10. Test file pointer options (using dummy file) */
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        fp = fopen("./dummy_file", "r");
        if (fp) {
            curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)Size);
            curl_easy_setopt(curl, CURLOPT_READDATA, fp);
            fclose(fp);
        }
    }

    /* Cleanup */
    curl_easy_cleanup(curl);
    free(url_str);

    return 0;
}