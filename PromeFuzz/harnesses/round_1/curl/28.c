// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_encoder at mime.c:1374:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_name at mime.c:1239:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data at mime.c:1273:10 in curl.h
// curl_mime_data_cb at mime.c:1415:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_filename at mime.c:1256:10 in curl.h
// curl_mime_filedata at mime.c:1300:10 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_mime_init at mime.c:1180:12 in curl.h
// curl_mime_addpart at mime.c:1214:16 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_mime_free at mime.c:1082:6 in curl.h
// curl_slist_append at slist.c:85:20 in curl.h
// curl_mime_headers at mime.c:1397:10 in curl.h
// curl_slist_free_all at slist.c:124:6 in curl.h
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

/* Callback functions for curl_mime_data_cb */
static size_t read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    size_t *state = (size_t *)arg;
    size_t available = *state;
    size_t to_copy = size * nitems;
    
    if (to_copy > available) {
        to_copy = available;
    }
    
    if (to_copy > 0) {
        /* Fill with dummy data */
        memset(buffer, 'A', to_copy);
        *state -= to_copy;
    }
    
    return to_copy;
}

static int seek_callback(void *arg, curl_off_t offset, int origin) {
    /* Always succeed for fuzzing */
    return CURL_SEEKFUNC_OK;
}

static void free_callback(void *arg) {
    /* Nothing to free */
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    /* Initialize curl if needed */
    static int curl_initialized = 0;
    if (!curl_initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        curl_initialized = 1;
    }
    
    /* Create a mime structure and part */
    curl_mime *mime = curl_mime_init(NULL);
    if (!mime) {
        return 0;
    }
    
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        return 0;
    }
    
    /* Use the input data to determine which functions to call and with what parameters */
    /* We'll use the first byte to select the primary function to test */
    if (Size == 0) {
        curl_mime_free(mime);
        return 0;
    }
    
    uint8_t selector = Data[0] % 6;
    const uint8_t *func_data = Data + 1;
    size_t func_size = Size - 1;
    
    switch (selector) {
        case 0: /* Test curl_mime_headers */
            {
                struct curl_slist *headers = NULL;
                /* Create a simple header list from the input data */
                if (func_size > 0) {
                    /* Split data by newlines if present */
                    char *data_copy = malloc(func_size + 1);
                    if (data_copy) {
                        memcpy(data_copy, func_data, func_size);
                        data_copy[func_size] = '\0';
                        
                        char *line = strtok(data_copy, "\n");
                        while (line) {
                            headers = curl_slist_append(headers, line);
                            line = strtok(NULL, "\n");
                        }
                        free(data_copy);
                    }
                }
                
                int take_ownership = (func_size > 0) ? 1 : 0;
                curl_mime_headers(part, headers, take_ownership);
                
                /* If we didn't transfer ownership, free the list */
                if (!take_ownership) {
                    curl_slist_free_all(headers);
                }
            }
            break;
            
        case 1: /* Test curl_mime_encoder */
            {
                char *encoding = NULL;
                if (func_size > 0) {
                    encoding = malloc(func_size + 1);
                    if (encoding) {
                        memcpy(encoding, func_data, func_size);
                        encoding[func_size] = '\0';
                        curl_mime_encoder(part, encoding);
                        free(encoding);
                    }
                } else {
                    curl_mime_encoder(part, NULL);
                }
            }
            break;
            
        case 2: /* Test curl_mime_name */
            {
                char *name = NULL;
                if (func_size > 0) {
                    name = malloc(func_size + 1);
                    if (name) {
                        memcpy(name, func_data, func_size);
                        name[func_size] = '\0';
                        curl_mime_name(part, name);
                        free(name);
                    }
                } else {
                    curl_mime_name(part, NULL);
                }
            }
            break;
            
        case 3: /* Test curl_mime_data */
            {
                char *data = NULL;
                if (func_size > 0) {
                    data = malloc(func_size);
                    if (data) {
                        memcpy(data, func_data, func_size);
                        curl_mime_data(part, data, func_size);
                        free(data);
                    }
                } else {
                    curl_mime_data(part, NULL, 0);
                }
            }
            break;
            
        case 4: /* Test curl_mime_data_cb */
            {
                static size_t read_state = 0;
                read_state = func_size;
                curl_mime_data_cb(part, (curl_off_t)func_size,
                                  read_callback,
                                  seek_callback,
                                  free_callback,
                                  &read_state);
            }
            break;
            
        case 5: /* Test curl_mime_filedata */
            {
                /* Write data to dummy file */
                FILE *fp = fopen("./dummy_file", "wb");
                if (fp) {
                    if (func_size > 0) {
                        fwrite(func_data, 1, func_size, fp);
                    }
                    fclose(fp);
                    
                    curl_mime_filedata(part, "./dummy_file");
                    
                    /* Clear the filename if set */
                    curl_mime_filename(part, NULL);
                    
                    /* Remove the dummy file */
                    remove("./dummy_file");
                } else {
                    curl_mime_filedata(part, NULL);
                }
            }
            break;
    }
    
    /* Clean up */
    curl_mime_free(mime);
    
    return 0;
}