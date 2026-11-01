// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formfree at formdata.c:665:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

static size_t formget_callback(void *userp, const char *ptr, size_t size) {
    /* Simple callback that accepts data without doing anything */
    (void)userp;
    (void)ptr;
    (void)size;
    return size;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl */
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    struct curl_httppost *formpost = NULL;
    struct curl_httppost *last = NULL;

    /* Create a dummy file for file upload tests */
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    /* Fuzz curl_mprintf with various format strings derived from input */
    char format_buf[64];
    size_t format_len = Size < sizeof(format_buf) - 1 ? Size : sizeof(format_buf) - 1;
    memcpy(format_buf, Data, format_len);
    format_buf[format_len] = '\0';

    /* Ensure format string is safe by limiting to simple patterns if needed */
    /* For fuzzing, we allow raw input to test internal parsing */
    curl_mprintf(format_buf, "test_value");

    /* Fuzz curl_formadd */
    /* Use input data to construct form parts */
    char name[32], contents[32];
    size_t name_len = Size < sizeof(name) - 1 ? Size : sizeof(name) - 1;
    size_t contents_len = Size < sizeof(contents) - 1 ? Size : sizeof(contents) - 1;
    memcpy(name, Data, name_len);
    name[name_len] = '\0';
    memcpy(contents, Data, contents_len);
    contents[contents_len] = '\0';

    /* Add a simple text field */
    curl_formadd(&formpost, &last,
                 CURLFORM_COPYNAME, name,
                 CURLFORM_COPYCONTENTS, contents,
                 CURLFORM_END);

    /* Add a file field using dummy_file */
    curl_formadd(&formpost, &last,
                 CURLFORM_COPYNAME, "file",
                 CURLFORM_FILE, "./dummy_file",
                 CURLFORM_END);

    /* Add a buffer field */
    curl_formadd(&formpost, &last,
                 CURLFORM_COPYNAME, "buffer",
                 CURLFORM_BUFFER, "buffer_data",
                 CURLFORM_BUFFERPTR, Data,
                 CURLFORM_BUFFERLENGTH, Size,
                 CURLFORM_END);

    /* Fuzz curl_formget */
    if (formpost) {
        curl_formget(formpost, NULL, formget_callback);
    }

    /* Fuzz curl_strequal */
    char str1[32], str2[32];
    size_t str1_len = Size < sizeof(str1) - 1 ? Size : sizeof(str1) - 1;
    size_t str2_len = Size < sizeof(str2) - 1 ? Size : sizeof(str2) - 1;
    memcpy(str1, Data, str1_len);
    str1[str1_len] = '\0';
    memcpy(str2, Data, str2_len);
    str2[str2_len] = '\0';

    curl_strequal(str1, str2);
    curl_strequal(NULL, NULL);
    curl_strequal(str1, NULL);

    /* Cleanup form data */
    if (formpost) {
        curl_formfree(formpost);
    }

    /* Cleanup curl handle */
    curl_easy_cleanup(curl);

    /* Remove dummy file */
    remove("./dummy_file");

    return 0;
}