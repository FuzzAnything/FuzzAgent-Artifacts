// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
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
}

static TIFF* open_dummy_tiff(const uint8_t *data, size_t size, const char* mode) {
    // Write fuzzer input to dummy file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    
    if (written != (ssize_t)size) {
        return NULL;
    }
    
    // Set error handlers to suppress messages
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Open the dummy file as TIFF
    return TIFFOpen("./dummy_file", mode);
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0; // Need minimum data for meaningful fuzzing
    
    // Test with read mode
    TIFF* tif = open_dummy_tiff(Data, Size, "r");
    if (tif) {
        // Get image dimensions if available
        uint32_t width = 0, height = 0;
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        
        // Test TIFFNumberOfTiles
        uint32_t num_tiles = TIFFNumberOfTiles(tif);
        
        // Test TIFFTileSize
        tmsize_t tile_size = TIFFTileSize(tif);
        
        // Test TIFFVStripSize with various row counts
        if (height > 0) {
            uint32_t nrows = height / 4;
            if (nrows == 0) nrows = 1;
            tmsize_t vstrip_size = TIFFVStripSize(tif, nrows);
        }
        
        // Test TIFFReadTile if image is tiled
        if (TIFFIsTiled(tif) && num_tiles > 0 && tile_size > 0) {
            void* buf = malloc(tile_size);
            if (buf) {
                // Try reading first tile coordinates
                uint32_t tile_width = 0, tile_length = 0;
                TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
                TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length);
                
                if (tile_width > 0 && tile_length > 0) {
                    // Read tile at (0,0,0) sample 0
                    TIFFReadTile(tif, buf, 0, 0, 0, 0);
                    
                    // Try another position if image is large enough
                    if (width > tile_width && height > tile_length) {
                        TIFFReadTile(tif, buf, tile_width, tile_length, 0, 0);
                    }
                }
                free(buf);
            }
        }
        
        // Test TIFFReadEncodedStrip
        uint32_t num_strips = TIFFNumberOfStrips(tif);
        if (num_strips > 0) {
            tmsize_t strip_size = TIFFStripSize(tif);
            if (strip_size > 0) {
                void* buf = malloc(strip_size);
                if (buf) {
                    // Try reading first strip
                    TIFFReadEncodedStrip(tif, 0, buf, strip_size);
                    
                    // Try reading with -1 size (full strip)
                    TIFFReadEncodedStrip(tif, 0, buf, -1);
                    
                    // Try reading a middle strip if available
                    if (num_strips > 2) {
                        uint32_t mid_strip = num_strips / 2;
                        TIFFReadEncodedStrip(tif, mid_strip, buf, strip_size);
                    }
                    free(buf);
                }
            }
        }
        
        TIFFClose(tif);
    }
    
    // Test with write mode
    tif = open_dummy_tiff(Data, Size, "w");
    if (tif) {
        // Set up minimal TIFF directory for writing tests
        uint32_t width = 256;
        uint32_t height = 256;
        uint16_t samplesperpixel = 1;
        uint16_t bitspersample = 8;
        uint16_t photometric = PHOTOMETRIC_MINISBLACK;
        uint16_t planarconfig = PLANARCONFIG_CONTIG;
        uint16_t compression = COMPRESSION_NONE;
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
        
        // Test TIFFTileSize in write mode
        tmsize_t tile_size = TIFFTileSize(tif);
        
        // Test TIFFWriteTile if we set up tiled image
        uint32_t tile_width = 64;
        uint32_t tile_length = 64;
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tile_width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tile_length);
        
        if (tile_size > 0) {
            void* buf = malloc(tile_size);
            if (buf) {
                // Initialize with some data
                memset(buf, 0xAA, tile_size);
                
                // Write tile at (0,0,0) sample 0
                TIFFWriteTile(tif, buf, 0, 0, 0, 0);
                
                // Write another tile
                if (width > tile_width && height > tile_length) {
                    TIFFWriteTile(tif, buf, tile_width, tile_length, 0, 0);
                }
                free(buf);
            }
        }
        
        // Test TIFFNumberOfTiles in write mode
        uint32_t num_tiles = TIFFNumberOfTiles(tif);
        
        // Test TIFFVStripSize in write mode
        uint32_t nrows = 32;
        tmsize_t vstrip_size = TIFFVStripSize(tif, nrows);
        
        TIFFClose(tif);
    }
    
    cleanup_dummy_file();
    return 0;
}