// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    // Use input data to determine max_single_mem_alloc, ensuring it's not zero to avoid trivial cases
    tmsize_t max_alloc = 0;
    if (Size > 0) {
        max_alloc = Data[0];
        if (max_alloc == 0) {
            max_alloc = 1024;
        }
    } else {
        max_alloc = 1024;
    }

    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_alloc);

    // Attempt to open the file for writing
    TIFF *tif = TIFFOpenExt("./dummy_file", "w", opts);

    TIFFOpenOptionsFree(opts);

    if (tif == NULL) {
        return 0;
    }

    // Set some fields to exercise the logic
    // Using simple values derived from input to vary state
    uint32_t width = (Size > 0) ? (Data[0] & 0xFF) : 1;
    uint32_t height = (Size > 1) ? (Data[1] & 0xFF) : 1;
    if (width == 0) width = 1;
    if (height == 0) height = 1;

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);

    // Write the directory
    TIFFWriteDirectory(tif);

    // Close the file
    TIFFClose(tif);

    // Attempt to close again to test NULL handling or double close safety if applicable
    // Note: tif is now invalid, but TIFFClose handles NULL gracefully.
    // We don't pass tif here as it's undefined behavior to use after close, 
    // but we can test the NULL path explicitly if needed.
    TIFFClose(NULL);

    return 0;
}