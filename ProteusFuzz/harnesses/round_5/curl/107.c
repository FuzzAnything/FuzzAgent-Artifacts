// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
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
// curl_multi_add_handle at multi.c:422:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Dummy callback implementations to satisfy type requirements */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_prereq_cb(CURL *curl, void *clientp) {
    (void)curl; (void)clientp;
    return 0;
}

static int dummy_resolver_start_cb(void *clientp, const char *hostname, int port,
                                   struct curl_sockaddr *addr, int numaddr) {
    (void)clientp; (void)hostname; (void)port; (void)addr; (void)numaddr;
    return 0;
}

static int dummy_ssh_hostkey_cb(CURL *easy, const char *host, int port,
                                const char *key, size_t keylen,
                                int keytype, void *userdata) {
    (void)easy; (void)host; (void)port; (void)key; (void)keylen;
    (void)keytype; (void)userdata;
    #ifdef CURLSSHOK
    return CURLSSHOK;
    #else
    return 0;
    #endif
}

static int dummy_push_cb(CURL *parent, CURL *easy, int num_headers,
                         struct curl_hentry *headers, void *userp) {
    (void)parent; (void)easy; (void)num_headers; (void)headers; (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    CURLM *multi = curl_multi_init();
    if (!multi) return 0;

    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    /* Initialize dummy file if needed for certain options */
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    /* Fuzz curl_easy_setopt with various option types using input data */
    long long_opt = (Size > 0) ? (long)Data[0] : 0;
    
    /* Ensure string is null-terminated for string options */
    char *safe_str = malloc(Size + 1);
    if (safe_str) {
        memcpy(safe_str, Data, Size);
        safe_str[Size] = '\0';
    }

    /* Set various options to exercise different code paths */
    if (safe_str) {
        curl_easy_setopt(easy, CURLOPT_URL, safe_str);
        curl_easy_setopt(easy, CURLOPT_POSTFIELDS, safe_str);
    }
    
    curl_easy_setopt(easy, CURLOPT_READFUNCTION, dummy_read_cb);
    curl_easy_setopt(easy, CURLOPT_READDATA, (void *)Data);
    curl_easy_setopt(easy, CURLOPT_PREREQFUNCTION, dummy_prereq_cb);
    curl_easy_setopt(easy, CURLOPT_PREREQDATA, (void *)Data);
    curl_easy_setopt(easy, CURLOPT_RESOLVER_START_FUNCTION, dummy_resolver_start_cb);
    curl_easy_setopt(easy, CURLOPT_RESOLVER_START_DATA, (void *)Data);
    curl_easy_setopt(easy, CURLOPT_SSH_HOSTKEYFUNCTION, dummy_ssh_hostkey_cb);
    curl_easy_setopt(easy, CURLOPT_SSH_HOSTKEYDATA, (void *)Data);
    
    if (safe_str) {
        curl_easy_setopt(easy, CURLOPT_POSTFIELDSIZE, (long)Size);
    }
    
    curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, long_opt & 1);
    curl_easy_setopt(easy, CURLOPT_TIMEOUT, (long)(long_opt & 0xFF));

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi, easy);

    /* Fuzz curl_multi_setopt */
    curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, dummy_push_cb);
    curl_multi_setopt(multi, CURLMOPT_PUSHDATA, (void *)Data);
    curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, (long)(long_opt & 0xFF));

    /* Fuzz curl_multi_wakeup */
    curl_multi_wakeup(multi);

    /* Cleanup */
    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);

    if (safe_str) free(safe_str);
    return 0;
}