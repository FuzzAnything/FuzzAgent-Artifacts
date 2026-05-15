// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
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
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff error/warning messages during fuzzing
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write data to dummy file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    ssize_t written = write(fd, Data, Size);
    close(fd);
    
    if (written != (ssize_t)Size) return 0;
    
    // Test 1: Try to open file for reading first
    TIFF* tif_read = TIFFOpen("./dummy_file", "r");
    if (tif_read) {
        // Test checking functions on read handle
        (void)TIFFIsByteSwapped(tif_read);
        (void)TIFFIsMSB2LSB(tif_read);
        (void)TIFFIsBigEndian(tif_read);
        
        // Try to read EXIF directory from various offsets
        if (Size >= sizeof(uint64_t)) {
            uint64_t offset = 0;
            memcpy(&offset, Data, sizeof(uint64_t));
            (void)TIFFReadEXIFDirectory(tif_read, offset);
        }
        
        TIFFClose(tif_read);
    }
    
    // Test 2: Try to open file for writing
    TIFF* tif_write = TIFFOpen("./dummy_file", "w");
    if (tif_write) {
        // Set minimal required tags for a valid TIFF
        uint32_t width = 1;
        uint32_t height = 1;
        uint16_t samples_per_pixel = 1;
        uint16_t bits_per_sample = 8;
        uint16_t photometric = PHOTOMETRIC_MINISBLACK;
        uint16_t planar_config = PLANARCONFIG_CONTIG;
        uint16_t compression = COMPRESSION_NONE;
        
        TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif_write, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
        TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
        TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif_write, TIFFTAG_PLANARCONFIG, planar_config);
        TIFFSetField(tif_write, TIFFTAG_COMPRESSION, compression);
        TIFFSetField(tif_write, TIFFTAG_ROWSPERSTRIP, height);
        
        // Test checking functions on write handle
        (void)TIFFIsByteSwapped(tif_write);
        (void)TIFFIsMSB2LSB(tif_write);
        (void)TIFFIsBigEndian(tif_write);
        
        // Try to write a directory
        (void)TIFFWriteDirectory(tif_write);
        
        // Try to write a scanline
        if (Size > 0) {
            uint8_t scanline[1] = {0};
            if (Size >= 1) scanline[0] = Data[0];
            
            uint32_t row = 0;
            uint16_t sample = 0;
            
            if (Size >= sizeof(uint32_t) + 1) {
                memcpy(&row, Data + 1, sizeof(uint32_t));
            }
            if (Size >= sizeof(uint32_t) + sizeof(uint16_t) + 1) {
                memcpy(&sample, Data + 1 + sizeof(uint32_t), sizeof(uint16_t));
            }
            
            (void)TIFFWriteScanline(tif_write, scanline, row, sample);
        }
        
        TIFFClose(tif_write);
    }
    
    // Clean up
    unlink("./dummy_file");
    
    return 0;
}