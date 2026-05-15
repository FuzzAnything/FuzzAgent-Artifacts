// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFReadScanline at tif_read.c:454:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
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

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t width = 100;
    uint32_t height = 100;
    uint16_t bits_per_sample = 8;
    uint16_t samples_per_pixel = 3;
    uint32_t rows_per_strip = 10;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint16_t photometric = PHOTOMETRIC_RGB;
    uint16_t compression = COMPRESSION_NONE;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    
    size_t strip_size = width * samples_per_pixel * (bits_per_sample / 8) * rows_per_strip;
    if (strip_size > 0 && Size > 100) {
        size_t write_size = Size - 100;
        if (write_size > strip_size) write_size = strip_size;
        
        void *strip_data = malloc(strip_size);
        if (strip_data) {
            memset(strip_data, 0, strip_size);
            size_t copy_size = write_size < strip_size ? write_size : strip_size;
            memcpy(strip_data, Data + 100, copy_size);
            
            TIFFWriteRawStrip(tif, 0, strip_data, copy_size);
            free(strip_data);
        }
    }
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t read_width = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &read_width);
    
    if (read_width > 0 && read_width < 10000) {
        tmsize_t scanline_size = TIFFScanlineSize(tif);
        if (scanline_size > 0 && scanline_size < 100000) {
            void *buf = malloc(scanline_size);
            if (buf) {
                TIFFReadScanline(tif, buf, 0, 0);
                free(buf);
            }
        }
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}