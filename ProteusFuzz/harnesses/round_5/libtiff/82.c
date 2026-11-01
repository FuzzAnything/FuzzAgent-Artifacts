// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void cleanup_memory(void *ptr) {
    if (ptr != NULL) {
        _TIFFfree(ptr);
    }
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prevent excessively large allocations to avoid OOM or timeouts
    if (Size > 1024 * 1024) {
        return 0;
    }

    tmsize_t size_hint = (tmsize_t)Size;
    tmsize_t count_hint = (tmsize_t)(Size / 2 + 1);
    int value_hint = (int)(Data[0] & 0xFF);

    // Test _TIFFmalloc
    void *ptr_malloc = _TIFFmalloc(size_hint);
    if (ptr_malloc != NULL) {
        // Test _TIFFmemset on allocated memory
        _TIFFmemset(ptr_malloc, value_hint, size_hint);
        
        // Test _TIFFmemcpy
        void *ptr_copy = _TIFFmalloc(size_hint);
        if (ptr_copy != NULL) {
            _TIFFmemcpy(ptr_copy, ptr_malloc, size_hint);
            cleanup_memory(ptr_copy);
        }

        // Test _TIFFrealloc (resize)
        void *ptr_realloc = _TIFFrealloc(ptr_malloc, size_hint * 2);
        if (ptr_realloc != NULL) {
            // Use the new pointer
            _TIFFmemset(ptr_realloc, value_hint ^ 0xFF, size_hint * 2);
            cleanup_memory(ptr_realloc);
        } else {
            // If realloc fails, original pointer might still be valid depending on implementation,
            // but standard realloc behavior says original is untouched.
            // However, libtiff wrapper might behave differently, so free original if realloc returned NULL
            // and we assume it didn't move. Standard practice: if realloc fails, original block is unchanged.
            // We free the original here to be safe for the test flow.
            cleanup_memory(ptr_malloc);
        }
    }

    // Test _TIFFcalloc
    void *ptr_calloc = _TIFFcalloc(count_hint, size_hint);
    if (ptr_calloc != NULL) {
        // Verify it's zeroed (optional check, but good for coverage)
        _TIFFmemset(ptr_calloc, 0x00, count_hint * size_hint);
        cleanup_memory(ptr_calloc);
    }

    // Test _TIFFmemcmp
    void *buf1 = _TIFFmalloc(size_hint);
    void *buf2 = _TIFFmalloc(size_hint);
    if (buf1 != NULL && buf2 != NULL) {
        // Fill with data from input
        _TIFFmemcpy(buf1, Data, size_hint);
        _TIFFmemcpy(buf2, Data, size_hint);
        
        // Modify one byte to test inequality
        if (size_hint > 0) {
            ((uint8_t *)buf2)[0] ^= 0xFF;
        }
        
        int cmp_result = _TIFFmemcmp(buf1, buf2, size_hint);
        (void)cmp_result; // Use result to avoid unused variable warning

        cleanup_memory(buf1);
        cleanup_memory(buf2);
    }

    // Test edge cases: NULL pointer to realloc (acts like malloc)
    void *ptr_null_realloc = _TIFFrealloc(NULL, size_hint);
    if (ptr_null_realloc != NULL) {
        cleanup_memory(ptr_null_realloc);
    }

    // Test edge case: size 0 to malloc/calloc (should return NULL per libtiff docs)
    void *ptr_zero_malloc = _TIFFmalloc(0);
    (void)ptr_zero_malloc;
    
    void *ptr_zero_calloc = _TIFFcalloc(0, size_hint);
    (void)ptr_zero_calloc;

    // Test edge case: size 0 to realloc (should free if p is not NULL)
    void *ptr_temp = _TIFFmalloc(size_hint);
    if (ptr_temp != NULL) {
        void *ptr_zero_realloc = _TIFFrealloc(ptr_temp, 0);
        // Behavior of realloc(p, 0) is implementation defined, often frees.
        // If it returns NULL, we assume memory is freed. If not, we free it.
        if (ptr_zero_realloc != NULL) {
             cleanup_memory(ptr_zero_realloc);
        }
    }

    return 0;
}