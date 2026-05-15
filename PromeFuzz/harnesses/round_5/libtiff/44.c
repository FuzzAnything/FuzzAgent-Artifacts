// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "libtiff/tiffio.h"

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return written == size;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }
    
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    if (width > 0 && height > 0) {
        uint32_t *raster = NULL;
        size_t raster_size = 0;
        char emsg[1024];
        int stop_on_error = Size % 2;
        int orientation = (Size % 8);
        
        /* Test TIFFRGBAImageOK */
        TIFFRGBAImageOK(tif, emsg);
        
        /* Test TIFFReadRGBAStripExt */
        if (!TIFFIsTiled(tif)) {
            uint32_t rows_per_strip = 0;
            TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
            if (rows_per_strip == 0) rows_per_strip = height;
            
            uint32_t strip = (Size % 1024) % ((height + rows_per_strip - 1) / rows_per_strip);
            uint32_t row = strip * rows_per_strip;
            
            if (row < height) {
                raster_size = width * rows_per_strip;
                if (raster_size > 0 && raster_size < 1000000) {
                    raster = (uint32_t *)malloc(raster_size * sizeof(uint32_t));
                    if (raster) {
                        TIFFReadRGBAStripExt(tif, row, raster, stop_on_error);
                        free(raster);
                        raster = NULL;
                    }
                }
            }
        }
        
        /* Test TIFFReadRGBAStrip */
        if (!TIFFIsTiled(tif)) {
            uint32_t rows_per_strip = 0;
            TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
            if (rows_per_strip == 0) rows_per_strip = height;
            
            uint32_t strip = (Size % 512) % ((height + rows_per_strip - 1) / rows_per_strip);
            uint32_t row = strip * rows_per_strip;
            
            if (row < height) {
                raster_size = width * rows_per_strip;
                if (raster_size > 0 && raster_size < 1000000) {
                    raster = (uint32_t *)malloc(raster_size * sizeof(uint32_t));
                    if (raster) {
                        TIFFReadRGBAStrip(tif, row, raster);
                        free(raster);
                        raster = NULL;
                    }
                }
            }
        }
        
        /* Test TIFFReadRGBATileExt */
        if (TIFFIsTiled(tif)) {
            uint32_t tile_width = 0, tile_height = 0;
            TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
            TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
            
            if (tile_width > 0 && tile_height > 0) {
                uint32_t col = ((Size % 256) % ((width + tile_width - 1) / tile_width)) * tile_width;
                uint32_t row = ((Size % 128) % ((height + tile_height - 1) / tile_height)) * tile_height;
                
                if (col < width && row < height) {
                    raster_size = tile_width * tile_height;
                    if (raster_size > 0 && raster_size < 1000000) {
                        raster = (uint32_t *)malloc(raster_size * sizeof(uint32_t));
                        if (raster) {
                            TIFFReadRGBATileExt(tif, col, row, raster, stop_on_error);
                            free(raster);
                            raster = NULL;
                        }
                    }
                }
            }
        }
        
        /* Test TIFFReadRGBAImage */
        if (width < 4096 && height < 4096 && width * height < 1000000) {
            raster_size = width * height;
            raster = (uint32_t *)malloc(raster_size * sizeof(uint32_t));
            if (raster) {
                TIFFReadRGBAImage(tif, width, height, raster, stop_on_error);
                free(raster);
                raster = NULL;
            }
        }
        
        /* Test TIFFReadRGBAImageOriented */
        if (width < 2048 && height < 2048 && width * height < 500000) {
            raster_size = width * height;
            raster = (uint32_t *)malloc(raster_size * sizeof(uint32_t));
            if (raster) {
                TIFFReadRGBAImageOriented(tif, width, height, raster, orientation, stop_on_error);
                free(raster);
                raster = NULL;
            }
        }
    }
    
    TIFFClose(tif);
    remove("./dummy_file");
    return 0;
}