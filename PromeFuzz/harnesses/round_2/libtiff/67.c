// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libtiff/tiffio.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    /* Write fuzzer input to dummy file */
    write_dummy_file(Data, Size);

    /* Test TIFFGetBitRevTable with various inputs */
    const unsigned char *table1 = TIFFGetBitRevTable(0);
    const unsigned char *table2 = TIFFGetBitRevTable(1);
    const unsigned char *table3 = TIFFGetBitRevTable(Data[0] & 1);
    const unsigned char *table4 = TIFFGetBitRevTable(Data[1] % 3);

    /* Test TIFFReverseBits on a buffer */
    size_t buf_size = Size > 1024 ? 1024 : Size;
    uint8_t *buffer = (uint8_t *)malloc(buf_size);
    if (!buffer) return 0;
    
    memcpy(buffer, Data, buf_size);
    TIFFReverseBits(buffer, buf_size);
    
    /* Try different sizes for bit reversal */
    if (buf_size > 8) {
        TIFFReverseBits(buffer, 8);
        TIFFReverseBits(buffer + 8, buf_size - 8);
    }
    
    /* Create a TIFF file to test other functions */
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        /* Test TIFFIsBigEndian */
        int is_big_endian = TIFFIsBigEndian(tif);
        
        /* Test TIFFIsMSB2LSB */
        int is_msb2lsb = TIFFIsMSB2LSB(tif);
        
        /* Test TIFFIsByteSwapped */
        int is_byte_swapped = TIFFIsByteSwapped(tif);
        
        /* Test TIFFWriteScanline if file is writable */
        TIFF *tif_write = TIFFOpen("./dummy_file", "a");
        if (tif_write) {
            /* Set up required fields for writing */
            uint32_t width = 100;
            uint32_t height = 100;
            uint16_t samples_per_pixel = 1;
            uint16_t bits_per_sample = 8;
            uint16_t photometric = PHOTOMETRIC_MINISBLACK;
            uint16_t planar_config = PLANARCONFIG_CONTIG;
            
            TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, width);
            TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, height);
            TIFFSetField(tif_write, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
            TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
            TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, photometric);
            TIFFSetField(tif_write, TIFFTAG_PLANARCONFIG, planar_config);
            TIFFSetField(tif_write, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif_write, 0));
            
            /* Prepare scanline buffer */
            uint8_t *scanline = (uint8_t *)malloc(width * samples_per_pixel);
            if (scanline) {
                size_t copy_size = Size > width ? width : Size;
                memcpy(scanline, Data, copy_size);
                
                /* Try writing scanlines with different row numbers */
                for (uint32_t row = 0; row < 10 && row < height; row++) {
                    uint16_t sample = Data[row] % (samples_per_pixel + 1);
                    TIFFWriteScanline(tif_write, scanline, row, sample);
                }
                
                free(scanline);
            }
            
            TIFFClose(tif_write);
        }
        
        TIFFClose(tif);
    }
    
    /* Test with a newly created TIFF file */
    TIFF *new_tif = TIFFOpen("./dummy_file", "w");
    if (new_tif) {
        /* Set basic required fields */
        uint32_t width = 50;
        uint32_t height = 50;
        uint16_t samples_per_pixel = 3;
        uint16_t bits_per_sample = 8;
        
        TIFFSetField(new_tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(new_tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(new_tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
        TIFFSetField(new_tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
        TIFFSetField(new_tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
        TIFFSetField(new_tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(new_tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(new_tif, 0));
        
        /* Test the query functions on new file */
        TIFFIsBigEndian(new_tif);
        TIFFIsMSB2LSB(new_tif);
        TIFFIsByteSwapped(new_tif);
        
        /* Try writing a scanline */
        uint8_t *scanline = (uint8_t *)malloc(width * samples_per_pixel);
        if (scanline) {
            size_t copy_size = Size > (width * samples_per_pixel) ? 
                              (width * samples_per_pixel) : Size;
            memcpy(scanline, Data, copy_size);
            
            /* Test with different sample values */
            for (uint16_t sample = 0; sample < 3; sample++) {
                TIFFWriteScanline(new_tif, scanline, Data[0] % height, sample);
            }
            
            free(scanline);
        }
        
        TIFFClose(new_tif);
    }
    
    free(buffer);
    return 0;
}