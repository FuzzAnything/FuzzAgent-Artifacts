// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void dummyErrorHandler(thandle_t handle, const char* module, const char* fmt, va_list ap) {
    (void)handle;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummyErrorHandlerR(TIFF* tif, const char* module, const char* fmt, va_list ap) {
    (void)tif;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    FILE* dummy = fopen("./dummy_file", "wb");
    if (!dummy) return 0;
    fwrite(Data, 1, Size, dummy);
    fclose(dummy);

    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        TIFFError("test_module", "Failed to open dummy file: %s", "./dummy_file");
        TIFFErrorExt((thandle_t)NULL, "test_module", "Failed to open dummy file: %s", "./dummy_file");
        TIFFWarningExt((thandle_t)NULL, "test_module", "Failed to open dummy file: %s", "./dummy_file");
    } else {
        TIFFErrorExtR(tif, "test_module", "Test error with TIFF*: %p", tif);
        TIFFWarningExt((thandle_t)tif, "test_module", "Test warning with thandle_t: %p", (thandle_t)tif);
        TIFFClose(tif);
    }

    TIFFErrorHandlerExt prevErrorHandler = TIFFSetErrorHandlerExt(dummyErrorHandler);
    TIFFErrorHandlerExt prevWarningHandler = TIFFSetWarningHandlerExt(dummyErrorHandler);

    if (Size > 10) {
        char* module = malloc(Size/2 + 1);
        char* message = malloc(Size/2 + 1);
        if (module && message) {
            memcpy(module, Data, Size/2);
            module[Size/2] = '\0';
            memcpy(message, Data + Size/2, Size - Size/2);
            message[Size - Size/2] = '\0';

            TIFFError(module, "%s", message);
            TIFFErrorExt((thandle_t)Data, module, "%s", message);
            TIFFWarningExt((thandle_t)Data, module, "%s", message);

            if (tif) {
                TIFFErrorExtR(tif, module, "%s", message);
            }

            free(module);
            free(message);
        }
    }

    TIFFSetErrorHandlerExt(prevErrorHandler);
    TIFFSetWarningHandlerExt(prevWarningHandler);

    remove("./dummy_file");
    return 0;
}