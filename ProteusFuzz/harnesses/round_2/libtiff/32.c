// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFReadBufferSetup at tif_read.c:1375:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
// TIFFWriteBufferSetup at tif_write.c:677:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tiffio.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0 && data != NULL) {
            fwrite(data, 1, size, f);
        } else {
            // Write a minimal valid TIFF header if input is empty or null
            // Basic TIFF header: Magic (II), Version (42), Offset to first IFD (8)
            uint16_t magic = 0x002A; // Little endian 42
            uint32_t offset = 8;
            fwrite("II", 1, 2, f);
            fwrite(&magic, 1, 2, f);
            fwrite(&offset, 1, 4, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Create a dummy file with input data
    write_dummy_file(Data, Size);

    // Open the file for reading to test read-related APIs
    TIFF *tif_read = TIFFOpen("./dummy_file", "r");
    if (tif_read != NULL) {
        // 1. TIFFGetMode: Check access mode
        int mode = TIFFGetMode(tif_read);
        (void)mode; // Suppress unused warning

        // 4. TIFFReadBufferSetup: Configure read buffer
        // Try with NULL buffer (let libtiff allocate) and size 0
        int ret_read_setup_null = TIFFReadBufferSetup(tif_read, NULL, 0);
        (void)ret_read_setup_null;

        // Try with a custom small buffer
        uint8_t *custom_buf = malloc(1024);
        if (custom_buf) {
            int ret_read_setup_custom = TIFFReadBufferSetup(tif_read, custom_buf, 1024);
            (void)ret_read_setup_custom;
            free(custom_buf);
        }

        // 6. TIFFFlushData: Force pending writes (no-op for read-only, but safe to call)
        int ret_flush_data = TIFFFlushData(tif_read);
        (void)ret_flush_data;

        // 2. TIFFFlush: Force buffered data to disk (no-op for read-only)
        int ret_flush = TIFFFlush(tif_read);
        (void)ret_flush;

        // Cleanup read handle
        TIFFClose(tif_read);
    }

    // Open the file for writing to test write-related APIs
    // Note: Overwriting the dummy file.
    TIFF *tif_write = TIFFOpen("./dummy_file", "w");
    if (tif_write != NULL) {
        // Initialize basic directory parameters to allow directory operations
        // Without setting image dimensions, some operations might fail, but we test the API calls.
        TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif_write, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif_write, TIFFTAG_SAMPLESPERPIXEL, 1);

        // 1. TIFFGetMode: Check access mode
        int mode = TIFFGetMode(tif_write);
        (void)mode;

        // 5. TIFFWriteBufferSetup: Configure write buffer
        // Try with NULL buffer (let libtiff allocate) and size -1 (auto)
        int ret_write_setup_auto = TIFFWriteBufferSetup(tif_write, NULL, -1);
        (void)ret_write_setup_auto;

        // Try with a custom buffer
        uint8_t *custom_wbuf = malloc(4096);
        if (custom_wbuf) {
            int ret_write_setup_custom = TIFFWriteBufferSetup(tif_write, custom_wbuf, 4096);
            (void)ret_write_setup_custom;
            free(custom_wbuf);
        }

        // 2. TIFFFlush: Force buffered data to disk
        int ret_flush = TIFFFlush(tif_write);
        (void)ret_flush;

        // 6. TIFFFlushData: Force pending write operations
        int ret_flush_data = TIFFFlushData(tif_write);
        (void)ret_flush_data;

        // 3. TIFFRewriteDirectory: Rewrite the current directory
        // This requires a directory to exist. We set fields above, so a directory is created.
        int ret_rewrite = TIFFRewriteDirectory(tif_write);
        (void)ret_rewrite;

        // Cleanup write handle
        TIFFClose(tif_write);
    }

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}