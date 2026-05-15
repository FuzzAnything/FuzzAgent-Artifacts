// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFRasterScanlineSize64 at tif_strip.c:357:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    // Write fuzzer input to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    // Open the file with libtiff
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    unlink("./dummy_file");
    
    return tif;
}

int LLVMFuzzerTestOneInput_47(const uint8_t* Data, size_t Size) {
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Create a TIFF handle from fuzzer input
    TIFF* tif = create_dummy_tiff(Data, Size);
    if (!tif) return 0;
    
    // Call target API functions with the TIFF handle
    uint64_t val1 = TIFFScanlineSize64(tif);
    uint64_t val2 = TIFFStripSize64(tif);
    
    // For TIFFVStripSize64, use various row counts
    uint32_t nrows = 1;
    if (Size > 0) {
        nrows = Data[0] % 100 + 1; // Random row count between 1-100
    }
    uint64_t val3 = TIFFVStripSize64(tif, nrows);
    
    uint64_t val4 = TIFFTileRowSize64(tif);
    uint64_t val5 = TIFFTileSize64(tif);
    uint64_t val6 = TIFFRasterScanlineSize64(tif);
    
    // Try to explore more states by calling with different parameters
    // Use different row counts for TIFFVStripSize64
    if (Size > 1) {
        uint32_t nrows2 = Data[1] % 1000 + 1;
        uint64_t val7 = TIFFVStripSize64(tif, nrows2);
    }
    
    // Clean up
    TIFFClose(tif);
    
    // Use the values to avoid compiler optimizations
    volatile uint64_t dummy = val1 + val2 + val3 + val4 + val5 + val6;
    (void)dummy;
    
    return 0;
}