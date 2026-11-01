// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {
    // Suppress error messages during fuzzing
}

static void dummy_warning_handler(const char *module, const char *fmt, va_list ap) {
    // Suppress warning messages during fuzzing
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write fuzzing data to a temporary file
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    if (write(fd, Data, Size) != (ssize_t)Size) {
        close(fd);
        return 0;
    }
    close(fd);

    // Open the file using libtiff
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    // Set custom error and warning handlers to suppress output
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandler(dummy_warning_handler);

    // Fuzz TIFFReadDirectory
    int dir_result = TIFFReadDirectory(tif);
    (void)dir_result; // Use result to avoid unused variable warning

    // Fuzz TIFFIsUpSampled
    int upsampled = TIFFIsUpSampled(tif);
    (void)upsampled;

    // Fuzz TIFFIsMSB2LSB
    int msb2lsb = TIFFIsMSB2LSB(tif);
    (void)msb2lsb;

    // Prepare buffer for RGBA image reading
    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height) &&
        width > 0 && height > 0 && width < 1024 && height < 1024) {
        
        uint32_t *buf = (uint32_t *)malloc(width * height * sizeof(uint32_t));
        if (buf) {
            // Fuzz TIFFReadRGBAImage
            int rgba_result = TIFFReadRGBAImage(tif, width, height, buf, 0);
            (void)rgba_result;

            // Fuzz TIFFReadRGBAImageOriented with different orientations
            int orientations[] = {ORIENTATION_BOTLEFT, ORIENTATION_TOPLEFT, ORIENTATION_BOTRIGHT, ORIENTATION_TOPRIGHT};
            for (int i = 0; i < 4; i++) {
                int oriented_result = TIFFReadRGBAImageOriented(tif, width, height, buf, orientations[i], 0);
                (void)oriented_result;
            }

            // Fuzz TIFFReadRGBAStripExt if the file is striped
            if (!TIFFIsTiled(tif)) {
                uint32_t rows_per_strip;
                if (TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip) && rows_per_strip > 0) {
                    // Try reading the first strip
                    int strip_result = TIFFReadRGBAStripExt(tif, 0, buf, 0);
                    (void)strip_result;
                }
            }

            free(buf);
        }
    }

    // Clean up
    TIFFClose(tif);
    unlink(filename);

    return 0;
}