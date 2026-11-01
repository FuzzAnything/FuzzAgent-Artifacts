// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    
    // Write a minimal TIFF header (BigTIFF or Classic)
    // Classic TIFF Header: 2 bytes magic (42 00 or 00 42), 2 bytes offset to first IFD
    uint16_t magic = 0x002A; // Little endian
    uint32_t ifd_offset = 8;
    
    fwrite(&magic, sizeof(magic), 1, f);
    fwrite(&ifd_offset, sizeof(ifd_offset), 1, f);
    
    // Write a minimal IFD
    // Number of directory entries (1 entry)
    uint16_t num_entries = 1;
    fwrite(&num_entries, sizeof(num_entries), 1, f);
    
    // Entry 1: Tag 256 (ImageWidth), Type 3 (SHORT), Count 1, Value 1
    uint16_t tag = 256;
    uint16_t type = 3; // SHORT
    uint32_t count = 1;
    uint32_t value = 1;
    
    fwrite(&tag, sizeof(tag), 1, f);
    fwrite(&type, sizeof(type), 1, f);
    fwrite(&count, sizeof(count), 1, f);
    fwrite(&value, sizeof(value), 1, f);
    
    // Next IFD offset (0 means end of chain)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(next_ifd), 1, f);
    
    // Pad with input data if available to make it look like image data
    // We write some data at offset 8 + 12 (IFD size) = 20
    if (size > 0) {
        size_t to_write = (size < 1024) ? size : 1024;
        fwrite(data, 1, to_write, f);
    }
    
    fclose(f);
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure we have enough data to create a dummy file
    write_dummy_tiff(Data, Size);

    // Open the dummy file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Test TIFFReadEncodedStrip
    // We need a valid strip index. In our dummy file, we have 1 entry for ImageWidth.
    // We assume strip 0 might exist if the file is valid enough.
    uint32_t strip = 0;
    tmsize_t strip_size = 1024;
    uint8_t *buf = (uint8_t *)malloc(strip_size);
    if (buf) {
        TIFFReadEncodedStrip(tif, strip, buf, strip_size);
        
        // Test TIFFReverseBits on the buffer
        if (strip_size > 0) {
            TIFFReverseBits(buf, strip_size);
        }
        
        // Test TIFFSwabArrayOfTriples (assuming RGB data)
        // Ensure size is multiple of 3
        tmsize_t triples = strip_size / 3;
        if (triples > 0) {
            TIFFSwabArrayOfTriples(buf, triples);
        }
        
        // Test TIFFSwabArrayOfLong
        // Ensure size is multiple of 4
        tmsize_t longs = strip_size / 4;
        if (longs > 0) {
            TIFFSwabArrayOfLong((uint32_t *)buf, longs);
        }
        
        free(buf);
    }

    // Test TIFFGetBitRevTable
    const unsigned char *rev_table = TIFFGetBitRevTable(1);
    const unsigned char *non_rev_table = TIFFGetBitRevTable(0);
    // Just use the pointers to ensure they are not null (though they shouldn't be)
    if (rev_table && non_rev_table) {
        // Dummy usage to avoid unused variable warning
        volatile unsigned char dummy = rev_table[0] ^ non_rev_table[0];
        (void)dummy;
    }

    // Test TIFFSwabLong
    uint32_t val = 0x12345678;
    TIFFSwabLong(&val);
    (void)val;

    // Cleanup
    TIFFClose(tif);
    
    // Clean up dummy file
    remove("./dummy_file");

    return 0;
}