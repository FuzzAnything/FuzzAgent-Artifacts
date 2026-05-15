// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silence libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to silent
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Create a dummy file for TIFF operations
    const char* filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    // Write fuzzer data to the file
    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        unlink(filename);
        return 0;
    }
    close(fd);
    
    // Try to open the file in different modes
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        // Try write mode if read fails
        tif = TIFFOpen(filename, "w");
        if (!tif) {
            unlink(filename);
            return 0;
        }
        
        // For write mode, set up some basic TIFF fields
        // to make subsequent operations more meaningful
        if (Size > 0) {
            uint16_t width = (Data[0] << 8) | (Size > 1 ? Data[1] : 0);
            uint16_t height = (Size > 2 ? Data[2] : 1);
            uint16_t samples = (Size > 3 ? Data[3] : 1);
            uint16_t bits = (Size > 4 ? Data[4] : 8);
            
            width = width ? width : 1;
            height = height ? height : 1;
            samples = samples ? samples : 1;
            bits = bits ? bits : 8;
            
            // Set only essential required fields
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            
            // Set compression to NONE to avoid complex code paths
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            
            // Test TIFFWriteDirectory - only once per image
            TIFFWriteDirectory(tif);
            
            // Test TIFFWriteScanline with some dummy data
            if (Size > 5) {
                uint32_t row = (Size > 9 ? 
                    (Data[5] << 24) | (Data[6] << 16) | (Data[7] << 8) | Data[8] : 0);
                uint16_t sample = (Size > 10 ? Data[9] : 0);
                
                // Allocate buffer for scanline
                tmsize_t scanline_size = TIFFScanlineSize(tif);
                if (scanline_size > 0) {
                    uint8_t* buf = (uint8_t*)malloc(scanline_size);
                    if (buf) {
                        // Fill buffer with fuzzer data if available
                        size_t copy_size = Size - 10;
                        if (copy_size > scanline_size) copy_size = scanline_size;
                        if (copy_size > 0) {
                            memcpy(buf, Data + 10, copy_size);
                        }
                        TIFFWriteScanline(tif, buf, row, sample);
                        free(buf);
                    }
                }
            }
        }
        
        // Close the TIFF file after writing
        TIFFClose(tif);
        
        // Reopen for reading to test query functions
        tif = TIFFOpen(filename, "r");
    }
    
    // Test the query functions if we have a valid TIFF handle
    if (tif) {
        // Test byte order functions
        TIFFIsByteSwapped(tif);
        TIFFIsMSB2LSB(tif);
        TIFFIsBigEndian(tif);
        
        // Test TIFFReadEXIFDirectory with offset from fuzzer data
        if (Size > 0) {
            // Use various parts of fuzzer data for different offset attempts
            toff_t offsets[3] = {0};
            
            // Try small offset
            offsets[0] = Size > 0 ? Data[0] : 0;
            
            // Try medium offset (using first 4 bytes if available)
            if (Size >= 4) {
                offsets[1] = (Data[0] << 24) | (Data[1] << 16) | 
                            (Data[2] << 8) | Data[3];
            }
            
            // Try larger offset (using more bytes if available)
            if (Size >= 8) {
                offsets[2] = ((uint64_t)Data[0] << 56) | 
                            ((uint64_t)Data[1] << 48) |
                            ((uint64_t)Data[2] << 40) |
                            ((uint64_t)Data[3] << 32) |
                            ((uint64_t)Data[4] << 24) |
                            ((uint64_t)Data[5] << 16) |
                            ((uint64_t)Data[6] << 8) |
                            Data[7];
            }
            
            // Try reading EXIF directory at different offsets
            for (int i = 0; i < 3; i++) {
                TIFFReadEXIFDirectory(tif, offsets[i]);
            }
        }
        
        TIFFClose(tif);
    }
    
    // Clean up
    unlink(filename);
    
    return 0;
}