// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
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
#include <sys/stat.h>

#include "libtiff/tiffio.h"

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

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    if (width > 0 && height > 0) {
        uint32_t* raster = (uint32_t*)_TIFFmalloc(width * height * sizeof(uint32_t));
        if (raster) {
            TIFFReadRGBAImage(tif, width, height, raster, 0);
            _TIFFfree(raster);
        }
    }
    
    uint32_t strip_size = TIFFStripSize(tif);
    if (strip_size > 0) {
        uint32_t* strip_buf = (uint32_t*)_TIFFmalloc(strip_size);
        if (strip_buf) {
            uint32_t row = 0;
            TIFFReadRGBAStrip(tif, row, strip_buf);
            _TIFFfree(strip_buf);
        }
    }
    
    if (TIFFIsTiled(tif)) {
        uint32_t tile_width = 0, tile_height = 0;
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
        
        if (tile_width > 0 && tile_height > 0) {
            uint32_t tile_size = TIFFTileSize(tif);
            if (tile_size > 0) {
                uint32_t* tile_buf = (uint32_t*)_TIFFmalloc(tile_size);
                if (tile_buf) {
                    uint32_t col = 0, row = 0;
                    TIFFReadRGBATileExt(tif, col, row, tile_buf, 0);
                    _TIFFfree(tile_buf);
                }
            }
        }
    }
    
    char emsg[1024];
    TIFFRGBAImageOK(tif, emsg);
    
    if (strip_size > 0) {
        uint32_t* strip_buf_ext = (uint32_t*)_TIFFmalloc(strip_size);
        if (strip_buf_ext) {
            uint32_t row = 0;
            TIFFReadRGBAStripExt(tif, row, strip_buf_ext, 0);
            _TIFFfree(strip_buf_ext);
        }
    }
    
    if (width > 0 && height > 0) {
        uint32_t* raster_orient = (uint32_t*)_TIFFmalloc(width * height * sizeof(uint32_t));
        if (raster_orient) {
            TIFFReadRGBAImageOriented(tif, width, height, raster_orient, ORIENTATION_TOPLEFT, 0);
            TIFFReadRGBAImageOriented(tif, width, height, raster_orient, ORIENTATION_BOTLEFT, 0);
            _TIFFfree(raster_orient);
        }
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}