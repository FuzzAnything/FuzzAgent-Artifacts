// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_mime_free at mime.c:1082:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl globally if not already done (safe to call multiple times) */
    curl_global_init(CURL_GLOBAL_ALL);

    /* Initialize global trace with a safe default or fuzzed string if safe */
    if (Size > 0) {
        /* Ensure null termination for string-based API */
        char *trace_config = malloc(Size + 1);
        if (trace_config) {
            memcpy(trace_config, Data, Size);
            trace_config[Size] = '\0';
            curl_global_trace(trace_config);
            free(trace_config);
        }
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Prepare a safe URL to avoid network issues during fuzzing */
    const char *safe_url = "http://127.0.0.1:1/";
    curl_easy_setopt(curl, CURLOPT_URL, safe_url);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    /* Create a dummy file for file-based operations */
    write_dummy_file(Data, Size);

    /* Initialize MIME structure for MIME-related functions */
    curl_mime *mime = curl_mime_init(curl);
    if (!mime) {
        curl_easy_cleanup(curl);
        return 0;
    }

    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
        return 0;
    }

    /* Fuzz curl_mime_filename */
    if (Size > 0) {
        char *filename_buf = malloc(Size + 1);
        if (filename_buf) {
            memcpy(filename_buf, Data, Size);
            filename_buf[Size] = '\0';
            curl_mime_filename(part, filename_buf);
            free(filename_buf);
        }
    }

    /* Fuzz curl_mime_filedata with the dummy file */
    curl_mime_filedata(part, "./dummy_file");

    /* Attach MIME to the easy handle */
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

    /* Perform the transfer */
    curl_easy_perform(curl);

    /* Fuzz curl_easy_getinfo */
    struct curl_certinfo certinfo;
    memset(&certinfo, 0, sizeof(certinfo));
    curl_easy_getinfo(curl, CURLINFO_CERTINFO, &certinfo);

    /* Fuzz curl_easy_upkeep */
    curl_easy_upkeep(curl);

    /* Cleanup */
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}