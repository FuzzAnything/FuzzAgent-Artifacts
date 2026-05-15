// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress error/warning messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

// Variadic helper function to call TIFFVSetField
static void call_vsetfield(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write input data to file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try read-only mode if read-write fails
        tif = TIFFOpen("./dummy_file", "r");
        if (!tif) {
            // Try creating a new file if reading fails
            tif = TIFFOpen("./dummy_file", "w");
            if (!tif) {
                return 0;
            }
        }
    }
    
    // Extract parameters from input data for more diverse testing
    uint32_t tag = 0;
    TIFFDataType dtype = TIFF_NOTYPE;
    
    if (Size >= sizeof(uint32_t)) {
        memcpy(&tag, Data, sizeof(uint32_t));
        if (Size >= sizeof(uint32_t) + 1) {
            dtype = (TIFFDataType)(Data[sizeof(uint32_t)] % (TIFF_IFD8 + 1));
        }
    }
    
    // Test TIFFUnsetField
    TIFFUnsetField(tif, tag);
    
    // Test TIFFRewriteDirectory
    TIFFRewriteDirectory(tif);
    
    // Test TIFFFindField
    const TIFFField *field = TIFFFindField(tif, tag, dtype);
    (void)field; // Use result to avoid unused variable warning
    
    // Test TIFFPrintDirectory
    FILE *out = fopen("/dev/null", "w");
    if (out) {
        long flags = 0;
        if (Size > 0) {
            flags = Data[0] % 256;
        }
        TIFFPrintDirectory(tif, out, flags);
        fclose(out);
    }
    
    // Test TIFFVSetField with a simple integer value
    if (Size >= sizeof(uint32_t) * 2) {
        int value = 0;
        memcpy(&value, Data + sizeof(uint32_t), sizeof(int));
        call_vsetfield(tif, tag, value);
    }
    
    // Test TIFFCreateCustomDirectory with NULL (to test error handling)
    // Note: The documentation says not to pass NULL, but fuzzing should test it
    TIFFCreateCustomDirectory(tif, NULL);
    
    // Cleanup
    TIFFClose(tif);
    
    return 0;
}