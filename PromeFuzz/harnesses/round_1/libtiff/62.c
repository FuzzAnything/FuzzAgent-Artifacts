// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
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
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <unistd.h>

static void cleanup_files(void) {
    unlink("./dummy_file");
    unlink("./dummy_file2");
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        atexit(cleanup_files);
        initialized = 1;
    }

    // Create dummy file for TIFF operations
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);

    // Open TIFF file for writing
    TIFF *tif_write = TIFFOpen("./dummy_file", "w");
    if (!tif_write) {
        return 0;
    }

    // Set basic TIFF parameters needed for strip/tile operations
    uint32_t width = 256;
    uint32_t height = 256;
    uint16_t samples_per_pixel = 1;
    uint16_t bits_per_sample = 8;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;

    TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif_write, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif_write, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif_write, TIFFTAG_COMPRESSION, compression);

    // Calculate strip size
    uint32_t rows_per_strip = 16;
    TIFFSetField(tif_write, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    tmsize_t strip_size = TIFFStripSize(tif_write);

    // Allocate buffer for strip/tile data
    void *buffer = NULL;
    if (strip_size > 0) {
        buffer = malloc(strip_size);
        if (buffer && Size > 0) {
            size_t copy_size = Size < (size_t)strip_size ? Size : (size_t)strip_size;
            memcpy(buffer, Data, copy_size);
        }
    }

    // Test TIFFWriteBufferSetup with various configurations
    if (buffer) {
        // Test with provided buffer
        TIFFWriteBufferSetup(tif_write, buffer, strip_size);
        
        // Test with NULL buffer (libtiff should allocate)
        TIFFWriteBufferSetup(tif_write, NULL, -1);
        
        // Test with zero size
        TIFFWriteBufferSetup(tif_write, buffer, 0);
    }

    // Test strip writing functions
    if (buffer && strip_size > 0) {
        uint32_t strip = 0;
        
        // Test TIFFWriteRawStrip
        TIFFWriteRawStrip(tif_write, strip, buffer, strip_size);
        
        // Test TIFFWriteEncodedStrip
        TIFFWriteEncodedStrip(tif_write, strip, buffer, strip_size);
        
        // Test with different strip indices
        if (TIFFNumberOfStrips(tif_write) > 1) {
            strip = 1;
            TIFFWriteRawStrip(tif_write, strip, buffer, strip_size);
            TIFFWriteEncodedStrip(tif_write, strip, buffer, strip_size);
        }
    }

    // Test tile operations if image is tiled
    uint32_t tile_width = 64;
    uint32_t tile_height = 64;
    TIFFSetField(tif_write, TIFFTAG_TILEWIDTH, tile_width);
    TIFFSetField(tif_write, TIFFTAG_TILELENGTH, tile_height);
    
    tmsize_t tile_size = TIFFTileSize(tif_write);
    void *tile_buffer = NULL;
    
    if (tile_size > 0) {
        tile_buffer = malloc(tile_size);
        if (tile_buffer && Size > 0) {
            size_t copy_size = Size < (size_t)tile_size ? Size : (size_t)tile_size;
            memcpy(tile_buffer, Data, copy_size);
        }
    }

    // Test TIFFWriteEncodedTile
    if (tile_buffer && tile_size > 0) {
        uint32_t tile = 0;
        TIFFWriteEncodedTile(tif_write, tile, tile_buffer, tile_size);
        
        // Test with different tile indices
        uint32_t tiles_across = (width + tile_width - 1) / tile_width;
        uint32_t tiles_down = (height + tile_height - 1) / tile_height;
        uint32_t total_tiles = tiles_across * tiles_down;
        
        if (total_tiles > 1) {
            tile = total_tiles - 1; // Last tile
            TIFFWriteEncodedTile(tif_write, tile, tile_buffer, tile_size);
        }
    }

    // Clean up write TIFF
    TIFFClose(tif_write);

    // Now test reading functions
    // First create a valid TIFF file for reading
    FILE *fp2 = fopen("./dummy_file2", "wb");
    if (fp2) {
        // Write minimal valid TIFF header
        uint8_t tiff_header[] = {
            0x49, 0x49, 0x2A, 0x00, // Little-endian, classic TIFF
            0x08, 0x00, 0x00, 0x00, // First IFD offset
            // IFD with minimal tags
            0x01, 0x00, // 1 entry
            // Tag 256: ImageWidth
            0x00, 0x01, 0x03, 0x00, // Tag 256, type SHORT
            0x01, 0x00, 0x00, 0x00, // Count 1
            0x01, 0x00, 0x00, 0x00, // Value: 1
            0x00, 0x00, 0x00, 0x00  // Next IFD offset
        };
        fwrite(tiff_header, 1, sizeof(tiff_header), fp2);
        fclose(fp2);
    }

    // Open for reading
    TIFF *tif_read = TIFFOpen("./dummy_file2", "r");
    if (tif_read) {
        // Test TIFFReadBufferSetup
        void *read_buffer = NULL;
        if (Size > 0) {
            read_buffer = malloc(Size);
            if (read_buffer) {
                memcpy(read_buffer, Data, Size);
                TIFFReadBufferSetup(tif_read, read_buffer, (tmsize_t)Size);
                free(read_buffer);
            }
        }
        
        // Test with NULL buffer
        TIFFReadBufferSetup(tif_read, NULL, 1024);
        
        // Test TIFFReadEncodedTile if image is tiled
        if (TIFFIsTiled(tif_read)) {
            uint32_t tile = 0;
            tmsize_t buf_size = 1024;
            void *tile_buf = malloc(buf_size);
            if (tile_buf) {
                TIFFReadEncodedTile(tif_read, tile, tile_buf, buf_size);
                free(tile_buf);
            }
        }
        
        TIFFClose(tif_read);
    }

    // Free allocated buffers
    if (buffer) free(buffer);
    if (tile_buffer) free(tile_buffer);

    return 0;
}