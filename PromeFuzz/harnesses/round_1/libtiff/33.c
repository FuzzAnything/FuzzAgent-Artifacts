// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "libtiff/tiffio.h"

static void fuzz_allocations(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    // Use first bytes to derive parameters
    tmsize_t alloc1 = (data[0] << 8) | data[1];
    tmsize_t alloc2 = (data[2] << 8) | data[3];
    tmsize_t nmemb = (data[4] << 8) | data[5];
    tmsize_t siz = (data[6] << 8) | data[7];
    int memset_val = data[8];
    tmsize_t copy_size = (data[9] << 8) | data[10];
    tmsize_t realloc_size = (data[11] << 8) | data[12];
    
    // Limit sizes to prevent excessive memory allocation
    if (alloc1 > 1024) alloc1 = 1024;
    if (alloc2 > 1024) alloc2 = 1024;
    if (nmemb > 1024) nmemb = 1024;
    if (siz > 1024) siz = 1024;
    if (copy_size > 1024) copy_size = 1024;
    if (realloc_size > 1024) realloc_size = 1024;
    
    // Test _TIFFmalloc
    void *ptr1 = _TIFFmalloc(alloc1);
    if (!ptr1 && alloc1 > 0) return;
    
    // Test _TIFFcalloc
    void *ptr2 = _TIFFcalloc(nmemb, siz);
    if (!ptr2 && nmemb > 0 && siz > 0) {
        _TIFFfree(ptr1);
        return;
    }
    
    // Test _TIFFmemcpy if we have data to copy
    if (ptr1 && ptr2 && copy_size > 0) {
        // Ensure we don't copy more than allocated
        tmsize_t actual_copy = copy_size;
        if (actual_copy > alloc1) actual_copy = alloc1;
        if (actual_copy > nmemb * siz) actual_copy = nmemb * siz;
        
        // Use remaining fuzzing data as source
        size_t data_offset = 13;
        if (data_offset < size) {
            size_t src_size = size - data_offset;
            if (src_size > actual_copy) src_size = actual_copy;
            _TIFFmemcpy(ptr1, data + data_offset, src_size);
        }
    }
    
    // Test _TIFFmemset
    if (ptr2 && nmemb * siz > 0) {
        tmsize_t memset_size = nmemb * siz;
        if (memset_size > 1024) memset_size = 1024;
        _TIFFmemset(ptr2, memset_val, memset_size);
    }
    
    // Test _TIFFrealloc
    void *ptr3 = _TIFFrealloc(ptr1, realloc_size);
    if (ptr3) {
        ptr1 = ptr3;
    }
    
    // Test _TIFFfree
    _TIFFfree(ptr1);
    _TIFFfree(ptr2);
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test allocation functions
    fuzz_allocations(Data, Size);
    
    // Test edge cases
    // Zero size allocations
    void *zero_ptr = _TIFFmalloc(0);
    if (zero_ptr) _TIFFfree(zero_ptr);
    
    // NULL pointer free (should be safe)
    _TIFFfree(NULL);
    
    // Realloc with NULL pointer (should act like malloc)
    void *realloc_null = _TIFFrealloc(NULL, 16);
    if (realloc_null) _TIFFfree(realloc_null);
    
    // Realloc to zero size
    void *small_ptr = _TIFFmalloc(8);
    if (small_ptr) {
        void *zero_realloc = _TIFFrealloc(small_ptr, 0);
        if (zero_realloc) _TIFFfree(zero_realloc);
    }
    
    // Test calloc with zero parameters
    void *zero_calloc = _TIFFcalloc(0, 10);
    if (zero_calloc) _TIFFfree(zero_calloc);
    
    zero_calloc = _TIFFcalloc(10, 0);
    if (zero_calloc) _TIFFfree(zero_calloc);
    
    // Test memcpy with zero size
    if (Size >= 2) {
        char buf1[16], buf2[16];
        _TIFFmemcpy(buf1, buf2, 0);
    }
    
    // Test memset with zero size
    if (Size >= 1) {
        char buf[16];
        _TIFFmemset(buf, Data[0] % 256, 0);
    }
    
    return 0;
}