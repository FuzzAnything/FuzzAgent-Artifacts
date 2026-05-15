// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_formget at formdata.c:627:5 in curl.h
// curl_formfree at formdata.c:665:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_type at mime.c:1348:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
// curl_formadd at formdata.c:609:14 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"

static size_t dummy_formget_callback(void *arg, const char *buf, size_t len) {
    // Dummy callback that does nothing
    return len;
}

static void dummy_free_callback(void *ptr) {
    // Dummy free callback
}

static size_t dummy_read_callback(char *buffer, size_t size, size_t nitems, void *instream) {
    // Dummy read callback
    return 0;
}

static int dummy_seek_callback(void *instream, curl_off_t offset, int origin) {
    // Dummy seek callback
    return CURL_SEEKFUNC_OK;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for potential file operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    // Initialize easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        return 0;
    }
    
    // Test curl_mime_init and curl_mime_free
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        // Create a mimepart and set some properties
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part && Size > 0) {
            // Use fuzzer data to set mimepart data
            char *data_copy = malloc(Size + 1);
            if (data_copy) {
                memcpy(data_copy, Data, Size);
                data_copy[Size] = '\0';
                curl_mime_data(part, data_copy, Size);
                free(data_copy);
            }
            
            // Set some mimepart options
            curl_mime_filename(part, "fuzzfile.txt");
            curl_mime_type(part, "text/plain");
            curl_mime_name(part, "fuzzfield");
            
            // Set the mime to easy handle
            curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
        }
        
        // Free the mime handle
        curl_mime_free(mime);
    }
    
    // Test deprecated form functions
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    if (Size > 0) {
        // Try to add form fields using fuzzer data
        char *name = "fuzzname";
        char *value = malloc(Size + 1);
        if (value) {
            memcpy(value, Data, Size);
            value[Size] = '\0';
            
            // Try different formadd options
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, name,
                        CURLFORM_COPYCONTENTS, value,
                        CURLFORM_END);
            
            // Try adding a file field
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, "filefield",
                        CURLFORM_FILE, "./dummy_file",
                        CURLFORM_END);
            
            free(value);
        }
        
        // Test curl_formget if we have a form
        if (formpost) {
            void *dummy_arg = NULL;
            curl_formget(formpost, dummy_arg, dummy_formget_callback);
            
            // Test setting the form to easy handle
            curl_easy_setopt(easy, CURLOPT_HTTPPOST, formpost);
        }
    }
    
    // Test various curl_easy_setopt options with fuzzer data
    if (Size > 0) {
        // Use first byte to determine some options
        uint8_t opt_selector = Data[0];
        
        // Try different setopt calls
        curl_easy_setopt(easy, CURLOPT_URL, "http://localhost/fuzz");
        
        if (Size > 1) {
            char *postdata = malloc(Size);
            if (postdata) {
                memcpy(postdata, Data, Size);
                curl_easy_setopt(easy, CURLOPT_POSTFIELDS, postdata);
                curl_easy_setopt(easy, CURLOPT_POSTFIELDSIZE, (long)Size);
                free(postdata);
            }
        }
        
        // Try callback options
        curl_easy_setopt(easy, CURLOPT_READFUNCTION, dummy_read_callback);
        curl_easy_setopt(easy, CURLOPT_SEEKFUNCTION, dummy_seek_callback);
        
        // Set some boolean options based on fuzzer data
        curl_easy_setopt(easy, CURLOPT_VERBOSE, (opt_selector & 0x01) ? 1L : 0L);
        curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, (opt_selector & 0x02) ? 1L : 0L);
    }
    
    // Free form data if created
    curl_formfree(formpost);
    
    // Clean up easy handle
    curl_easy_cleanup(easy);
    
    return 0;
}