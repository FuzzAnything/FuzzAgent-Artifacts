// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
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
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Open a dummy TIFF file for writing
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        return 0;
    }

    // Set various TIFF fields using the fuzzer input
    // We'll extract values from the input data for different fields
    // Use modulo operations to keep values in reasonable ranges
    
    if (Size > 0) {
        uint32_t width = (Data[0] % 1000) + 1;
        uint32_t length = (Data[Size > 1 ? 1 : 0] % 1000) + 1;
        uint16_t bits_per_sample = (Data[Size > 2 ? 2 : 0] % 16) + 1;
        uint16_t compression = Data[Size > 3 ? 3 : 0] % 7; // Various compression schemes
        uint16_t photometric = Data[Size > 4 ? 4 : 0] % 5; // Various photometric interpretations
        uint16_t samples_per_pixel = (Data[Size > 5 ? 5 : 0] % 4) + 1;
        uint32_t rows_per_strip = (Data[Size > 6 ? 6 : 0] % 100) + 1;
        uint16_t planar_config = Data[Size > 7 ? 7 : 0] % 2; // CONTIG or SEPARATE
        uint16_t orientation = Data[Size > 8 ? 8 : 0] % 8; // Various orientations
        uint16_t fill_order = Data[Size > 9 ? 9 : 0] % 2; // MSB2LSB or LSB2MSB
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, length);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
        TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);
        TIFFSetField(tif, TIFFTAG_FILLORDER, fill_order);
    } else {
        // Default values if input is empty
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 100);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
        TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
    }

    // Allocate memory for scanline buffer
    // Calculate buffer size based on image parameters
    uint32_t width = 100; // Default
    uint16_t samples_per_pixel = 3; // Default
    uint16_t bits_per_sample = 8; // Default
    
    // Try to get actual values if they were set
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    
    // Calculate scanline size in bytes
    tmsize_t scanline_size = (tmsize_t)((width * samples_per_pixel * bits_per_sample + 7) / 8);
    if (scanline_size == 0) {
        scanline_size = 300; // Default for 100x100 RGB 8-bit
    }
    
    // Limit size to prevent excessive memory allocation
    if (scanline_size > 100000) {
        scanline_size = 100000;
    }
    
    void *buf = _TIFFmalloc(scanline_size);
    if (!buf) {
        TIFFClose(tif);
        return 0;
    }
    
    // Copy data from fuzzer input to buffer
    // Use min of scanline_size and available data
    tmsize_t copy_size = scanline_size;
    if (Size < (size_t)scanline_size) {
        copy_size = (tmsize_t)Size;
    }
    
    if (copy_size > 0) {
        _TIFFmemcpy(buf, Data, copy_size);
    } else {
        // Initialize with some data if no input
        memset(buf, 0, scanline_size);
    }
    
    // Write a scanline
    TIFFWriteScanline(tif, buf, 0, 0);
    
    // Clean up
    _TIFFfree(buf);
    TIFFClose(tif);
    
    return 0;
}