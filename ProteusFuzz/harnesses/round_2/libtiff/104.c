// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
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

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the TIFF file
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    // Set error handlers to prevent crashes from printf
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    // Fuzz TIFFReadDirectory
    TIFFReadDirectory(tif);

    // Fuzz TIFFIsMSB2LSB
    (void)TIFFIsMSB2LSB(tif);

    // Fuzz TIFFIsUpSampled
    (void)TIFFIsUpSampled(tif);

    // Attempt to read image dimensions for buffer allocation
    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {

        // Allocate buffer for RGBA data
        // Limit size to prevent OOM during fuzzing
        if (width > 0 && height > 0 && width <= 1024 && height <= 1024) {
            size_t buf_size = width * height * sizeof(uint32_t);
            uint32_t *buf = (uint32_t *)malloc(buf_size);
            if (buf) {
                // Fuzz TIFFReadRGBAImageOriented
                // Try different orientations using integer constants directly
                // ORIENTATION_TOPLEFT=1, ORIENTATION_TOPRIGHT=2, ORIENTATION_BOTTOMLEFT=3, ORIENTATION_BOTTOMRIGHT=4
                int orientations[] = {1, 2, 3, 4};
                for (int i = 0; i < 4; i++) {
                    (void)TIFFReadRGBAImageOriented(tif, width, height, buf, orientations[i], 0);
                }

                // Fuzz TIFFReadRGBAStrip
                // Try a few strip indices (0, 1, and a random one from input)
                uint32_t strip_indices[] = {0, 1, (Size > 0 ? Data[0] % 10 : 0)};
                for (int i = 0; i < 3; i++) {
                    (void)TIFFReadRGBAStrip(tif, strip_indices[i], buf);
                }

                // Fuzz TIFFReadRGBAStripExt
                // Try different stop_on_error values
                int stop_on_error_values[] = {0, 1};
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 2; j++) {
                        (void)TIFFReadRGBAStripExt(tif, strip_indices[i], buf, stop_on_error_values[j]);
                    }
                }

                free(buf);
            }
        }
    }

    // Cleanup
    TIFFClose(tif);

    // Remove dummy file
    unlink(filename);

    return 0;
}