// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandlerExt at tif_error.c:39:21 in tiffio.h
// TIFFSetWarningHandlerExt at tif_warning.c:39:21 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFError at tif_error.c:46:6 in tiffio.h
// TIFFWarning at tif_warning.c:46:6 in tiffio.h
// TIFFErrorExt at tif_error.c:63:6 in tiffio.h
// TIFFWarningExt at tif_warning.c:63:6 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_error_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warning_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warning_handler_ext(thandle_t clientdata, const char *module, const char *fmt, va_list ap) {
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (!f) {
        return 0;
    }

    uint8_t header[8];
    header[0] = (Data[0] & 0xFF);
    header[1] = (Data[1] & 0xFF);
    header[2] = 0x2A; 
    header[3] = 0x00;
    header[4] = 0x08; 
    header[5] = 0x00;
    header[6] = 0x00;
    header[7] = 0x00;
    fwrite(header, 1, 8, f);
    fclose(f);

    TIFF *tif = TIFFOpen(filename, "r");
    
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandler(dummy_warning_handler);
    TIFFSetErrorHandlerExt(dummy_error_handler_ext);
    TIFFSetWarningHandlerExt(dummy_warning_handler_ext);

    char fmt_buf[64];
    size_t fmt_len = (Size > 63) ? 63 : Size;
    memcpy(fmt_buf, Data, fmt_len);
    fmt_buf[fmt_len] = '\0';

    TIFFError("FuzzModule", fmt_buf);
    TIFFWarning("FuzzModule", fmt_buf);

    thandle_t dummy_handle = (thandle_t)0x12345;
    TIFFErrorExt(dummy_handle, "FuzzModule", fmt_buf);
    TIFFWarningExt(dummy_handle, "FuzzModule", fmt_buf);

    if (tif) {
        TIFFErrorExtR(tif, "FuzzModule", fmt_buf);
        TIFFWarningExtR(tif, "FuzzModule", fmt_buf);

        TIFFError("FuzzModule", fmt_buf);
        TIFFWarning("FuzzModule", fmt_buf);

        thandle_t clientdata = (thandle_t)0x12345;
        TIFFErrorExt(clientdata, "FuzzModule", fmt_buf);
        TIFFWarningExt(clientdata, "FuzzModule", fmt_buf);

        TIFFSetDirectory(tif, 0);

        TIFFClose(tif);
    }

    remove(filename);

    return 0;
}