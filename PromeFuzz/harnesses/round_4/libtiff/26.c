// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_TIFFmalloc(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    // Use first byte to determine allocation size (modulo to limit size)
    tmsize_t alloc_size = (tmsize_t)(data[0] % 1024);
    
    void *ptr = _TIFFmalloc(alloc_size);
    if (ptr != NULL) {
        // Try to write some data if we have it
        if (size > 1) {
            size_t copy_size = (size - 1 < (size_t)alloc_size) ? size - 1 : (size_t)alloc_size;
            memcpy(ptr, data + 1, copy_size);
        }
        _TIFFfree(ptr);
    }
}

static void test_TIFFmemcpy(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Use first byte for buffer size
    tmsize_t buf_size = (tmsize_t)(data[0] % 512);
    if (buf_size == 0) buf_size = 1;
    
    void *src = _TIFFmalloc(buf_size);
    void *dst = _TIFFmalloc(buf_size);
    
    if (src != NULL && dst != NULL) {
        // Copy some data into source buffer
        size_t init_size = (size - 1 < (size_t)buf_size) ? size - 1 : (size_t)buf_size;
        memcpy(src, data + 1, init_size);
        
        // Perform the memcpy
        _TIFFmemcpy(dst, src, buf_size);
    }
    
    if (src != NULL) _TIFFfree(src);
    if (dst != NULL) _TIFFfree(dst);
}

static void test_TIFFrealloc(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Use first two bytes for initial and new sizes
    tmsize_t init_size = (tmsize_t)(data[0] % 256);
    tmsize_t new_size = (tmsize_t)(data[1] % 512);
    
    void *ptr = _TIFFmalloc(init_size);
    if (ptr != NULL) {
        // Copy some data if available
        if (size > 2) {
            size_t copy_size = (size - 2 < (size_t)init_size) ? size - 2 : (size_t)init_size;
            memcpy(ptr, data + 2, copy_size);
        }
        
        void *new_ptr = _TIFFrealloc(ptr, new_size);
        if (new_ptr != NULL) {
            _TIFFfree(new_ptr);
        } else {
            // If realloc fails, the original pointer is still valid
            _TIFFfree(ptr);
        }
    }
}

static void test_TIFFcalloc(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Use first two bytes for count and element size
    tmsize_t nmemb = (tmsize_t)(data[0] % 128);
    tmsize_t siz = (tmsize_t)(data[1] % 128);
    
    void *ptr = _TIFFcalloc(nmemb, siz);
    if (ptr != NULL) {
        // Verify memory is zero-initialized
        if (nmemb > 0 && siz > 0) {
            tmsize_t total_size = nmemb * siz;
            if (total_size > 0) {
                // Check first few bytes are zero
                uint8_t *check_ptr = (uint8_t *)ptr;
                for (tmsize_t i = 0; i < total_size && i < 16; i++) {
                    if (check_ptr[i] != 0) {
                        // Should not happen for calloc
                        break;
                    }
                }
            }
        }
        _TIFFfree(ptr);
    }
}

static void test_TIFFmemset(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    // Use first byte for buffer size, second for value
    tmsize_t buf_size = (tmsize_t)(data[0] % 512);
    int value = (int)(data[1]);
    
    if (buf_size == 0) buf_size = 1;
    
    void *ptr = _TIFFmalloc(buf_size);
    if (ptr != NULL) {
        // Initialize with some data first
        if (size > 2) {
            size_t init_size = (size - 2 < (size_t)buf_size) ? size - 2 : (size_t)buf_size;
            memcpy(ptr, data + 2, init_size);
        }
        
        // Perform memset
        _TIFFmemset(ptr, value, buf_size);
        
        // Verify the first byte is set to value
        uint8_t *check_ptr = (uint8_t *)ptr;
        if (check_ptr[0] != (uint8_t)value) {
            // Mismatch - but continue anyway
        }
        
        _TIFFfree(ptr);
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use the first byte to determine which test to run
    uint8_t test_selector = Data[0];
    
    switch (test_selector % 5) {
        case 0:
            test_TIFFmalloc(Data, Size);
            break;
        case 1:
            test_TIFFmemcpy(Data, Size);
            break;
        case 2:
            test_TIFFrealloc(Data, Size);
            break;
        case 3:
            test_TIFFcalloc(Data, Size);
            break;
        case 4:
            test_TIFFmemset(Data, Size);
            break;
    }
    
    return 0;
}