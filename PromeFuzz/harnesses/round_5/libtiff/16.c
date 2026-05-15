// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
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
#include <stdio.h>

#include "tiffio.h"

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    
    uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
    fwrite(header, 1, sizeof(header), fp);
    
    size_t write_size = Size > 1024 ? 1024 : Size;
    fwrite(Data, 1, write_size, fp);
    fclose(fp);
    
    TIFF* tif = TIFFOpen(filename, "w");
    if (!tif) {
        remove(filename);
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
    
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    if (scanline_size > 0) {
        uint8_t* buf = (uint8_t*)malloc(scanline_size);
        if (buf) {
            for (tmsize_t i = 0; i < scanline_size; i++) {
                buf[i] = Data[i % Size];
            }
            
            TIFFWriteScanline(tif, buf, 0, 0);
            free(buf);
        }
    }
    
    TIFFWriteDirectory(tif);
    
    if (TIFFCreateEXIFDirectory(tif)) {
        const char* exif_data = "EXIF Test Data";
        TIFFSetField(tif, TIFFTAG_IMAGEDESCRIPTION, exif_data);
        
        uint64_t offset = 0;
        TIFFWriteCustomDirectory(tif, &offset);
    }
    
    TIFFClose(tif);
    remove(filename);
    
    return 0;
}