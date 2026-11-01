// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFVGetFieldDefaulted at tif_aux.c:217:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        } else {
            // Write a minimal valid TIFF header if input is empty to avoid immediate crash
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            write(fd, header, sizeof(header));
        }
        close(fd);
    }
}

// Helper function to wrap va_list usage. Must be variadic to use va_start.
static void test_va_list_functions(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    
    // Test TIFFVGetField
    TIFFVGetField(tif, tag, ap);
    va_end(ap);

    // Test TIFFVSetField
    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);

    // Test TIFFVGetFieldDefaulted
    va_start(ap, tag);
    TIFFVGetFieldDefaulted(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (tif == NULL) {
        // Try opening in write mode if read+ fails
        tif = TIFFOpen("./dummy_file", "w");
        if (tif == NULL) {
            return 0;
        }
        // Write a minimal directory to make the file valid
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFWriteDirectory(tif);
        TIFFClose(tif);
        
        // Reopen for reading/writing
        tif = TIFFOpen("./dummy_file", "r+");
        if (tif == NULL) {
            return 0;
        }
    }

    // Ensure we are on a valid directory
    if (!TIFFReadDirectory(tif)) {
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFWriteDirectory(tif);
    }

    uint32_t tag = 0;
    if (Size >= 4) {
        tag = *(uint32_t *)Data;
    } else if (Size >= 2) {
        tag = *(uint16_t *)Data;
    } else if (Size >= 1) {
        tag = Data[0];
    }

    // Test TIFFGetField
    uint32_t val32;
    uint16_t val16;
    uint8_t val8;
    float valf;
    double vald;
    TIFFGetField(tif, tag, &val32);
    TIFFGetField(tif, tag, &val16);
    TIFFGetField(tif, tag, &val8);
    TIFFGetField(tif, tag, &valf);
    TIFFGetField(tif, tag, &vald);

    // Test TIFFGetFieldDefaulted
    TIFFGetFieldDefaulted(tif, tag, &val32);
    TIFFGetFieldDefaulted(tif, tag, &val16);
    TIFFGetFieldDefaulted(tif, tag, &val8);
    TIFFGetFieldDefaulted(tif, tag, &valf);
    TIFFGetFieldDefaulted(tif, tag, &vald);

    // Test TIFFUnsetField
    TIFFUnsetField(tif, tag);

    // Test va_list functions via helper
    test_va_list_functions(tif, tag);

    TIFFClose(tif);
    return 0;
}