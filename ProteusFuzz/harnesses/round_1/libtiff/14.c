// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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

static void dummy_warn_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    // We cannot directly access struct members due to incomplete type in tiffio.h.
    // We rely on the library's internal initialization or set fields via public API if available.
    // TIFFOpenOptionsSetMaxSingleMemAlloc is the only setter we have for options in this context.
    
    // Set max single memory allocation based on input data to explore states
    tmsize_t limit = 0;
    if (Size >= sizeof(tmsize_t)) {
        memcpy(&limit, Data, sizeof(tmsize_t));
        if (limit < 0) limit = 0;
    } else if (Size > 0) {
        limit = Data[0];
    }
    
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, limit);

    // 2. Write data to dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    } else {
        TIFFOpenOptionsFree(opts);
        return 0;
    }

    // 3. Invoke target functions in order
    // TIFFOpenExt: Open for writing ("w")
    TIFF *tif = TIFFOpenExt(filename, "w", opts);
    
    if (tif == NULL) {
        TIFFOpenOptionsFree(opts);
        return 0;
    }

    // TIFFOpenOptionsFree: Free the options structure
    TIFFOpenOptionsFree(opts);

    // TIFFSetField: First call
    uint32_t tag1 = 0;
    if (Size > 0) tag1 = Data[0] | 0x100; 
    TIFFSetField(tif, tag1, 0);

    // TIFFSetField: Second call
    uint32_t tag2 = 0;
    if (Size > 1) tag2 = Data[1] | 0x100;
    TIFFSetField(tif, tag2, 0);

    // TIFFWriteDirectory: Write the directory
    TIFFWriteDirectory(tif);

    // TIFFClose: First close
    TIFFClose(tif);

    // Re-open for the second phase of operations to ensure valid state for subsequent calls
    TIFF *tif2 = TIFFOpenExt(filename, "w", NULL);
    if (tif2 != NULL) {
        // TIFFSetField: Third call (sequence item 6)
        uint32_t tag3 = 0;
        if (Size > 2) tag3 = Data[2] | 0x100;
        TIFFSetField(tif2, tag3, 0);
        
        // TIFFSetField: Fourth call (sequence item 7)
        uint32_t tag4 = 0;
        if (Size > 3) tag4 = Data[3] | 0x100;
        TIFFSetField(tif2, tag4, 0);
        
        // TIFFWriteDirectory: Second write (sequence item 8)
        TIFFWriteDirectory(tif2);
        
        // TIFFClose: Second close (sequence item 9)
        TIFFClose(tif2);
    }

    return 0;
}