// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void safe_cleanup(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for TIFFReadRawStrip testing
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        // Write minimal valid TIFF header (Little Endian, 42 magic, 8 offset to first IFD)
        // Magic: 0x002A (42)
        // Offset: 8 (0x00000008)
        uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
        fwrite(header, 1, sizeof(header), fp);
        // Write a minimal IFD (0 entries, 0 next dir)
        uint8_t ifd[6] = {0x00, 0x00, 0x00, 0x00, 0x00};
        fwrite(ifd, 1, sizeof(ifd), fp);
        fclose(fp);
    }

    TIFF *tif = NULL;
    uint8_t *buf = NULL;
    uint32_t *long_buf = NULL;
    uint64_t *long8_buf = NULL;
    uint8_t *bit_buf = NULL;
    uint8_t *copy_src = NULL;
    uint8_t *copy_dst = NULL;

    // Allocate buffers based on input size, ensuring minimum sizes for safety
    // We allocate exactly Size bytes to avoid overflow when copying input data
    size_t alloc_size = Size;
    if (alloc_size < 8) alloc_size = 8;

    buf = (uint8_t *)malloc(alloc_size);
    long_buf = (uint32_t *)malloc(alloc_size);
    long8_buf = (uint64_t *)malloc(alloc_size);
    bit_buf = (uint8_t *)malloc(alloc_size);
    copy_src = (uint8_t *)malloc(alloc_size);
    copy_dst = (uint8_t *)malloc(alloc_size);

    if (!buf || !long_buf || !long8_buf || !bit_buf || !copy_src || !copy_dst) {
        free(buf); free(long_buf); free(long8_buf); free(bit_buf); free(copy_src); free(copy_dst);
        return 0;
    }

    // Initialize buffers with input data
    // Only copy up to the actual input size (Size), not alloc_size if alloc_size > Size
    size_t copy_len = (Size < alloc_size) ? Size : alloc_size;
    memcpy(buf, Data, copy_len);
    memcpy(copy_src, Data, copy_len);
    memset(long_buf, 0, alloc_size);
    memset(long8_buf, 0, alloc_size);
    memset(bit_buf, 0, alloc_size);

    // 1. Fuzz TIFFSwabArrayOfLong8
    // Ensure count is safe (alloc_size / sizeof(uint64_t))
    tmsize_t count8 = alloc_size / sizeof(uint64_t);
    if (count8 > 0) {
        TIFFSwabArrayOfLong8(long8_buf, count8);
    }

    // 2. Fuzz _TIFFmemcpy
    if (alloc_size > 0) {
        _TIFFmemcpy(copy_dst, copy_src, alloc_size);
    }

    // 3. Fuzz TIFFSwabLong
    // Ensure we have at least one uint32_t
    tmsize_t count4 = alloc_size / sizeof(uint32_t);
    if (count4 > 0) {
        TIFFSwabLong(long_buf);
    }

    // 4. Fuzz TIFFReadRawStrip
    // Open the dummy file
    tif = TIFFOpen(dummy_file, "r");
    if (tif) {
        // Try to read strip 0
        // Size can be -1 (full strip) or a positive value from input
        tmsize_t read_size = -1;
        if (Size > 0 && Data[0] != 0) {
            read_size = (tmsize_t)Data[0];
        }
        TIFFReadRawStrip(tif, 0, buf, read_size);
        safe_cleanup(tif);
        tif = NULL;
    }

    // 5. Fuzz TIFFReverseBits
    if (alloc_size > 0) {
        TIFFReverseBits(bit_buf, alloc_size);
    }

    // 6. Fuzz TIFFSwabArrayOfLong
    if (count4 > 0) {
        TIFFSwabArrayOfLong(long_buf, count4);
    }

    // Cleanup
    free(buf);
    free(long_buf);
    free(long8_buf);
    free(bit_buf);
    free(copy_src);
    free(copy_dst);

    // Remove dummy file
    remove(dummy_file);

    return 0;
}