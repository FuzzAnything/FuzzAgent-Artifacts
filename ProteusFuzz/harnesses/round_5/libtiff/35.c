// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file for the fuzzer input
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        return 0;
    }
    if (fwrite(Data, 1, Size, f) != Size) {
        fclose(f);
        return 0;
    }
    fclose(f);

    // Open the TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Set custom error handlers. 
    // Note: In this version of libtiff, TIFFSetErrorHandlerExt takes only the handler function.
    // The user data is typically set via TIFFSetClientInfo or similar, but for simple fuzzing
    // we just set the handler.
    TIFFSetErrorHandlerExt((TIFFErrorHandlerExt)dummy_error_handler);
    TIFFSetWarningHandlerExt((TIFFErrorHandlerExt)dummy_warn_handler);

    // Get image dimensions to allocate buffers
    uint32_t width = 0, height = 0;
    uint16_t tile_width = 0, tile_length = 0;
    uint32_t strip_count = 0;

    // Use TIFFTAG_STRIPCOUNT if available, otherwise fallback to TIFFTAG_ROWSPERSTRIP logic if needed
    // But first, try to get basic dimensions.
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        
        // Limit dimensions to prevent OOM
        if (width == 0 || height == 0 || width > 10000 || height > 10000) {
            TIFFClose(tif);
            return 0;
        }

        // Allocate buffer for RGBA image
        size_t buf_size = (size_t)width * (size_t)height * sizeof(uint32_t);
        // Limit allocation size to prevent OOM on malformed inputs claiming huge sizes
        if (buf_size > 10 * 1024 * 1024) {
            buf_size = 10 * 1024 * 1024;
        }
        uint32_t *raster = (uint32_t *)malloc(buf_size);
        if (!raster) {
            TIFFClose(tif);
            return 0;
        }

        // Fuzz TIFFReadRGBAImage
        TIFFReadRGBAImage(tif, width, height, raster, 0);

        // Fuzz TIFFReadRGBAImageOriented with different orientations
        int orientations[] = {ORIENTATION_TOPLEFT, ORIENTATION_TOPRIGHT, 
                              ORIENTATION_BOTLEFT, ORIENTATION_BOTRIGHT,
                              ORIENTATION_LEFTTOP, ORIENTATION_RIGHTTOP,
                              ORIENTATION_LEFTBOT, ORIENTATION_RIGHTBOT};
        for (int i = 0; i < 8; i++) {
            TIFFReadRGBAImageOriented(tif, width, height, raster, orientations[i], 0);
        }

        // Fuzz TIFFRGBAImageGet (requires initialization)
        TIFFRGBAImage img;
        char errbuf[1024];
        
        // TIFFRGBAImageOK signature: int TIFFRGBAImageOK(TIFF *, char[1024])
        if (TIFFRGBAImageOK(tif, errbuf)) {
            // TIFFRGBAImageBegin signature: int TIFFRGBAImageBegin(TIFFRGBAImage *, TIFF *, int, char[1024])
            // Parameters: img, tif, stop_on_error (0), errbuf
            if (TIFFRGBAImageBegin(&img, tif, 0, errbuf)) {
                // Try to get data
                TIFFRGBAImageGet(&img, raster, width, height);
                TIFFRGBAImageEnd(&img);
            }
        }

        // Fuzz Strip-based reading
        // Try to get strip count. If TIFFTAG_STRIPCOUNT is not defined in this version, 
        // we can try to get it via TIFFGetField with the constant value if known, 
        // or just try reading strip 0.
        // Standard TIFF tag for strip count is 279 (0x117).
        if (TIFFGetField(tif, 279, &strip_count)) {
            if (strip_count > 0 && strip_count < 100) {
                // Read a specific strip (index 0 is safe if count > 0)
                TIFFReadRGBAStrip(tif, 0, raster);
            }
        }

        // Fuzz Tile-based reading
        if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width) &&
            TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length)) {
            if (tile_width > 0 && tile_length > 0) {
                size_t tile_buf_size = (size_t)tile_width * (size_t)tile_length * sizeof(uint32_t);
                if (tile_buf_size > 10 * 1024 * 1024) {
                    tile_buf_size = 10 * 1024 * 1024;
                }
                uint32_t *tile_raster = (uint32_t *)malloc(tile_buf_size);
                if (tile_raster) {
                    // Read tile at (0,0)
                    TIFFReadRGBATile(tif, 0, 0, tile_raster);
                    TIFFReadRGBATileExt(tif, 0, 0, tile_raster, 0);
                    TIFFReadRGBATileExt(tif, 0, 0, tile_raster, 1);
                    free(tile_raster);
                }
            }
        }

        free(raster);
    }

    TIFFClose(tif);
    return 0;
}