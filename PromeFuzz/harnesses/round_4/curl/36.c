// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_subparts at mime.c:1489:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_subparts at mime.c:1489:10 in curl.h
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

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
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

    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy);
        return 0;
    }

    if (Size > 0) {
        uint8_t mode = Data[0] % 4;
        const uint8_t *content = Data + 1;
        size_t content_size = Size - 1;

        switch (mode) {
            case 0: {
                CURLcode res = curl_mime_data(part, (const char *)content, content_size);
                (void)res;
                break;
            }
            case 1: {
                FILE *fp = fopen("./dummy_file", "wb");
                if (fp) {
                    fwrite(content, 1, content_size, fp);
                    fclose(fp);
                    CURLcode res = curl_mime_filedata(part, "./dummy_file");
                    (void)res;
                    curl_mime_filename(part, NULL);
                    remove("./dummy_file");
                }
                break;
            }
            case 2: {
                if (content_size > 0) {
                    CURLcode res = curl_mime_data(part, (const char *)content, content_size);
                    (void)res;
                }
                curl_mime *submime = curl_mime_init(easy);
                if (submime) {
                    curl_mimepart *subpart = curl_mime_addpart(submime);
                    if (subpart && content_size > 0) {
                        CURLcode res = curl_mime_data(subpart, (const char *)content, content_size);
                        (void)res;
                    }
                    CURLcode res = curl_mime_subparts(part, submime);
                    (void)res;
                }
                break;
            }
            case 3: {
                FILE *fp = fopen("./dummy_file", "wb");
                if (fp) {
                    fwrite(content, 1, content_size, fp);
                    fclose(fp);
                    CURLcode res = curl_mime_filedata(part, "./dummy_file");
                    (void)res;
                    remove("./dummy_file");
                    
                    curl_mime *submime = curl_mime_init(easy);
                    if (submime) {
                        curl_mimepart *subpart = curl_mime_addpart(submime);
                        if (subpart && content_size > 0) {
                            CURLcode res2 = curl_mime_data(subpart, (const char *)content, content_size);
                            (void)res2;
                        }
                        CURLcode res2 = curl_mime_subparts(part, submime);
                        (void)res2;
                    }
                }
                break;
            }
        }
    }

    curl_mime_free(mime);
    curl_easy_cleanup(easy);
    
    return 0;
}