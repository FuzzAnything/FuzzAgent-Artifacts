// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tiffio.h>

static void dummy_error_handler(const char *module, va_list ap) {
    (void)module;
    (void)ap;
}

static void dummy_warn_handler(const char *module, va_list ap) {
    (void)module;
    (void)ap;
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write input data to a temporary file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF *tif = NULL;
    int ret = 0;

    // Attempt to open for writing (create new or overwrite)
    tif = TIFFOpen(filename, "w");
    if (tif) {
        // Set error handlers. Note: TIFFSetErrorHandlerExt takes only the handler function.
        // The user data is set via TIFFSetWarningHandlerExt/TIFFSetErrorHandlerExt in newer versions
        // or via TIFFSetClientInfo, but based on the error, this version expects 1 arg.
        // We cast to the expected type.
        TIFFSetWarningHandlerExt((TIFFErrorHandlerExt)dummy_warn_handler);
        TIFFSetErrorHandlerExt((TIFFErrorHandlerExt)dummy_error_handler);

        // Fuzz TIFFCreateDirectory
        ret = TIFFCreateDirectory(tif);
        (void)ret;

        // Set some basic fields to make the directory valid for writing
        uint32_t width = 0;
        uint32_t height = 0;
        if (Size >= 2) {
            width = Data[0];
            height = Data[1];
        }
        if (width == 0) width = 1;
        if (height == 0) height = 1;

        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height);

        // Fuzz TIFFWriteDirectory
        ret = TIFFWriteDirectory(tif);
        (void)ret;

        // Fuzz TIFFCreateDirectory again
        ret = TIFFCreateDirectory(tif);
        (void)ret;

        // Fuzz TIFFFreeDirectory
        TIFFFreeDirectory(tif);

        // Re-open for reading to test TIFFSetDirectory
        TIFFClose(tif);
        tif = TIFFOpen(filename, "r");
        if (tif) {
            tdir_t dir_idx = 0;
            if (Size >= 2) {
                dir_idx = (tdir_t)(Data[0] | (Data[1] << 8));
            }
            ret = TIFFSetDirectory(tif, dir_idx);
            (void)ret;
            
            // Try to read directory if successful
            if (ret) {
                TIFFFreeDirectory(tif);
            }
            
            TIFFClose(tif);
        }

        // Re-open for writing to test custom/exif directories
        tif = TIFFOpen(filename, "w");
        if (tif) {
            // Setup basic fields again
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height);

            // Fuzz TIFFCreateEXIFDirectory
            ret = TIFFCreateEXIFDirectory(tif);
            (void)ret;
            if (ret) {
                // Try to write some dummy EXIF data if created
                uint16_t dummy_exif = 0;
                if (Size >= 2) dummy_exif = Data[0] | (Data[1] << 8);
                TIFFSetField(tif, TIFFTAG_EXIFIFD, dummy_exif);
                TIFFWriteDirectory(tif);
            }

            // Cleanup
            TIFFFreeDirectory(tif);
            TIFFClose(tif);
        }
    }

    // Cleanup file
    unlink(filename);

    return 0;
}