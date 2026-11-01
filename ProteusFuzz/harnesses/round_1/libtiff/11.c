// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static int write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    size_t written = fwrite(Data, 1, Size, f);
    fclose(f);
    return (written == Size) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Prepare environment: write input to a dummy file
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }

    // Open the file to get a valid TIFF handle
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Step 1: Invoke TIFFStripSize
    // This function requires a valid TIFF handle. It may return 0 if the file is invalid or strip count is 0.
    tmsize_t strip_size = TIFFStripSize(tif);

    // Step 2: Invoke _TIFFmalloc
    // Use the strip_size or a derived value from input to allocate memory.
    // Ensure size is not negative (tmsize_t is signed).
    tmsize_t alloc_size = 0;
    if (strip_size > 0) {
        alloc_size = strip_size;
    } else {
        // Fallback to input data size if strip_size is 0, capped to avoid huge allocations
        alloc_size = (Size > 1024 * 1024) ? 1024 * 1024 : Size;
    }

    void *ptr = _TIFFmalloc(alloc_size);

    // Step 3: Invoke _TIFFfree (first time)
    // Free the memory allocated by _TIFFmalloc
    if (ptr != NULL) {
        _TIFFfree(ptr);
        ptr = NULL; // Ensure we don't double free if logic changes
    }

    // Step 4: Invoke _TIFFfree (second time)
    // The requirement asks to invoke _TIFFfree twice.
    // Since we already freed 'ptr', we must create a new allocation or pass NULL.
    // Passing NULL is safe according to the description ("passing NULL is safe").
    // However, to explore more states, let's allocate a small dummy block and free it.
    void *ptr2 = _TIFFmalloc(1);
    if (ptr2 != NULL) {
        _TIFFfree(ptr2);
    }
    // Now invoke the second _TIFFfree as requested in the sequence.
    // We pass NULL here to satisfy the "invoke _TIFFfree" requirement safely after the first free.
    _TIFFfree(NULL);

    // Cleanup
    TIFFClose(tif);

    return 0;
}