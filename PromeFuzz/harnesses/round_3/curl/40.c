// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    write_dummy_file(Data, Size);

    CURLcode res;
    const char *url = "file://./dummy_file";

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);

    res = curl_easy_perform(curl);
    const char *error_str = curl_easy_strerror(res);

    if (res == CURLE_OK) {
        size_t sent = 0;
        uint8_t send_buf[256];
        size_t send_len = Size < sizeof(send_buf) ? Size : sizeof(send_buf);
        memcpy(send_buf, Data, send_len);

        curl_easy_send(curl, send_buf, send_len, &sent);

        uint8_t recv_buf[256];
        size_t recv_len = 0;
        const struct curl_ws_frame *meta = NULL;
        curl_ws_recv(curl, recv_buf, sizeof(recv_buf), &recv_len, &meta);
    }

    double total_time = 0;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

    curl_easy_cleanup(curl);
    remove_dummy_file();

    (void)error_str;
    (void)total_time;
    (void)response_code;

    return 0;
}