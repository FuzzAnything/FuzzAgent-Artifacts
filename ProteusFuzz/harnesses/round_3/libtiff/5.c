// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetErrorHandlerExtR at tif_open.c:121:6 in tiffio.h
// TIFFOpenOptionsSetWarningHandlerExtR at tif_open.c:129:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFErrorExtR at tif_error.c:107:6 in tiffio.h
// TIFFWarningExtR at tif_warning.c:80:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static int dummy_error_handler(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

static int dummy_warn_handler(TIFF *tif, void *clientdata, const char *module, const char *fmt, va_list ap) {
    (void)tif;
    (void)clientdata;
    (void)module;
    (void)fmt;
    (void)ap;
    return 0;
}

static void write_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        // Write a minimal valid TIFF header (4-byte magic, 2-byte version, 4-byte offset)
        uint8_t header[8];
        // Little-endian magic (II)
        header[0] = 'I';
        header[1] = 'I';
        // Version 42 (0x002A)
        header[2] = 0x2A;
        header[3] = 0x00;
        // First IFD offset (8)
        header[4] = 0x08;
        header[5] = 0x00;
        header[6] = 0x00;
        header[7] = 0x00;
        fwrite(header, 1, 8, f);
        // Write a minimal IFD (count, tag, type, count, value)
        uint8_t ifd[8];
        // 0 directories (0x0000)
        ifd[0] = 0x00;
        ifd[1] = 0x00;
        // Next IFD offset (0)
        ifd[2] = 0x00;
        ifd[3] = 0x00;
        ifd[4] = 0x00;
        ifd[5] = 0x00;
        ifd[6] = 0x00;
        ifd[7] = 0x00;
        fwrite(ifd, 1, 8, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file();

    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        return 0;
    }

    TIFFOpenOptionsSetErrorHandlerExtR(opts, dummy_error_handler, (void *)Data);
    TIFFOpenOptionsSetWarningHandlerExtR(opts, dummy_warn_handler, (void *)Data);

    // Use the fuzzed data to determine mode, but ensure it's valid
    const char *mode = (Size > 0 && Data[0] % 2 == 0) ? "r" : "w";
    const char *filename = "./dummy_file";

    TIFF *tif = TIFFOpenExt(filename, mode, opts);

    if (tif) {
        // Invoke error and warning handlers on the open TIFF instance
        TIFFErrorExtR(tif, "Fuzzer", "Test error message: %zu", Size);
        TIFFWarningExtR(tif, "Fuzzer", "Test warning message: %zu", Size);
        
        // Close the file
        TIFFClose(tif);
    }

    // Free the options
    TIFFOpenOptionsFree(opts);

    return 0;
}