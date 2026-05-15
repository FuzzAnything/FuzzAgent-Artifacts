// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_subparts at mime.c:1489:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"
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

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *easy = curl_easy_init();
    if (!easy) return 0;
    
    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        return 0;
    }
    
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        return 0;
    }
    
    unsigned char selector = Data[0] % 6;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size > 1 ? Size - 1 : 0;
    
    switch (selector) {
        case 0: {
            write_dummy_file(payload, payload_size);
            CURLcode res = curl_mime_filedata(part, "./dummy_file");
            if (res == CURLE_OK) {
                curl_mime_filename(part, NULL);
            }
            remove_dummy_file();
            break;
        }
        case 1: {
            curl_mime *subparts = curl_mime_init(easy);
            if (subparts) {
                curl_mimepart *subpart = curl_mime_addpart(subparts);
                if (subpart && payload_size > 0) {
                    curl_mime_data(subpart, (const char*)payload, payload_size % 1024);
                }
                curl_mime_subparts(part, subparts);
            }
            break;
        }
        case 2: {
            if (payload_size > 0) {
                curl_mime_data(part, (const char*)payload, payload_size % 1024);
            }
            break;
        }
        case 3: {
            curl_mimepart *new_part = curl_mime_addpart(mime);
            if (new_part && payload_size > 0) {
                curl_mime_data(new_part, (const char*)payload, payload_size % 1024);
            }
            break;
        }
        case 4: {
            curl_mime_free(mime);
            mime = curl_mime_init(easy);
            if (mime && payload_size > 0) {
                part = curl_mime_addpart(mime);
                if (part) {
                    curl_mime_data(part, (const char*)payload, payload_size % 1024);
                }
            }
            break;
        }
        case 5: {
            curl_mime_free(mime);
            mime = NULL;
            curl_mime_free(mime);
            mime = curl_mime_init(easy);
            if (mime) {
                part = curl_mime_addpart(mime);
                if (part && payload_size > 0) {
                    write_dummy_file(payload, payload_size);
                    curl_mime_filedata(part, "./dummy_file");
                    remove_dummy_file();
                }
            }
            break;
        }
    }
    
    if (mime) {
        curl_mime_free(mime);
    }
    curl_easy_cleanup(easy);
    
    return 0;
}