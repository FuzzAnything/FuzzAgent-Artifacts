// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_subparts at mime.c:1489:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
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
    (void)arg;
    (void)buf;
    return len;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    /* Initialize CURL easy handle */
    CURL *easy = curl_easy_init();
    if (!easy) {
        return 0;
    }

    /* Test curl_mime_init and curl_mime_free */
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        /* Create a subparts mime handle */
        curl_mime *subparts = curl_mime_init(easy);
        if (subparts && Size > 0) {
            /* Create a part in the main mime handle */
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                /* Use fuzz data to set some fields if possible */
                if (Size > 1) {
                    /* Use first byte as flags for data setting */
                    uint8_t mode = Data[0];
                    size_t data_len = Size > 100 ? 100 : Size - 1;
                    
                    if (mode & 0x01) {
                        /* Try to set data */
                        curl_mime_data(part, (const char *)(Data + 1), data_len);
                    }
                    if (mode & 0x02 && Size > 2) {
                        /* Try to set name */
                        size_t name_len = Size > 50 ? 50 : Size - 2;
                        char *name = (char *)malloc(name_len + 1);
                        if (name) {
                            memcpy(name, Data + 2, name_len);
                            name[name_len] = '\0';
                            curl_mime_name(part, name);
                            free(name);
                        }
                    }
                }
                
                /* Test curl_mime_subparts */
                curl_mime_subparts(part, subparts);
            }
        }
        curl_mime_free(mime);
        /* subparts is now owned by part and will be freed with mime */
    }

    /* Test deprecated form functions */
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastpost = NULL;
    
    if (Size > 0) {
        /* Use fuzz data to create form fields */
        size_t name_len = Size > 50 ? 50 : Size;
        size_t content_len = Size > 100 ? 50 : (Size > 50 ? Size - 50 : 0);
        
        char *name = NULL;
        char *content = NULL;
        
        if (name_len > 0) {
            name = (char *)malloc(name_len + 1);
            if (name) {
                memcpy(name, Data, name_len);
                name[name_len] = '\0';
            }
        }
        
        if (content_len > 0 && Size > 50) {
            content = (char *)malloc(content_len + 1);
            if (content) {
                memcpy(content, Data + 50, content_len);
                content[content_len] = '\0';
            }
        }
        
        /* Try different formadd options based on fuzz data */
        if (name && content) {
            /* Test with COPYNAME and COPYCONTENTS */
            curl_formadd(&formpost, &lastpost,
                        CURLFORM_COPYNAME, name,
                        CURLFORM_COPYCONTENTS, content,
                        CURLFORM_END);
        } else if (name) {
            /* Test with just name */
            curl_formadd(&formpost, &lastpost,
                        CURLFORM_COPYNAME, name,
                        CURLFORM_END);
        } else if (Size > 0) {
            /* Test with PTR options using fuzz data directly */
            curl_formadd(&formpost, &lastpost,
                        CURLFORM_PTRNAME, (char *)Data,
                        CURLFORM_NAMELENGTH, (long)(Size > 100 ? 100 : Size),
                        CURLFORM_END);
        }
        
        free(name);
        free(content);
        
        /* Test curl_formget if we have a form */
        if (formpost) {
            curl_formget(formpost, NULL, dummy_formget_callback);
        }
    }
    
    /* Test curl_formfree - safe with NULL */
    curl_formfree(formpost);
    curl_formfree(NULL);
    
    /* Test curl_mime_free with NULL */
    curl_mime_free(NULL);
    
    /* Cleanup */
    curl_easy_cleanup(easy);
    
    return 0;
}