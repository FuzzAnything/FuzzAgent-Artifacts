// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <curl/multi.h>

/* Dummy callback implementations to satisfy type requirements */
static size_t dummy_conv_callback(char *ptr, size_t size, void *userptr) {
    (void)ptr; (void)size; (void)userptr;
    return size;
}

static int dummy_chunk_bgn_callback(void *ptr, int files, void *infiles, void *outfiles) {
    (void)ptr; (void)files; (void)infiles; (void)outfiles;
    return CURL_CHUNK_BGN_FUNC_OK;
}

static int dummy_ssh_key_callback(CURL *curl, const struct curl_khkey *knownkey,
                                  const struct curl_khkey *foundkey,
                                  enum curl_khmatch match, void *userptr) {
    (void)curl; (void)knownkey; (void)foundkey; (void)match; (void)userptr;
    return CURLKHSTAT_FINE_REPLACE;
}

static int dummy_push_callback(CURL *parent, CURL *easy, size_t num_headers,
                               struct curl_slist *headers, void *userp) {
    (void)parent; (void)easy; (void)num_headers; (void)headers; (void)userp;
    return 1; /* Accept */
}

/* Helper to safely create a multi handle */
static CURLM* safe_curl_multi_init(void) {
    return curl_multi_init();
}

/* Helper to safely create an easy handle */
static CURL* safe_curl_easy_init(void) {
    return curl_easy_init();
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize global curl environment once per run (idempotent) */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    CURLM *multi = safe_curl_multi_init();
    CURL *easy = safe_curl_easy_init();

    if (!multi || !easy) {
        curl_global_cleanup();
        return 0;
    }

    /* Prepare data buffers from fuzz input */
    char *fuzz_str = (char *)malloc(Size + 1);
    if (!fuzz_str) {
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }
    memcpy(fuzz_str, Data, Size);
    fuzz_str[Size] = '\0';

    /* Fuzz curl_multi_setopt with various options */
    /* Test CURLMOPT_PUSHFUNCTION */
    curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, dummy_push_callback);
    curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, NULL);

    /* Test other multi options if available in the build */
    curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, (long)Size % 100);
    curl_multi_setopt(multi, CURLMOPT_PIPELINING, (long)(Size % 3));

    /* Fuzz curl_easy_setopt with various callback types */
    
    /* Test conversion callbacks (Wcurl_easy_setopt_err_conv_cb context) */
    curl_easy_setopt(easy, CURLOPT_CONV_TO_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(easy, CURLOPT_CONV_FROM_NETWORK_FUNCTION, dummy_conv_callback);
    curl_easy_setopt(easy, CURLOPT_CONV_FROM_UTF8_FUNCTION, dummy_conv_callback);

    /* Test chunk callbacks (Wcurl_easy_setopt_err_chunk_bgn_cb context) */
    curl_easy_setopt(easy, CURLOPT_CHUNK_BGN_FUNCTION, dummy_chunk_bgn_callback);
    curl_easy_setopt(easy, CURLOPT_CHUNK_END_FUNCTION, NULL);

    /* Test SSH key callbacks (Wcurl_easy_setopt_err_ssh_key_cb context) */
    curl_easy_setopt(easy, CURLOPT_SSH_KEYFUNCTION, dummy_ssh_key_callback);
    curl_easy_setopt(easy, CURLOPT_SSH_KEYDATA, (void*)fuzz_str);

    /* Test string options */
    curl_easy_setopt(easy, CURLOPT_URL, fuzz_str);
    curl_easy_setopt(easy, CURLOPT_USERAGENT, fuzz_str);

    /* Test slist options */
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, fuzz_str);
    curl_easy_setopt(easy, CURLOPT_HTTPHEADER, headers);
    
    /* Test long options */
    curl_easy_setopt(easy, CURLOPT_TIMEOUT, (long)(Size % 60));
    curl_easy_setopt(easy, CURLOPT_CONNECTTIMEOUT, (long)(Size % 30));

    /* Test data pointers */
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, (void*)fuzz_str);
    curl_easy_setopt(easy, CURLOPT_READDATA, (void*)fuzz_str);

    /* Fuzz curl_multi_wakeup */
    curl_multi_wakeup(multi);

    /* Cleanup */
    curl_slist_free_all(headers);
    free(fuzz_str);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_global_cleanup();

    return 0;
}