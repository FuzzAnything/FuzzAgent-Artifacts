// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetErrorHandlerExtR at tif_open.c:121:6 in tiffio.h
// TIFFOpenOptionsSetWarningHandlerExtR at tif_open.c:129:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <stdarg.h>

// Correct signature for TIFFErrorHandlerExtR:
// int (*TIFFErrorHandlerExtR)(TIFF*, void*, const char*, const char*, va_list)
static int dummy_error_handler_ext(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

// Correct signature for TIFFErrorHandlerExtR (used for warnings as well in this context)
static int dummy_warn_handler_ext(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file with the fuzzing data
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // 1. TIFFOpenOptionsAlloc
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    // 2. TIFFOpenOptionsSetErrorHandlerExtR
    // Pass the TIFF pointer as NULL since we haven't opened the file yet, 
    // but the handler signature requires it. The handler will receive the actual TIFF* when called.
    TIFFOpenOptionsSetErrorHandlerExtR(opts, dummy_error_handler_ext, (void*)Data);

    // 3. TIFFOpenOptionsSetWarningHandlerExtR
    TIFFOpenOptionsSetWarningHandlerExtR(opts, dummy_warn_handler_ext, (void*)Data);

    // 4. TIFFOpenExt
    // Try opening in read mode first
    TIFF *tif = TIFFOpenExt(filename, "r", opts);
    
    // If read failed, try write mode (to test different code paths)
    if (tif == NULL) {
        tif = TIFFOpenExt(filename, "w", opts);
    }

    // 5. TIFFOpenOptionsFree
    // Free options regardless of open success
    TIFFOpenOptionsFree(opts);
    opts = NULL;

    if (tif != NULL) {
        // 6. TIFFErrorExtR
        TIFFErrorExtR(tif, "FuzzerTest", "Simulated error: %s", "test_message");

        // 7. TIFFWarningExtR
        TIFFWarningExtR(tif, "FuzzerTest", "Simulated warning: %s", "test_message");

        // 8. TIFFClose
        TIFFClose(tif);
    }

    // Cleanup dummy file
    remove(filename);

    return 0;
}