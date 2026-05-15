// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
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

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test TIFFReverseBits
    size_t rev_size = Size > 1024 ? 1024 : Size;
    uint8_t *rev_buf = (uint8_t *)malloc(rev_size);
    if (rev_buf) {
        memcpy(rev_buf, Data, rev_size);
        TIFFReverseBits(rev_buf, (tmsize_t)rev_size);
        free(rev_buf);
    }

    // Test TIFFGetBitRevTable
    int reversed = Data[0] & 1;
    const unsigned char *table = TIFFGetBitRevTable(reversed);
    (void)table; // Use to avoid unused variable warning

    // Create dummy file for TIFF operations
    write_dummy_file(Data, Size);

    // Try to open the dummy file as TIFF
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // Test TIFFIsByteSwapped
        (void)TIFFIsByteSwapped(tif);

        // Test TIFFIsMSB2LSB
        (void)TIFFIsMSB2LSB(tif);

        // Test TIFFIsBigEndian
        (void)TIFFIsBigEndian(tif);

        // Test TIFFWriteScanline - need to set up basic directory first
        // Set minimal required fields for writing
        uint32_t width = 1;
        uint32_t height = 1;
        uint16_t samplesperpixel = 1;
        uint16_t bitspersample = 8;
        uint16_t photometric = PHOTOMETRIC_MINISBLACK;
        uint16_t planarconfig = PLANARCONFIG_CONTIG;
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

        // Prepare scanline buffer
        tmsize_t scanline_size = TIFFScanlineSize(tif);
        if (scanline_size > 0) {
            uint8_t *buf = (uint8_t *)malloc(scanline_size);
            if (buf) {
                size_t copy_size = Size > scanline_size ? scanline_size : Size;
                memcpy(buf, Data, copy_size);
                
                // Try writing scanline
                (void)TIFFWriteScanline(tif, buf, 0, 0);
                free(buf);
            }
        }

        TIFFClose(tif);
    }

    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}