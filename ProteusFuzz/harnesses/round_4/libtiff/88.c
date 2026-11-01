// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        } else {
            // Write a minimal valid TIFF header if input is empty to ensure file exists
            uint16_t header[] = {42, 0, 0, 0, 8}; // Little endian, offset to first IFD
            fwrite(header, sizeof(uint16_t), 5, fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have enough data for basic operations
    if (Size < 4) {
        return 0;
    }

    // 1. Prepare environment
    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Allocate buffers for testing
    size_t buf_size = Size > 65536 ? 65536 : Size;
    uint8_t *buf = (uint8_t *)malloc(buf_size);
    uint32_t *long_buf = (uint32_t *)malloc(buf_size);
    uint8_t *triple_buf = (uint8_t *)malloc(buf_size);

    if (!buf || !long_buf || !triple_buf) {
        TIFFClose(tif);
        free(buf);
        free(long_buf);
        free(triple_buf);
        return 0;
    }

    // Initialize buffers with input data
    memset(buf, 0, buf_size);
    memset(long_buf, 0, buf_size);
    memset(triple_buf, 0, buf_size);
    memcpy(buf, Data, Size);
    memcpy(long_buf, Data, Size);
    memcpy(triple_buf, Data, Size);

    // 2. Fuzz Target Functions

    // Test TIFFGetBitRevTable
    // Try both 0 and 1
    const unsigned char *table0 = TIFFGetBitRevTable(0);
    const unsigned char *table1 = TIFFGetBitRevTable(1);
    if (table0) {
        // Just access to ensure it's valid
        volatile unsigned char v0 = table0[0];
        (void)v0;
    }
    if (table1) {
        volatile unsigned char v1 = table1[0];
        (void)v1;
    }

    // Test TIFFSwabLong
    // Fuzz with different values from input
    if (Size >= 4) {
        uint32_t val = *(uint32_t *)Data;
        TIFFSwabLong(&val);
    }

    // Test TIFFSwabArrayOfLong
    // Ensure alignment and size
    size_t num_longs = (Size / sizeof(uint32_t));
    if (num_longs > 0) {
        TIFFSwabArrayOfLong(long_buf, num_longs);
    }

    // Test TIFFSwabArrayOfTriples
    // Ensure size is multiple of 3
    size_t num_triples = (Size / 3);
    if (num_triples > 0) {
        TIFFSwabArrayOfTriples(triple_buf, num_triples);
    }

    // Test TIFFReverseBits
    // Safe to call on any buffer
    if (Size > 0) {
        TIFFReverseBits(buf, Size);
    }

    // Test TIFFReadEncodedStrip
    // Try reading strip 0 with various buffer sizes
    // We cast Size to tmsize_t, ensuring it's positive
    if (Size > 0) {
        tmsize_t result = TIFFReadEncodedStrip(tif, 0, buf, (tmsize_t)Size);
        (void)result; // Ignore result, just checking for crashes
    }

    // 3. Cleanup
    free(buf);
    free(long_buf);
    free(triple_buf);
    TIFFClose(tif);

    return 0;
}