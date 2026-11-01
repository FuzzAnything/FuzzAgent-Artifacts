// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include "tiffio.h"

static void DummyErrorHandler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void DummyErrorHandlerExt(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *filename = "./dummy_file";
    TIFF *tif = NULL;
    TIFFErrorHandler old_handler = NULL;

    // Prepare file
    FILE *fp = fopen(filename, "w+b");
    if (fp == NULL) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open file for writing
    tif = TIFFOpen(filename, "w");
    if (tif == NULL) {
        return 0;
    }

    // Set some basic fields to make TIFF valid for writing
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 1);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    // Sequence of API calls as requested
    // 1. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 2. TIFFForceStrileArrayWriting
    TIFFForceStrileArrayWriting(tif);

    // 3. TIFFFlushData
    TIFFFlushData(tif);

    // 4. TIFFForceStrileArrayWriting
    TIFFForceStrileArrayWriting(tif);

    // 5. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 6. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 7. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 8. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 9. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 10. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 11. TIFFReadDirectory
    TIFFReadDirectory(tif);

    // 12. TIFFSetField
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);

    // 13. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 14. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 15. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 16. TIFFSetErrorHandler
    old_handler = TIFFSetErrorHandler(DummyErrorHandler);

    // 17. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 18. TIFFSetErrorHandler
    old_handler = TIFFSetErrorHandler(DummyErrorHandler);

    // 19. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 20. TIFFSetErrorHandler
    old_handler = TIFFSetErrorHandler(DummyErrorHandler);

    // 21. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 22. TIFFSetErrorHandler
    old_handler = TIFFSetErrorHandler(DummyErrorHandler);

    // 23. TIFFSetErrorHandler
    old_handler = TIFFSetErrorHandler(DummyErrorHandler);

    // 24. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 25. TIFFSetErrorHandler
    old_handler = TIFFSetErrorHandler(DummyErrorHandler);

    // 26. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 27. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 28. TIFFClose
    TIFFClose(tif);
    tif = NULL;

    // 29. TIFFOpen (read mode)
    tif = TIFFOpen(filename, "r");
    if (tif != NULL) {
        // 30. TIFFSetDirectory
        TIFFSetDirectory(tif, 0);

        // 31. TIFFClose
        TIFFClose(tif);
        tif = NULL;
    }

    // 32. TIFFClose (should be safe on NULL, but we ensure tif is NULL here)
    TIFFClose(tif);

    // Cleanup
    unlink(filename);

    return 0;
}