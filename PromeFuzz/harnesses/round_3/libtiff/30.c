// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
// TIFFGetBitRevTable at tif_swab.c:305:22 in tiffio.h
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

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write fuzz data to dummy file
    write_dummy_file(Data, Size);
    
    // Open TIFF file for testing
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Try opening for writing instead
        tif = TIFFOpen("./dummy_file", "w");
    }
    
    if (tif) {
        // Test TIFFIsByteSwapped
        int swapped = TIFFIsByteSwapped(tif);
        (void)swapped; // Avoid unused variable warning
        
        // Test TIFFIsMSB2LSB
        int msb2lsb = TIFFIsMSB2LSB(tif);
        (void)msb2lsb;
        
        // Test TIFFIsBigEndian
        int big_endian = TIFFIsBigEndian(tif);
        (void)big_endian;
        
        // Test TIFFWriteScanline with various parameters
        uint8_t scanline_buf[1024];
        size_t buf_size = Size < sizeof(scanline_buf) ? Size : sizeof(scanline_buf);
        memcpy(scanline_buf, Data, buf_size);
        
        // Try different row and sample values
        for (uint32_t row = 0; row < 10; row++) {
            for (uint16_t sample = 0; sample < 5; sample++) {
                if (Size > row * 5 + sample) {
                    TIFFWriteScanline(tif, scanline_buf, row, sample);
                }
            }
        }
        
        TIFFClose(tif);
    }
    
    // Test TIFFReverseBits with various buffer sizes
    if (Size > 0) {
        uint8_t* reverse_buf = (uint8_t*)malloc(Size);
        if (reverse_buf) {
            memcpy(reverse_buf, Data, Size);
            TIFFReverseBits(reverse_buf, Size);
            
            // Test with smaller chunks
            if (Size > 8) {
                TIFFReverseBits(reverse_buf, 8);
            }
            if (Size > 1) {
                TIFFReverseBits(reverse_buf, 1);
            }
            
            free(reverse_buf);
        }
    }
    
    // Test TIFFGetBitRevTable with different inputs
    const unsigned char* table;
    for (int i = 0; i < 2; i++) {
        table = TIFFGetBitRevTable(i);
        (void)table; // Avoid unused variable warning
    }
    
    // Try with random boolean based on input data
    if (Size > 0) {
        int random_bool = Data[0] & 1;
        table = TIFFGetBitRevTable(random_bool);
        (void)table;
    }
    
    // Clean up
    unlink("./dummy_file");
    
    return 0;
}