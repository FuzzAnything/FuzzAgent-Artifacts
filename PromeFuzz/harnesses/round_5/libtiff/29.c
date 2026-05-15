// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silence warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Silence libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);

    // Test TIFFReverseBits
    if (Size > 0) {
        uint8_t *buffer = (uint8_t *)malloc(Size);
        if (buffer) {
            memcpy(buffer, Data, Size);
            TIFFReverseBits(buffer, Size);
            free(buffer);
        }
    }

    // Test TIFFGetBitRevTable
    if (Size > 0) {
        int reversed = Data[0] & 1;
        const unsigned char *table = TIFFGetBitRevTable(reversed);
        (void)table; // Use result to avoid compiler warnings
    }

    // Create a dummy TIFF file for testing file-related functions
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        // Write minimal valid TIFF header
        if (Size >= 8) {
            // Big/little endian signature
            if (Data[0] & 1) {
                fwrite("\x4d\x4d\x00\x2a", 1, 4, fp); // Big endian
            } else {
                fwrite("\x49\x49\x2a\x00", 1, 4, fp); // Little endian
            }
            // First IFD offset
            fwrite("\x00\x00\x00\x08", 1, 4, fp);
        } else {
            // Default minimal header
            fwrite("\x49\x49\x2a\x00\x00\x00\x00\x08", 1, 8, fp);
        }
        
        // Write some additional data from fuzzer input
        if (Size > 0) {
            size_t write_size = Size > 1024 ? 1024 : Size;
            fwrite(Data, 1, write_size, fp);
        }
        fclose(fp);
    }

    // Test file-related functions
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // Test TIFFIsBigEndian
        int is_big_endian = TIFFIsBigEndian(tif);
        (void)is_big_endian;
        
        // Test TIFFIsMSB2LSB
        int is_msb2lsb = TIFFIsMSB2LSB(tif);
        (void)is_msb2lsb;
        
        // Test TIFFIsByteSwapped
        int is_byte_swapped = TIFFIsByteSwapped(tif);
        (void)is_byte_swapped;
        
        // Test TIFFWriteScanline if we can write to the file
        TIFFClose(tif);
        
        // Reopen for writing
        tif = TIFFOpen("./dummy_file", "w");
        if (tif) {
            // Set minimal required fields for writing
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            
            // Try to write a scanline
            uint8_t scanline_data[1] = {0};
            if (Size > 0) {
                scanline_data[0] = Data[0];
            }
            uint32_t row = 0;
            uint16_t sample = 0;
            if (Size > 4) {
                row = *(uint32_t *)Data % 100;
                sample = *(uint16_t *)(Data + 4) % 10;
            }
            TIFFWriteScanline(tif, scanline_data, row, sample);
            
            TIFFClose(tif);
        }
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}