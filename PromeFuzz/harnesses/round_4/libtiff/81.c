// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static tmsize_t get_size_from_data(const uint8_t *data, size_t size, size_t offset) {
    if (offset + sizeof(tmsize_t) <= size) {
        tmsize_t result;
        memcpy(&result, data + offset, sizeof(tmsize_t));
        return result;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(tmsize_t) * 4) {
        return 0;
    }

    // Test _TIFFmalloc
    tmsize_t malloc_size = get_size_from_data(Data, Size, 0);
    void *malloc_ptr = _TIFFmalloc(malloc_size);
    
    // Test _TIFFcalloc
    tmsize_t calloc_nmemb = get_size_from_data(Data, Size, sizeof(tmsize_t));
    tmsize_t calloc_size = get_size_from_data(Data, Size, sizeof(tmsize_t) * 2);
    void *calloc_ptr = _TIFFcalloc(calloc_nmemb, calloc_size);
    
    // Test _TIFFrealloc
    tmsize_t realloc_size = get_size_from_data(Data, Size, sizeof(tmsize_t) * 3);
    void *realloc_ptr = _TIFFrealloc(malloc_ptr, realloc_size);
    
    // Update pointer if realloc succeeded
    if (realloc_ptr != NULL) {
        malloc_ptr = realloc_ptr;
    }
    
    // Test _TIFFmemset on calloc'd memory
    if (calloc_ptr != NULL && calloc_nmemb > 0 && calloc_size > 0) {
        tmsize_t memset_size = calloc_nmemb * calloc_size;
        if (memset_size / calloc_nmemb == calloc_size) { // Check for overflow
            int memset_val = (Size > 0) ? Data[0] : 0;
            _TIFFmemset(calloc_ptr, memset_val, memset_size);
        }
    }
    
    // Test _TIFFmemcpy between buffers
    if (malloc_ptr != NULL && calloc_ptr != NULL && 
        malloc_size > 0 && calloc_nmemb > 0 && calloc_size > 0) {
        tmsize_t copy_size = (malloc_size < calloc_nmemb * calloc_size) ? 
                             malloc_size : calloc_nmemb * calloc_size;
        if (copy_size > 0) {
            _TIFFmemcpy(calloc_ptr, malloc_ptr, copy_size);
        }
    }
    
    // Test _TIFFmemcpy with overlapping regions (if we have enough data)
    if (malloc_ptr != NULL && malloc_size >= 2 * sizeof(tmsize_t)) {
        size_t offset = sizeof(tmsize_t) * 4;
        if (offset < Size) {
            size_t data_to_copy = (Size - offset < malloc_size) ? 
                                  Size - offset : malloc_size;
            if (data_to_copy > 0) {
                memcpy(malloc_ptr, Data + offset, data_to_copy);
                
                // Now test overlapping copy within the same buffer
                if (malloc_size >= 2 * data_to_copy && data_to_copy > 0) {
                    void *src = malloc_ptr;
                    void *dst = (uint8_t*)malloc_ptr + data_to_copy;
                    _TIFFmemcpy(dst, src, data_to_copy);
                }
            }
        }
    }
    
    // Clean up
    if (malloc_ptr != NULL) {
        _TIFFfree(malloc_ptr);
    }
    if (calloc_ptr != NULL) {
        _TIFFfree(calloc_ptr);
    }
    
    // Additional test: allocate and free with zero size
    void *zero_ptr = _TIFFmalloc(0);
    if (zero_ptr != NULL) {
        _TIFFfree(zero_ptr);
    }
    
    // Test _TIFFrealloc with NULL pointer (should behave like malloc)
    tmsize_t new_size = get_size_from_data(Data, Size, 0) % 1024;
    void *new_ptr = _TIFFrealloc(NULL, new_size);
    if (new_ptr != NULL) {
        _TIFFfree(new_ptr);
    }
    
    return 0;
}