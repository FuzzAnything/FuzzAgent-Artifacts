// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFReadFromUserBuffer at tif_read.c:1545:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file for testing
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open TIFF for reading
    TIFF *tif_read = TIFFOpen(filename, "rb");
    if (!tif_read) {
        return 0;
    }

    // Test TIFFGetMode
    int mode = TIFFGetMode(tif_read);
    (void)mode;

    // Test TIFFStripSize64
    uint64_t strip_size_64 = TIFFStripSize64(tif_read);
    (void)strip_size_64;

    // Test TIFFVStripSize
    uint32_t nrows = 0;
    if (Size >= 4) {
        nrows = *(const uint32_t *)(Data);
        nrows %= 10000; // Limit to reasonable value
    }
    tmsize_t vstrip_size = TIFFVStripSize(tif_read, nrows);
    (void)vstrip_size;

    // Test TIFFReadBufferSetup
    void *user_buf = NULL;
    tmsize_t buf_size = 0;
    if (Size >= 8) {
        buf_size = *(const tmsize_t *)(Data + 4);
        if (buf_size > 0 && buf_size < (1 << 20)) {
            user_buf = malloc(buf_size);
            if (user_buf) {
                memset(user_buf, 0, buf_size);
            }
        }
    }
    int ret_read_buf = TIFFReadBufferSetup(tif_read, user_buf, buf_size);
    (void)ret_read_buf;

    // Test TIFFReadFromUserBuffer
    if (user_buf && buf_size > 0) {
        uint32_t strile = 0;
        if (Size >= 12) {
            strile = *(const uint32_t *)(Data + 8);
            strile %= 100;
        }
        tmsize_t outsize = 0;
        if (Size >= 16) {
            outsize = *(const tmsize_t *)(Data + 12);
            if (outsize > 0 && outsize < (1 << 20)) {
                void *outbuf = malloc(outsize);
                if (outbuf) {
                    int ret_user_buf = TIFFReadFromUserBuffer(tif_read, strile, user_buf, buf_size, outbuf, outsize);
                    (void)ret_user_buf;
                    free(outbuf);
                }
            }
        }
    }

    TIFFClose(tif_read);

    // Open TIFF for writing
    TIFF *tif_write = TIFFOpen(filename, "wb");
    if (!tif_write) {
        return 0;
    }

    // Set basic tags to allow writing
    uint16_t bitspersample = 8;
    TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, bitspersample);
    uint32_t samplesperpixel = 1;
    TIFFSetField(tif_write, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, photometric);
    uint32_t rowsperstrip = 0; // Use default
    TIFFSetField(tif_write, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
    uint32_t imagewidth = 100;
    uint32_t imageheight = 100;
    TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, imagewidth);
    TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, imageheight);
    uint16_t compression = COMPRESSION_NONE;
    TIFFSetField(tif_write, TIFFTAG_COMPRESSION, compression);

    // Test TIFFWriteBufferSetup
    void *write_buf = NULL;
    tmsize_t write_buf_size = -1; // Let libtiff calculate
    if (Size >= 20) {
        write_buf_size = *(const tmsize_t *)(Data + 16);
        if (write_buf_size > 0 && write_buf_size < (1 << 20)) {
            write_buf = malloc(write_buf_size);
            if (write_buf) {
                memset(write_buf, 0, write_buf_size);
            }
        }
    }
    int ret_write_buf = TIFFWriteBufferSetup(tif_write, write_buf, write_buf_size);
    (void)ret_write_buf;

    // Test TIFFGetMode for write
    mode = TIFFGetMode(tif_write);
    (void)mode;

    // Test TIFFStripSize64 for write
    strip_size_64 = TIFFStripSize64(tif_write);
    (void)strip_size_64;

    // Test TIFFVStripSize for write
    vstrip_size = TIFFVStripSize(tif_write, nrows);
    (void)vstrip_size;

    // Cleanup
    if (user_buf) {
        free(user_buf);
    }
    if (write_buf) {
        free(write_buf);
    }

    TIFFClose(tif_write);

    // Remove temporary file
    unlink(filename);

    return 0;
}