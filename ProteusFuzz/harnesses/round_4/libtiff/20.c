// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid-ish header if input is empty to avoid immediate open failure
            // Minimal TIFF header (little endian)
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Ensure we have a valid directory to work with
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    uint32_t width, height;
    uint32_t *raster = NULL;
    uint16_t photometric;
    uint16_t orientation;
    uint32_t rows_per_strip;

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
    TIFFGetField(tif, TIFFTAG_ORIENTATION, &orientation);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);

    // Allocate buffer for RGBA data
    if (width > 0 && height > 0 && width <= 4096 && height <= 4096) {
        raster = (uint32_t *)malloc(width * height * sizeof(uint32_t));
        if (raster) {
            memset(raster, 0, width * height * sizeof(uint32_t));
        }
    }

    // 1. Test TIFFReadRGBAImage
    if (raster && width > 0 && height > 0) {
        TIFFReadRGBAImage(tif, width, height, raster, 0);
        TIFFReadRGBAImage(tif, width, height, raster, 1);
    }

    // 2. Test TIFFReadRGBAImageOriented
    if (raster && width > 0 && height > 0) {
        TIFFReadRGBAImageOriented(tif, width, height, raster, ORIENTATION_BOTLEFT, 0);
        TIFFReadRGBAImageOriented(tif, width, height, raster, ORIENTATION_TOPLEFT, 0);
        TIFFReadRGBAImageOriented(tif, width, height, raster, ORIENTATION_BOTRIGHT, 0);
        TIFFReadRGBAImageOriented(tif, width, height, raster, ORIENTATION_TOPRIGHT, 0);
        // Test with invalid orientation to trigger error paths
        TIFFReadRGBAImageOriented(tif, width, height, raster, 999, 0);
    }

    // 3. Test TIFFRGBAImageGet
    {
        TIFFRGBAImage img;
        char emsg[1024];
        if (TIFFRGBAImageOK(tif, &img)) {
            // TIFFRGBAImageBegin signature: (TIFFRGBAImage *, TIFF *, int, char[1024])
            // The 'int' is stop_on_error
            if (TIFFRGBAImageBegin(&img, tif, 0, emsg)) {
                TIFFRGBAImageGet(&img, raster, width, height);
                TIFFRGBAImageEnd(&img);
            }
            // Try with stop_on_error = 1
            if (TIFFRGBAImageBegin(&img, tif, 1, emsg)) {
                TIFFRGBAImageGet(&img, raster, width, height);
                TIFFRGBAImageEnd(&img);
            }
        }
    }

    // 4. Test TIFFReadRGBAStrip and TIFFReadRGBAStripExt
    if (raster && width > 0 && height > 0 && rows_per_strip > 0) {
        // Try reading strip 0
        TIFFReadRGBAStrip(tif, 0, raster);
        TIFFReadRGBAStripExt(tif, 0, raster, 0);
        TIFFReadRGBAStripExt(tif, 0, raster, 1);
        
        // Try reading a potentially out-of-bounds strip
        TIFFReadRGBAStrip(tif, 1000, raster);
        TIFFReadRGBAStripExt(tif, 1000, raster, 0);
    }

    // 5. Test TIFFReadRGBATileExt
    {
        uint32_t tw, tl;
        if (TIFFIsTiled(tif)) {
            TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tw);
            TIFFGetField(tif, TIFFTAG_TILELENGTH, &tl);
            
            if (tw > 0 && tl > 0 && tw <= 4096 && tl <= 4096) {
                uint32_t *tile_buf = (uint32_t *)malloc(tw * tl * sizeof(uint32_t));
                if (tile_buf) {
                    memset(tile_buf, 0, tw * tl * sizeof(uint32_t));
                    // Read tile 0
                    TIFFReadRGBATileExt(tif, 0, 0, tile_buf, 0);
                    TIFFReadRGBATileExt(tif, 0, 0, tile_buf, 1);
                    // Read invalid tile
                    TIFFReadRGBATileExt(tif, 1000, 1000, tile_buf, 0);
                    free(tile_buf);
                }
            }
        }
    }

    if (raster) {
        free(raster);
    }

    TIFFClose(tif);
    return 0;
}