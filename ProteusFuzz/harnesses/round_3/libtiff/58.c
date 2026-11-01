// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "tiffio.h"

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

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
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

    // Set custom error handlers to suppress output during fuzzing
    TIFFSetErrorHandlerExt(dummy_error_handler);
    TIFFSetWarningHandlerExt(dummy_warn_handler);

    // Open the file in read mode first to parse potential headers
    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    // Extract fuzzing parameters from input data
    uint32_t param1 = *(const uint32_t *)Data;
    uint32_t param2 = *(const uint32_t *)(Data + 4);
    uint64_t offset_val = (uint64_t)param1 | ((uint64_t)param2 << 32);
    uint32_t strile_idx = param2 % 1000;

    // 1. TIFFSetSubDirectory
    // Test with various offsets derived from input
    TIFFSetSubDirectory(tif, offset_val);
    TIFFSetSubDirectory(tif, 0);
    TIFFSetSubDirectory(tif, (uint64_t)param1);

    // 2. TIFFCurrentDirOffset
    uint64_t current_offset = TIFFCurrentDirOffset(tif);
    (void)current_offset;

    // 3. TIFFGetStrileOffset
    uint64_t strile_offset = TIFFGetStrileOffset(tif, strile_idx);
    (void)strile_offset;

    // Close read-only handle
    TIFFClose(tif);

    // Open the file in write mode to test write-related functions
    // Note: This might fail if the file is not a valid TIFF, which is expected in fuzzing
    tif = TIFFOpen(filename, "w");
    if (!tif) {
        return 0;
    }

    // Set minimal required fields for write operations to avoid immediate crashes
    // We use random values from the input to explore different states
    uint16_t width = (uint16_t)(param1 % 10000) + 1;
    uint16_t height = (uint16_t)(param2 % 10000) + 1;
    uint16_t bits = (uint16_t)(param1 % 32) + 1;
    uint16_t samples = (uint16_t)(param2 % 10) + 1;
    uint16_t photometric = (param1 % 3) + 1; // 1, 2, or 3

    if (TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width) < 0) {
        TIFFClose(tif);
        return 0;
    }
    if (TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height) < 0) {
        TIFFClose(tif);
        return 0;
    }
    if (TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits) < 0) {
        TIFFClose(tif);
        return 0;
    }
    if (TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples) < 0) {
        TIFFClose(tif);
        return 0;
    }
    if (TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric) < 0) {
        TIFFClose(tif);
        return 0;
    }

    // 4. TIFFWriteCheck
    // Test with different scanline/tile indices
    int check_result = TIFFWriteCheck(tif, 0, "fuzz_test");
    (void)check_result;

    // 5. TIFFSetWriteOffset
    // Set write offset to various values
    TIFFSetWriteOffset(tif, (toff_t)offset_val);
    TIFFSetWriteOffset(tif, 0);
    TIFFSetWriteOffset(tif, (toff_t)param1);

    // 6. TIFFRewriteDirectory
    // Attempt to rewrite the directory. This might fail if the directory is not fully set up,
    // which is a valid state to explore.
    int rewrite_result = TIFFRewriteDirectory(tif);
    (void)rewrite_result;

    // Cleanup
    TIFFClose(tif);

    return 0;
}