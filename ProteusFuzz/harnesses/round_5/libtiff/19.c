// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have enough data to create a minimal valid TIFF header if needed
    // For this fuzz driver, we will try to open the file in both read and write modes.
    // We write the input data to a dummy file first.
    write_dummy_file(Data, Size);

    TIFF *tif = NULL;
    uint8_t *buf = NULL;
    tmsize_t buf_size = 0;

    // Allocate a buffer for read operations. 
    // Limit size to avoid huge allocations from fuzz input.
    if (Size > 0 && Size < (1024 * 1024)) {
        buf = (uint8_t *)malloc(Size);
        if (!buf) return 0;
        memcpy(buf, Data, Size);
        buf_size = Size;
    }

    // Attempt to open in read mode first to test read functions
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t tile = 0;
        uint32_t strip = 0;
        tmsize_t read_size;

        // Fuzz TIFFReadRawTile
        // Try a few tile indices derived from input or small constants
        if (Size >= 4) {
            uint32_t input_tile = *(uint32_t *)Data;
            // Limit tile index to reasonable range to avoid infinite loops or huge allocations
            if (input_tile < 1000) {
                read_size = TIFFReadRawTile(tif, input_tile, buf, buf_size);
                (void)read_size; // Suppress unused warning
            }
        }
        // Try small constant indices
        read_size = TIFFReadRawTile(tif, 0, buf, buf_size);
        (void)read_size;

        // Fuzz TIFFReadRawStrip
        if (Size >= 4) {
            uint32_t input_strip = *(uint32_t *)(Data + 4);
            if (input_strip < 1000) {
                read_size = TIFFReadRawStrip(tif, input_strip, buf, buf_size);
                (void)read_size;
            }
        }
        read_size = TIFFReadRawStrip(tif, 0, buf, buf_size);
        (void)read_size;

        // Fuzz _TIFFmemcpy
        // Copy data within our buffer
        if (buf_size > 10) {
            _TIFFmemcpy(buf + 5, buf, 5);
        }

        TIFFClose(tif);
        tif = NULL;
    }

    // Attempt to open in write mode to test write functions
    // We overwrite the dummy file.
    tif = TIFFOpen("./dummy_file", "w");
    if (tif) {
        // Set some basic tags to make the file valid for writing
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 10);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 10);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        
        // Ensure we have a buffer for writing
        if (!buf) {
            buf = (uint8_t *)malloc(100);
            if (!buf) {
                TIFFClose(tif);
                return 0;
            }
            buf_size = 100;
            // Fill with some data
            memset(buf, 0x42, buf_size);
        }

        // Fuzz TIFFWriteRawTile
        // Try writing to tile 0 and a tile derived from input
        if (Size >= 4) {
            uint32_t input_tile = *(uint32_t *)Data;
            if (input_tile < 100) {
                TIFFWriteRawTile(tif, input_tile, buf, buf_size);
            }
        }
        TIFFWriteRawTile(tif, 0, buf, buf_size);

        // Fuzz TIFFWriteEncodedTile
        if (Size >= 4) {
            uint32_t input_tile2 = *(uint32_t *)(Data + 4);
            if (input_tile2 < 100) {
                TIFFWriteEncodedTile(tif, input_tile2, buf, buf_size);
            }
        }
        TIFFWriteEncodedTile(tif, 0, buf, buf_size);

        // Fuzz TIFFWriteRawStrip
        if (Size >= 8) {
            uint32_t input_strip = *(uint32_t *)(Data + 8);
            if (input_strip < 100) {
                TIFFWriteRawStrip(tif, input_strip, buf, buf_size);
            }
        }
        TIFFWriteRawStrip(tif, 0, buf, buf_size);

        // Additional _TIFFmemcpy usage in write context
        if (buf_size > 20) {
            _TIFFmemcpy(buf + 10, buf, 10);
        }

        TIFFClose(tif);
        tif = NULL;
    }

    if (buf) {
        free(buf);
    }

    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}