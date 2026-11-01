// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// TIFFCurrentRow at tif_open.c:864:10 in tiffio.h
// TIFFCurrentStrip at tif_open.c:874:10 in tiffio.h
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
            // Write minimal valid TIFF header if input is empty to avoid immediate crash
            uint16_t header[] = {42, 0, 0, 0, 8, 0, 0, 0}; // Little-endian, offset 8
            fwrite(header, 1, sizeof(header), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Open the dummy file for reading/writing
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // If open fails, try opening for writing (creates new file)
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            return 0;
        }
        // Set minimal required tags for a valid TIFF to allow operations
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
    }

    // Prepare a small buffer for read/write operations
    uint8_t *buf = (uint8_t *)malloc(256);
    if (!buf) {
        TIFFClose(tif);
        return 0;
    }
    memset(buf, 0, 256);

    // Fuzz parameters derived from input data
    uint32_t row = 0;
    uint16_t sample = 0;
    uint32_t nrows = 1;

    if (Size >= 4) {
        row = *(uint32_t *)(Data);
        nrows = *(uint32_t *)(Data);
    }
    if (Size >= 6) {
        sample = *(uint16_t *)(Data + 4);
    }

    // 1. TIFFComputeStrip
    uint32_t strip_idx = TIFFComputeStrip(tif, row, sample);
    (void)strip_idx; // Use return value to avoid unused warning

    // 2. TIFFWriteScanline
    // Ensure we have written at least one directory if in write mode
    if (TIFFIsTiled(tif) == 0) {
        // Try to write a scanline
        TIFFWriteScanline(tif, buf, row, sample);
    }

    // 3. TIFFVStripSize
    tmsize_t strip_size = TIFFVStripSize(tif, nrows);
    (void)strip_size;

    // 4. TIFFReadScanline
    // Attempt to read a scanline (may fail if file is write-only or empty)
    TIFFReadScanline(tif, buf, row, sample);

    // 5. TIFFCurrentRow
    uint32_t current_row = TIFFCurrentRow(tif);
    (void)current_row;

    // 6. TIFFCurrentStrip
    uint32_t current_strip = TIFFCurrentStrip(tif);
    (void)current_strip;

    // Cleanup
    free(buf);
    TIFFClose(tif);

    return 0;
}