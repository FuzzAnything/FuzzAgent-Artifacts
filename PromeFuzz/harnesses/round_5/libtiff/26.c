// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (tif != NULL) {
        uint32_t tag_value = 0;
        uint32_t width = 0;
        uint32_t height = 0;
        uint16_t samples_per_pixel = 0;
        uint16_t bits_per_sample = 0;
        uint16_t photometric = 0;
        uint16_t planar_config = 0;
        uint16_t compression = 0;
        
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
        TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
        TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar_config);
        TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);
        
        size_t cmp_size = Size > 1024 ? 1024 : Size;
        tmsize_t tiff_cmp_size = (tmsize_t)cmp_size;
        
        if (tiff_cmp_size > 0) {
            uint8_t* buf1 = (uint8_t*)malloc(tiff_cmp_size);
            uint8_t* buf2 = (uint8_t*)malloc(tiff_cmp_size);
            
            if (buf1 && buf2) {
                size_t copy_size = cmp_size;
                if (copy_size > Size) copy_size = Size;
                
                memcpy(buf1, Data, copy_size);
                memcpy(buf2, Data, copy_size);
                
                _TIFFmemcmp(buf1, buf2, tiff_cmp_size);
                
                if (copy_size > 1) {
                    _TIFFmemcmp(buf1, buf2 + 1, tiff_cmp_size - 1);
                }
                
                memset(buf2, 0, tiff_cmp_size);
                _TIFFmemcmp(buf1, buf2, tiff_cmp_size);
            }
            
            free(buf1);
            free(buf2);
        }
        
        TIFFClose(tif);
    }
    
    cleanup_dummy_file();
    return 0;
}