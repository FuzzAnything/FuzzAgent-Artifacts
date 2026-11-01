// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFVGetFieldDefaulted at tif_aux.c:217:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid-ish TIFF header if input is empty
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, sizeof(header), f);
        }
        fclose(f);
    }
}

// Helper functions to properly construct va_list for V* API calls
static int call_VSetField(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int ret = TIFFVSetField(tif, tag, ap);
    va_end(ap);
    return ret;
}

static int call_VGetField(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int ret = TIFFVGetField(tif, tag, ap);
    va_end(ap);
    return ret;
}

static int call_VGetFieldDefaulted(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int ret = TIFFVGetFieldDefaulted(tif, tag, ap);
    va_end(ap);
    return ret;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Open the file in read/write mode to allow setting fields
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try opening for writing if read+write fails
        tif = TIFFOpen("./dummy_file", "w");
    }

    if (!tif) {
        return 0;
    }

    // Extract some values from input data to use as tags and parameters
    uint32_t tag1 = (Size >= 4) ? *(uint32_t *)(Data) : 0;
    uint32_t tag2 = (Size >= 8) ? *(uint32_t *)(Data + 4) : 0;
    
    // Ensure tags are within reasonable bounds
    if (tag1 > 0xFFFF) tag1 = 0; 
    if (tag2 > 0xFFFF) tag2 = 0;

    // Variables to store results
    uint32_t val32;
    uint16_t val16;
    uint32_t fuzz_val = (Size > 0) ? Data[0] : 0;

    // 1. Test TIFFGetFieldDefaulted
    TIFFGetFieldDefaulted(tif, tag1, &val32);
    TIFFGetFieldDefaulted(tif, tag2, &val16);
    TIFFGetFieldDefaulted(tif, TIFFTAG_IMAGEWIDTH, &val32);
    TIFFGetFieldDefaulted(tif, TIFFTAG_IMAGELENGTH, &val32);

    // 2. Test TIFFVSetField via helper
    call_VSetField(tif, tag1, fuzz_val);
    call_VSetField(tif, tag2, fuzz_val);
    call_VSetField(tif, TIFFTAG_IMAGEWIDTH, fuzz_val);

    // 3. Test TIFFVGetField via helper
    call_VGetField(tif, tag1, &val32);
    call_VGetField(tif, tag2, &val16);
    call_VGetField(tif, TIFFTAG_IMAGEWIDTH, &val32);

    // 4. Test TIFFUnsetField
    TIFFUnsetField(tif, tag1);
    TIFFUnsetField(tif, tag2);
    TIFFUnsetField(tif, TIFFTAG_IMAGEWIDTH);

    // 5. Test TIFFVGetFieldDefaulted via helper
    call_VGetFieldDefaulted(tif, tag1, &val32);
    call_VGetFieldDefaulted(tif, tag2, &val16);
    call_VGetFieldDefaulted(tif, TIFFTAG_IMAGEWIDTH, &val32);

    // 6. Test TIFFGetField
    TIFFGetField(tif, tag1, &val32);
    TIFFGetField(tif, tag2, &val16);
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &val32);

    TIFFClose(tif);
    return 0;
}