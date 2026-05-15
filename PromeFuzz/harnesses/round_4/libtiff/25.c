// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t width = 0;
        uint32_t height = 0;
        uint16_t samples_per_pixel = 0;
        uint16_t bits_per_sample = 0;
        uint16_t photometric = 0;
        uint16_t planar_config = 0;
        uint32_t rows_per_strip = 0;
        uint32_t strip_byte_counts = 0;
        uint32_t strip_offsets = 0;
        
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
        TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
        TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar_config);
        TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
        TIFFGetField(tif, TIFFTAG_STRIPBYTECOUNTS, &strip_byte_counts);
        TIFFGetField(tif, TIFFTAG_STRIPOFFSETS, &strip_offsets);
        
        static const uint8_t pattern1[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
        static const uint8_t pattern2[8] = {0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8};
        static const uint8_t pattern3[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                            0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
        
        const uint8_t *data_ptr = Data;
        size_t data_size = Size;
        
        if (data_size >= 8) {
            _TIFFmemcmp(data_ptr, pattern1, 8);
        }
        
        if (data_size >= 8) {
            _TIFFmemcmp(data_ptr, pattern2, 8);
        }
        
        if (data_size >= 16) {
            _TIFFmemcmp(data_ptr, pattern3, 16);
        }
        
        TIFFClose(tif);
    }
    
    remove_dummy_file();
    return 0;
}