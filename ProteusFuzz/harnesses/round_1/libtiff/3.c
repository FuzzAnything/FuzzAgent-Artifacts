// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
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
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "tiffio.h"

static void dummy_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    const char *mode = "w";
    TIFF *tif = NULL;
    TIFFErrorHandler old_handler = NULL;

    // Write input data to dummy file
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    } else {
        return 0;
    }

    // Open file for writing
    tif = TIFFOpen(filename, mode);
    if (!tif) {
        return 0;
    }

    // 1. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 2. TIFFForceStrileArrayWriting
    (void)TIFFForceStrileArrayWriting(tif);

    // 3. TIFFFlushData
    (void)TIFFFlushData(tif);

    // 4. TIFFForceStrileArrayWriting
    (void)TIFFForceStrileArrayWriting(tif);

    // 5. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 6. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 7. TIFFWriteDirectory
    (void)TIFFWriteDirectory(tif);

    // 8. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 9. TIFFWriteDirectory
    (void)TIFFWriteDirectory(tif);

    // 10. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 11. TIFFReadDirectory
    (void)TIFFReadDirectory(tif);

    // 12. TIFFSetField
    (void)TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)100);

    // 13. TIFFWriteDirectory
    (void)TIFFWriteDirectory(tif);

    // 14. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 15. TIFFWriteDirectory
    (void)TIFFWriteDirectory(tif);

    // 16. TIFFSetErrorHandler
    old_handler = TIFFSetErrorHandler(dummy_error_handler);

    // 17. TIFFWriteDirectory
    (void)TIFFWriteDirectory(tif);

    // 18. TIFFSetErrorHandler
    TIFFSetErrorHandler(old_handler);

    // 19. TIFFWriteDirectory
    (void)TIFFWriteDirectory(tif);

    // 20. TIFFSetErrorHandler
    old_handler = TIFFSetErrorHandler(dummy_error_handler);

    // 21. TIFFWriteDirectory
    (void)TIFFWriteDirectory(tif);

    // 22. TIFFSetErrorHandler
    TIFFSetErrorHandler(old_handler);

    // 23. TIFFWriteDirectory
    (void)TIFFWriteDirectory(tif);

    // 24. TIFFSetErrorHandler
    old_handler = TIFFSetErrorHandler(dummy_error_handler);

    // 25. TIFFWriteDirectory
    (void)TIFFWriteDirectory(tif);

    // 26. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 27. TIFFClose
    TIFFClose(tif);
    tif = NULL;

    // 28. TIFFOpen (read mode)
    tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    // 29. TIFFSetDirectory
    TIFFSetDirectory(tif, 0);

    // 30. TIFFClose
    TIFFClose(tif);
    tif = NULL;

    // 31. TIFFClose (on NULL, should be safe)
    TIFFClose(tif);

    return 0;
}