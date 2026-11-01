// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <unistd.h>

/*
 * Fuzz driver for libtiff API functions:
 * - TIFFSwabArrayOfLong8
 * - _TIFFmemcpy
 * - TIFFSwabLong
 * - TIFFReadRawStrip
 * - TIFFReverseBits
 * - TIFFSwabArrayOfLong
 */

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Allocate a buffer large enough to hold various test data */
    size_t alloc_size = Size > 4096 ? 4096 : Size;
    uint8_t *buf = (uint8_t *)malloc(alloc_size);
    if (!buf) {
        return 0;
    }
    memcpy(buf, Data, alloc_size);

    /* 1. Test TIFFSwabArrayOfLong8 */
    /* Ensure we have at least 8 bytes for one uint64_t */
    if (alloc_size >= sizeof(uint64_t)) {
        uint64_t *lp64 = (uint64_t *)buf;
        tmsize_t n64 = (alloc_size / sizeof(uint64_t));
        if (n64 > 0) {
            TIFFSwabArrayOfLong8(lp64, n64);
        }
    }

    /* 2. Test _TIFFmemcpy */
    /* Ensure we have at least 2 bytes to copy from and to */
    if (alloc_size >= 2) {
        uint8_t *dst = (uint8_t *)malloc(alloc_size);
        if (dst) {
            _TIFFmemcpy(dst, buf, alloc_size);
            free(dst);
        }
    }

    /* 3. Test TIFFSwabLong */
    /* Ensure we have at least 4 bytes for one uint32_t */
    if (alloc_size >= sizeof(uint32_t)) {
        uint32_t *lp32 = (uint32_t *)buf;
        TIFFSwabLong(lp32);
    }

    /* 4. Test TIFFReadRawStrip */
    /* Create a dummy TIFF file to test file-based operations */
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        /* Write a minimal valid TIFF header (BigTIFF or Classic) */
        /* Classic TIFF header: 2 bytes magic, 2 bytes offset to first IFD */
        uint16_t magic = 0x002A; /* Little-endian magic for TIFF */
        uint32_t ifd_offset = 8; /* First IFD at offset 8 */
        fwrite(&magic, sizeof(uint16_t), 1, fp);
        fwrite(&ifd_offset, sizeof(uint32_t), 1, fp);
        /* Write a minimal IFD (2 bytes count, 2 bytes next IFD, then entries) */
        uint16_t count = 0; /* No tags */
        uint32_t next_ifd = 0; /* No next IFD */
        fwrite(&count, sizeof(uint16_t), 1, fp);
        fwrite(&next_ifd, sizeof(uint32_t), 1, fp);
        /* Pad to 8 bytes if needed */
        fseek(fp, 8, SEEK_SET);
        fwrite(&count, sizeof(uint16_t), 1, fp);
        fwrite(&next_ifd, sizeof(uint32_t), 1, fp);
        fclose(fp);

        /* Open the TIFF file */
        TIFF *tif = TIFFOpen(dummy_file, "r");
        if (tif) {
            /* Attempt to read a raw strip (strip 0) */
            uint8_t *read_buf = (uint8_t *)malloc(alloc_size);
            if (read_buf) {
                tmsize_t result = TIFFReadRawStrip(tif, 0, read_buf, alloc_size);
                (void)result; /* Suppress unused variable warning */
                free(read_buf);
            }
            TIFFClose(tif);
        }
        /* Clean up dummy file */
        unlink(dummy_file);
    }

    /* 5. Test TIFFReverseBits */
    /* Ensure we have at least 1 byte */
    if (alloc_size >= 1) {
        TIFFReverseBits(buf, alloc_size);
    }

    /* 6. Test TIFFSwabArrayOfLong */
    /* Ensure we have at least 4 bytes for one uint32_t */
    if (alloc_size >= sizeof(uint32_t)) {
        uint32_t *lp32_arr = (uint32_t *)buf;
        tmsize_t n32 = (alloc_size / sizeof(uint32_t));
        if (n32 > 0) {
            TIFFSwabArrayOfLong(lp32_arr, n32);
        }
    }

    free(buf);
    return 0;
}