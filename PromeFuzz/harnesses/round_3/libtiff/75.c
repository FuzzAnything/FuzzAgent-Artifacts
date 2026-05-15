// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenOptionsSetWarnAboutUnknownTags at tif_open.c:115:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libtiff/tiffio.h"

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing - suppress errors during fuzzing
}

static void dummy_warn_handler(const char* module, const char* fmt, va_list ap) {
    // Do nothing - suppress warnings during fuzzing
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Write fuzz data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Allocate options
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        return 0;
    }

    // Set various options based on fuzz data
    // Use first 8 bytes for memory limits (if available)
    tmsize_t max_cumulated = 0;
    tmsize_t max_single = 0;
    
    if (Size >= 8) {
        // Use first 4 bytes for max_cumulated (but ensure positive)
        max_cumulated = (tmsize_t)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
        if (max_cumulated < 0) max_cumulated = -max_cumulated;
        
        // Use next 4 bytes for max_single
        max_single = (tmsize_t)(Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24));
        if (max_single < 0) max_single = -max_single;
    } else {
        // Use Size for both if not enough data
        max_cumulated = (tmsize_t)Size * 1024;
        max_single = (tmsize_t)Size * 512;
    }

    // Set memory limits
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, max_cumulated);
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_single);
    
    // Set warning option based on first byte
    int warn_unknown = (Data[0] & 1) ? 1 : 0;
    TIFFOpenOptionsSetWarnAboutUnknownTags(opts, warn_unknown);

    // Try to open the file with options
    TIFF *tif = TIFFOpenExt("./dummy_file", "r", opts);
    
    if (tif) {
        // If opened successfully, try some operations
        uint32_t width = 0, height = 0;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        
        // Try to read a scanline if possible
        if (width > 0 && height > 0) {
            tmsize_t scanline_size = TIFFScanlineSize(tif);
            if (scanline_size > 0) {
                uint8_t *buffer = (uint8_t*)_TIFFmalloc(scanline_size);
                if (buffer) {
                    for (uint32_t row = 0; row < height && row < 10; row++) {
                        TIFFReadScanline(tif, buffer, row, 0);
                    }
                    _TIFFfree(buffer);
                }
            }
        }
        
        TIFFClose(tif);
    }

    // Free options
    TIFFOpenOptionsFree(opts);
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}