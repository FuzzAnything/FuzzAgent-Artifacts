// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_tiff(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;

    // Write a minimal valid TIFF header (BigTIFF or Classic)
    // Classic TIFF Header:
    // 2 bytes: Byte order (II = little, MM = big)
    // 2 bytes: Magic number (42)
    // 4 bytes: Offset to first IFD
    uint8_t header[8];
    header[0] = 'I'; // Little endian
    header[1] = 'I';
    header[2] = 42;
    header[3] = 0;
    // Offset to first IFD (set to 8, start of data)
    uint32_t offset = 8;
    memcpy(header + 4, &offset, 4);

    fwrite(header, 1, 8, fp);

    // Write a minimal IFD
    // 2 bytes: Number of directory entries
    uint16_t num_entries = 1;
    fwrite(&num_entries, 2, 1, fp);

    // 12 bytes: Directory entry
    // 2 bytes: Tag (ImageWidth)
    uint16_t tag = 256;
    fwrite(&tag, 2, 1, fp);
    // 2 bytes: Type (SHORT)
    uint16_t type = 3;
    fwrite(&type, 2, 1, fp);
    // 4 bytes: Count
    uint32_t count = 1;
    fwrite(&count, 4, 1, fp);
    // 4 bytes: Value/Offset (width = 100)
    uint32_t value = 100;
    fwrite(&value, 4, 1, fp);

    // 4 bytes: Offset to next IFD (0 for last)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, 4, 1, fp);

    // Write some raw data if available
    if (size > 0) {
        fwrite(data, 1, size, fp);
    }

    fclose(fp);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create dummy file
    write_dummy_tiff(Data, Size);

    // Open the file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // 1. Fuzz TIFFSwabLong
    uint32_t val = 0x12345678;
    TIFFSwabLong(&val);

    // 2. Fuzz TIFFReadEncodedStrip
    // Prepare buffer
    size_t buf_size = Size > 1024 ? 1024 : Size;
    uint8_t *buf = (uint8_t *)malloc(buf_size);
    if (buf) {
        // Try reading strip 0
        tmsize_t result = TIFFReadEncodedStrip(tif, 0, buf, buf_size);
        (void)result; // Suppress unused warning
        free(buf);
    }

    // 3. Fuzz TIFFGetBitRevTable
    const unsigned char *table0 = TIFFGetBitRevTable(0);
    const unsigned char *table1 = TIFFGetBitRevTable(1);
    if (table0) (void)table0[0];
    if (table1) (void)table1[0];

    // 4. Fuzz TIFFSwabArrayOfLong
    uint32_t *arr_long = (uint32_t *)malloc(Size > 4 ? Size / 4 * 4 : 4);
    if (arr_long) {
        size_t n_long = (Size > 4 ? Size / 4 * 4 : 4) / 4;
        memset(arr_long, 0x12, n_long * 4);
        TIFFSwabArrayOfLong(arr_long, n_long);
        free(arr_long);
    }

    // 5. Fuzz TIFFSwabArrayOfTriples
    uint8_t *arr_triples = (uint8_t *)malloc(Size > 3 ? Size / 3 * 3 : 3);
    if (arr_triples) {
        size_t n_triples = (Size > 3 ? Size / 3 * 3 : 3) / 3;
        memset(arr_triples, 0x12, n_triples * 3);
        TIFFSwabArrayOfTriples(arr_triples, n_triples);
        free(arr_triples);
    }

    // 6. Fuzz TIFFReverseBits
    uint8_t *arr_bits = (uint8_t *)malloc(Size > 0 ? Size : 1);
    if (arr_bits) {
        memset(arr_bits, 0xAA, Size > 0 ? Size : 1);
        TIFFReverseBits(arr_bits, Size > 0 ? Size : 1);
        free(arr_bits);
    }

    // Cleanup
    TIFFClose(tif);
    return 0;
}