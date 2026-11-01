// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file with the fuzzing data
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Open the TIFF file
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        unlink(filename);
        return 0;
    }

    // 1. Invoke TIFFStripSize
    // This function returns the size of the current strip.
    // It may return 0 if the file is invalid or strip count is zero.
    tmsize_t strip_size = TIFFStripSize(tif);

    // 2. Invoke _TIFFmalloc
    // Allocate memory based on the strip size or a fuzzed value if strip_size is 0.
    // We use a safe upper bound to prevent excessive allocation.
    tmsize_t alloc_size = 0;
    if (strip_size > 0 && strip_size < (1024 * 1024)) {
        alloc_size = strip_size;
    } else if (Size > 0 && Size < (1024 * 1024)) {
        alloc_size = Size;
    } else {
        alloc_size = 1; // Fallback to minimal allocation
    }

    void *ptr = _TIFFmalloc(alloc_size);
    if (ptr) {
        // Write some data to the allocated buffer to ensure it's touched
        memset(ptr, 0xAA, alloc_size);
    }

    // 3. Invoke _TIFFfree (first time)
    // Free the memory allocated by _TIFFmalloc
    if (ptr) {
        _TIFFfree(ptr);
        ptr = NULL;
    }

    // 4. Invoke _TIFFfree (second time)
    // Passing NULL is safe according to the specification.
    // This satisfies the requirement to call _TIFFfree twice.
    _TIFFfree(ptr);

    // Cleanup
    TIFFClose(tif);
    unlink(filename);

    return 0;
}