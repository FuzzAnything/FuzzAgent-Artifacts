// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
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
#include <tiffio.h>

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
    TIFF *tif = NULL;
    tdir_t dir_index = 0;

    // Write initial data to dummy file to ensure it exists and has some content
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    // Open file for writing to allow directory operations
    tif = TIFFOpen(filename, "w");
    if (!tif) {
        return 0;
    }

    // 1. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index);

    // 2. TIFFForceStrileArrayWriting
    TIFFForceStrileArrayWriting(tif);

    // 3. TIFFFlushData
    TIFFFlushData(tif);

    // 4. TIFFForceStrileArrayWriting
    TIFFForceStrileArrayWriting(tif);

    // 5. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index);

    // 6. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index);

    // 7. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 8. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index);

    // 9. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 10. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index);

    // 11. TIFFReadDirectory
    TIFFReadDirectory(tif);

    // 12. TIFFSetField
    // Attempt to set a simple field, e.g., ImageWidth (tag 256)
    uint32_t width = (Size > 0) ? Data[0] : 1;
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);

    // 13. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 14. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index);

    // 15. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 16. TIFFSetErrorHandler
    TIFFSetErrorHandler(dummy_error_handler);

    // 17. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 18. TIFFSetErrorHandler
    TIFFSetErrorHandler(NULL);

    // 19. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 20. TIFFSetErrorHandler
    TIFFSetErrorHandler(dummy_error_handler);

    // 21. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 22. TIFFSetErrorHandler
    TIFFSetErrorHandler(NULL);

    // 23. TIFFSetErrorHandler
    TIFFSetErrorHandler(dummy_error_handler);

    // 24. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 25. TIFFSetErrorHandler
    TIFFSetErrorHandler(NULL);

    // 26. TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // 27. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index);

    // 28. TIFFClose
    TIFFClose(tif);
    tif = NULL;

    // 29. TIFFOpen (Read mode to test read path)
    tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    // 30. TIFFSetDirectory
    TIFFSetDirectory(tif, dir_index);

    // 31. TIFFClose
    TIFFClose(tif);
    tif = NULL;

    // 32. TIFFClose (on NULL, should be safe)
    TIFFClose(tif);

    return 0;
}