// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;

    // Write a minimal valid TIFF header
    // Byte order: Little Endian (II)
    uint16_t byte_order = 0x002A; // Magic number for little endian
    uint16_t magic = 42;
    uint32_t ifd_offset = 8;

    fwrite(&byte_order, sizeof(uint16_t), 1, f);
    fwrite(&magic, sizeof(uint16_t), 1, f);
    fwrite(&ifd_offset, sizeof(uint32_t), 1, f);

    // Write a minimal IFD at offset 8
    // Number of directory entries: 1
    uint16_t num_entries = 1;
    uint16_t tag = 256; // ImageWidth
    uint16_t type = 3;  // SHORT
    uint32_t count = 1;
    uint32_t value = 1; // Width = 1

    fwrite(&num_entries, sizeof(uint16_t), 1, f);
    fwrite(&tag, sizeof(uint16_t), 1, f);
    fwrite(&type, sizeof(uint16_t), 1, f);
    fwrite(&count, sizeof(uint32_t), 1, f);
    fwrite(&value, sizeof(uint32_t), 1, f);

    // Next IFD offset: 0 (end of chain)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(uint32_t), 1, f);

    // Write some dummy strip data if available
    if (data && size > 0) {
        fwrite(data, 1, size, f);
    }

    fclose(f);
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare environment for TIFFReadEncodedStrip
    write_dummy_tiff(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) return 0;

    // Allocate buffers for testing
    uint8_t *buffer = (uint8_t *)malloc(Size + 1024);
    if (!buffer) {
        TIFFClose(tif);
        return 0;
    }

    // 2. Fuzz TIFFGetBitRevTable
    // Try both 0 and 1
    const unsigned char *rev_table_0 = TIFFGetBitRevTable(0);
    const unsigned char *rev_table_1 = TIFFGetBitRevTable(1);
    
    // Use the tables to ensure they are accessed (avoid unused variable warnings)
    if (rev_table_0 && rev_table_1) {
        volatile unsigned char dummy = rev_table_0[0] ^ rev_table_1[0];
        (void)dummy;
    }

    // 3. Fuzz TIFFSwabLong
    uint32_t val = 0;
    if (Size >= 4) {
        memcpy(&val, Data, 4);
    } else {
        val = Size;
    }
    TIFFSwabLong(&val);
    (void)val;

    // 4. Fuzz TIFFSwabArrayOfLong
    if (Size >= 4) {
        uint32_t *long_arr = (uint32_t *)malloc(Size);
        if (long_arr) {
            memcpy(long_arr, Data, Size);
            TIFFSwabArrayOfLong(long_arr, Size / sizeof(uint32_t));
            free(long_arr);
        }
    }

    // 5. Fuzz TIFFSwabArrayOfTriples
    if (Size >= 3) {
        uint8_t *triples = (uint8_t *)malloc(Size);
        if (triples) {
            memcpy(triples, Data, Size);
            TIFFSwabArrayOfTriples(triples, Size / 3);
            free(triples);
        }
    }

    // 6. Fuzz TIFFReverseBits
    if (Size > 0) {
        uint8_t *bits = (uint8_t *)malloc(Size);
        if (bits) {
            memcpy(bits, Data, Size);
            TIFFReverseBits(bits, Size);
            free(bits);
        }
    }

    // 7. Fuzz TIFFReadEncodedStrip
    // We try to read strip 0. If the file is valid, this might succeed or fail gracefully.
    // We pass the buffer and a safe size.
    if (buffer) {
        tmsize_t bytes_read = TIFFReadEncodedStrip(tif, 0, buffer, Size);
        // bytes_read can be -1 on error, which is expected for malformed inputs
        (void)bytes_read;
    }

    // 8. Cleanup
    free(buffer);
    TIFFClose(tif);
    
    // Remove dummy file
    remove("./dummy_file");

    return 0;
}