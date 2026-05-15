// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
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
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0; // Ensure minimal input size
    
    // Write fuzzer data to dummy file for TIFFOpen
    write_dummy_file(Data, Size);
    
    // First sequence: Write mode operations
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) return 0;
    
    // Set various fields using fuzzer data to influence values
    uint32_t width = (Data[0] << 8) | Data[1];
    uint32_t height = (Data[2] << 8) | Data[3];
    uint16_t bits_per_sample = Data[4] % 16 + 1;
    uint16_t samples_per_pixel = Data[5] % 4 + 1;
    uint16_t compression = Data[6] % 6 + 1;
    uint16_t photometric = Data[7] % 6;
    uint16_t planar_config = Data[8] % 2 + 1;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    
    // Calculate strip size and write raw strip
    uint32_t strip_size = width * height * samples_per_pixel * ((bits_per_sample + 7) / 8);
    if (strip_size > 0 && strip_size < 1000000) { // Reasonable size limit
        void *strip_data = malloc(strip_size);
        if (strip_data) {
            size_t data_to_copy = Size > strip_size ? strip_size : Size;
            memcpy(strip_data, Data, data_to_copy);
            TIFFWriteRawStrip(tif, 0, strip_data, strip_size);
            free(strip_data);
        }
    }
    
    TIFFClose(tif);
    
    // Second sequence: Read mode operations
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) return 0;
    
    // Get field values
    uint32_t retrieved_width = 0;
    uint32_t retrieved_height = 0;
    uint16_t retrieved_bps = 0;
    uint16_t retrieved_spp = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &retrieved_width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &retrieved_height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &retrieved_bps);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &retrieved_spp);
    
    // Try to read a scanline if dimensions are reasonable
    if (retrieved_width > 0 && retrieved_width < 10000 && 
        retrieved_height > 0 && retrieved_height < 10000 &&
        retrieved_bps > 0 && retrieved_bps <= 32 &&
        retrieved_spp > 0 && retrieved_spp <= 4) {
        
        size_t scanline_size = retrieved_width * retrieved_spp * ((retrieved_bps + 7) / 8);
        if (scanline_size > 0 && scanline_size < 100000) {
            void *buf = malloc(scanline_size);
            if (buf) {
                TIFFReadScanline(tif, buf, 0, 0);
                free(buf);
            }
        }
    }
    
    TIFFClose(tif);
    
    return 0;
}