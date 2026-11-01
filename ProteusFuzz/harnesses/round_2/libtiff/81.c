// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    if (TIFFReadDirectory(tif) > 0) {
        uint32_t width, height;
        if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
            TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {

            size_t bufsize = (size_t)width * (size_t)height * 4;
            // Limit allocation to prevent OOM
            if (bufsize > 0 && bufsize < (1024 * 1024 * 1024)) {
                uint32_t *rgba_buf = (uint32_t *)malloc(bufsize);
                if (rgba_buf) {
                    // Fuzz TIFFReadRGBAImageOriented
                    // Orientation constants: 1=TOPLEFT, 2=TOPRIGHT, 3=BOTRIGHT, 4=BOTLEFT
                    int orientations[] = {1, 2, 3, 4};
                    
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            TIFFReadRGBAImageOriented(tif, width, height, rgba_buf, orientations[i], orientations[j]);
                        }
                    }
                    
                    // Fuzz TIFFReadRGBAStrip
                    uint32_t strip_row = 0;
                    TIFFReadRGBAStrip(tif, strip_row, rgba_buf);
                    
                    // Fuzz TIFFReadRGBAStripExt
                    int stop_on_error = 1;
                    TIFFReadRGBAStripExt(tif, strip_row, rgba_buf, stop_on_error);
                    stop_on_error = 0;
                    TIFFReadRGBAStripExt(tif, strip_row, rgba_buf, stop_on_error);

                    free(rgba_buf);
                }
            }

            // Fuzz TIFFIsMSB2LSB
            (void)TIFFIsMSB2LSB(tif);

            // Fuzz TIFFIsUpSampled
            (void)TIFFIsUpSampled(tif);
        }
    }

    TIFFClose(tif);
    return 0;
}