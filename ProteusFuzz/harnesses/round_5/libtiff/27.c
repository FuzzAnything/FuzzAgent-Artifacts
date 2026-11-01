// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
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
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Open file for writing to test write functions
    TIFF *tif_w = TIFFOpen("./dummy_file", "w8");
    if (tif_w) {
        // Set basic image properties to make the TIFF valid for strip/tile operations
        TIFFSetField(tif_w, TIFFTAG_IMAGEWIDTH, 16);
        TIFFSetField(tif_w, TIFFTAG_IMAGELENGTH, 16);
        TIFFSetField(tif_w, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif_w, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif_w, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif_w, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif_w, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

        // Allocate a buffer for data
        size_t buf_size = Size < 1024 ? Size : 1024;
        uint8_t *buf = (uint8_t *)malloc(buf_size);
        if (buf) {
            memcpy(buf, Data, buf_size);

            // Test TIFFVStripSize
            uint32_t nrows = (Data[0] % 10) + 1;
            tmsize_t vsize = TIFFVStripSize(tif_w, nrows);
            (void)vsize;

            // Test TIFFWriteRawStrip
            if (buf_size > 0) {
                TIFFWriteRawStrip(tif_w, 0, buf, buf_size);
            }

            // Test TIFFWriteRawTile (requires tiling setup)
            // Reset and reconfigure for tiling to test this specific API
            TIFFClose(tif_w);
            tif_w = TIFFOpen("./dummy_file", "w8");
            if (tif_w) {
                TIFFSetField(tif_w, TIFFTAG_IMAGEWIDTH, 16);
                TIFFSetField(tif_w, TIFFTAG_IMAGELENGTH, 16);
                TIFFSetField(tif_w, TIFFTAG_BITSPERSAMPLE, 8);
                TIFFSetField(tif_w, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
                TIFFSetField(tif_w, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
                TIFFSetField(tif_w, TIFFTAG_SAMPLESPERPIXEL, 1);
                TIFFSetField(tif_w, TIFFTAG_TILEWIDTH, 16);
                TIFFSetField(tif_w, TIFFTAG_TILELENGTH, 16);
                
                if (buf_size > 0) {
                    TIFFWriteRawTile(tif_w, 0, buf, buf_size);
                }
                TIFFClose(tif_w);
            }
            
            free(buf);
        }
    }

    // Open file for reading to test read functions
    // We need a valid TIFF file, so we create a minimal one if the previous write failed or didn't produce a readable file
    // Re-open the dummy file we just wrote to
    TIFF *tif_r = TIFFOpen("./dummy_file", "r");
    if (tif_r) {
        // Ensure directory is read
        if (TIFFReadDirectory(tif_r)) {
            uint32_t strip_count = 0;
            // TIFFTAG_STRIPCOUNT is not a standard tag name in some versions, use TIFFTAG_ROWSPERSTRIP or calculate
            // However, the standard tag for strip count is usually derived or accessed via TIFFNumberOfStrips
            // Let's use TIFFNumberOfStrips which is the standard function to get this count
            uint32_t nstrips = TIFFNumberOfStrips(tif_r);
            strip_count = nstrips;
            
            // Allocate buffer for reading
            size_t read_buf_size = Size < 2048 ? Size : 2048;
            uint8_t *read_buf = (uint8_t *)malloc(read_buf_size);
            
            if (read_buf && strip_count > 0) {
                // Test TIFFReadRawStrip
                uint32_t strip_idx = Data[0] % strip_count;
                tmsize_t read_res = TIFFReadRawStrip(tif_r, strip_idx, read_buf, read_buf_size);
                (void)read_res;

                // Test TIFFReadEncodedStrip
                tmsize_t enc_res = TIFFReadEncodedStrip(tif_r, strip_idx, read_buf, read_buf_size);
                (void)enc_res;
            }

            // Test _TIFFmemcpy
            if (read_buf && Size > 1) {
                uint8_t *dest = (uint8_t *)malloc(Size);
                if (dest) {
                    _TIFFmemcpy(dest, read_buf, Size < read_buf_size ? Size : read_buf_size);
                    free(dest);
                }
            }

            if (read_buf) {
                free(read_buf);
            }
        }
        TIFFClose(tif_r);
    }

    return 0;
}