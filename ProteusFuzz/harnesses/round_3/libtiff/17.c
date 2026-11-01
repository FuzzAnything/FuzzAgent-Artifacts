// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
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

static void cleanup_file(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for file-based operations
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Test TIFFSwabArrayOfShort
    size_t n_short = Size / sizeof(uint16_t);
    if (n_short > 0) {
        uint16_t *short_buf = (uint16_t *)malloc(n_short * sizeof(uint16_t));
        if (short_buf != NULL) {
            memcpy(short_buf, Data, n_short * sizeof(uint16_t));
            TIFFSwabArrayOfShort(short_buf, n_short);
            free(short_buf);
        }
    }

    // Test TIFFSwabArrayOfLong
    size_t n_long = Size / sizeof(uint32_t);
    if (n_long > 0) {
        uint32_t *long_buf = (uint32_t *)malloc(n_long * sizeof(uint32_t));
        if (long_buf != NULL) {
            memcpy(long_buf, Data, n_long * sizeof(uint32_t));
            TIFFSwabArrayOfLong(long_buf, n_long);
            free(long_buf);
        }
    }

    // Test TIFFSwabArrayOfLong8
    size_t n_long8 = Size / sizeof(uint64_t);
    if (n_long8 > 0) {
        uint64_t *long8_buf = (uint64_t *)malloc(n_long8 * sizeof(uint64_t));
        if (long8_buf != NULL) {
            memcpy(long8_buf, Data, n_long8 * sizeof(uint64_t));
            TIFFSwabArrayOfLong8(long8_buf, n_long8);
            free(long8_buf);
        }
    }

    // Test _TIFFmemcpy
    if (Size > 10) {
        void *dst = malloc(Size);
        if (dst != NULL) {
            _TIFFmemcpy(dst, Data, Size);
            free(dst);
        }
    }

    // Test file-based functions
    TIFF *tif = TIFFOpen(filename, "rb");
    if (tif != NULL) {
        uint32_t x, y, z, s;
        uint32_t strip;
        tmsize_t size;

        // Extract some values from input for coordinates
        if (Size >= 16) {
            x = *(uint32_t *)(Data + 0);
            y = *(uint32_t *)(Data + 4);
            z = *(uint32_t *)(Data + 8);
            s = *(uint16_t *)(Data + 12);
            strip = *(uint32_t *)(Data + 14);
            size = *(tmsize_t *)(Data + 16);
        } else {
            x = y = z = s = strip = 0;
            size = Size;
        }

        // Limit coordinates to reasonable values to avoid huge allocations
        if (x > 10000) x = 10000;
        if (y > 10000) y = 10000;
        if (z > 100) z = 100;
        if (s > 10) s = 10;
        if (size > 1000000) size = 1000000;

        void *buf = malloc(size > 0 ? size : 1);
        if (buf != NULL) {
            // Test TIFFReadTile
            TIFFReadTile(tif, buf, x, y, z, s);

            // Test TIFFReadRawStrip
            TIFFReadRawStrip(tif, strip, buf, size);
            free(buf);
        }

        cleanup_file(tif);
    }

    // Clean up dummy file
    remove(filename);

    return 0;
}