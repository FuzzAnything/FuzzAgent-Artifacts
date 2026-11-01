// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Static helper to safely initialize curl_multi handle */
static CURLM *init_multi_handle(void) {
    CURLM *multi = curl_multi_init();
    return multi;
}

/* Static helper to safely cleanup curl_multi handle */
static void cleanup_multi_handle(CURLM *multi) {
    if (multi) {
        curl_multi_cleanup(multi);
    }
}

/* Static helper to safely initialize curl_easy handle */
static CURL *init_easy_handle(void) {
    return curl_easy_init();
}

/* Static helper to safely cleanup curl_easy handle */
static void cleanup_easy_handle(CURL *easy) {
    if (easy) {
        curl_easy_cleanup(easy);
    }
}

/* Dummy callback for testing */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

/* Dummy callback for testing */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    CURLM *multi = init_multi_handle();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }

    CURL *easy = init_easy_handle();
    if (!easy) {
        cleanup_multi_handle(multi);
        curl_global_cleanup();
        return 0;
    }

    /* Prepare dummy file if needed */
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Fuzz curl_multi_socket */
    curl_socket_t sock = -1;
    if (Size >= sizeof(curl_socket_t)) {
        memcpy(&sock, Data, sizeof(curl_socket_t));
    }
    int running_handles = 0;
    curl_multi_socket(multi, sock, &running_handles);

    /* Fuzz curl_multi_socket_all */
    curl_multi_socket_all(multi, &running_handles);

    /* Fuzz curl_easy_setopt with various options to trigger type checks */
    if (Size >= 1) {
        long opt_long = (long)Data[0];
        curl_easy_setopt(easy, CURLOPT_TIMEOUT, opt_long);
    }

    if (Size >= 2) {
        char *str_data = (char *)malloc(Size + 1);
        if (str_data) {
            memcpy(str_data, Data, Size);
            str_data[Size] = '\0';
            curl_easy_setopt(easy, CURLOPT_URL, str_data);
            free(str_data);
        }
    }

    /* Test callback options */
    curl_easy_setopt(easy, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, dummy_write_callback);

    /* Test slist options */
    struct curl_slist *list = NULL;
    if (Size >= 4) {
        char header[256];
        size_t header_len = Size < 255 ? Size : 255;
        memcpy(header, Data, header_len);
        header[header_len] = '\0';
        list = curl_slist_append(list, header);
    }
    if (list) {
        curl_easy_setopt(easy, CURLOPT_HTTPHEADER, list);
        curl_slist_free_all(list);
    }

    /* Test postfields */
    if (Size > 0) {
        curl_easy_setopt(easy, CURLOPT_POSTFIELDS, (void *)Data);
        curl_easy_setopt(easy, CURLOPT_POSTFIELDSIZE, (long)Size);
    }

    /* Cleanup */
    cleanup_easy_handle(easy);
    cleanup_multi_handle(multi);

    /* Cleanup dummy file */
    remove(dummy_file);

    curl_global_cleanup();
    return 0;
}