// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t width = 100;
    uint32_t height = 100;
    uint16_t bits_per_sample = 8;
    uint16_t samples_per_pixel = 3;
    uint16_t compression = COMPRESSION_NONE;
    uint16_t photometric = PHOTOMETRIC_RGB;
    uint16_t orientation = ORIENTATION_TOPLEFT;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    
    unsigned char *scanline = (unsigned char *)malloc(width * samples_per_pixel);
    if (scanline) {
        for (uint32_t row = 0; row < height; row++) {
            size_t bytes_to_copy = width * samples_per_pixel;
            if (bytes_to_copy > Size) bytes_to_copy = Size;
            memcpy(scanline, Data, bytes_to_copy);
            TIFFWriteScanline(tif, scanline, row, 0);
        }
        free(scanline);
    }
    
    TIFFWriteDirectory(tif);
    
    TIFFCreateEXIFDirectory(tif);
    
    double exposure_time = 1.0/60.0;
    TIFFSetField(tif, EXIFTAG_EXPOSURETIME, exposure_time);
    
    uint64_t offset = 0;
    TIFFWriteCustomDirectory(tif, &offset);
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}