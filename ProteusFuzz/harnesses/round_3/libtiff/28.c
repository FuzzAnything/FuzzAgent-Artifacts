// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFVGetFieldDefaulted at tif_aux.c:217:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>
#include <sys/stat.h>
#include <unistd.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid TIFF header if input is empty
            // Byte order (II), Magic (42), Offset to first IFD (8)
            uint8_t header[8] = {0x49, 0x49, 0x2a, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

// Helper function to handle va_list operations for TIFFVSetField
// Must be variadic to use va_start correctly
static void test_vsetfield(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);
}

// Helper function to handle va_list operations for TIFFVGetFieldDefaulted
// Must be variadic to use va_start correctly
static void test_vgetfielddefaulted(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVGetFieldDefaulted(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Open the dummy file for reading
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // If open fails, try opening for writing to test write-side APIs
        setup_dummy_file(Data, Size);
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            return 0;
        }
    }

    // Ensure we have a valid directory to work with
    if (TIFFReadDirectory(tif) == 0) {
        // If read fails, try to write a minimal directory if in write mode
        // or just proceed to test APIs that might handle invalid states gracefully
    }

    // Fuzzing parameters derived from input data
    uint32_t tag = 0;
    if (Size > 0) tag |= ((uint32_t)Data[0]);
    if (Size > 1) tag |= ((uint32_t)Data[1] << 8);
    if (Size > 2) tag |= ((uint32_t)Data[2] << 16);
    if (Size > 3) tag |= ((uint32_t)Data[3] << 24);

    // Variables for GetField calls
    uint32_t val32;
    uint16_t val16;
    uint8_t val8;
    int32_t val32s;
    int16_t val16s;
    double val64;
    float val32f;

    // 1. Test TIFFVSetField via helper
    test_vsetfield(tif, tag);

    // 2. Test TIFFGetField
    TIFFGetField(tif, tag, &val32);
    TIFFGetField(tif, tag, &val16);
    TIFFGetField(tif, tag, &val8);
    TIFFGetField(tif, tag, &val32s);
    TIFFGetField(tif, tag, &val16s);
    TIFFGetField(tif, tag, &val64);
    TIFFGetField(tif, tag, &val32f);

    // 3. Test TIFFGetFieldDefaulted
    TIFFGetFieldDefaulted(tif, tag, &val32);
    TIFFGetFieldDefaulted(tif, tag, &val16);
    TIFFGetFieldDefaulted(tif, tag, &val8);
    TIFFGetFieldDefaulted(tif, tag, &val32s);
    TIFFGetFieldDefaulted(tif, tag, &val16s);
    TIFFGetFieldDefaulted(tif, tag, &val64);
    TIFFGetFieldDefaulted(tif, tag, &val32f);

    // 4. Test TIFFGetTagListCount
    int count = TIFFGetTagListCount(tif);
    (void)count; // Suppress unused warning

    // 5. Test TIFFVGetFieldDefaulted via helper
    test_vgetfielddefaulted(tif, tag);

    // 6. Test TIFFUnsetField
    TIFFUnsetField(tif, tag);

    // Cleanup
    TIFFClose(tif);
    
    // Clean up the dummy file
    unlink("./dummy_file");

    return 0;
}