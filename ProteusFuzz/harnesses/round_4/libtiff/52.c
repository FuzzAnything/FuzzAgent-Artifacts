// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

/* Fuzz driver for libtiff memory management and utility functions */

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Prepare environment: allocate buffers using libtiff wrappers */
    tmsize_t alloc_size = Size;
    if (alloc_size > 1000000) {
        alloc_size = 1000000; /* Cap allocation to prevent OOM */
    }

    void *buf1 = _TIFFmalloc(alloc_size);
    void *buf2 = _TIFFmalloc(alloc_size);
    void *buf3 = _TIFFcalloc(alloc_size, 1);
    void *buf4 = _TIFFcalloc(1, alloc_size);

    if (!buf1 || !buf2 || !buf3 || !buf4) {
        /* Cleanup on allocation failure */
        _TIFFfree(buf1);
        _TIFFfree(buf2);
        _TIFFfree(buf3);
        _TIFFfree(buf4);
        return 0;
    }

    /* Initialize buffers with fuzz data */
    _TIFFmemcpy(buf1, Data, Size);
    _TIFFmemcpy(buf2, Data, Size);
    _TIFFmemset(buf3, 0xAA, alloc_size);
    _TIFFmemset(buf4, 0xBB, alloc_size);

    /* Test _TIFFmemcmp */
    int cmp_result = _TIFFmemcmp(buf1, buf2, Size);
    (void)cmp_result; /* Suppress unused warning */

    /* Test _TIFFrealloc */
    tmsize_t new_size = Size + 10;
    if (new_size > 1000000) {
        new_size = 1000000;
    }

    void *reallocated = _TIFFrealloc(buf1, new_size);
    if (reallocated) {
        buf1 = reallocated;
        /* Write some data to the new region if expanded */
        if (new_size > Size) {
            _TIFFmemset((char *)buf1 + Size, 0xFF, new_size - Size);
        }
    } else {
        /* realloc failed, buf1 is still valid if it was not NULL originally */
        /* In this case, we just proceed to free the original */
    }

    /* Test realloc with NULL pointer (acts like malloc) */
    void *null_realloc = _TIFFrealloc(NULL, 64);
    if (null_realloc) {
        _TIFFmemset(null_realloc, 0, 64);
        _TIFFfree(null_realloc);
    }

    /* Test realloc with size 0 (often frees) */
    void *zero_realloc = _TIFFrealloc(buf2, 0);
    /* zero_realloc might be NULL or a unique pointer, handle safely */
    if (zero_realloc) {
        _TIFFfree(zero_realloc);
    } else {
        /* If it freed, buf2 is now invalid, do not free again */
        buf2 = NULL;
    }

    /* Test _TIFFmemcpy with overlapping regions. 
       _TIFFmemcpy wraps memcpy, which has undefined behavior for overlapping regions.
       To avoid AddressSanitizer errors, we use a separate source buffer or memcpy logic.
       Here we create a temporary copy to simulate the operation safely without overlap.
    */
    if (Size > 10) {
        void *temp_buf = _TIFFmalloc(Size - 5);
        if (temp_buf) {
            _TIFFmemcpy(temp_buf, buf3, Size - 5);
            /* Now copy from temp_buf to the overlapping region in buf3 */
            _TIFFmemcpy((char *)buf3 + 5, temp_buf, Size - 5);
            _TIFFfree(temp_buf);
        }
    }

    /* Test _TIFFmemset with various values */
    _TIFFmemset(buf3, -1, alloc_size);
    _TIFFmemset(buf4, 128, alloc_size);

    /* Cleanup */
    _TIFFfree(buf1);
    _TIFFfree(buf2);
    _TIFFfree(buf3);
    _TIFFfree(buf4);

    return 0;
}