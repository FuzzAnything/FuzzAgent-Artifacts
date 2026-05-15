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

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    int is_tiled = TIFFIsTiled(tif);
    
    tmsize_t alloc_size1 = Size % 1024 + 1;
    tmsize_t alloc_size2 = (Size / 256) % 1024 + 1;
    
    void* mem1 = _TIFFmalloc(alloc_size1);
    void* mem2 = _TIFFmalloc(alloc_size2);
    
    TIFFRGBAImage img;
    char emsg[1024];
    int begin_result = TIFFRGBAImageBegin(&img, tif, 1, emsg);
    
    if (begin_result) {
        uint32_t width = img.width;
        uint32_t height = img.height;
        if (width > 0 && height > 0) {
            uint32_t rows_to_read = (Size % 10) + 1;
            if (rows_to_read > height) rows_to_read = height;
            
            uint32_t* raster = (uint32_t*)_TIFFmalloc(width * rows_to_read * sizeof(uint32_t));
            if (raster) {
                TIFFRGBAImageGet(&img, raster, 0, rows_to_read);
                _TIFFfree(raster);
            }
        }
        TIFFRGBAImageEnd(&img);
    }
    
    if (mem1) _TIFFfree(mem1);
    if (mem2) _TIFFfree(mem2);
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}