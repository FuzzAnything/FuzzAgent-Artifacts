// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(void *client_data, const char *module, const char *fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(void *client_data, const char *module, const char *fmt, va_list ap) {
    (void)client_data;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    if (fwrite(Data, 1, Size, fp) != Size) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    // Open the file with libtiff
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    // Set custom error handlers to prevent crashes on malformed data
    // The extended handlers require a client_data pointer as the first argument
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    // Test TIFFReadDirectory
    TIFFReadDirectory(tif);

    // Test TIFFIsUpSampled
    TIFFIsUpSampled(tif);

    // Test TIFFIsMSB2LSB
    TIFFIsMSB2LSB(tif);

    // Prepare for RGBA reading
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t *raster = NULL;

    // Get image dimensions safely
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        if (width > 0 && height > 0 && width < 8192 && height < 8192) {
            size_t raster_size = width * height * sizeof(uint32_t);
            if (raster_size > 0 && raster_size <= 100 * 1024 * 1024) { // Limit memory usage
                raster = (uint32_t *)malloc(raster_size);
                if (raster) {
                    // Test TIFFReadRGBAImage
                    TIFFReadRGBAImage(tif, width, height, raster, 0);

                    // Test TIFFReadRGBAImageOriented with different orientations
                    int orientations[] = {ORIENTATION_TOPLEFT, ORIENTATION_TOPRIGHT,
                                         ORIENTATION_BOTLEFT, ORIENTATION_BOTRIGHT,
                                         ORIENTATION_LEFTTOP, ORIENTATION_RIGHTTOP,
                                         ORIENTATION_LEFTBOT, ORIENTATION_RIGHTBOT};
                    for (int i = 0; i < 8; i++) {
                        TIFFReadRGBAImageOriented(tif, width, height, raster, 0, orientations[i]);
                    }

                    // Test TIFFReadRGBAStripExt
                    // We try a few rows, ensuring they are valid strip boundaries if possible
                    // Since we don't know strip configuration, we try row 0 and a random row
                    TIFFReadRGBAStripExt(tif, 0, raster, 0);
                    if (height > 1) {
                        TIFFReadRGBAStripExt(tif, 1, raster, 0);
                    }
                    
                    free(raster);
                    raster = NULL;
                }
            }
        }
    }

    // Cleanup
    TIFFClose(tif);
    return 0;
}