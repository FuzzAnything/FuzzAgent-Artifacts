// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void cleanup(void *ptr) {
    if (ptr != NULL) {
        _TIFFfree(ptr);
    }
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    tmsize_t size1 = 0, size2 = 0, count = 0;
    void *ptr1 = NULL, *ptr2 = NULL, *ptr3 = NULL, *ptr4 = NULL;
    int result;

    // Extract parameters from input if available, ensuring bounds checks
    if (Size >= 12) {
        size1 = (tmsize_t)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
        size2 = (tmsize_t)(Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24));
        count = (tmsize_t)(Data[8] | (Data[9] << 8) | (Data[10] << 16) | (Data[11] << 24));
    } else if (Size >= 8) {
        size1 = (tmsize_t)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
        size2 = (tmsize_t)(Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24));
        count = Size;
    } else if (Size >= 4) {
        size1 = (tmsize_t)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
        size2 = Size;
        count = Size;
    } else {
        size1 = Size;
        size2 = Size;
        count = Size;
    }

    // Limit sizes to prevent excessive allocation and potential overflows
    if (size1 > 1024 * 1024) size1 = 1024 * 1024;
    if (size2 > 1024 * 1024) size2 = 1024 * 1024;
    if (count > 1024 * 1024) count = 1024 * 1024;

    // Ensure sizes are non-negative (tmsize_t is signed)
    if (size1 < 0) size1 = 0;
    if (size2 < 0) size2 = 0;
    if (count < 0) count = 0;

    // Test _TIFFmalloc
    ptr1 = _TIFFmalloc(size1);
    if (ptr1 != NULL) {
        // Test _TIFFmemset
        _TIFFmemset(ptr1, 0xAA, size1);
        cleanup(ptr1);
    }

    // Test _TIFFcalloc
    ptr2 = _TIFFcalloc(size1, size2);
    if (ptr2 != NULL) {
        // Test _TIFFmemcpy
        tmsize_t calloc_size = size1 * size2;
        if (calloc_size > 0 && calloc_size <= 1024 * 1024) {
            _TIFFmemset(ptr2, 0xBB, calloc_size);
        }
        cleanup(ptr2);
    }

    // Test _TIFFrealloc with NULL (acts like malloc)
    ptr3 = _TIFFrealloc(NULL, size1);
    if (ptr3 != NULL) {
        _TIFFmemset(ptr3, 0xCC, size1);
        cleanup(ptr3);
    }

    // Test _TIFFrealloc with valid pointer
    ptr4 = _TIFFmalloc(size1);
    if (ptr4 != NULL) {
        _TIFFmemset(ptr4, 0xDD, size1);
        ptr4 = _TIFFrealloc(ptr4, size2);
        if (ptr4 != NULL) {
            _TIFFmemset(ptr4, 0xEE, size2);
            cleanup(ptr4);
        }
    }

    // Test _TIFFrealloc with size 0 (may free)
    ptr4 = _TIFFmalloc(size1);
    if (ptr4 != NULL) {
        ptr4 = _TIFFrealloc(ptr4, 0);
        // ptr4 may be NULL or a valid pointer depending on implementation
        if (ptr4 != NULL) {
            cleanup(ptr4);
        }
    }

    // Test _TIFFmemcmp
    ptr1 = _TIFFmalloc(count);
    ptr2 = _TIFFmalloc(count);
    if (ptr1 != NULL && ptr2 != NULL) {
        _TIFFmemset(ptr1, 0x11, count);
        _TIFFmemset(ptr2, 0x22, count);
        result = _TIFFmemcmp(ptr1, ptr2, count);
        (void)result; // Suppress unused variable warning

        // Test with identical data
        _TIFFmemset(ptr2, 0x11, count);
        result = _TIFFmemcmp(ptr1, ptr2, count);
        (void)result;

        cleanup(ptr1);
        cleanup(ptr2);
    }

    return 0;
}