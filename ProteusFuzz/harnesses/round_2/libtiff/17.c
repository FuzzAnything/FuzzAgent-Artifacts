// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_tiff(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    } else {
        // Write a minimal valid TIFF header if input is empty to ensure open succeeds
        uint16_t header[] = {42, 0, 0, 0, 8}; // Little endian, offset to first IFD
        fwrite(header, sizeof(uint16_t), 5, fp);
    }
    fclose(fp);
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;

    // Prepare environment: create a dummy file from fuzz input
    write_dummy_tiff(Data, Size);

    // Attempt to open the file in read mode
    tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        // If opening fails, try write mode to create a valid structure if possible
        tif = TIFFOpen("./dummy_file", "w");
        if (tif != NULL) {
            // Set some basic tags to ensure directory is valid for size calculations
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            // Try to write a minimal strip to make it a valid image
            uint8_t *buf = malloc(100);
            if (buf) {
                memset(buf, 0, 100);
                TIFFWriteScanline(tif, buf, 0, 0);
                free(buf);
            }
            TIFFClose(tif);
            tif = NULL;
        }
    }

    if (tif != NULL) {
        // Fuzz Target 1: TIFFTileSize64
        // Only valid if image is tiled
        if (TIFFIsTiled(tif)) {
            (void)TIFFTileSize64(tif);
        }

        // Fuzz Target 2: TIFFScanlineSize64
        (void)TIFFScanlineSize64(tif);

        // Fuzz Target 3: TIFFVStripSize64
        // Use fuzz data to determine nrows, ensuring it's within reasonable bounds or -1
        uint32_t nrows = 0;
        if (Size >= 4) {
            nrows = *(uint32_t *)Data;
        } else if (Size > 0) {
            nrows = Data[0];
        }
        // Pass a valid row count or 0 (which is often handled as full height or error)
        (void)TIFFVStripSize64(tif, nrows);

        // Fuzz Target 4: TIFFTileRowSize64
        if (TIFFIsTiled(tif)) {
            (void)TIFFTileRowSize64(tif);
        }

        // Fuzz Target 5: TIFFGetStrileByteCount
        // Use fuzz data to determine strile index
        uint32_t strile = 0;
        if (Size >= 4) {
            strile = *(uint32_t *)(Data + 4);
        } else if (Size > 4) {
            strile = Data[4];
        }
        (void)TIFFGetStrileByteCount(tif, strile);

        // Fuzz Target 6: TIFFStripSize64
        (void)TIFFStripSize64(tif);

        // Cleanup
        TIFFClose(tif);
    }

    return 0;
}