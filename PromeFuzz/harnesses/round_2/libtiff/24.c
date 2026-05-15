// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t width = 0;
    uint32_t height = 0;
    uint16_t samples = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples);
    
    size_t data_size = Size > 1024 ? 1024 : Size;
    size_t half_size = data_size / 2;
    
    if (half_size > 0) {
        uint8_t* buf1 = malloc(data_size);
        uint8_t* buf2 = malloc(data_size);
        
        if (buf1 && buf2) {
            memcpy(buf1, Data, data_size);
            memcpy(buf2, Data, data_size);
            
            _TIFFmemcmp(buf1, buf2, half_size);
            _TIFFmemcmp(buf1 + half_size, buf2, half_size);
            _TIFFmemcmp(buf1, buf2 + half_size, half_size);
        }
        
        free(buf1);
        free(buf2);
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}