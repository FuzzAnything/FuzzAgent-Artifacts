// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy callback for curl_formget with correct signature */
static size_t dummy_formget_callback(void *arg, const char *buf, size_t len) {
    /* Just consume the data, do nothing */
    return len;
}

/* Helper to create a dummy curl_slist */
static struct curl_slist* create_dummy_slist(const uint8_t *data, size_t size) {
    struct curl_slist *list = NULL;
    if (size > 0) {
        char *str = malloc(size + 1);
        if (str) {
            memcpy(str, data, size);
            str[size] = '\0';
            list = curl_slist_append(list, str);
            free(str);
        }
    }
    return list;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    /* Initialize curl if needed - though not strictly required for these functions */
    CURL *easy_handle = curl_easy_init();
    
    /* Test curl_unescape and curl_free */
    if (Size > 0) {
        char *unescaped = curl_unescape((const char*)Data, (int)(Size > 1024 ? 1024 : Size));
        if (unescaped) {
            curl_free(unescaped);
        }
    }
    
    /* Test curl_easy_unescape */
    if (easy_handle && Size > 0) {
        int outlength = 0;
        char *easy_unescaped = curl_easy_unescape(easy_handle, (const char*)Data, 
                                                  (int)(Size > 1024 ? 1024 : Size), &outlength);
        if (easy_unescaped) {
            curl_free(easy_unescaped);
        }
    }
    
    /* Test curl_formadd, curl_formget, and curl_formfree */
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    /* Create a dummy file if needed */
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size > 4096 ? 4096 : Size, fp);
        }
        fclose(fp);
    }
    
    /* Try different formadd variations based on input data */
    if (Size > 0) {
        /* Use first byte to determine formadd variant */
        uint8_t variant = Data[0];
        
        switch (variant % 4) {
            case 0:
                /* Simple name-value pair */
                curl_formadd(&formpost, &lastptr,
                            CURLFORM_COPYNAME, "field1",
                            CURLFORM_COPYCONTENTS, "value1",
                            CURLFORM_END);
                break;
            case 1:
                /* With file upload */
                curl_formadd(&formpost, &lastptr,
                            CURLFORM_COPYNAME, "file",
                            CURLFORM_FILE, filename,
                            CURLFORM_END);
                break;
            case 2:
                /* With buffer data */
                if (Size > 1) {
                    char *buffer = malloc(Size - 1);
                    if (buffer) {
                        memcpy(buffer, Data + 1, Size - 1);
                        curl_formadd(&formpost, &lastptr,
                                    CURLFORM_COPYNAME, "bufferfield",
                                    CURLFORM_BUFFER, "buffer.txt",
                                    CURLFORM_BUFFERPTR, buffer,
                                    CURLFORM_BUFFERLENGTH, (long)(Size - 1),
                                    CURLFORM_END);
                        free(buffer);
                    }
                }
                break;
            case 3:
                /* Multiple fields */
                curl_formadd(&formpost, &lastptr,
                            CURLFORM_COPYNAME, "name1",
                            CURLFORM_COPYCONTENTS, "content1",
                            CURLFORM_END);
                curl_formadd(&formpost, &lastptr,
                            CURLFORM_COPYNAME, "name2",
                            CURLFORM_COPYCONTENTS, "content2",
                            CURLFORM_END);
                break;
        }
        
        /* Add content headers if we have data */
        if (Size > 2) {
            struct curl_slist *headers = create_dummy_slist(Data + 1, Size > 100 ? 100 : Size - 1);
            if (headers) {
                curl_formadd(&formpost, &lastptr,
                            CURLFORM_COPYNAME, "headerfield",
                            CURLFORM_COPYCONTENTS, "headercontent",
                            CURLFORM_CONTENTHEADER, headers,
                            CURLFORM_END);
                curl_slist_free_all(headers);
            }
        }
    }
    
    /* Test curl_formget if we have a form */
    if (formpost) {
        void *dummy_arg = NULL;
        curl_formget(formpost, dummy_arg, dummy_formget_callback);
    }
    
    /* Clean up form */
    curl_formfree(formpost);
    
    /* Clean up easy handle */
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    /* Remove dummy file */
    remove(filename);
    
    return 0;
}