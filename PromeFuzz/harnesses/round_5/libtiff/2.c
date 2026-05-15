// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Create dummy file with fuzzer data
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open TIFF file for reading/writing
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try opening as write-only if read-write fails
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            remove("./dummy_file");
            return 0;
        }
        
        // Set some basic fields to make the file valid
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 64);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 64);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 16);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    }
    
    // Call functions in specified order
    
    // 1. TIFFFileName
    const char *filename = TIFFFileName(tif);
    (void)filename; // Use result to avoid unused variable warning
    
    // 2-5. TIFFFieldWithTag (called 4 times)
    // Use various tags from fuzzer data or common TIFF tags
    uint32_t tags[4];
    if (Size >= sizeof(tags)) {
        memcpy(tags, Data, sizeof(tags));
    } else {
        // Default tags if not enough data
        tags[0] = TIFFTAG_IMAGEWIDTH;
        tags[1] = TIFFTAG_IMAGELENGTH;
        tags[2] = TIFFTAG_BITSPERSAMPLE;
        tags[3] = TIFFTAG_COMPRESSION;
    }
    
    for (int i = 0; i < 4; i++) {
        const TIFFField *field = TIFFFieldWithTag(tif, tags[i]);
        (void)field; // Use result
    }
    
    // 6. TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // 7. TIFFFieldWithTag (again)
    const TIFFField *field_again = TIFFFieldWithTag(tif, TIFFTAG_SAMPLESPERPIXEL);
    (void)field_again;
    
    // 8. _TIFFmalloc
    // Allocate buffer using tile_size or a reasonable default
    tmsize_t alloc_size = tile_size > 0 ? tile_size : 1024;
    if (alloc_size > 1024 * 1024) { // Limit allocation size
        alloc_size = 1024 * 1024;
    }
    void *buffer = _TIFFmalloc(alloc_size);
    
    if (buffer && alloc_size > 0) {
        // Initialize buffer with fuzzer data if available
        size_t copy_size = Size > alloc_size ? alloc_size : Size;
        if (copy_size > 0) {
            memcpy(buffer, Data, copy_size);
        } else {
            memset(buffer, 0, alloc_size);
        }
    }
    
    // 9. TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // 10. TIFFWriteEncodedTile
    if (buffer && num_tiles > 0) {
        // Use first tile if available
        uint32_t tile_to_write = 0;
        if (Size > sizeof(tags) + 4) {
            // Extract tile index from fuzzer data
            uint32_t tile_index;
            memcpy(&tile_index, Data + sizeof(tags), sizeof(tile_index));
            tile_to_write = tile_index % (num_tiles > 0 ? num_tiles : 1);
        }
        
        tmsize_t write_size = tile_size > 0 ? tile_size : 1024;
        if (write_size > alloc_size) {
            write_size = alloc_size;
        }
        
        TIFFWriteEncodedTile(tif, tile_to_write, buffer, write_size);
    }
    
    // 11-12. TIFFWriteScanline (called twice)
    if (buffer) {
        uint32_t rows_written = 0;
        uint32_t max_rows = 64; // From our IMAGELENGTH setting
        
        // First call
        uint32_t row1 = 0;
        if (Size > sizeof(tags) + 8) {
            memcpy(&row1, Data + sizeof(tags) + 4, sizeof(row1));
            row1 %= max_rows;
        }
        TIFFWriteScanline(tif, buffer, row1, 0);
        rows_written++;
        
        // Second call
        uint32_t row2 = (row1 + 1) % max_rows;
        TIFFWriteScanline(tif, buffer, row2, 0);
        rows_written++;
    }
    
    // 13. _TIFFfree
    if (buffer) {
        _TIFFfree(buffer);
    }
    
    // Cleanup
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}