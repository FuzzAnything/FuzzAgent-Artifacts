// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare environment for TIFFReverseBits
    // Allocate buffer for bit reversal
    uint8_t *buf_rev = (uint8_t *)malloc(Size);
    if (buf_rev == NULL) return 0;
    memcpy(buf_rev, Data, Size);
    
    // Call TIFFReverseBits
    TIFFReverseBits(buf_rev, Size);
    free(buf_rev);

    // 2. Prepare environment for TIFFSwabArrayOfLong
    // Ensure size is multiple of 4 for uint32_t array
    size_t num_longs = Size / sizeof(uint32_t);
    if (num_longs > 0) {
        uint32_t *buf_long = (uint32_t *)malloc(num_longs * sizeof(uint32_t));
        if (buf_long != NULL) {
            memcpy(buf_long, Data, num_longs * sizeof(uint32_t));
            TIFFSwabArrayOfLong(buf_long, num_longs);
            free(buf_long);
        }
    }

    // 3. Prepare environment for TIFFSwabLong
    // Use a single uint32_t from data if available
    if (Size >= sizeof(uint32_t)) {
        uint32_t val;
        memcpy(&val, Data, sizeof(uint32_t));
        TIFFSwabLong(&val);
    }

    // 4. Prepare environment for TIFFReadEncodedStrip
    // Create a dummy file with minimal valid TIFF header to test reading
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f != NULL) {
        // Write a minimal valid TIFF header (Little Endian)
        // 2 bytes: II (Little Endian)
        // 2 bytes: 42 (Magic number)
        // 4 bytes: 8 (Offset to first IFD)
        uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
        fwrite(header, 1, 8, f);
        
        // Write a minimal IFD at offset 8
        // 2 bytes: Number of directory entries (0)
        // 4 bytes: Offset to next IFD (0)
        uint8_t ifd[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        fwrite(ifd, 1, 6, f);
        
        // Pad with input data to make it look like image data if needed
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);

        TIFF *tif = TIFFOpen(dummy_file, "r");
        if (tif != NULL) {
            // Attempt to read a strip. 
            // Since the file is minimal, this might fail, which is expected for fuzzing.
            uint8_t *strip_buf = (uint8_t *)malloc(Size > 0 ? Size : 1);
            if (strip_buf != NULL) {
                TIFFReadEncodedStrip(tif, 0, strip_buf, Size > 0 ? Size : 1);
                free(strip_buf);
            }
            TIFFClose(tif);
        }
        // Cleanup dummy file
        unlink(dummy_file);
    }

    // 5. Prepare environment for TIFFGetBitRevTable
    // This function just returns a pointer, no state change needed.
    const unsigned char *table_rev = TIFFGetBitRevTable(1);
    const unsigned char *table_non_rev = TIFFGetBitRevTable(0);
    
    // Just ensure pointers are not NULL (they shouldn't be)
    if (table_rev != NULL && table_non_rev != NULL) {
        // Access a few bytes to ensure they are valid
        (void)table_rev[0];
        (void)table_non_rev[0];
    }

    // 6. Prepare environment for TIFFSwabArrayOfTriples
    // Ensure size is multiple of 3 for triples
    size_t num_triples = Size / 3;
    if (num_triples > 0) {
        uint8_t *buf_triples = (uint8_t *)malloc(num_triples * 3);
        if (buf_triples != NULL) {
            memcpy(buf_triples, Data, num_triples * 3);
            TIFFSwabArrayOfTriples(buf_triples, num_triples);
            free(buf_triples);
        }
    }

    return 0;
}