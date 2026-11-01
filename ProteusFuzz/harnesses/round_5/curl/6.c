// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_mime_free at mime.c:1082:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

/* Helper to create a simple slist from fuzz data */
static struct curl_slist *create_slist_from_data(const uint8_t *data, size_t size) {
    struct curl_slist *list = NULL;
    size_t idx = 0;
    
    while (idx < size) {
        size_t len = 0;
        while (idx + len < size && data[idx + len] != '\0' && len < 1024) {
            len++;
        }
        
        if (len > 0) {
            char *str = (char *)malloc(len + 1);
            if (str) {
                memcpy(str, &data[idx], len);
                str[len] = '\0';
                struct curl_slist *new_entry = curl_slist_append(list, str);
                if (new_entry) {
                    list = new_entry;
                } else {
                    free(str);
                }
            }
        }
        idx += len + 1;
    }
    return list;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Initialize global curl environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    /* Use fuzz data to set URL and other options */
    char url_buf[256];
    size_t url_len = (Size > 255) ? 255 : Size;
    memcpy(url_buf, Data, url_len);
    url_buf[url_len] = '\0';

    /* Set a dummy URL to ensure perform has a target */
    if (curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/dummy") != CURLE_OK) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    /* Set timeout to avoid hanging */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

    /* Test curl_easy_perform */
    curl_easy_perform(curl);

    /* Test curl_easy_pause with different bitmasks derived from input */
    int pause_mask = Data[0] & 0x03; /* CURLPAUSE_RECV | CURLPAUSE_SEND */
    curl_easy_pause(curl, pause_mask);
    curl_easy_pause(curl, CURLPAUSE_CONT);

    /* Test curl_share_init */
    CURLSH *share = curl_share_init();
    if (share) {
        curl_easy_setopt(curl, CURLOPT_SHARE, share);
        curl_easy_perform(curl);
        curl_easy_upkeep(curl);
        /* Ensure cleanup of share handle to prevent leaks */
        curl_share_cleanup(share);
        /* Remove share from curl handle to avoid double free or dangling ref */
        curl_easy_setopt(curl, CURLOPT_SHARE, NULL);
    }

    /* Test curl_mime functions */
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            /* Set data from fuzz input */
            if (Size > 10) {
                curl_mime_data(part, (char *)Data, Size - 10);
            }
            
            /* Create headers list */
            struct curl_slist *headers = create_slist_from_data(Data + 10, Size > 10 ? Size - 10 : 0);
            if (headers) {
                curl_mime_headers(part, headers, 1); /* Take ownership */
            }

            /* Set filename and name */
            if (Size > 20) {
                char fname[32];
                size_t fname_len = (Size - 20 > 31) ? 31 : Size - 20;
                memcpy(fname, Data + 20, fname_len);
                fname[fname_len] = '\0';
                curl_mime_name(part, fname);
                curl_mime_filename(part, fname);
            }
        }
        
        /* Set MIME as POST data */
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        curl_easy_perform(curl);
        
        /* Free mime structure */
        curl_mime_free(mime);
        /* Clear the option to ensure no dangling pointers */
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, NULL);
    }

    /* Test curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /* Cleanup */
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}