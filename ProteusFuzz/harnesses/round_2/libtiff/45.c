// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    if (size > 0 && data != NULL) {
        size_t written = fwrite(data, 1, size, f);
        if (written != size) {
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    return 1;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    if (!write_dummy_file(Data, Size)) {
        return 0;
    }

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    uint32_t *rgba_buffer = NULL;
    size_t buffer_size = 0;

    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) && 
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        if (width > 0 && height > 0 && width <= 4096 && height <= 4096) {
            buffer_size = width * height * sizeof(uint32_t);
            if (buffer_size > 0 && buffer_size <= 100 * 1024 * 1024) {
                rgba_buffer = (uint32_t *)malloc(buffer_size);
            }
        }
    }

    if (!rgba_buffer) {
        rgba_buffer = (uint32_t *)malloc(4096 * sizeof(uint32_t));
        if (!rgba_buffer) {
            TIFFClose(tif);
            return 0;
        }
    }

    int stop_on_error = 0;
    int result = 0;
    char errbuf[1024];

    // 1. TIFFReadRGBAStripExt
    uint32_t row = 0;
    result = TIFFReadRGBAStripExt(tif, row, rgba_buffer, stop_on_error);
    (void)result;

    // 2. TIFFReadRGBAImage
    if (rgba_buffer && width > 0 && height > 0) {
        result = TIFFReadRGBAImage(tif, width, height, rgba_buffer, stop_on_error);
        (void)result;
    }

    // 3. TIFFRGBAImageGet
    TIFFRGBAImage img;
    // TIFFRGBAImageOK signature: int TIFFRGBAImageOK(TIFF *, char[1024])
    if (TIFFRGBAImageOK(tif, errbuf)) {
        // TIFFRGBAImageBegin signature: int TIFFRGBAImageBegin(TIFFRGBAImage *, TIFF *, int, char[1024])
        // Note: The int parameter is typically 'stop_on_error'
        if (TIFFRGBAImageBegin(&img, tif, stop_on_error, errbuf)) {
            if (rgba_buffer && width > 0 && height > 0) {
                result = TIFFRGBAImageGet(&img, rgba_buffer, width, height);
                (void)result;
            }
            TIFFRGBAImageEnd(&img);
        }
    }

    // 4. TIFFReadRGBAStrip
    result = TIFFReadRGBAStrip(tif, row, rgba_buffer);
    (void)result;

    // 5. TIFFReadRGBATileExt
    uint32_t xtile = 0, ytile = 0;
    result = TIFFReadRGBATileExt(tif, xtile, ytile, rgba_buffer, stop_on_error);
    (void)result;

    // 6. TIFFReadRGBAImageOriented
    if (rgba_buffer && width > 0 && height > 0) {
        int orientation = ORIENTATION_BOTLEFT;
        result = TIFFReadRGBAImageOriented(tif, width, height, rgba_buffer, orientation, stop_on_error);
        (void)result;
    }

    // Cleanup
    if (rgba_buffer) {
        free(rgba_buffer);
    }
    TIFFClose(tif);

    return 0;
}