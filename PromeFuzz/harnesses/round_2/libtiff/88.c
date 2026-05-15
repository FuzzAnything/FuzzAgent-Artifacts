// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCountWithErr at tif_dirread.c:8373:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the dummy file as a TIFF
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }
    
    // Get basic image information
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    uint32_t rowsperstrip = 0;
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    if (rowsperstrip == 0 || rowsperstrip > height) {
        rowsperstrip = height;
    }
    
    // Determine if image is tiled or stripped
    uint32_t is_tiled = TIFFIsTiled(tif);
    uint32_t strile_count = 0;
    
    if (is_tiled) {
        uint32_t tilewidth = 0, tilelength = 0;
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tilewidth);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tilelength);
        
        if (tilewidth > 0 && tilelength > 0) {
            uint32_t tiles_across = (width + tilewidth - 1) / tilewidth;
            uint32_t tiles_down = (height + tilelength - 1) / tilelength;
            strile_count = tiles_across * tiles_down;
        }
    } else {
        strile_count = TIFFNumberOfStrips(tif);
    }
    
    // Test the target functions with various parameters
    if (strile_count > 0) {
        // Test with first strile
        uint32_t strile1 = 0;
        
        // TIFFGetStrileOffsetWithErr
        int err1 = 0;
        uint64_t offset1 = TIFFGetStrileOffsetWithErr(tif, strile1, &err1);
        
        // TIFFGetStrileByteCount
        uint64_t bytecount1 = TIFFGetStrileByteCount(tif, strile1);
        
        // TIFFGetStrileByteCountWithErr
        int err2 = 0;
        uint64_t bytecount2 = TIFFGetStrileByteCountWithErr(tif, strile1, &err2);
        
        // TIFFGetStrileOffset
        uint64_t offset2 = TIFFGetStrileOffset(tif, strile1);
        
        // Test with middle strile if available
        if (strile_count > 2) {
            uint32_t strile2 = strile_count / 2;
            
            int err3 = 0;
            TIFFGetStrileOffsetWithErr(tif, strile2, &err3);
            
            TIFFGetStrileByteCount(tif, strile2);
            
            int err4 = 0;
            TIFFGetStrileByteCountWithErr(tif, strile2, &err4);
            
            TIFFGetStrileOffset(tif, strile2);
        }
        
        // Test with last strile
        uint32_t strile3 = strile_count - 1;
        
        int err5 = 0;
        TIFFGetStrileOffsetWithErr(tif, strile3, &err5);
        
        TIFFGetStrileByteCount(tif, strile3);
        
        int err6 = 0;
        TIFFGetStrileByteCountWithErr(tif, strile3, &err6);
        
        TIFFGetStrileOffset(tif, strile3);
    }
    
    // Test TIFFVStripSize64 with various row counts
    if (height > 0) {
        // Test with 1 row
        TIFFVStripSize64(tif, 1);
        
        // Test with rowsperstrip
        TIFFVStripSize64(tif, rowsperstrip);
        
        // Test with half of rowsperstrip
        if (rowsperstrip > 1) {
            TIFFVStripSize64(tif, rowsperstrip / 2);
        }
        
        // Test with full image height
        TIFFVStripSize64(tif, height);
    }
    
    // Test TIFFRawStripSize64 with various strip indices
    if (!is_tiled && strile_count > 0) {
        // Test with first strip
        TIFFRawStripSize64(tif, 0);
        
        // Test with middle strip if available
        if (strile_count > 2) {
            TIFFRawStripSize64(tif, strile_count / 2);
        }
        
        // Test with last strip
        TIFFRawStripSize64(tif, strile_count - 1);
        
        // Test with out-of-bounds strip (should handle gracefully)
        if (strile_count < UINT32_MAX) {
            TIFFRawStripSize64(tif, strile_count);
        }
    }
    
    // Clean up
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}