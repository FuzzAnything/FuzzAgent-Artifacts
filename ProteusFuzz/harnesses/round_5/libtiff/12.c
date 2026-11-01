// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
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

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {}
static void dummy_warn_handler(const char *module, const char *fmt, va_list ap) {}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    // Set max cumulated memory allocation based on input data to explore states
    // Clamp to reasonable limits to avoid immediate OOM or invalid states
    tmsize_t limit = (tmsize_t)Data[0];
    if (limit == 0) {
        limit = 1024 * 1024; // 1MB default if input is 0
    }
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, limit);

    // Write data to dummy file to ensure it exists and has content
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Open the file for writing
    TIFF *tif = TIFFOpenExt("./dummy_file", "w", opts);
    
    // Free options immediately after use as per sequence
    TIFFOpenOptionsFree(opts);

    if (tif == NULL) {
        return 0;
    }

    // Set some fields to trigger internal logic
    // Use input data to vary tag values
    uint16_t width = (Size > 0) ? (uint16_t)(Data[0] & 0xFF) : 1;
    if (width == 0) width = 1;
    uint16_t height = (Size > 1) ? (uint16_t)(Data[1] & 0xFF) : 1;
    if (height == 0) height = 1;

    // First TIFFSetField call
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    
    // Second TIFFSetField call
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);

    // Write the directory
    TIFFWriteDirectory(tif);

    // Close the file
    TIFFClose(tif);

    return 0;
}