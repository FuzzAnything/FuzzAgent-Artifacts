// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Test TIFFGetBitRevTable
    int reversed = Data[0] & 1;
    const unsigned char *table = TIFFGetBitRevTable(reversed);
    (void)table; // Suppress unused variable warning

    // Test TIFFReverseBits
    size_t rev_size = Size > 1024 ? 1024 : Size;
    uint8_t *buf = malloc(rev_size);
    if (buf) {
        memcpy(buf, Data, rev_size);
        TIFFReverseBits(buf, rev_size);
        free(buf);
    }

    // Create dummy TIFF file for remaining functions
    write_dummy_file(Data, Size);
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // Test TIFFIsByteSwapped
        int swapped = TIFFIsByteSwapped(tif);
        (void)swapped;

        // Test TIFFIsMSB2LSB
        int msb2lsb = TIFFIsMSB2LSB(tif);
        (void)msb2lsb;

        // Test TIFFIsBigEndian
        int big_endian = TIFFIsBigEndian(tif);
        (void)big_endian;

        // Test TIFFWriteScanline with minimal setup
        if (Size >= 4) {
            uint32_t row = *(uint32_t *)Data % 100; // Limit row to reasonable value
            uint16_t sample = *(uint16_t *)(Data + 2) % 4; // Limit sample to reasonable value
            
            size_t scanline_size = Size > 256 ? 256 : Size;
            void *scanline_buf = malloc(scanline_size);
            if (scanline_buf) {
                memcpy(scanline_buf, Data, scanline_size);
                TIFFWriteScanline(tif, scanline_buf, row, sample);
                free(scanline_buf);
            }
        }

        TIFFClose(tif);
    }

    // Clean up
    unlink("./dummy_file");
    return 0;
}