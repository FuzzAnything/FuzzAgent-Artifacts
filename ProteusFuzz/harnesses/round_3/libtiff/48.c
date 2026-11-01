// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
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

static void setup_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        } else {
            // Write a minimal valid TIFF header if input is empty
            // BigTIFF header: 0x0000000000000000 (magic), 0x00000000 (offset)
            // Actually, let's just write a minimal Classic TIFF header
            // Magic: 0x4949 (II), ByteOrder
            // Version: 0x002A
            // First IFD offset: 0x00000008
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file with the fuzzed input
    setup_dummy_file(Data, Size);

    // Open the file for reading
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // We need to set some basic tags to make the file valid for operations
    // However, since we are fuzzing raw data, we might not have a valid directory.
    // We will attempt operations that handle errors gracefully.

    uint32_t strip = 0;
    if (Size >= 4) {
        strip = *(uint32_t *)Data;
        // Modulo to keep it somewhat reasonable, though fuzzing should test bounds
        strip = strip % 1000;
    }

    // Allocate a buffer for reading/writing
    // Size limit to prevent OOM
    size_t buf_size = (Size > 0) ? Size : 1;
    if (buf_size > 1024 * 1024) {
        buf_size = 1024 * 1024;
    }
    
    uint8_t *buf = (uint8_t *)malloc(buf_size);
    if (!buf) {
        TIFFClose(tif);
        return 0;
    }

    // 1. TIFFReadRawStrip
    // Try with size -1 (full strip) and with a specific size
    tmsize_t res = TIFFReadRawStrip(tif, strip, buf, -1);
    (void)res; // Ignore return for fuzzing stability, just trigger code paths
    
    res = TIFFReadRawStrip(tif, strip, buf, buf_size);
    (void)res;

    // 2. TIFFWriteRawStrip
    // This might fail if file is read-only or not writable, which is fine for fuzzing
    // We need to ensure we have data to write
    if (buf_size > 0) {
        memset(buf, 0xAA, buf_size);
        res = TIFFWriteRawStrip(tif, strip, buf, buf_size);
        (void)res;
    }

    // 3. TIFFStripSize
    // This requires a valid directory to be loaded. If not, it returns 0.
    res = TIFFStripSize(tif);
    (void)res;

    // 4. TIFFVStripSize
    // Calculate vertical strip size for a number of rows
    uint32_t nrows = 0;
    if (Size >= 8) {
        nrows = *(uint32_t *)(Data + 4);
        nrows = nrows % 10000;
    }
    res = TIFFVStripSize(tif, nrows);
    (void)res;

    // 5. TIFFReadEncodedStrip
    // Similar to ReadRawStrip but handles decompression
    res = TIFFReadEncodedStrip(tif, strip, buf, -1);
    (void)res;
    res = TIFFReadEncodedStrip(tif, strip, buf, buf_size);
    (void)res;

    // 6. TIFFRawStripSize
    // Get the raw size of a specific strip
    res = TIFFRawStripSize(tif, strip);
    (void)res;

    // Cleanup
    free(buf);
    TIFFClose(tif);
    
    return 0;
}