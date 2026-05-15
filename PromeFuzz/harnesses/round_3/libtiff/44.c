// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "libtiff/tiffio.h"

static void test_tiff_memory_functions(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    // Use first 4 bytes to seed some parameters
    uint32_t seed = *(const uint32_t*)data;
    size_t offset = 4;
    
    // Test _TIFFmalloc with various sizes
    tmsize_t alloc_size = (tmsize_t)(seed % 1024) + 1;
    void *ptr1 = _TIFFmalloc(alloc_size);
    if (ptr1) {
        // Test _TIFFmemset
        int fill_value = (seed >> 8) & 0xFF;
        _TIFFmemset(ptr1, fill_value, alloc_size);
        
        // Test _TIFFfree
        _TIFFfree(ptr1);
    }
    
    // Test _TIFFcalloc
    if (size - offset >= 8) {
        uint32_t nmemb = *(const uint32_t*)(data + offset);
        uint32_t elem_size = *(const uint32_t*)(data + offset + 4);
        offset += 8;
        
        tmsize_t tiff_nmemb = (tmsize_t)(nmemb % 256) + 1;
        tmsize_t tiff_elem_size = (tmsize_t)(elem_size % 256) + 1;
        
        void *ptr2 = _TIFFcalloc(tiff_nmemb, tiff_elem_size);
        if (ptr2) {
            _TIFFfree(ptr2);
        }
    }
    
    // Test _TIFFrealloc
    if (size - offset >= 4) {
        uint32_t realloc_size = *(const uint32_t*)(data + offset);
        offset += 4;
        
        tmsize_t initial_size = (tmsize_t)((seed >> 16) % 512) + 1;
        tmsize_t new_size = (tmsize_t)(realloc_size % 1024) + 1;
        
        void *ptr3 = _TIFFmalloc(initial_size);
        if (ptr3) {
            // Test _TIFFmemcpy if we have data
            if (size - offset > 0) {
                size_t copy_size = (size - offset < (size_t)initial_size) ? 
                                  (size - offset) : (size_t)initial_size;
                _TIFFmemcpy(ptr3, data + offset, (tmsize_t)copy_size);
            }
            
            // Test _TIFFrealloc
            void *ptr4 = _TIFFrealloc(ptr3, new_size);
            if (ptr4) {
                _TIFFfree(ptr4);
            } else {
                _TIFFfree(ptr3);
            }
        }
    }
    
    // Test edge cases
    // Zero size allocations
    void *zero_ptr = _TIFFmalloc(0);
    if (zero_ptr) {
        _TIFFfree(zero_ptr);
    }
    
    // NULL pointer free (should be safe)
    _TIFFfree(NULL);
    
    // Test _TIFFcalloc with zero parameters
    void *zero_call = _TIFFcalloc(0, 10);
    if (zero_call) {
        _TIFFfree(zero_call);
    }
    
    zero_call = _TIFFcalloc(10, 0);
    if (zero_call) {
        _TIFFfree(zero_call);
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Call the test function
    test_tiff_memory_functions(Data, Size);
    
    return 0;
}