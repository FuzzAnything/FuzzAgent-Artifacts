// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_slist_append at slist.c:85:20 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_slist_free_all(const uint8_t *Data, size_t Size) {
    struct curl_slist *list = NULL;
    size_t idx = 0;

    while (idx < Size) {
        size_t len = 0;
        if (idx + 1 < Size) {
            len = Data[idx] % 100;
            idx++;
        }
        if (idx + len > Size) {
            len = Size - idx;
        }
        if (len > 0) {
            char *str = (char *)malloc(len + 1);
            if (str) {
                memcpy(str, Data + idx, len);
                str[len] = '\0';
                list = curl_slist_append(list, str);
                free(str);
            }
        }
        idx += len;
    }

    curl_slist_free_all(list);
}

static void fuzz_mime_functions(const uint8_t *Data, size_t Size) {
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) return;

    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        return;
    }

    size_t idx = 0;
    while (idx < Size) {
        uint8_t cmd = Data[idx] % 5;
        idx++;

        size_t len = 0;
        if (idx < Size) {
            len = Data[idx] % 100;
            idx++;
        }
        if (idx + len > Size) {
            len = Size - idx;
        }
        if (len == 0 && Size > 0) len = 1;

        char *str = (char *)malloc(len + 1);
        if (!str) {
            curl_mime_free(mime);
            return;
        }
        memcpy(str, Data + idx, len);
        str[len] = '\0';
        idx += len;

        switch (cmd) {
            case 0:
                curl_mime_type(part, str);
                break;
            case 1:
                curl_mime_filename(part, str);
                break;
            case 2: {
                const char *encodings[] = {"base64", "quoted-printable", "7bit", "8bit", "binary"};
                const char *enc = encodings[Data[idx % Size] % 5];
                curl_mime_encoder(part, enc);
                break;
            }
            case 3: {
                struct curl_slist *headers = NULL;
                headers = curl_slist_append(headers, str);
                curl_mime_headers(part, headers, 1);
                break;
            }
            case 4:
                curl_mime_headers(part, NULL, 0);
                break;
        }
        free(str);
    }

    curl_mime_free(mime);
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    uint8_t mode = Data[0] % 2;

    if (mode == 0) {
        fuzz_slist_free_all(Data + 1, Size - 1);
    } else {
        fuzz_mime_functions(Data + 1, Size - 1);
    }

    return 0;
}