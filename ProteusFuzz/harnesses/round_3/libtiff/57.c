// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void cleanup(void *ptr) {
    if (ptr != NULL) {
        _TIFFfree(ptr);
    }
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tmsize_t len = (tmsize_t)Size;
    tmsize_t nmemb = 0;
    tmsize_t siz = 0;

    // Safely derive allocation parameters from input to avoid overflow
    if (len > 0) {
        nmemb = len;
        siz = 1;
        if (len > 1024) {
            nmemb = 1024;
            siz = len / 1024;
        }
    }

    // Test _TIFFmalloc
    void *ptr_malloc = _TIFFmalloc(len);
    if (ptr_malloc != NULL) {
        _TIFFmemset(ptr_malloc, 0xAA, len);
        _TIFFfree(ptr_malloc);
    }

    // Test _TIFFcalloc
    void *ptr_calloc = _TIFFcalloc(nmemb, siz);
    if (ptr_calloc != NULL) {
        _TIFFmemset(ptr_calloc, 0xBB, nmemb * siz);
        _TIFFfree(ptr_calloc);
    }

    // Test _TIFFrealloc
    void *ptr_realloc = _TIFFmalloc(len);
    if (ptr_realloc != NULL) {
        _TIFFmemset(ptr_realloc, 0xCC, len);
        void *ptr_realloc_new = _TIFFrealloc(ptr_realloc, len * 2);
        if (ptr_realloc_new != NULL) {
            _TIFFmemset(ptr_realloc_new, 0xDD, len * 2);
            _TIFFfree(ptr_realloc_new);
        } else {
            _TIFFfree(ptr_realloc);
        }
    }

    // Test _TIFFmemcpy and _TIFFmemcmp
    if (len > 0) {
        void *src = _TIFFmalloc(len);
        void *dst = _TIFFmalloc(len);
        if (src != NULL && dst != NULL) {
            _TIFFmemset(src, 0xEE, len);
            _TIFFmemcpy(dst, src, len);
            int cmp_result = _TIFFmemcmp(src, dst, len);
            (void)cmp_result; // Use result to avoid unused warning
            
            // Test memcmp with different data
            _TIFFmemset(dst, 0xFF, len);
            cmp_result = _TIFFmemcmp(src, dst, len);
            (void)cmp_result;

            _TIFFfree(src);
            _TIFFfree(dst);
        }
    }

    // Test edge cases for calloc with zero arguments
    void *zero_alloc1 = _TIFFcalloc(0, 10);
    cleanup(zero_alloc1);
    void *zero_alloc2 = _TIFFcalloc(10, 0);
    cleanup(zero_alloc2);

    // Test realloc with NULL pointer (acts like malloc)
    void *null_realloc = _TIFFrealloc(NULL, len);
    cleanup(null_realloc);

    // Test realloc with size 0 (implementation defined, often frees)
    void *ptr_for_zero_realloc = _TIFFmalloc(len);
    if (ptr_for_zero_realloc != NULL) {
        void *zero_realloc = _TIFFrealloc(ptr_for_zero_realloc, 0);
        cleanup(zero_realloc); // If it returned a pointer, free it; if NULL, it was freed
    }

    return 0;
}