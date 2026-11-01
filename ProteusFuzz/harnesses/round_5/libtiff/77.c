// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFrealloc at tif_unix.c:351:7 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void cleanup_memory(void *ptr) {
    if (ptr != NULL) {
        _TIFFfree(ptr);
    }
}

static tmsize_t safe_read_tmsize(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + sizeof(tmsize_t)) {
        return 0;
    }
    tmsize_t val;
    memcpy(&val, Data + offset, sizeof(tmsize_t));
    return val;
}

static int safe_read_int(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + sizeof(int)) {
        return 0;
    }
    int val;
    memcpy(&val, Data + offset, sizeof(int));
    return val;
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Extract parameters from input data safely
    tmsize_t size1 = safe_read_tmsize(Data, Size, 0);
    tmsize_t size2 = safe_read_tmsize(Data, Size, sizeof(tmsize_t));
    tmsize_t count = safe_read_tmsize(Data, Size, 2 * sizeof(tmsize_t));
    int value = safe_read_int(Data, Size, 3 * sizeof(tmsize_t));

    // Clamp sizes to reasonable limits to avoid crashes or OOM
    if (size1 > 1024 * 1024) size1 = 1024 * 1024;
    if (size2 > 1024 * 1024) size2 = 1024 * 1024;
    if (count > 1024 * 1024) count = 1024 * 1024;

    // Test _TIFFmalloc
    void *ptr_malloc = _TIFFmalloc(size1);
    if (ptr_malloc != NULL) {
        // Test _TIFFmemset
        _TIFFmemset(ptr_malloc, value, size1);
        // Test _TIFFmemcpy
        if (Size > 0) {
            tmsize_t copy_len = (Size < size1) ? Size : size1;
            _TIFFmemcpy(ptr_malloc, Data, copy_len);
        }
        // Test _TIFFrealloc (grow)
        void *ptr_realloc_grow = _TIFFrealloc(ptr_malloc, size1 + size2);
        // ptr_malloc is no longer valid after realloc, do not free it
        if (ptr_realloc_grow != NULL) {
            _TIFFmemset(ptr_realloc_grow, value + 1, size1 + size2);
            // Test _TIFFrealloc (shrink)
            void *ptr_realloc_shrink = _TIFFrealloc(ptr_realloc_grow, size1 / 2);
            // ptr_realloc_grow is no longer valid after realloc, do not free it
            if (ptr_realloc_shrink != NULL) {
                _TIFFmemset(ptr_realloc_shrink, value + 2, size1 / 2);
                cleanup_memory(ptr_realloc_shrink);
            }
            // If shrink failed, we still own ptr_realloc_grow
            if (ptr_realloc_shrink == NULL) {
                cleanup_memory(ptr_realloc_grow);
            }
        } else {
            // If grow failed, we still own ptr_malloc
            cleanup_memory(ptr_malloc);
        }
    }

    // Test _TIFFcalloc
    void *ptr_calloc = _TIFFcalloc(size1, size2);
    if (ptr_calloc != NULL) {
        // Verify it's zeroed
        if (_TIFFmemcmp(ptr_calloc, ptr_calloc, 1) != 0) {
            // This check is trivial but exercises the memcmp path
        }
        // Test _TIFFrealloc on calloc result
        void *ptr_calloc_realloc = _TIFFrealloc(ptr_calloc, size1 * size2 + 100);
        // ptr_calloc is no longer valid after realloc
        if (ptr_calloc_realloc != NULL) {
            _TIFFmemset(ptr_calloc_realloc, value, size1 * size2 + 100);
            cleanup_memory(ptr_calloc_realloc);
        } else {
            // If realloc failed, we still own ptr_calloc
            cleanup_memory(ptr_calloc);
        }
    }

    // Test _TIFFrealloc with NULL (acts like malloc)
    void *ptr_null_realloc = _TIFFrealloc(NULL, size1);
    if (ptr_null_realloc != NULL) {
        _TIFFmemset(ptr_null_realloc, value, size1);
        cleanup_memory(ptr_null_realloc);
    }

    // Test _TIFFrealloc with size 0 (should free)
    void *ptr_free_test = _TIFFmalloc(size1);
    if (ptr_free_test != NULL) {
        void *ptr_zero_realloc = _TIFFrealloc(ptr_free_test, 0);
        // ptr_free_test is freed by realloc(, 0), do not access or free it
        // ptr_zero_realloc should be NULL
        if (ptr_zero_realloc != NULL) {
            // This branch should ideally not be taken if realloc(,0) behaves as free
            cleanup_memory(ptr_zero_realloc);
        }
    }

    // Test _TIFFmemcmp with various inputs
    if (Size >= 2) {
        const uint8_t *p1 = Data;
        const uint8_t *p2 = Data + 1;
        tmsize_t cmp_len = (Size < count) ? Size : count;
        if (cmp_len > 0) {
            int cmp_result = _TIFFmemcmp(p1, p2, cmp_len);
            (void)cmp_result; // Use result to avoid unused warning
        }
    }

    // Test edge case: _TIFFcalloc with zero arguments
    void *ptr_calloc_zero1 = _TIFFcalloc(0, size1);
    if (ptr_calloc_zero1 != NULL) {
        cleanup_memory(ptr_calloc_zero1);
    }
    void *ptr_calloc_zero2 = _TIFFcalloc(size1, 0);
    if (ptr_calloc_zero2 != NULL) {
        cleanup_memory(ptr_calloc_zero2);
    }

    // Test _TIFFmalloc with size 0
    void *ptr_malloc_zero = _TIFFmalloc(0);
    if (ptr_malloc_zero != NULL) {
        cleanup_memory(ptr_malloc_zero);
    }

    return 0;
}