// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Limit allocation sizes to prevent OOM or extremely long runs
    const size_t max_alloc = 1024 * 1024; // 1MB limit
    size_t alloc_size = (Size > max_alloc) ? max_alloc : Size;

    // Test _TIFFmalloc
    void *ptr1 = _TIFFmalloc(alloc_size);
    if (ptr1 == NULL && alloc_size > 0) {
        return 0;
    }

    // Test _TIFFmemset
    _TIFFmemset(ptr1, 0xAA, alloc_size);

    // Test _TIFFmemcpy
    void *ptr2 = _TIFFmalloc(alloc_size);
    if (ptr2 != NULL) {
        _TIFFmemcpy(ptr2, ptr1, alloc_size);
        
        // Test _TIFFmemcmp
        int cmp_result = _TIFFmemcmp(ptr1, ptr2, alloc_size);
        (void)cmp_result; // Suppress unused variable warning

        // Test _TIFFrealloc on ptr2
        size_t new_size = (alloc_size * 2) > max_alloc ? max_alloc : (alloc_size * 2);
        void *ptr3 = _TIFFrealloc(ptr2, new_size);
        if (ptr3 != NULL) {
            _TIFFmemset(ptr3, 0xBB, new_size);
            _TIFFfree(ptr3);
        } else {
            _TIFFfree(ptr2);
        }
    }

    // Test _TIFFcalloc
    tmsize_t nmemb = (alloc_size > 0) ? 1 : 0;
    tmsize_t siz = (alloc_size > 0) ? alloc_size : 0;
    void *ptr4 = _TIFFcalloc(nmemb, siz);
    if (ptr4 != NULL) {
        _TIFFmemset(ptr4, 0xCC, nmemb * siz);
        _TIFFfree(ptr4);
    }

    // Test _TIFFrealloc with NULL (acts like malloc)
    void *ptr5 = _TIFFrealloc(NULL, alloc_size);
    if (ptr5 != NULL) {
        _TIFFmemset(ptr5, 0xDD, alloc_size);
        _TIFFfree(ptr5);
    }

    // Test _TIFFrealloc with size 0 (behavior depends on implementation, often frees)
    if (ptr1 != NULL) {
        void *ptr6 = _TIFFrealloc(ptr1, 0);
        if (ptr6 != NULL) {
            _TIFFfree(ptr6);
        }
    }

    // Cleanup
    if (ptr1 != NULL) {
        _TIFFfree(ptr1);
    }

    return 0;
}