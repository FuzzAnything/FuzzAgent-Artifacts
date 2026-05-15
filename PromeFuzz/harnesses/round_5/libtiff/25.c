// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALLOC_SIZE 65536
#define DUMMY_FILE "./dummy_file"

static tmsize_t read_tmsize_t(const uint8_t **data, size_t *size) {
    if (*size < sizeof(tmsize_t)) {
        return 0;
    }
    tmsize_t result;
    memcpy(&result, *data, sizeof(tmsize_t));
    *data += sizeof(tmsize_t);
    *size -= sizeof(tmsize_t);
    return result;
}

static int read_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) {
        return 0;
    }
    int result;
    memcpy(&result, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return result;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const uint8_t *data_ptr = Data;
    size_t data_size = Size;

    // Use first byte to determine which function to test
    uint8_t function_selector = data_ptr[0] % 6;
    data_ptr++;
    data_size--;

    switch (function_selector) {
        case 0: {  // _TIFFmemset
            tmsize_t c = read_tmsize_t(&data_ptr, &data_size);
            if (c < 0 || c > MAX_ALLOC_SIZE) {
                c = c % (MAX_ALLOC_SIZE / 2);
            }
            
            int v = read_int(&data_ptr, &data_size);
            void *p = _TIFFmalloc(c);
            if (p) {
                _TIFFmemset(p, v, c);
                _TIFFfree(p);
            }
            break;
        }
        
        case 1: {  // _TIFFrealloc
            tmsize_t s1 = read_tmsize_t(&data_ptr, &data_size);
            if (s1 < 0 || s1 > MAX_ALLOC_SIZE) {
                s1 = s1 % (MAX_ALLOC_SIZE / 2);
            }
            
            tmsize_t s2 = read_tmsize_t(&data_ptr, &data_size);
            if (s2 < 0 || s2 > MAX_ALLOC_SIZE) {
                s2 = s2 % (MAX_ALLOC_SIZE / 2);
            }
            
            void *p = _TIFFmalloc(s1);
            if (p) {
                void *new_p = _TIFFrealloc(p, s2);
                if (new_p) {
                    // If realloc succeeded, p is either freed or same as new_p
                    // Only free new_p, not p
                    _TIFFfree(new_p);
                } else {
                    // If realloc failed, p is still valid and must be freed
                    _TIFFfree(p);
                }
            }
            break;
        }
        
        case 2: {  // _TIFFmemcpy
            tmsize_t c = read_tmsize_t(&data_ptr, &data_size);
            if (c < 0 || c > MAX_ALLOC_SIZE) {
                c = c % (MAX_ALLOC_SIZE / 2);
            }
            
            void *d = _TIFFmalloc(c);
            void *s = _TIFFmalloc(c);
            
            if (d && s) {
                // Fill source with some data
                size_t copy_size = data_size < (size_t)c ? data_size : (size_t)c;
                if (copy_size > 0) {
                    memcpy(s, data_ptr, copy_size);
                }
                
                _TIFFmemcpy(d, s, c);
            }
            
            if (d) _TIFFfree(d);
            if (s) _TIFFfree(s);
            break;
        }
        
        case 3: {  // _TIFFmalloc
            tmsize_t s = read_tmsize_t(&data_ptr, &data_size);
            if (s < 0 || s > MAX_ALLOC_SIZE) {
                s = s % (MAX_ALLOC_SIZE / 2);
            }
            
            void *p = _TIFFmalloc(s);
            if (p) {
                _TIFFfree(p);
            }
            break;
        }
        
        case 4: {  // _TIFFfree
            tmsize_t s = read_tmsize_t(&data_ptr, &data_size);
            if (s < 0 || s > MAX_ALLOC_SIZE) {
                s = s % (MAX_ALLOC_SIZE / 2);
            }
            
            void *p = _TIFFmalloc(s);
            _TIFFfree(p);
            
            // Test with NULL pointer
            _TIFFfree(NULL);
            break;
        }
        
        case 5: {  // _TIFFcalloc
            tmsize_t nmemb = read_tmsize_t(&data_ptr, &data_size);
            if (nmemb < 0 || nmemb > MAX_ALLOC_SIZE) {
                nmemb = nmemb % (MAX_ALLOC_SIZE / 2);
            }
            
            tmsize_t siz = read_tmsize_t(&data_ptr, &data_size);
            if (siz < 0 || siz > MAX_ALLOC_SIZE) {
                siz = siz % (MAX_ALLOC_SIZE / 2);
            }
            
            void *p = _TIFFcalloc(nmemb, siz);
            if (p) {
                _TIFFfree(p);
            }
            break;
        }
    }

    return 0;
}