// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
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
#include <fcntl.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff error/warning messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static TIFF* open_dummy_tiff(const uint8_t* data, size_t size, const char* mode) {
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    if (written != (ssize_t)size) return NULL;
    
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    return TIFFOpen("./dummy_file", mode);
}

static void cleanup_tiff(TIFF* tif) {
    if (tif) TIFFClose(tif);
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_68(const uint8_t* Data, size_t Size) {
    if (Size < 100) return 0; // Need minimum data for meaningful fuzzing
    
    // Test with read mode first
    TIFF* tif = open_dummy_tiff(Data, Size, "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Get number of tiles/strips
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    if (num_tiles > 0) {
        // Allocate buffer for tile operations
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < 10*1024*1024) { // Limit to 10MB
            void* buf = malloc(tile_size);
            if (buf) {
                // Test a few different tile indices
                uint32_t test_tiles[] = {0, num_tiles/2, num_tiles-1};
                for (size_t i = 0; i < sizeof(test_tiles)/sizeof(test_tiles[0]); i++) {
                    uint32_t tile = test_tiles[i];
                    if (tile < num_tiles) {
                        // Test reading raw tile
                        tmsize_t raw_read = TIFFReadRawTile(tif, tile, buf, tile_size);
                        (void)raw_read; // Result intentionally ignored
                        
                        // Test reading encoded tile
                        tmsize_t encoded_read = TIFFReadEncodedTile(tif, tile, buf, tile_size);
                        (void)encoded_read; // Result intentionally ignored
                    }
                }
                free(buf);
            }
        }
    }
    
    cleanup_tiff(tif);
    
    // Test with write mode
    tif = open_dummy_tiff(Data, Size, "r+");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Setup some basic directory for writing
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 256);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 256);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 16);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    
    // Force directory write
    TIFFWriteDirectory(tif);
    
    // Test strip writing if applicable
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    if (num_strips > 0) {
        tmsize_t strip_size = TIFFStripSize(tif);
        if (strip_size > 0 && strip_size < 10*1024*1024) {
            void* write_buf = malloc(strip_size);
            if (write_buf) {
                // Fill with some data
                memset(write_buf, 0xAA, strip_size);
                
                // Test writing to a few strips
                uint32_t test_strips[] = {0, num_strips/2, num_strips-1};
                for (size_t i = 0; i < sizeof(test_strips)/sizeof(test_strips[0]); i++) {
                    uint32_t strip = test_strips[i];
                    if (strip < num_strips) {
                        tmsize_t written = TIFFWriteRawStrip(tif, strip, write_buf, strip_size);
                        (void)written; // Result intentionally ignored
                    }
                }
                free(write_buf);
            }
        }
    }
    
    // Setup tiled configuration for tile writing tests
    TIFFSetField(tif, TIFFTAG_TILEWIDTH, 64);
    TIFFSetField(tif, TIFFTAG_TILELENGTH, 64);
    
    // Force another directory write
    TIFFWriteDirectory(tif);
    
    // Re-read directory to get updated tile count
    TIFFSetDirectory(tif, 1);
    num_tiles = TIFFNumberOfTiles(tif);
    
    if (num_tiles > 0) {
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < 10*1024*1024) {
            void* write_buf = malloc(tile_size);
            if (write_buf) {
                // Fill with some data
                memset(write_buf, 0x55, tile_size);
                
                // Test writing to a few tiles
                uint32_t test_tiles[] = {0, num_tiles/2, num_tiles-1};
                for (size_t i = 0; i < sizeof(test_tiles)/sizeof(test_tiles[0]); i++) {
                    uint32_t tile = test_tiles[i];
                    if (tile < num_tiles) {
                        // Test writing raw tile
                        tmsize_t raw_written = TIFFWriteRawTile(tif, tile, write_buf, tile_size);
                        (void)raw_written; // Result intentionally ignored
                        
                        // Test writing encoded tile
                        tmsize_t encoded_written = TIFFWriteEncodedTile(tif, tile, write_buf, tile_size);
                        (void)encoded_written; // Result intentionally ignored
                    }
                }
                free(write_buf);
            }
        }
    }
    
    cleanup_tiff(tif);
    return 0;
}