// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_formadd at formdata.c:609:14 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_formfree at formdata.c:665:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_append at slist.c:85:20 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_slist_free_all at slist.c:124:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/typecheck-gcc.h>

/* Define callback function outside of LLVMFuzzerTestOneInput to avoid syntax errors */
static size_t dummy_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    size_t idx = 0;

    /* 1. Set URL */
    if (idx < Size) {
        char url_buf[256];
        size_t url_len = (Size - idx < 255) ? (Size - idx) : 255;
        memcpy(url_buf, Data + idx, url_len);
        url_buf[url_len] = '\0';
        idx += url_len;

        if (url_len > 0) {
            curl_easy_setopt(curl, CURLOPT_URL, url_buf);
        }
    }

    /* 2. Fuzz curl_easy_setopt with various option types */
    while (idx < Size) {
        unsigned char option_byte = Data[idx];
        idx++;

        if (idx >= Size) break;

        switch (option_byte % 10) {
            case 0: {
                /* Long option: CURLOPT_TIMEOUT */
                long timeout = (long)(Data[idx] & 0xFF);
                idx++;
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
                break;
            }
            case 1: {
                /* String option: CURLOPT_USERAGENT */
                size_t str_len = 0;
                if (idx < Size) {
                    str_len = Data[idx];
                    idx++;
                }
                if (str_len > 0 && idx + str_len <= Size) {
                    char *str = (char *)(Data + idx);
                    idx += str_len;
                    char *copy = malloc(str_len + 1);
                    if (copy) {
                        memcpy(copy, str, str_len);
                        copy[str_len] = '\0';
                        curl_easy_setopt(curl, CURLOPT_USERAGENT, copy);
                        free(copy);
                    }
                }
                break;
            }
            case 2: {
                /* curl_slist option: CURLOPT_HTTPHEADER */
                struct curl_slist *list = NULL;
                if (idx < Size) {
                    size_t str_len = Data[idx];
                    idx++;
                    if (str_len > 0 && idx + str_len <= Size) {
                        char *str = (char *)(Data + idx);
                        idx += str_len;
                        char *copy = malloc(str_len + 1);
                        if (copy) {
                            memcpy(copy, str, str_len);
                            copy[str_len] = '\0';
                            list = curl_slist_append(list, copy);
                            free(copy);
                        }
                    }
                }
                if (list) {
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
                    curl_slist_free_all(list);
                }
                break;
            }
            case 3: {
                /* POST data: CURLOPT_POSTFIELDS */
                size_t data_len = 0;
                if (idx < Size) {
                    data_len = Data[idx];
                    idx++;
                }
                if (data_len > 0 && idx + data_len <= Size) {
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (const char *)(Data + idx));
                    idx += data_len;
                }
                break;
            }
            case 4: {
                /* Callback option: CURLOPT_WRITEFUNCTION */
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_cb);
                break;
            }
            case 5: {
                /* CURL * option: CURLOPT_STREAM_DEPENDS */
                curl_easy_setopt(curl, CURLOPT_STREAM_DEPENDS, NULL);
                break;
            }
            case 6: {
                /* FILE * option: CURLOPT_READDATA */
                curl_easy_setopt(curl, CURLOPT_READDATA, NULL);
                break;
            }
            case 7: {
                /* curl_off_t option: CURLOPT_MAXFILESIZE */
                curl_off_t maxsize = (curl_off_t)(Data[idx] & 0xFF);
                idx++;
                curl_easy_setopt(curl, CURLOPT_MAXFILESIZE, maxsize);
                break;
            }
            case 8: {
                /* CURLOPT_HTTPPOST (struct curl_httppost *) */
                struct curl_httppost *post = NULL;
                struct curl_httppost *last = NULL;
                if (idx < Size) {
                    size_t name_len = Data[idx];
                    idx++;
                    if (idx < Size) {
                        size_t val_len = Data[idx];
                        idx++;
                        if (idx + name_len + val_len <= Size) {
                            char *name = (char *)(Data + idx);
                            idx += name_len;
                            char *val = (char *)(Data + idx);
                            idx += val_len;
                            curl_formadd(&post, &last,
                                CURLFORM_COPYNAME, name,
                                CURLFORM_COPYCONTENTS, val,
                                CURLFORM_END);
                        }
                    }
                }
                if (post) {
                    curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
                    curl_formfree(post);
                }
                break;
            }
            case 9: {
                /* CURLOPT_INFILESIZE_LARGE */
                curl_off_t size = (curl_off_t)(Data[idx] & 0xFF);
                idx++;
                curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, size);
                break;
            }
        }
    }

    /* 3. Invoke curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /* 4. Invoke curl_easy_getinfo with various CURLINFO options */
    if (idx < Size) {
        unsigned char info_type = Data[idx] % 10;
        idx++;

        switch (info_type) {
            case 0: {
                long response_code;
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
                break;
            }
            case 1: {
                double download_size;
                curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &download_size);
                break;
            }
            case 2: {
                char *url;
                curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
                (void)url;
                break;
            }
            case 3: {
                long connect_code;
                curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &connect_code);
                break;
            }
            case 4: {
                double total_time;
                curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
                break;
            }
            case 5: {
                struct curl_slist *cookies;
                curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
                if (cookies) {
                    curl_slist_free_all(cookies);
                }
                break;
            }
            case 6: {
                struct curl_certinfo *certinfo;
                curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);
                (void)certinfo;
                break;
            }
            case 7: {
                struct curl_tlssessioninfo *tls_info;
                curl_easy_getinfo(curl, CURLINFO_TLS_SESSION, &tls_info);
                (void)tls_info;
                break;
            }
            case 8: {
                char *private_data;
                curl_easy_getinfo(curl, CURLINFO_PRIVATE, &private_data);
                (void)private_data;
                break;
            }
            case 9: {
                long http_version;
                curl_easy_getinfo(curl, CURLINFO_HTTP_VERSION, &http_version);
                break;
            }
        }
    }

    /* 5. Cleanup */
    curl_easy_cleanup(curl);

    return 0;
}