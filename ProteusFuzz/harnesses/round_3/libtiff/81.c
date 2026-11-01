// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFComputeTile at tif_tile.c:35:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static tmsize_t dummy_read(thandle_t clientdata, tdata_t buf, tmsize_t size) {
    (void)clientdata;
    (void)buf;
    (void)size;
    return 0;
}

static tmsize_t dummy_write(thandle_t clientdata, const tdata_t buf, tmsize_t size) {
    (void)clientdata;
    (void)buf;
    (void)size;
    return 0;
}

static toff_t dummy_seek(thandle_t clientdata, toff_t off, int whence) {
    (void)clientdata;
    (void)off;
    (void)whence;
    return 0;
}

static int dummy_close(thandle_t clientdata) {
    (void)clientdata;
    return 0;
}

static toff_t dummy_size(thandle_t clientdata) {
    (void)clientdata;
    return 0;
}

static int dummy_map(thandle_t clientdata, tdata_t *pbase, toff_t *psize) {
    (void)clientdata;
    (void)pbase;
    (void)psize;
    return 0;
}

static void dummy_unmap(thandle_t clientdata, tdata_t base, toff_t size) {
    (void)clientdata;
    (void)base;
    (void)size;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write input data to a dummy file to simulate a TIFF file
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF *tif = NULL;
    uint32_t x, y, z;
    uint16_t s;
    uint32_t row;

    // Open the file in read mode
    tif = TIFFOpen(filename, "r");
    
    if (tif != NULL) {
        // Attempt to read directory to initialize internal state
        // This might fail if the file is not a valid TIFF, which is expected in fuzzing
        if (TIFFReadDirectory(tif)) {
            // Fuzz TIFFCurrentTile
            uint32_t current_tile = TIFFCurrentTile(tif);
            (void)current_tile;

            // Fuzz TIFFNumberOfTiles
            uint32_t num_tiles = TIFFNumberOfTiles(tif);
            (void)num_tiles;

            // Fuzz TIFFComputeStrip
            // Use data from input for row and sample
            if (Size >= 5) {
                row = Data[0] | ((uint32_t)Data[1] << 8) | ((uint32_t)Data[2] << 16) | ((uint32_t)Data[3] << 24);
                s = Data[4] | ((uint16_t)(Size > 5 ? Data[5] : 0) << 8);
                uint32_t strip_idx = TIFFComputeStrip(tif, row, s);
                (void)strip_idx;
            }

            // Fuzz TIFFCurrentRow
            uint32_t current_row = TIFFCurrentRow(tif);
            (void)current_row;

            // Fuzz TIFFComputeTile
            if (Size >= 9) {
                x = Data[0] | ((uint32_t)Data[1] << 8) | ((uint32_t)Data[2] << 16) | ((uint32_t)Data[3] << 24);
                y = Data[4] | ((uint32_t)Data[5] << 8) | ((uint32_t)Data[6] << 16) | ((uint32_t)Data[7] << 24);
                z = Data[8] | ((uint32_t)(Size > 9 ? Data[9] : 0) << 8) | ((uint32_t)(Size > 10 ? Data[10] : 0) << 16) | ((uint32_t)(Size > 11 ? Data[11] : 0) << 24);
                s = Size > 12 ? (Data[12] | ((uint16_t)Data[13] << 8)) : 0;
                
                uint32_t tile_idx = TIFFComputeTile(tif, x, y, z, s);
                (void)tile_idx;
            }

            // Fuzz TIFFCheckTile
            if (Size >= 9) {
                x = Data[0] | ((uint32_t)Data[1] << 8) | ((uint32_t)Data[2] << 16) | ((uint32_t)Data[3] << 24);
                y = Data[4] | ((uint32_t)Data[5] << 8) | ((uint32_t)Data[6] << 16) | ((uint32_t)Data[7] << 24);
                z = Data[8] | ((uint32_t)(Size > 9 ? Data[9] : 0) << 8) | ((uint32_t)(Size > 10 ? Data[10] : 0) << 16) | ((uint32_t)(Size > 11 ? Data[11] : 0) << 24);
                s = Size > 12 ? (Data[12] | ((uint16_t)Data[13] << 8)) : 0;

                int check_result = TIFFCheckTile(tif, x, y, z, s);
                (void)check_result;
            }
        }
        
        // Try to read a tile or strip to update internal cursors, even if it fails
        // This helps explore states where tif_curtile or tif_row might change
        uint8_t *buf = (uint8_t *)malloc(1024);
        if (buf) {
            TIFFReadTile(tif, buf, 0, 0, 0, 0);
            // TIFFReadScanline requires 4 arguments: tif, buf, row, sample
            if (Size >= 5) {
                row = Data[0] | ((uint32_t)Data[1] << 8) | ((uint32_t)Data[2] << 16) | ((uint32_t)Data[3] << 24);
                s = Data[4] | ((uint16_t)(Size > 5 ? Data[5] : 0) << 8);
                TIFFReadScanline(tif, buf, row, s);
            } else {
                TIFFReadScanline(tif, buf, 0, 0);
            }
            free(buf);
        }

        // Re-call functions after potential state changes
        if (TIFFReadDirectory(tif)) {
            (void)TIFFCurrentTile(tif);
            (void)TIFFNumberOfTiles(tif);
            (void)TIFFCurrentRow(tif);
        }

        TIFFClose(tif);
    }

    // Also try opening in write mode with custom IO to test different code paths
    // This creates a minimal valid header structure in memory
    tif = TIFFClientOpen("fuzz_write", "w", NULL, dummy_read, dummy_write, dummy_seek, dummy_close, dummy_size, dummy_map, dummy_unmap);
    if (tif != NULL) {
        // Set some basic tags to make directory valid
        uint16_t bits = 8;
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits);
        uint32_t width = Size > 0 ? Data[0] : 1;
        uint32_t height = Size > 1 ? Data[1] : 1;
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        uint16_t samples = 1;
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples);
        uint16_t photometric = PHOTOMETRIC_MINISBLACK;
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        uint16_t compression = COMPRESSION_NONE;
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);

        // Write directory
        TIFFWriteDirectory(tif);

        // Test functions in write mode
        (void)TIFFCurrentTile(tif);
        (void)TIFFNumberOfTiles(tif);
        (void)TIFFCurrentRow(tif);
        
        if (Size >= 5) {
            row = Data[0] | ((uint32_t)Data[1] << 8) | ((uint32_t)Data[2] << 16) | ((uint32_t)Data[3] << 24);
            s = Data[4] | ((uint16_t)(Size > 5 ? Data[5] : 0) << 8);
            (void)TIFFComputeStrip(tif, row, s);
        }

        if (Size >= 9) {
            x = Data[0] | ((uint32_t)Data[1] << 8) | ((uint32_t)Data[2] << 16) | ((uint32_t)Data[3] << 24);
            y = Data[4] | ((uint32_t)Data[5] << 8) | ((uint32_t)Data[6] << 16) | ((uint32_t)Data[7] << 24);
            z = Data[8] | ((uint32_t)(Size > 9 ? Data[9] : 0) << 8) | ((uint32_t)(Size > 10 ? Data[10] : 0) << 16) | ((uint32_t)(Size > 11 ? Data[11] : 0) << 24);
            s = Size > 12 ? (Data[12] | ((uint16_t)Data[13] << 8)) : 0;
            
            (void)TIFFComputeTile(tif, x, y, z, s);
            (void)TIFFCheckTile(tif, x, y, z, s);
        }

        TIFFClose(tif);
    }

    return 0;
}