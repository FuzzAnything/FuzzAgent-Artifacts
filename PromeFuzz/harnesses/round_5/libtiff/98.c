// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdarg.h>

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress error messages during fuzzing
}

static void dummy_warning_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress warning messages during fuzzing
}

static void test_vsetfield(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandler(dummy_warning_handler);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Try opening for writing if reading fails
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            remove("./dummy_file");
            return 0;
        }
    }
    
    // Extract parameters from fuzzer input
    uint32_t tag = 0;
    TIFFDataType dt = TIFF_NOTYPE;
    long flags = 0;
    
    if (Size >= sizeof(uint32_t)) {
        memcpy(&tag, Data, sizeof(uint32_t));
        if (Size >= sizeof(uint32_t) + 1) {
            dt = (TIFFDataType)(Data[sizeof(uint32_t)] % (TIFF_IFD8 + 1));
        }
        if (Size >= sizeof(uint32_t) + sizeof(long)) {
            memcpy(&flags, Data + sizeof(uint32_t), sizeof(long));
        }
    }
    
    // Test TIFFUnsetField
    TIFFUnsetField(tif, tag);
    
    // Test TIFFRewriteDirectory
    TIFFRewriteDirectory(tif);
    
    // Test TIFFFindField
    const TIFFField *field = TIFFFindField(tif, tag, dt);
    (void)field; // Use result to avoid unused variable warning
    
    // Test TIFFPrintDirectory
    FILE *out = fopen("/dev/null", "w");
    if (out) {
        TIFFPrintDirectory(tif, out, flags);
        fclose(out);
    }
    
    // Test TIFFVSetField with different argument types
    test_vsetfield(tif, tag, 256);
    test_vsetfield(tif, tag, "test");
    test_vsetfield(tif, tag, 1.0f);
    
    // Test TIFFCreateCustomDirectory
    // We can't create valid TIFFFieldArray structures since they're opaque,
    // but we can try calling with NULL to test error handling
    TIFFCreateCustomDirectory(tif, NULL);
    
    // Cleanup
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}