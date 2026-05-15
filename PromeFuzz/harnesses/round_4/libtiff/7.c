// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzing data to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }
    
    // Call functions in specified order
    // 1. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);
    
    // 2. TIFFCurrentDirOffset
    TIFFCurrentDirOffset(tif);
    
    // 3. TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // 4. TIFFTileSize64
    uint64_t tile_size = TIFFTileSize64(tif);
    
    // 5. TIFFReadEncodedTile (only if we have tiles)
    if (num_tiles > 0 && tile_size > 0 && tile_size < 1024*1024) { // Limit to 1MB
        void *tile_buf = malloc((size_t)tile_size);
        if (tile_buf) {
            TIFFReadEncodedTile(tif, 0, tile_buf, (tmsize_t)tile_size);
            free(tile_buf);
        }
    }
    
    // 6. TIFFScanlineSize64
    uint64_t scanline_size = TIFFScanlineSize64(tif);
    
    // 7. TIFFReadScanline (only if we have reasonable scanline size)
    if (scanline_size > 0 && scanline_size < 1024*1024) { // Limit to 1MB
        void *scanline_buf = malloc((size_t)scanline_size);
        if (scanline_buf) {
            TIFFReadScanline(tif, scanline_buf, 0, 0);
            free(scanline_buf);
        }
    }
    
    // 8. TIFFClose (first call)
    TIFFClose(tif);
    
    // 9. TIFFClose (second call - on already closed file)
    // Note: TIFFClose on NULL is safe
    TIFFClose(NULL);
    
    // Clean up
    remove("./dummy_file");
    
    return 0;
}