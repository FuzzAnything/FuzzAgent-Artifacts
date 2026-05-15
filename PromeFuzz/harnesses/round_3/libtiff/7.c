// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
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

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }

    int is_tiled = TIFFIsTiled(tif);
    
    tmsize_t alloc_size1 = (tmsize_t)(Data[0] + 1);
    void* mem1 = _TIFFmalloc(alloc_size1);
    
    tmsize_t alloc_size2 = (tmsize_t)(Data[Size > 1 ? 1 : 0] + 1);
    void* mem2 = _TIFFmalloc(alloc_size2);
    
    TIFFRGBAImage img;
    char emsg[1024] = {0};
    int begin_result = TIFFRGBAImageBegin(&img, tif, 0, emsg);
    
    if (begin_result == 1) {
        uint32_t width = img.width;
        uint32_t height = img.height;
        
        if (width > 0 && height > 0) {
            uint32_t* raster = (uint32_t*)_TIFFmalloc(width * height * sizeof(uint32_t));
            if (raster) {
                uint32_t rows_to_get = height;
                if (Size > 2) {
                    rows_to_get = (uint32_t)Data[2] % (height + 1);
                }
                TIFFRGBAImageGet(&img, raster, width, rows_to_get);
                _TIFFfree(raster);
            }
        }
    }
    
    TIFFRGBAImageEnd(&img);
    
    if (mem1) _TIFFfree(mem1);
    if (mem2) _TIFFfree(mem2);
    
    TIFFClose(tif);
    remove_dummy_file();
    
    return 0;
}