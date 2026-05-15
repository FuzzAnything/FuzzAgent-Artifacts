// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_subparts at mime.c:1489:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURL *easy = curl_easy_init();
    if (!easy) {
        return 0;
    }

    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        return 0;
    }

    curl_mimepart *part1 = curl_mime_addpart(mime);
    if (part1) {
        size_t data_size = Size > 100 ? 100 : Size;
        curl_mime_data(part1, (const char *)Data, data_size);
    }

    curl_mimepart *part2 = curl_mime_addpart(mime);
    if (part2) {
        write_dummy_file(Data, Size);
        curl_mime_filedata(part2, "./dummy_file");
        remove_dummy_file();
    }

    curl_mime *sub_mime = curl_mime_init(easy);
    if (sub_mime) {
        curl_mimepart *sub_part = curl_mime_addpart(sub_mime);
        if (sub_part) {
            size_t sub_data_size = Size > 50 ? 50 : Size;
            const uint8_t *sub_data = Data + (Size > 50 ? Size - 50 : 0);
            curl_mime_data(sub_part, (const char *)sub_data, sub_data_size);
        }

        curl_mimepart *part3 = curl_mime_addpart(mime);
        if (part3) {
            curl_mime_subparts(part3, sub_mime);
        } else {
            curl_mime_free(sub_mime);
        }
    }

    curl_mime_free(mime);
    curl_easy_cleanup(easy);

    return 0;
}