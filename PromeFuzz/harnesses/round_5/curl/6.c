// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_subparts at mime.c:1489:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0) {
            fwrite(data, 1, size, fp);
        }
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    CURL *easy = curl_easy_init();
    if (!easy) {
        return 0;
    }

    write_dummy_file(Data, Size);

    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            if (Size > 0) {
                curl_mime_data(part, (const char *)Data, Size);
            }

            curl_mime_filedata(part, "./dummy_file");

            curl_mime *sub_mime = curl_mime_init(easy);
            if (sub_mime) {
                curl_mimepart *sub_part = curl_mime_addpart(sub_mime);
                if (sub_part && Size > 0) {
                    size_t half_size = Size / 2;
                    curl_mime_data(sub_part, (const char *)Data, half_size);
                }
                curl_mime_subparts(part, sub_mime);
            }
        }

        curl_mime_free(mime);
    }

    remove_dummy_file();
    curl_easy_cleanup(easy);
    return 0;
}