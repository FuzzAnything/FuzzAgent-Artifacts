// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFcalloc at tif_unix.c:341:7 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemset at tif_unix.c:353:6 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
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

static void cleanup_memory(void *ptr) {
    if (ptr != NULL) {
        _TIFFfree(ptr);
    }
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Extract parameters from input data if possible
    tmsize_t size1 = 0;
    tmsize_t size2 = 0;
    tmsize_t count = 0;
    int value = 0;

    if (Size >= sizeof(tmsize_t)) {
        memcpy(&size1, Data, sizeof(tmsize_t));
        size1 = (size1 > 0 && size1 < 1024 * 1024) ? size1 : 100; // Limit size for safety
    }
    if (Size >= 2 * sizeof(tmsize_t)) {
        memcpy(&size2, Data + sizeof(tmsize_t), sizeof(tmsize_t));
        size2 = (size2 > 0 && size2 < 1024 * 1024) ? size2 : 100;
    }
    if (Size >= 3 * sizeof(tmsize_t)) {
        memcpy(&count, Data + 2 * sizeof(tmsize_t), sizeof(tmsize_t));
        count = (count > 0 && count < 1024 * 1024) ? count : 100;
    }
    if (Size >= 4 * sizeof(tmsize_t)) {
        memcpy(&value, Data + 3 * sizeof(tmsize_t), sizeof(int));
    }

    // Test _TIFFmalloc
    void *mem1 = _TIFFmalloc(size1);
    if (mem1 == NULL && size1 != 0) {
        return 0;
    }

    // Test _TIFFcalloc
    void *mem2 = _TIFFcalloc(size2, sizeof(uint8_t));
    if (mem2 == NULL && size2 != 0) {
        cleanup_memory(mem1);
        return 0;
    }

    // Test _TIFFmemcpy
    if (mem1 != NULL && mem2 != NULL) {
        _TIFFmemcpy(mem1, mem2, (size1 < size2) ? size1 : size2);
    }

    // Test _TIFFmemset
    if (mem1 != NULL) {
        _TIFFmemset(mem1, value, size1);
    }

    // Test _TIFFmemcmp
    if (mem1 != NULL && mem2 != NULL) {
        tmsize_t cmp_size = (size1 < size2) ? size1 : size2;
        int cmp_result = _TIFFmemcmp(mem1, mem2, cmp_size);
        (void)cmp_result; // Use result to avoid unused variable warning
    }

    // Test _TIFFrealloc
    void *mem3 = _TIFFrealloc(mem1, size2);
    if (mem3 == NULL && mem1 != NULL && size2 != 0) {
        // Realloc failed, mem1 is still valid
        cleanup_memory(mem1);
        cleanup_memory(mem2);
        return 0;
    }
    if (mem1 != NULL) {
        // mem1 was freed by realloc if successful, so don't free it again
        mem1 = NULL;
    }
    mem1 = mem3;

    // Cleanup
    cleanup_memory(mem1);
    cleanup_memory(mem2);

    return 0;
}