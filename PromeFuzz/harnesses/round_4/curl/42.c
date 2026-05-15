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
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_subparts at mime.c:1489:10 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
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

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        initialized = 1;
    }

    CURL *easy = curl_easy_init();
    if (!easy) {
        return 0;
    }

    /* Test curl_mime_init and curl_mime_addpart */
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

    /* Test curl_mime_data with various inputs */
    if (Size > 0) {
        /* Use first half of data for memory data */
        size_t half = Size / 2;
        curl_mime_data(part, (const char *)Data, half);
        
        /* Test with NULL data */
        curl_mime_data(part, NULL, 0);
        
        /* Test with full data */
        curl_mime_data(part, (const char *)Data, Size);
    } else {
        curl_mime_data(part, NULL, 0);
    }

    /* Test curl_mime_filedata */
    write_dummy_file(Data, Size);
    curl_mime_filedata(part, "./dummy_file");
    
    /* Clear unwanted filename as suggested */
    curl_mime_filename(part, NULL);
    
    /* Test with NULL filename */
    curl_mime_filedata(part, NULL);
    
    /* Test with empty string filename */
    curl_mime_filedata(part, "");

    /* Test curl_mime_subparts */
    curl_mime *submime = curl_mime_init(easy);
    if (submime) {
        curl_mimepart *subpart = curl_mime_addpart(submime);
        if (subpart && Size > 0) {
            curl_mime_data(subpart, (const char *)Data, Size > 100 ? 100 : Size);
        }
        curl_mime_subparts(part, submime);
        /* Note: ownership transferred, don't free submime separately */
    }

    /* Add another part to test multiple parts */
    curl_mimepart *part2 = curl_mime_addpart(mime);
    if (part2 && Size > 0) {
        /* Use different data for second part */
        size_t quarter = Size / 4;
        if (quarter > 0) {
            curl_mime_data(part2, (const char *)Data + quarter, quarter);
        }
        
        /* Test file data on second part */
        curl_mime_filedata(part2, "./dummy_file");
    }

    /* Clean up */
    curl_mime_free(mime);
    curl_easy_cleanup(easy);
    remove_dummy_file();

    return 0;
}