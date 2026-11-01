// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
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

    uint8_t flag1 = Data[0] & 0xFF;
    uint8_t flag2 = (Size > 1) ? Data[1] & 0xFF : 0;
    uint8_t flag3 = (Size > 2) ? Data[2] & 0xFF : 0;

    if (flag1 & 0x01) {
        curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
    }
    if (flag1 & 0x02) {
        int max_conns = (flag2 % 10) + 1;
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_conns);
    }
    if (flag1 & 0x04) {
        long pipebuf = (flag2 % 1024) + 64;
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipebuf);
    }

    if (flag2 & 0x01) {
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    if (flag2 & 0x02) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }

    if (flag3 & 0x01) {
        curl_socket_t sock = -1;
        void *sockp = (void *)(uintptr_t)(flag2 % 256);
        curl_multi_assign(multi_handle, sock, sockp);
    }
    if (flag3 & 0x02) {
        curl_socket_t sock = (curl_socket_t)(flag2 % 10);
        void *sockp = (void *)(uintptr_t)(flag3 % 256);
        curl_multi_assign(multi_handle, sock, sockp);
    }

    if (flag3 & 0x04) {
        curl_multi_wakeup(multi_handle);
    }

    for (int i = 0; i < 10 && i < (int)Size; i++) {
        CURLMcode code = (CURLMcode)(Data[i] % 100);
        const char *msg = curl_multi_strerror(code);
        (void)msg;
    }

    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }

    return 0;
}