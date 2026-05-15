// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    // Write fuzz data to a file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Open the TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // Get basic image information
        uint32_t width = 0, height = 0;
        uint16_t samplesperpixel = 1, bitspersample = 8;
        uint32_t rowsperstrip = 0;
        uint16_t planarconfig = PLANARCONFIG_CONTIG;
        
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
        TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig);
        
        // Call target API functions
        
        // 1. TIFFTileSize64
        uint64_t tile_size = TIFFTileSize64(tif);
        (void)tile_size;
        
        // 2. TIFFRawStripSize64 - try with different strip indices
        uint32_t strip_count = TIFFNumberOfStrips(tif);
        if (strip_count > 0) {
            // Try first strip
            uint64_t raw_strip_size = TIFFRawStripSize64(tif, 0);
            (void)raw_strip_size;
            
            // Try middle strip if exists
            if (strip_count > 1) {
                raw_strip_size = TIFFRawStripSize64(tif, strip_count / 2);
                (void)raw_strip_size;
            }
            
            // Try last strip
            raw_strip_size = TIFFRawStripSize64(tif, strip_count - 1);
            (void)raw_strip_size;
        }
        
        // 3. TIFFStripSize64
        uint64_t strip_size = TIFFStripSize64(tif);
        (void)strip_size;
        
        // 4. TIFFGetStrileByteCount - try with different strile indices
        uint32_t strile_count = TIFFIsTiled(tif) ? TIFFNumberOfTiles(tif) : strip_count;
        if (strile_count > 0) {
            // Try first strile
            uint64_t strile_bytecount = TIFFGetStrileByteCount(tif, 0);
            (void)strile_bytecount;
            
            // Try middle strile if exists
            if (strile_count > 1) {
                strile_bytecount = TIFFGetStrileByteCount(tif, strile_count / 2);
                (void)strile_bytecount;
            }
            
            // Try last strile
            strile_bytecount = TIFFGetStrileByteCount(tif, strile_count - 1);
            (void)strile_bytecount;
        }
        
        // 5. TIFFVStripSize64 - try with different row counts
        if (height > 0) {
            // Try with 1 row
            uint64_t vstrip_size = TIFFVStripSize64(tif, 1);
            (void)vstrip_size;
            
            // Try with rows per strip if defined
            if (rowsperstrip > 0) {
                vstrip_size = TIFFVStripSize64(tif, rowsperstrip);
                (void)vstrip_size;
            }
            
            // Try with half height
            vstrip_size = TIFFVStripSize64(tif, height / 2);
            (void)vstrip_size;
            
            // Try with full height
            vstrip_size = TIFFVStripSize64(tif, height);
            (void)vstrip_size;
        }
        
        // 6. TIFFVTileSize64 - try with different row counts
        if (height > 0) {
            // Try with 1 row
            uint64_t vtile_size = TIFFVTileSize64(tif, 1);
            (void)vtile_size;
            
            // Get tile height if tiled
            uint32_t tilelength = 0;
            if (TIFFGetField(tif, TIFFTAG_TILELENGTH, &tilelength) && tilelength > 0) {
                vtile_size = TIFFVTileSize64(tif, tilelength);
                (void)vtile_size;
            }
            
            // Try with half height
            vtile_size = TIFFVTileSize64(tif, height / 2);
            (void)vtile_size;
            
            // Try with full height
            vtile_size = TIFFVTileSize64(tif, height);
            (void)vtile_size;
        }
        
        // Close the TIFF file
        TIFFClose(tif);
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}