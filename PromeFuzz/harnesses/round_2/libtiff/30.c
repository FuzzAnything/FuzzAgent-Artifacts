// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsCODECConfigured at tif_codec.c:146:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void dummy_file_cleanup(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Register cleanup
    atexit(dummy_file_cleanup);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        dummy_file_cleanup();
        return 0;
    }
    
    // Extract parameters from fuzzer input
    uint32_t x = 0, y = 0, z = 0, row = 0, tile_row = 0, tile_col = 0;
    uint16_t s = 0, codec = 0;
    int stop_on_error = 0;
    
    // Use first bytes for parameters if available
    if (Size >= 4) memcpy(&x, Data, 4);
    if (Size >= 8) memcpy(&y, Data + 4, 4);
    if (Size >= 12) memcpy(&z, Data + 8, 4);
    if (Size >= 14) memcpy(&s, Data + 12, 2);
    if (Size >= 16) memcpy(&row, Data + 14, 2);
    if (Size >= 18) memcpy(&tile_row, Data + 16, 2);
    if (Size >= 20) memcpy(&tile_col, Data + 18, 2);
    if (Size >= 22) memcpy(&codec, Data + 20, 2);
    if (Size >= 23) stop_on_error = Data[22] & 1;
    
    // Call TIFFIsCODECConfigured
    TIFFIsCODECConfigured(codec);
    
    // Call TIFFIsTiled
    int is_tiled = TIFFIsTiled(tif);
    
    // Allocate buffer for RGBA data
    uint32_t *rgba_buffer = NULL;
    size_t buffer_size = 0;
    
    // Try to get image dimensions to allocate appropriate buffer
    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        buffer_size = width * height;
        if (buffer_size > 0 && buffer_size < 1024 * 1024) { // Limit to 1MP
            rgba_buffer = (uint32_t *)malloc(buffer_size * sizeof(uint32_t));
        }
    }
    
    if (!rgba_buffer) {
        // Fallback to reasonable size
        buffer_size = 256 * 256;
        rgba_buffer = (uint32_t *)malloc(buffer_size * sizeof(uint32_t));
    }
    
    if (rgba_buffer) {
        // Call TIFFReadRGBATile if tiled
        if (is_tiled) {
            TIFFReadRGBATile(tif, tile_col, tile_row, rgba_buffer);
        }
        
        // Call TIFFReadRGBATileExt if tiled
        if (is_tiled) {
            TIFFReadRGBATileExt(tif, tile_col, tile_row, rgba_buffer, stop_on_error);
        }
        
        // Call TIFFReadRGBAStrip
        TIFFReadRGBAStrip(tif, row, rgba_buffer);
        
        free(rgba_buffer);
    }
    
    // Call TIFFCheckTile
    TIFFCheckTile(tif, x, y, z, s);
    
    // Cleanup
    TIFFClose(tif);
    dummy_file_cleanup();
    
    return 0;
}