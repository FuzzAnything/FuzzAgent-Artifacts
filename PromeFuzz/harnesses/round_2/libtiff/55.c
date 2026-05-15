// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
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

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t width = 100;
    uint32_t height = 100;
    uint16_t samples_per_pixel = 3;
    uint16_t bits_per_sample = 8;
    uint16_t photometric = PHOTOMETRIC_RGB;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint16_t compression = COMPRESSION_NONE;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif, 0));
    
    uint32_t strip_count = TIFFNumberOfStrips(tif);
    uint32_t rows_per_strip = 0;
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    
    for (uint32_t strip = 0; strip < strip_count && strip < 10; strip++) {
        tmsize_t raw_size = TIFFRawStripSize(tif, strip);
        tmsize_t vstrip_size = TIFFVStripSize(tif, rows_per_strip);
        
        if (raw_size > 0 && raw_size < 1000000) {
            void* raw_buffer = malloc(raw_size);
            if (raw_buffer) {
                memset(raw_buffer, 0, raw_size);
                
                TIFFWriteRawStrip(tif, strip, raw_buffer, raw_size);
                TIFFReadRawStrip(tif, strip, raw_buffer, raw_size);
                
                free(raw_buffer);
            }
        }
        
        if (vstrip_size > 0 && vstrip_size < 1000000) {
            void* encoded_buffer = malloc(vstrip_size);
            if (encoded_buffer) {
                memset(encoded_buffer, 0, vstrip_size);
                
                TIFFWriteEncodedStrip(tif, strip, encoded_buffer, vstrip_size);
                TIFFReadEncodedStrip(tif, strip, encoded_buffer, vstrip_size);
                
                free(encoded_buffer);
            }
        }
        
        for (uint32_t nrows = 1; nrows <= rows_per_strip * 2; nrows *= 2) {
            TIFFVStripSize(tif, nrows);
        }
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}