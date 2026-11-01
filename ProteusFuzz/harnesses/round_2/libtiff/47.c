// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
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

static void write_dummy_tiff(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return;
    }
    // Write a minimal valid TIFF header (Little Endian)
    // Magic number (0x002A)
    uint16_t magic = 0x002A;
    fwrite(&magic, sizeof(uint16_t), 1, fp);
    
    // Offset to first IFD (4 bytes)
    uint32_t ifd_offset = 8;
    fwrite(&ifd_offset, sizeof(uint32_t), 1, fp);
    
    // IFD entry count (2 bytes)
    uint16_t entry_count = 0;
    fwrite(&entry_count, sizeof(uint16_t), 1, fp);
    
    // Next IFD offset (4 bytes, 0 for none)
    uint32_t next_ifd = 0;
    fwrite(&next_ifd, sizeof(uint32_t), 1, fp);
    
    // Write the fuzzing data as raw payload
    if (Size > 0 && Data != NULL) {
        fwrite(Data, 1, Size, fp);
    }
    
    fclose(fp);
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment for TIFFReadRawStrip
    write_dummy_tiff(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif != NULL) {
        // Allocate buffer for reading
        uint8_t *buf = (uint8_t *)malloc(Size > 0 ? Size : 1);
        if (buf != NULL) {
            // Fuzz TIFFReadRawStrip with varying strip indices and sizes
            // Strip 0 is the most likely to exist in a minimal file
            tmsize_t result = TIFFReadRawStrip(tif, 0, buf, Size);
            (void)result; // Use result to suppress warnings
            
            // Try reading with size -1 (full strip) if Size is large enough
            if (Size > 100) {
                TIFFReadRawStrip(tif, 0, buf, -1);
            }
            
            // Try invalid strip index
            TIFFReadRawStrip(tif, 9999, buf, Size);
            
            free(buf);
        }
        TIFFClose(tif);
    }

    // Fuzz TIFFSwabArrayOfShort
    if (Size >= 2) {
        uint16_t *short_buf = (uint16_t *)malloc(Size);
        if (short_buf != NULL) {
            _TIFFmemcpy(short_buf, Data, Size);
            tmsize_t n_shorts = Size / sizeof(uint16_t);
            TIFFSwabArrayOfShort(short_buf, n_shorts);
            free(short_buf);
        }
    }

    // Fuzz TIFFReverseBits
    if (Size > 0) {
        uint8_t *byte_buf = (uint8_t *)malloc(Size);
        if (byte_buf != NULL) {
            _TIFFmemcpy(byte_buf, Data, Size);
            TIFFReverseBits(byte_buf, Size);
            free(byte_buf);
        }
    }

    // Fuzz TIFFSwabArrayOfLong
    if (Size >= 4) {
        uint32_t *long_buf = (uint32_t *)malloc(Size);
        if (long_buf != NULL) {
            _TIFFmemcpy(long_buf, Data, Size);
            tmsize_t n_longs = Size / sizeof(uint32_t);
            TIFFSwabArrayOfLong(long_buf, n_longs);
            free(long_buf);
        }
    }

    // Fuzz _TIFFmemcpy directly
    if (Size > 0) {
        uint8_t *dst = (uint8_t *)malloc(Size);
        if (dst != NULL) {
            _TIFFmemcpy(dst, Data, Size);
            free(dst);
        }
    }

    // Fuzz TIFFSwabLong
    if (Size >= 4) {
        uint32_t val = 0;
        _TIFFmemcpy(&val, Data, sizeof(uint32_t));
        TIFFSwabLong(&val);
    }

    return 0;
}