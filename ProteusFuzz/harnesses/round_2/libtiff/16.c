// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
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

static void safe_free(void *ptr) {
    if (ptr != NULL) {
        _TIFFfree(ptr);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Extract parameters from fuzz input to drive diversity
    uint32_t seed = 0;
    if (Size >= 4) {
        seed = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }

    // Determine sizes, clamping to reasonable limits to avoid OOM or crashes
    tmsize_t size1 = (seed % 1024) + 1;
    tmsize_t size2 = ((seed >> 16) % 1024) + 1;
    tmsize_t count = (seed % 256) + 1;
    int fill_val = (int)(Data[0]);

    // 1. Test _TIFFmalloc and _TIFFmemset
    void *ptr1 = _TIFFmalloc(size1);
    if (ptr1 != NULL) {
        _TIFFmemset(ptr1, fill_val, size1);
        
        // Test _TIFFmemcpy
        void *ptr2 = _TIFFmalloc(size1);
        if (ptr2 != NULL) {
            _TIFFmemcpy(ptr2, ptr1, size1);
            safe_free(ptr2);
        }

        // Test _TIFFmemcmp (compare with itself)
        int cmp_result = _TIFFmemcmp(ptr1, ptr1, size1);
        (void)cmp_result; // Suppress unused warning

        safe_free(ptr1);
    }

    // 2. Test _TIFFcalloc
    void *ptr3 = _TIFFcalloc(count, size2);
    if (ptr3 != NULL) {
        // Verify it is zeroed (optional check for logic, not strictly required for fuzzing)
        // _TIFFmemset(ptr3, 0xFF, count * size2); // Overwrite to test cleanup
        safe_free(ptr3);
    }

    // 3. Test _TIFFrealloc
    void *ptr4 = _TIFFmalloc(size1);
    if (ptr4 != NULL) {
        // Realloc to a new size
        tmsize_t new_size = ((seed >> 8) % 2048) + 1;
        void *ptr5 = _TIFFrealloc(ptr4, new_size);
        if (ptr5 != NULL) {
            // Use the new buffer
            _TIFFmemset(ptr5, fill_val, new_size);
            safe_free(ptr5);
        } else {
            // If realloc fails, original ptr4 might be invalid or valid depending on impl
            // Standard realloc behavior: if new size is 0, it might free.
            // If it returns NULL, ptr4 is usually freed by the library or remains valid?
            // libtiff _TIFFrealloc usually follows standard realloc: if new size is 0, it frees.
            // If it fails to resize, ptr4 is still valid.
            safe_free(ptr4);
        }
    }

    // 4. Test edge cases with 0 sizes
    void *ptr_zero = _TIFFmalloc(0);
    if (ptr_zero != NULL) {
        safe_free(ptr_zero);
    }

    void *ptr_calloc_zero = _TIFFcalloc(0, 10);
    if (ptr_calloc_zero != NULL) {
        safe_free(ptr_calloc_zero);
    }

    void *ptr_realloc_zero = _TIFFrealloc(NULL, 0);
    if (ptr_realloc_zero != NULL) {
        safe_free(ptr_realloc_zero);
    }

    // 5. Test _TIFFmemset with large size if input allows (bounded)
    if (Size > 10) {
        tmsize_t large_size = (Data[Size-1] * 100) + 10;
        void *ptr_large = _TIFFmalloc(large_size);
        if (ptr_large != NULL) {
            _TIFFmemset(ptr_large, 0x00, large_size);
            safe_free(ptr_large);
        }
    }

    return 0;
}