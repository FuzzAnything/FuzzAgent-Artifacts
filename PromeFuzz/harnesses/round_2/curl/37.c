// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formfree at formdata.c:665:6 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formget at formdata.c:627:5 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"

/* Callback function for curl_formget with correct signature */
static size_t formget_callback(void *arg, const char *buf, size_t len) {
    /* Do nothing with the data, just return the length */
    (void)arg;
    (void)buf;
    return len;
}

/* Dummy write callback for curl_easy_setopt */
static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *arg) {
    size_t total = size * nmemb;
    (void)ptr;
    (void)arg;
    return total;
}

/* Dummy read callback for form data */
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
    (void)ptr;
    (void)stream;
    return 0;
}

/* Dummy seek callback for form data */
static int seek_callback(void *instream, curl_off_t offset, int origin) {
    (void)instream;
    (void)offset;
    (void)origin;
    return CURL_SEEKFUNC_OK;
}

/* Dummy free callback for form data */
static void free_callback(void *ptr) {
    (void)ptr;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    /* Initialize CURL global state */
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* Create a dummy file for potential file operations */
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }
    
    /* Create an easy handle */
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }
    
    /* Use the first byte to determine which API to test */
    uint8_t selector = Size > 0 ? Data[0] : 0;
    
    /* Test different APIs based on selector */
    switch (selector % 6) {
        case 0: {
            /* Test curl_formadd and curl_formget */
            struct curl_httppost *formpost = NULL;
            struct curl_httppost *lastpost = NULL;
            
            /* Add form fields using fuzz data */
            if (Size > 10) {
                /* Use parts of fuzz data for form fields */
                const char *name = (const char *)(Data + 1);
                size_t name_len = Size > 100 ? 50 : Size/2;
                const char *contents = (const char *)(Data + name_len);
                size_t contents_len = Size - name_len;
                
                curl_formadd(&formpost, &lastpost,
                            CURLFORM_COPYNAME, name,
                            CURLFORM_NAMELENGTH, (long)name_len,
                            CURLFORM_COPYCONTENTS, contents,
                            CURLFORM_CONTENTSLENGTH, (long)contents_len,
                            CURLFORM_END);
                
                /* Test curl_formget if form was created */
                if (formpost) {
                    char dummy_arg = 'A';
                    curl_formget(formpost, &dummy_arg, formget_callback);
                    curl_formfree(formpost);
                }
            }
            break;
        }
        
        case 1: {
            /* Test curl_formfree with NULL */
            curl_formfree(NULL);
            
            /* Test with a simple form */
            struct curl_httppost *formpost = NULL;
            struct curl_httppost *lastpost = NULL;
            
            if (Size > 5) {
                curl_formadd(&formpost, &lastpost,
                            CURLFORM_COPYNAME, "test",
                            CURLFORM_COPYCONTENTS, "data",
                            CURLFORM_END);
                
                curl_formfree(formpost);
            }
            break;
        }
        
        case 2: {
            /* Test curl_easy_setopt with various options */
            if (Size > 20) {
                /* Set URL */
                curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
                
                /* Set verbose mode based on fuzz data */
                curl_easy_setopt(easy, CURLOPT_VERBOSE, (long)(Data[1] & 1));
                
                /* Set timeout */
                curl_easy_setopt(easy, CURLOPT_TIMEOUT, (long)5);
                
                /* Set follow location */
                curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, (long)(Data[2] & 1));
                
                /* Set user agent */
                curl_easy_setopt(easy, CURLOPT_USERAGENT, "fuzzer/1.0");
                
                /* Set write function */
                curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, write_callback);
                
                /* Set read function for POST data */
                curl_easy_setopt(easy, CURLOPT_READFUNCTION, read_callback);
                
                /* Set seek function */
                curl_easy_setopt(easy, CURLOPT_SEEKFUNCTION, seek_callback);
            }
            break;
        }
        
        case 3: {
            /* Test curl_formadd with various options */
            struct curl_httppost *formpost = NULL;
            struct curl_httppost *lastpost = NULL;
            
            if (Size > 30) {
                /* Add multiple form fields */
                curl_formadd(&formpost, &lastpost,
                            CURLFORM_COPYNAME, "field1",
                            CURLFORM_COPYCONTENTS, "value1",
                            CURLFORM_END);
                
                curl_formadd(&formpost, &lastpost,
                            CURLFORM_COPYNAME, "field2",
                            CURLFORM_COPYCONTENTS, "value2",
                            CURLFORM_CONTENTTYPE, "text/plain",
                            CURLFORM_END);
                
                /* Add a file field */
                curl_formadd(&formpost, &lastpost,
                            CURLFORM_COPYNAME, "file",
                            CURLFORM_FILENAME, "dummy_file",
                            CURLFORM_FILE, "./dummy_file",
                            CURLFORM_END);
                
                if (formpost) {
                    curl_formfree(formpost);
                }
            }
            break;
        }
        
        case 4: {
            /* Test curl_mime_init */
            curl_mime *mime = curl_mime_init(easy);
            if (mime) {
                /* Create a mime part */
                curl_mimepart *part = curl_mime_addpart(mime);
                if (part && Size > 10) {
                    /* Set part data from fuzz input */
                    curl_mime_data(part, (const char *)Data, Size);
                    curl_mime_name(part, "fuzzdata");
                    
                    /* Set MIME type */
                    curl_mime_type(part, "application/octet-stream");
                    
                    /* Set filename */
                    curl_mime_filename(part, "fuzz.bin");
                }
                
                /* Free the mime structure */
                curl_mime_free(mime);
            }
            break;
        }
        
        case 5: {
            /* Test curl_mime_free with NULL */
            curl_mime_free(NULL);
            
            /* Test with a simple mime structure */
            curl_mime *mime = curl_mime_init(easy);
            if (mime) {
                /* Add a text part */
                curl_mimepart *part1 = curl_mime_addpart(mime);
                if (part1) {
                    curl_mime_data(part1, "Hello World", CURL_ZERO_TERMINATED);
                    curl_mime_name(part1, "greeting");
                }
                
                /* Add a file part */
                curl_mimepart *part2 = curl_mime_addpart(mime);
                if (part2) {
                    curl_mime_filedata(part2, "./dummy_file");
                    curl_mime_name(part2, "attachment");
                }
                
                curl_mime_free(mime);
            }
            break;
        }
    }
    
    /* Clean up */
    curl_easy_cleanup(easy);
    curl_global_cleanup();
    
    /* Remove dummy file */
    remove("./dummy_file");
    
    return 0;
}