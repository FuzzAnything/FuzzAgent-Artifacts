// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Allocate options
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    // Set max single memory allocation limit using input data
    // Ensure the value is positive and reasonable to avoid immediate errors
    tmsize_t limit = 0;
    if (Size >= sizeof(tmsize_t)) {
        memcpy(&limit, Data, sizeof(tmsize_t));
        if (limit <= 0) {
            limit = 1;
        }
    } else {
        limit = 1;
    }
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, limit);

    // Create a dummy file for testing
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp != NULL) {
        // Write some dummy data to the file to ensure it's not empty
        size_t write_size = (Size > 1024) ? 1024 : Size;
        if (write_size > 0) {
            fwrite(Data, 1, write_size, fp);
        }
        fclose(fp);
    }

    // Open the file with custom options (Write mode)
    TIFF *tif = TIFFOpenExt(filename, "w", opts);
    
    // Free options as per requirement
    TIFFOpenOptionsFree(opts);

    if (tif == NULL) {
        return 0;
    }

    // Invoke TIFFSetField twice as requested
    // First call: Set ImageWidth (Tag 256)
    uint32_t width = 0;
    if (Size >= sizeof(uint32_t)) {
        memcpy(&width, Data, sizeof(uint32_t));
        if (width == 0) width = 1;
    } else {
        width = 1;
    }
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);

    // Second call: Set ImageLength (Tag 257)
    uint32_t length = 0;
    if (Size >= sizeof(uint32_t) * 2) {
        memcpy(&length, Data + sizeof(uint32_t), sizeof(uint32_t));
        if (length == 0) length = 1;
    } else {
        length = 1;
    }
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, length);

    // Write the directory
    TIFFWriteDirectory(tif);

    // Close the file (First TIFFClose)
    TIFFClose(tif);

    // Continue with the sequence: SetField -> SetField -> WriteDirectory -> Close
    // These calls are made on a closed handle to test error handling/robustness
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, length);
    
    TIFFWriteDirectory(tif);
    
    // TIFFClose on already closed handle
    TIFFClose(tif);

    // Cleanup: Remove dummy file
    unlink(filename);

    return 0;
}