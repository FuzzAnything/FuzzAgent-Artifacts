// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFGetStrileOffsetWithErr at tif_dirread.c:8356:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
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
            // Write a minimal valid-ish TIFF header if input is empty to avoid immediate open failure
            uint16_t header[8] = {0x002A, 0x0000, 0x0000, 0x0008, 0x0000, 0x0000, 0x0000, 0x0000};
            fwrite(header, sizeof(uint16_t), 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare the environment
    setup_dummy_file(Data, Size);

    // Open for reading to test read functions
    TIFF *tif_read = TIFFOpen("./dummy_file", "r");
    if (tif_read) {
        // Fuzz TIFFGetStrileOffset
        uint32_t strile_idx = 0;
        if (Size >= 4) {
            memcpy(&strile_idx, Data, 4);
        }
        // Use modulo to keep index somewhat reasonable but still fuzzable
        strile_idx %= 1000000;
        (void)TIFFGetStrileOffset(tif_read, strile_idx);

        // Fuzz TIFFGetStrileOffsetWithErr
        int err_code = 0;
        (void)TIFFGetStrileOffsetWithErr(tif_read, strile_idx, &err_code);

        // Fuzz TIFFReadEncodedTile
        // Allocate a buffer for reading
        void *read_buf = malloc(Size > 0 ? Size : 1024);
        if (read_buf) {
            uint32_t tile_idx = 0;
            if (Size >= 8) {
                memcpy(&tile_idx, Data + 4, 4);
                tile_idx %= 1000000;
            }
            // Try reading with various sizes
            tmsize_t read_size = (tmsize_t)(Size > 0 ? Size : 1024);
            (void)TIFFReadEncodedTile(tif_read, tile_idx, read_buf, read_size);
            // Try reading with -1 (auto-detect size)
            (void)TIFFReadEncodedTile(tif_read, tile_idx, read_buf, -1);
            free(read_buf);
        }

        TIFFClose(tif_read);
    }

    // Open for writing to test write functions
    TIFF *tif_write = TIFFOpen("./dummy_file", "w");
    if (tif_write) {
        // Set some basic tags to make the file more valid for writing operations
        TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, 100);
        TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, 100);
        TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif_write, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif_write, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

        // Fuzz TIFFWriteEncodedStrip
        uint32_t strip_idx = 0;
        if (Size >= 4) {
            memcpy(&strip_idx, Data, 4);
            strip_idx %= 1000000;
        }
        tmsize_t cc = (tmsize_t)(Size > 0 ? Size : 1024);
        (void)TIFFWriteEncodedStrip(tif_write, strip_idx, (void*)Data, cc);

        // Fuzz TIFFWriteEncodedTile
        uint32_t tile_idx = 0;
        if (Size >= 4) {
            memcpy(&tile_idx, Data, 4);
            tile_idx %= 1000000;
        }
        (void)TIFFWriteEncodedTile(tif_write, tile_idx, (void*)Data, cc);

        // Fuzz TIFFWriteTile
        // Extract coordinates from data if available
        uint32_t x = 0, y = 0, z = 0;
        uint16_t s = 0;
        if (Size >= 16) {
            memcpy(&x, Data, 4);
            memcpy(&y, Data + 4, 4);
            memcpy(&z, Data + 8, 4);
            memcpy(&s, Data + 12, 2);
            // Clamp to reasonable ranges to avoid immediate overflow issues in logic
            x %= 10000;
            y %= 10000;
            z %= 1000;
            s %= 100;
        }
        (void)TIFFWriteTile(tif_write, (void*)Data, x, y, z, s);

        // Cleanup for write mode
        TIFFClose(tif_write);
    }

    return 0;
}