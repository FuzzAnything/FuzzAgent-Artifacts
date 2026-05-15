// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
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

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    if (width > 0 && height > 0 && width <= 4096 && height <= 4096) {
        uint32_t *raster = (uint32_t *)_TIFFmalloc(width * height * sizeof(uint32_t));
        if (raster) {
            TIFFReadRGBAImageOriented(tif, width, height, raster, ORIENTATION_TOPLEFT, 1);
            TIFFReadRGBAImage(tif, width, height, raster, 1);
            
            uint32_t rows_per_strip = 0;
            if (TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip)) {
                uint32_t strip = 0;
                uint32_t strip_size = width * rows_per_strip;
                uint32_t *strip_buf = (uint32_t *)_TIFFmalloc(strip_size * sizeof(uint32_t));
                if (strip_buf) {
                    TIFFReadRGBAStrip(tif, strip, strip_buf);
                    TIFFReadRGBAStripExt(tif, strip, strip_buf, 0);
                    _TIFFfree(strip_buf);
                }
            }
            
            uint32_t tile_width = 0, tile_height = 0;
            if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width) &&
                TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height)) {
                uint32_t tile_size = tile_width * tile_height;
                uint32_t *tile_buf = (uint32_t *)_TIFFmalloc(tile_size * sizeof(uint32_t));
                if (tile_buf) {
                    TIFFReadRGBATileExt(tif, 0, 0, tile_buf, 0);
                    _TIFFfree(tile_buf);
                }
            }
            
            _TIFFfree(raster);
        }
    }
    
    char emsg[1024];
    TIFFRGBAImageOK(tif, emsg);
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}