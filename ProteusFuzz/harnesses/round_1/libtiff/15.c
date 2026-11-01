// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. TIFFOpenOptionsAlloc
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    // 2. TIFFOpenOptionsSetMaxCumulatedMemAlloc
    tmsize_t limit = 0;
    if (Size >= 4) {
        uint32_t raw_val = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        limit = (raw_val % 1048576) + 1024; 
    } else {
        limit = 1024;
    }
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, limit);

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    } else {
        TIFFOpenOptionsFree(opts);
        return 0;
    }

    // 3. TIFFOpenExt
    TIFF *tif = TIFFOpenExt(filename, "w", opts);
    
    // 4. TIFFOpenOptionsFree
    TIFFOpenOptionsFree(opts);

    if (tif == NULL) {
        return 0;
    }

    // 5. TIFFClose (First close - closing the file immediately)
    TIFFClose(tif);
    
    // 6. TIFFSetField (Attempt to set field on closed file)
    uint32_t width = (Size > 0) ? (Data[0] % 1000) + 1 : 1;
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);

    // 7. TIFFSetField (Attempt to set another field on closed file)
    uint32_t height = (Size > 1) ? (Data[1] % 1000) + 1 : 1;
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);

    // 8. TIFFWriteDirectory (Attempt to write directory on closed file)
    TIFFWriteDirectory(tif);

    // 9. TIFFClose (Second close)
    TIFFClose(tif);

    return 0;
}