// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
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

static void cleanup_files(void) {
    unlink("./dummy_file");
    unlink("./dummy_file_write");
}

static int write_data_to_file(const uint8_t *data, size_t size, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    return written == size;
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        atexit(cleanup_files);
        initialized = 1;
    }
    
    // Write fuzzer input to file for reading
    if (!write_data_to_file(Data, Size, "./dummy_file")) {
        return 0;
    }
    
    // Try to open as TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }
    
    // Test TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // Test tile reading functions if we have tiles
    if (num_tiles > 0) {
        // Use first tile for testing
        uint32_t test_tile = 0;
        
        // Allocate buffer for tile data
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0) {
            void *buf = malloc(tile_size);
            if (buf) {
                // Test TIFFReadRawTile
                tmsize_t raw_read = TIFFReadRawTile(tif, test_tile, buf, tile_size);
                (void)raw_read; // Use result to avoid unused variable warning
                
                // Test TIFFReadEncodedTile
                tmsize_t encoded_read = TIFFReadEncodedTile(tif, test_tile, buf, tile_size);
                (void)encoded_read; // Use result to avoid unused variable warning
                
                free(buf);
            }
        }
    }
    
    TIFFClose(tif);
    
    // Test writing functions - create a new file for writing
    // First, create a minimal valid TIFF file for writing tests
    TIFF *tif_write = TIFFOpen("./dummy_file_write", "w");
    if (tif_write) {
        // Set basic required tags for a valid TIFF
        uint32_t width = 64;
        uint32_t height = 64;
        uint16_t samples_per_pixel = 1;
        uint16_t bits_per_sample = 8;
        uint16_t photometric = PHOTOMETRIC_MINISBLACK;
        uint16_t planar_config = PLANARCONFIG_CONTIG;
        
        TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif_write, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
        TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
        TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif_write, TIFFTAG_PLANARCONFIG, planar_config);
        
        // Test with tiled configuration
        uint32_t tile_width = 32;
        uint32_t tile_height = 32;
        TIFFSetField(tif_write, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif_write, TIFFTAG_TILELENGTH, tile_height);
        
        // Allocate buffer for tile data
        tmsize_t write_tile_size = TIFFTileSize(tif_write);
        if (write_tile_size > 0) {
            void *write_buf = malloc(write_tile_size);
            if (write_buf) {
                // Fill buffer with some data (use fuzzer input if available)
                size_t copy_size = Size < (size_t)write_tile_size ? Size : (size_t)write_tile_size;
                if (copy_size > 0) {
                    memcpy(write_buf, Data, copy_size);
                }
                if (copy_size < (size_t)write_tile_size) {
                    memset((uint8_t*)write_buf + copy_size, 0, write_tile_size - copy_size);
                }
                
                // Test TIFFWriteRawTile
                tmsize_t raw_written = TIFFWriteRawTile(tif_write, 0, write_buf, write_tile_size);
                (void)raw_written; // Use result to avoid unused variable warning
                
                // Test TIFFWriteEncodedTile
                tmsize_t encoded_written = TIFFWriteEncodedTile(tif_write, 0, write_buf, write_tile_size);
                (void)encoded_written; // Use result to avoid unused variable warning
                
                free(write_buf);
            }
        }
        
        // Also test strip-based writing
        TIFFSetField(tif_write, TIFFTAG_ROWSPERSTRIP, 16);
        
        // Allocate buffer for strip data
        tmsize_t strip_size = TIFFStripSize(tif_write);
        if (strip_size > 0) {
            void *strip_buf = malloc(strip_size);
            if (strip_buf) {
                // Fill buffer with some data
                size_t copy_size = Size < (size_t)strip_size ? Size : (size_t)strip_size;
                if (copy_size > 0) {
                    memcpy(strip_buf, Data, copy_size);
                }
                if (copy_size < (size_t)strip_size) {
                    memset((uint8_t*)strip_buf + copy_size, 0, strip_size - copy_size);
                }
                
                // Test TIFFWriteRawStrip
                tmsize_t strip_written = TIFFWriteRawStrip(tif_write, 0, strip_buf, strip_size);
                (void)strip_written; // Use result to avoid unused variable warning
                
                free(strip_buf);
            }
        }
        
        TIFFClose(tif_write);
    }
    
    return 0;
}