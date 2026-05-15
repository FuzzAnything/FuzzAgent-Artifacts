// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
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

#include "tiffio.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    uint32_t width = 100;
    uint32_t height = 100;
    uint16_t samples_per_pixel = 3;
    uint16_t bits_per_sample = 8;
    uint16_t photometric = PHOTOMETRIC_RGB;
    uint16_t compression = COMPRESSION_NONE;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    uint32_t rows_per_strip = 10;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    
    unsigned char *buf = (unsigned char *)malloc(width * samples_per_pixel);
    if (buf) {
        size_t buf_size = width * samples_per_pixel;
        size_t copy_size = Size > buf_size ? buf_size : Size;
        memcpy(buf, Data, copy_size);
        if (copy_size < buf_size) {
            memset(buf + copy_size, 0, buf_size - copy_size);
        }
        
        for (uint32_t row = 0; row < height; row++) {
            TIFFWriteScanline(tif, buf, row, 0);
        }
        free(buf);
    }
    
    TIFFWriteDirectory(tif);
    
    if (TIFFCreateEXIFDirectory(tif)) {
        const char *exif_data = "EXIF Test Data";
        TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, exif_data);
        
        uint64_t offset = 0;
        TIFFWriteCustomDirectory(tif, &offset);
    }
    
    TIFFClose(tif);
    remove_dummy_file();
    
    return 0;
}