// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid-ish TIFF header if input is empty
            // Byte order (II), Magic (42), Offset (8)
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Open in read mode
    TIFF *tif_r = TIFFOpen("./dummy_file", "r");
    if (tif_r) {
        // Fuzz TIFFVStripSize
        uint32_t nrows = 0;
        if (Size >= 4) {
            nrows = *(const uint32_t *)(Data);
        }
        (void)TIFFVStripSize(tif_r, nrows);

        // Fuzz TIFFStripSize
        (void)TIFFStripSize(tif_r);

        // Fuzz TIFFRawStripSize
        uint32_t strip = 0;
        if (Size >= 8) {
            strip = *(const uint32_t *)(Data + 4);
        }
        (void)TIFFRawStripSize(tif_r, strip);

        // Fuzz TIFFReadEncodedStrip
        tmsize_t buf_size = 0;
        if (Size >= 12) {
            buf_size = *(const tmsize_t *)(Data + 8);
        }
        // Ensure buffer size is reasonable to avoid huge allocations
        if (buf_size < 0 || buf_size > 1024 * 1024) {
            buf_size = 4096;
        }
        void *buf = malloc(buf_size);
        if (buf) {
            (void)TIFFReadEncodedStrip(tif_r, strip, buf, buf_size);
            free(buf);
        }

        // Fuzz TIFFReadRawStrip
        buf_size = 0;
        if (Size >= 16) {
            buf_size = *(const tmsize_t *)(Data + 12);
        }
        if (buf_size < 0 || buf_size > 1024 * 1024) {
            buf_size = 4096;
        }
        buf = malloc(buf_size);
        if (buf) {
            (void)TIFFReadRawStrip(tif_r, strip, buf, buf_size);
            free(buf);
        }

        TIFFClose(tif_r);
    }

    // Open in write mode for write functions
    TIFF *tif_w = TIFFOpen("./dummy_file", "w");
    if (tif_w) {
        // Set minimal required tags to make the file valid enough for strip operations
        uint16_t bits = 8;
        uint32_t width = 1, height = 1;
        uint16_t samples = 1;
        uint16_t photometric = PHOTOMETRIC_MINISBLACK;
        uint16_t compression = COMPRESSION_NONE;

        if (Size >= 20) {
            width = *(const uint32_t *)(Data + 16);
            height = *(const uint32_t *)(Data + 20);
            if (width == 0) width = 1;
            if (height == 0) height = 1;
        }

        TIFFSetField(tif_w, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif_w, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif_w, TIFFTAG_BITSPERSAMPLE, bits);
        TIFFSetField(tif_w, TIFFTAG_COMPRESSION, compression);
        TIFFSetField(tif_w, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif_w, TIFFTAG_SAMPLESPERPIXEL, samples);

        // Fuzz TIFFWriteRawStrip
        uint32_t strip_w = 0;
        if (Size >= 24) {
            strip_w = *(const uint32_t *)(Data + 24);
        }
        
        tmsize_t write_size = 0;
        if (Size >= 28) {
            write_size = *(const tmsize_t *)(Data + 28);
        }
        if (write_size < 0 || write_size > 1024 * 1024) {
            write_size = 4096;
        }
        
        void *write_buf = malloc(write_size);
        if (write_buf) {
            memset(write_buf, 0, write_size);
            (void)TIFFWriteRawStrip(tif_w, strip_w, write_buf, write_size);
            free(write_buf);
        }

        // Fuzz TIFFVStripSize on write handle
        (void)TIFFVStripSize(tif_w, height);

        // Fuzz TIFFStripSize on write handle
        (void)TIFFStripSize(tif_w);

        // Fuzz TIFFRawStripSize on write handle
        (void)TIFFRawStripSize(tif_w, strip_w);

        TIFFClose(tif_w);
    }

    return 0;
}