// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
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

static void write_dummy_tiff(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return;
    }
    // Write a minimal valid TIFF header (Big Endian: 0x4949 is Little Endian, 0x4D4D is Big Endian)
    // We try both to test endianness logic if the fuzzer provides enough data
    uint16_t magic;
    if (Size >= 2) {
        magic = (Data[0] << 8) | Data[1];
        // If the fuzzer data looks like a valid magic (0x4949 or 0x4D4D), use it, otherwise default to Big Endian
        if (magic != 0x4949 && magic != 0x4D4D) {
            magic = 0x4D4D; // Big Endian
        }
    } else {
        magic = 0x4D4D;
    }
    fwrite(&magic, sizeof(uint16_t), 1, fp);
    
    // Write magic number (42)
    uint16_t version = 42;
    fwrite(&version, sizeof(uint16_t), 1, fp);
    
    // Write offset to first IFD (8 bytes for BigTIFF, 4 for classic, we write 4 for classic)
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, fp);
    
    // Write a minimal IFD: 0 entries (end of IFD)
    uint16_t entries = 0;
    fwrite(&entries, sizeof(uint16_t), 1, fp);
    
    // Next IFD offset (0)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(uint32_t), 1, fp);
    
    // Append raw fuzzer data if any, to potentially confuse parsers or fill buffers
    if (Size > 4) {
        fwrite(Data, 1, Size, fp);
    }
    
    fclose(fp);
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    // Create a dummy file with minimal TIFF structure
    write_dummy_tiff(Data, Size);

    // Open the file for reading
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    
    if (tif) {
        // 2. Invoke target functions with valid handle
        
        // TIFFIsBigEndian
        int is_be = TIFFIsBigEndian(tif);
        (void)is_be; // Suppress unused warning

        // TIFFIsByteSwapped
        int is_swapped = TIFFIsByteSwapped(tif);
        (void)is_swapped;

        // TIFFIsMSB2LSB
        int is_msb = TIFFIsMSB2LSB(tif);
        (void)is_msb;

        // 3. Explore more program states
        // TIFFGetBitRevTable: Try both 0 and 1
        const unsigned char *table_rev = TIFFGetBitRevTable(1);
        const unsigned char *table_norm = TIFFGetBitRevTable(0);
        
        if (table_rev) {
            // Just access a few bytes to ensure table is valid
            (void)table_rev[0];
        }
        if (table_norm) {
            (void)table_norm[0];
        }

        // TIFFReverseBits: Create a buffer to reverse
        uint8_t *buffer = (uint8_t *)malloc(Size > 0 ? Size : 1);
        if (buffer) {
            memcpy(buffer, Data, Size > 0 ? Size : 1);
            // Reverse bits in the buffer
            TIFFReverseBits(buffer, Size > 0 ? Size : 1);
            // Reverse again to restore (or just to exercise logic)
            TIFFReverseBits(buffer, Size > 0 ? Size : 1);
            free(buffer);
        }

        // TIFFSwabLong: Create a value to swap
        uint32_t val = 0;
        if (Size >= 4) {
            val = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        }
        TIFFSwabLong(&val);
        TIFFSwabLong(&val); // Swap back

        // 4. Cleanup
        TIFFClose(tif);
    } else {
        // Test behavior with invalid handle (though the API docs say it fails/unpredictable)
        // We don't call the functions here to avoid crashes, but the fuzzer might explore
        // other paths if we had more complex logic.
        // For safety, we just return.
    }

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}