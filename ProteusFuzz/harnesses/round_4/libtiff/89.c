// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid TIFF header if input is empty to avoid immediate crash on open
            uint16_t header[8] = {0x4949, 42, 8, 0, 0, 0, 0, 0};
            fwrite(header, sizeof(header), 1, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // 1. Fuzz TIFFGetBitRevTable
    // Input: 0 or 1 (deterministic)
    const unsigned char *table0 = TIFFGetBitRevTable(0);
    const unsigned char *table1 = TIFFGetBitRevTable(1);
    
    // Use the tables to ensure they are not NULL and access them safely
    if (table0) {
        volatile unsigned char val0 = table0[0];
        (void)val0;
    }
    if (table1) {
        volatile unsigned char val1 = table1[0];
        (void)val1;
    }

    // 2. Fuzz TIFFReadEncodedStrip
    // We need a buffer and a strip index.
    // Use input data to determine strip index and buffer size.
    uint32_t strip_index = (Data[0] % 100);
    size_t buf_size = (Size > 1024) ? 1024 : Size;
    
    uint8_t *read_buf = (uint8_t *)malloc(buf_size);
    if (read_buf) {
        tmsize_t result = TIFFReadEncodedStrip(tif, strip_index, read_buf, buf_size);
        (void)result; // Result is ignored for fuzzing, just checking for crashes
        free(read_buf);
    }

    // 3. Fuzz TIFFSwabLong
    uint32_t swab_val = 0x12345678;
    if (Size > 1) {
        swab_val ^= ((uint32_t)Data[1] << 8) ^ Data[2];
    }
    TIFFSwabLong(&swab_val);
    (void)swab_val;

    // 4. Fuzz TIFFSwabArrayOfTriples
    // Prepare a buffer with triples (3 bytes each)
    size_t triple_count = (Size > 10) ? (Size / 3) : 1;
    uint8_t *triples_buf = (uint8_t *)malloc(triple_count * 3);
    if (triples_buf && triple_count > 0) {
        // Fill with fuzz data
        size_t fill_len = (Size < triple_count * 3) ? Size : triple_count * 3;
        memcpy(triples_buf, Data, fill_len);
        // Pad if necessary
        if (fill_len < triple_count * 3) {
            memset(triples_buf + fill_len, 0, (triple_count * 3) - fill_len);
        }
        
        TIFFSwabArrayOfTriples(triples_buf, triple_count);
        free(triples_buf);
    }

    // 5. Fuzz TIFFReverseBits
    size_t bits_buf_size = (Size > 100) ? 100 : Size;
    uint8_t *bits_buf = (uint8_t *)malloc(bits_buf_size);
    if (bits_buf && bits_buf_size > 0) {
        memcpy(bits_buf, Data, bits_buf_size);
        TIFFReverseBits(bits_buf, bits_buf_size);
        free(bits_buf);
    }

    // 6. Fuzz TIFFSwabArrayOfLong
    size_t long_count = (Size > 4) ? (Size / 4) : 1;
    uint32_t *long_buf = (uint32_t *)malloc(long_count * sizeof(uint32_t));
    if (long_buf && long_count > 0) {
        size_t fill_len = (Size < long_count * 4) ? Size : long_count * 4;
        memcpy(long_buf, Data, fill_len);
        if (fill_len < long_count * 4) {
            memset((uint8_t *)long_buf + fill_len, 0, (long_count * 4) - fill_len);
        }
        
        TIFFSwabArrayOfLong(long_buf, long_count);
        free(long_buf);
    }

    // Cleanup
    TIFFClose(tif);
    return 0;
}