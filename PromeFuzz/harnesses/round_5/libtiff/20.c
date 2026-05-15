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
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t width = ((uint32_t)Data[0] << 24) | ((uint32_t)Data[1] << 16) | 
                     ((uint32_t)Data[2] << 8) | Data[3];
    uint32_t height = ((uint32_t)Data[4] << 24) | ((uint32_t)Data[5] << 16) | 
                      ((uint32_t)Data[6] << 8) | Data[7];
    uint16_t bits_per_sample = ((uint16_t)Data[8] << 8) | Data[9];
    uint16_t samples_per_pixel = ((uint16_t)Data[10] << 8) | Data[11];
    uint16_t photometric = ((uint16_t)Data[12] << 8) | Data[13];
    uint16_t planar_config = ((uint16_t)Data[14] << 8) | Data[15];
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    
    uint32_t strip = ((uint32_t)Data[16] << 24) | ((uint32_t)Data[17] << 16) | 
                     ((uint32_t)Data[18] << 8) | Data[19];
    tmsize_t cc = ((tmsize_t)Data[20] << 24) | ((tmsize_t)Data[21] << 16) | 
                  ((tmsize_t)Data[22] << 8) | Data[23];
    
    if (cc > Size - 24) {
        cc = Size - 24;
    }
    
    void *raw_data = malloc(cc);
    if (raw_data) {
        memcpy(raw_data, Data + 24, cc);
        TIFFWriteRawStrip(tif, strip, raw_data, cc);
        free(raw_data);
    }
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t retrieved_width;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &retrieved_width);
    
    uint32_t row = ((uint32_t)Data[24] << 24) | ((uint32_t)Data[25] << 16) | 
                   ((uint32_t)Data[26] << 8) | Data[27];
    uint16_t sample = ((uint16_t)Data[28] << 8) | Data[29];
    
    size_t scanline_size = TIFFScanlineSize(tif);
    if (scanline_size > 0 && scanline_size < 1024 * 1024) {
        void *buf = malloc(scanline_size);
        if (buf) {
            TIFFReadScanline(tif, buf, row, sample);
            free(buf);
        }
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}