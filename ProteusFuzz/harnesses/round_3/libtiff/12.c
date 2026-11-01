// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    // Set max cumulated memory allocation using fuzzed data
    tmsize_t limit = 0;
    if (Size >= sizeof(tmsize_t)) {
        memcpy(&limit, Data, sizeof(tmsize_t));
        // Ensure limit is positive and reasonable for testing
        if (limit <= 0) limit = 1024 * 1024; // 1MB default if fuzzed value is bad
    } else {
        limit = 1024 * 1024;
    }
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, limit);

    // Create a dummy file with fuzzed data
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Open the file for writing (mode "w")
    TIFF *tif = TIFFOpenExt(filename, "w", opts);
    
    // Free options immediately after opening as per the required sequence
    TIFFOpenOptionsFree(opts);

    if (tif == NULL) {
        // Cleanup if opening failed
        remove(filename);
        return 0;
    }

    // Invoke TIFFClose as per the first step in the sequence
    TIFFClose(tif);
    tif = NULL; // Ensure we know it's closed

    // Re-open the file to perform subsequent operations on a valid handle
    // The sequence requires TIFFSetField, TIFFWriteDirectory, and TIFFClose
    // which generally require a valid, open TIFF handle to avoid NULL pointer dereferences.
    TIFFOpenOptions *opts2 = TIFFOpenOptionsAlloc();
    if (opts2) {
        TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts2, limit);
        tif = TIFFOpenExt(filename, "w", opts2);
        TIFFOpenOptionsFree(opts2);
    }

    if (tif == NULL) {
        remove(filename);
        return 0;
    }

    // Invoke TIFFSetField
    uint32_t width = (Size > 0) ? Data[0] : 1;
    uint32_t length = (Size > 1) ? Data[1] : 1;
    
    // First TIFFSetField
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    
    // Second TIFFSetField
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, length);

    // Invoke TIFFWriteDirectory
    TIFFWriteDirectory(tif);

    // Final TIFFClose
    TIFFClose(tif);

    // Cleanup dummy file
    remove(filename);

    return 0;
}