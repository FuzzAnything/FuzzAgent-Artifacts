// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFRasterScanlineSize64 at tif_strip.c:357:10 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    // Write data to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Open the dummy file
    return TIFFOpen("./dummy_file", "r");
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    TIFF* tif = create_dummy_tiff(Data, Size);
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Call all target API functions
    uint64_t result;
    
    result = TIFFRasterScanlineSize64(tif);
    result = TIFFTileRowSize64(tif);
    result = TIFFScanlineSize64(tif);
    result = TIFFTileSize64(tif);
    result = TIFFStripSize64(tif);
    
    // For TIFFVStripSize64, generate nrows from input data
    uint32_t nrows = 1;
    if (Size >= 8) {
        // Use first 4 bytes for nrows, ensuring it's not zero
        memcpy(&nrows, Data, sizeof(uint32_t));
        if (nrows == 0) nrows = 1;
    }
    result = TIFFVStripSize64(tif, nrows);
    
    // Try to explore more states by calling with different nrows values
    if (Size >= 12) {
        uint32_t nrows2;
        memcpy(&nrows2, Data + 4, sizeof(uint32_t));
        if (nrows2 != 0 && nrows2 != nrows) {
            result = TIFFVStripSize64(tif, nrows2);
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}