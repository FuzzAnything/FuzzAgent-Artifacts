// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings and errors during fuzzing
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

    // Write fuzzer input to a dummy file
    const char* filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    write(fd, Data, Size);
    close(fd);

    // Open the TIFF file for reading/writing
    TIFF* tif = TIFFOpen(filename, "r+");
    if (!tif) {
        // Try opening as read-only if read/write fails
        tif = TIFFOpen(filename, "r");
        if (!tif) {
            unlink(filename);
            return 0;
        }
    }

    // Test TIFFGetTagListCount
    int tag_count = TIFFGetTagListCount(tif);
    (void)tag_count; // Use result to avoid unused variable warning

    // Test TIFFRewriteDirectory if we have write access
    // Check if file was opened in write mode by trying to set a field
    uint32_t width = 0;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) || 1) {
        // Try to set some basic tags before rewriting
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        
        TIFFRewriteDirectory(tif);
    }

    // Test TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // Test custom directory functions
    // Since TIFFFieldArray is opaque, we cannot create a proper instance
    // We'll use NULL for the field array and rely on the library to handle it
    TIFFFieldArray* infoarray = NULL;

    // Test TIFFCreateCustomDirectory
    TIFFCreateCustomDirectory(tif, infoarray);

    // Test TIFFWriteCustomDirectory
    uint64_t offset = 0;
    TIFFWriteCustomDirectory(tif, &offset);

    // Test TIFFReadCustomDirectory
    // Try to read from a reasonable offset if file has content
    if (Size > 8) {
        uint64_t diroff = 0;
        if (Size >= sizeof(diroff)) {
            memcpy(&diroff, Data, sizeof(diroff));
            // Ensure offset is within reasonable bounds
            if (diroff > Size) {
                diroff = Size - 1;
            }
        }
        TIFFReadCustomDirectory(tif, diroff, infoarray);
    }

    // Clean up
    TIFFClose(tif);
    unlink(filename);
    
    return 0;
}