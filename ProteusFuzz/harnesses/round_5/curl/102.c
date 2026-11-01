// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
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
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/* Dummy callback implementations to satisfy type checking macros */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr; (void)size; (void)nmemb; (void)userdata;
    return 0;
}

static int dummy_seek_cb(void *instream, curl_off_t offset, int origin) {
    (void)instream; (void)offset; (void)origin;
    return 0; /* CURL_SEEK_OK is 0, but we use literal to avoid missing enum if not included */
}

static int dummy_hstsread_cb(void *userdata, char *buf, size_t size) {
    (void)userdata; (void)buf; (void)size;
    return 0;
}

static int dummy_hstswrite_cb(void *userdata, const char *buf, size_t size) {
    (void)userdata; (void)buf; (void)size;
    return 0;
}

static int dummy_notify_cb(CURLM *multi, void *userdata) {
    (void)multi; (void)userdata;
    return 0;
}

static int dummy_socket_cb(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    (void)easy; (void)s; (void)what; (void)userp; (void)socketp;
    return 0;
}

static int dummy_timer_cb(CURLM *multi, long timeout_ms, void *userp) {
    (void)multi; (void)timeout_ms; (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Initialize a dummy file for HSTS callbacks if needed */
    FILE *fp = fopen("./dummy_file", "w+");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    /* Test curl_multi_setopt with various options using fuzz data */
    if (Size >= 1) {
        unsigned int option = Data[0] % 100;
        
        /* Test CURLMOPT_PIPELINING (long) */
        if (option == 0) {
            curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)(Data[0] & 0xFF));
        }
        /* Test CURLMOPT_MAX_HOST_CONNECTIONS (long) */
        else if (option == 1) {
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, (long)(Data[0] & 0xFF));
        }
        /* Test CURLMOPT_MAX_TOTAL_CONNECTIONS (long) */
        else if (option == 2) {
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, (long)(Data[0] & 0xFF));
        }
        /* Test CURLMOPT_MAXCONNECTS (long) */
        else if (option == 3) {
            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)(Data[0] & 0xFF));
        }
        /* Test CURLMOPT_PUSHFUNCTION (callback) */
        else if (option == 4) {
            /* We use a dummy callback, but the macro check is what matters */
            curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, dummy_notify_cb);
            curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, (void*)(intptr_t)(Data[0]));
        }
        /* Test CURLMOPT_SOCKETFUNCTION (callback) */
        else if (option == 5) {
            curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_cb);
            curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, (void*)(intptr_t)(Data[0]));
        }
        /* Test CURLMOPT_TIMERFUNCTION (callback) */
        else if (option == 6) {
            curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_cb);
            curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, (void*)(intptr_t)(Data[0]));
        }
        /* Test CURLMOPT_MAXCONNECTS (long) again with different data */
        else if (option == 7) {
            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)(Size));
        }
        /* Test CURLMOPT_PIPELINING (long) with different data */
        else if (option == 8) {
            curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)(Size % 2));
        }
        /* Test CURLMOPT_MAX_HOST_CONNECTIONS (long) with different data */
        else if (option == 9) {
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, (long)(Size % 10));
        }
        /* Test CURLMOPT_MAX_TOTAL_CONNECTIONS (long) with different data */
        else if (option == 10) {
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, (long)(Size % 10));
        }
    }

    /* Test curl_multi_wakeup */
    curl_multi_wakeup(multi_handle);

    /* Test curl_easy_setopt with various options using fuzz data */
    if (Size >= 1) {
        unsigned int easy_option = Data[0] % 20;
        
        /* Test CURLOPT_READFUNCTION (callback) */
        if (easy_option == 0) {
            curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_cb);
            curl_easy_setopt(easy_handle, CURLOPT_READDATA, (void*)(intptr_t)(Data[0]));
        }
        /* Test CURLOPT_SEEKFUNCTION (callback) */
        else if (easy_option == 1) {
            curl_easy_setopt(easy_handle, CURLOPT_SEEKFUNCTION, dummy_seek_cb);
            curl_easy_setopt(easy_handle, CURLOPT_SEEKDATA, (void*)(intptr_t)(Data[0]));
        }
        /* Test CURLOPT_HSTSREADFUNCTION (callback) */
        else if (easy_option == 2) {
            curl_easy_setopt(easy_handle, CURLOPT_HSTSREADFUNCTION, dummy_hstsread_cb);
            curl_easy_setopt(easy_handle, CURLOPT_HSTSREADDATA, (void*)(intptr_t)(Data[0]));
        }
        /* Test CURLOPT_HSTSWRITEFUNCTION (callback) */
        else if (easy_option == 3) {
            curl_easy_setopt(easy_handle, CURLOPT_HSTSWRITEFUNCTION, dummy_hstswrite_cb);
            curl_easy_setopt(easy_handle, CURLOPT_HSTSWRITEDATA, (void*)(intptr_t)(Data[0]));
        }
        /* Test CURLOPT_URL (string) */
        else if (easy_option == 4) {
            char url[1024];
            size_t len = Size < 1023 ? Size : 1023;
            memcpy(url, Data, len);
            url[len] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_URL, url);
        }
        /* Test CURLOPT_POSTFIELDS (void*) */
        else if (easy_option == 5) {
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, (void*)Data);
            curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, (long)Size);
        }
        /* Test CURLOPT_FOLLOWLOCATION (long) */
        else if (easy_option == 6) {
            curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, (long)(Data[0] & 0xFF));
        }
        /* Test CURLOPT_TIMEOUT (long) */
        else if (easy_option == 7) {
            curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, (long)(Data[0] & 0xFF));
        }
        /* Test CURLOPT_CONNECTTIMEOUT (long) */
        else if (easy_option == 8) {
            curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, (long)(Data[0] & 0xFF));
        }
        /* Test CURLOPT_MAXREDIRS (long) */
        else if (easy_option == 9) {
            curl_easy_setopt(easy_handle, CURLOPT_MAXREDIRS, (long)(Data[0] & 0xFF));
        }
        /* Test CURLOPT_VERBOSE (long) */
        else if (easy_option == 10) {
            curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, (long)(Data[0] & 0xFF));
        }
        /* Test CURLOPT_USERAGENT (string) */
        else if (easy_option == 11) {
            char ua[1024];
            size_t len = Size < 1023 ? Size : 1023;
            memcpy(ua, Data, len);
            ua[len] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, ua);
        }
        /* Test CURLOPT_REFERER (string) */
        else if (easy_option == 12) {
            char ref[1024];
            size_t len = Size < 1023 ? Size : 1023;
            memcpy(ref, Data, len);
            ref[len] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_REFERER, ref);
        }
        /* Test CURLOPT_COOKIE (string) */
        else if (easy_option == 13) {
            char cookie[1024];
            size_t len = Size < 1023 ? Size : 1023;
            memcpy(cookie, Data, len);
            cookie[len] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_COOKIE, cookie);
        }
        /* Test CURLOPT_PROXY (string) */
        else if (easy_option == 14) {
            char proxy[1024];
            size_t len = Size < 1023 ? Size : 1023;
            memcpy(proxy, Data, len);
            proxy[len] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_PROXY, proxy);
        }
        /* Test CURLOPT_INTERFACE (string) */
        else if (easy_option == 15) {
            char iface[1024];
            size_t len = Size < 1023 ? Size : 1023;
            memcpy(iface, Data, len);
            iface[len] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_INTERFACE, iface);
        }
        /* Test CURLOPT_CAINFO (string) */
        else if (easy_option == 16) {
            char cafile[1024];
            size_t len = Size < 1023 ? Size : 1023;
            memcpy(cafile, Data, len);
            cafile[len] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_CAINFO, cafile);
        }
        /* Test CURLOPT_CAPATH (string) */
        else if (easy_option == 17) {
            char capath[1024];
            size_t len = Size < 1023 ? Size : 1023;
            memcpy(capath, Data, len);
            capath[len] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_CAPATH, capath);
        }
        /* Test CURLOPT_SSLCERT (string) */
        else if (easy_option == 18) {
            char sslcert[1024];
            size_t len = Size < 1023 ? Size : 1023;
            memcpy(sslcert, Data, len);
            sslcert[len] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_SSLCERT, sslcert);
        }
        /* Test CURLOPT_SSLKEY (string) */
        else if (easy_option == 19) {
            char sslkey[1024];
            size_t len = Size < 1023 ? Size : 1023;
            memcpy(sslkey, Data, len);
            sslkey[len] = '\0';
            curl_easy_setopt(easy_handle, CURLOPT_SSLKEY, sslkey);
        }
    }

    /* Test curl_easy_getinfo with various info types */
    if (Size >= 1) {
        unsigned int info_option = Data[0] % 10;
        double d_val;
        long l_val;
        char *s_val;
        struct curl_slist *slist_val;
        curl_socket_t socket_val;
        curl_off_t off_val;

        /* Test CURLINFO_TOTAL_TIME (double) */
        if (info_option == 0) {
            curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &d_val);
        }
        /* Test CURLINFO_NAMELOOKUP_TIME (double) */
        else if (info_option == 1) {
            curl_easy_getinfo(easy_handle, CURLINFO_NAMELOOKUP_TIME, &d_val);
        }
        /* Test CURLINFO_CONNECT_TIME (double) */
        else if (info_option == 2) {
            curl_easy_getinfo(easy_handle, CURLINFO_CONNECT_TIME, &d_val);
        }
        /* Test CURLINFO_PRETRANSFER_TIME (double) */
        else if (info_option == 3) {
            curl_easy_getinfo(easy_handle, CURLINFO_PRETRANSFER_TIME, &d_val);
        }
        /* Test CURLINFO_STARTTRANSFER_TIME (double) */
        else if (info_option == 4) {
            curl_easy_getinfo(easy_handle, CURLINFO_STARTTRANSFER_TIME, &d_val);
        }
        /* Test CURLINFO_SIZE_UPLOAD (double) */
        else if (info_option == 5) {
            curl_easy_getinfo(easy_handle, CURLINFO_SIZE_UPLOAD, &d_val);
        }
        /* Test CURLINFO_SIZE_DOWNLOAD (double) */
        else if (info_option == 6) {
            curl_easy_getinfo(easy_handle, CURLINFO_SIZE_DOWNLOAD, &d_val);
        }
        /* Test CURLINFO_SPEED_DOWNLOAD (double) */
        else if (info_option == 7) {
            curl_easy_getinfo(easy_handle, CURLINFO_SPEED_DOWNLOAD, &d_val);
        }
        /* Test CURLINFO_SPEED_UPLOAD (double) */
        else if (info_option == 8) {
            curl_easy_getinfo(easy_handle, CURLINFO_SPEED_UPLOAD, &d_val);
        }
        /* Test CURLINFO_HTTP_CODE (long) */
        else if (info_option == 9) {
            curl_easy_getinfo(easy_handle, CURLINFO_HTTP_CODE, &l_val);
        }
    }

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}