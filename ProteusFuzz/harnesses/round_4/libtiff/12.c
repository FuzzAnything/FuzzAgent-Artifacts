// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    // Use fuzzing data to set max_single_mem_alloc
    tmsize_t max_alloc = 0;
    if (Size >= sizeof(tmsize_t)) {
        memcpy(&max_alloc, Data, sizeof(tmsize_t));
        // Clamp to a reasonable range to avoid immediate allocation failures or OOM
        if (max_alloc < 0) max_alloc = 0;
        if (max_alloc > 1024 * 1024 * 1024) max_alloc = 1024 * 1024 * 1024; // 1GB limit
    } else {
        max_alloc = Size;
    }

    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_alloc);

    // Write fuzzing data to a dummy file to ensure it exists for TIFFOpenExt
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp != NULL) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Try to open the file for writing
    TIFF *tif = TIFFOpenExt("./dummy_file", "w", opts);
    
    // Free options immediately as per the requested sequence
    TIFFOpenOptionsFree(opts);

    if (tif == NULL) {
        return 0;
    }

    // Follow the requested sequence strictly:
    // 1. TIFFOpenExt (done)
    // 2. TIFFOpenOptionsFree (done)
    // 3. TIFFClose
    TIFFClose(tif);
    tif = NULL; 

    // 4. TIFFSetField (on closed/NULL handle to test error paths)
    uint32_t tag_id = 0;
    if (Size > 4) {
        tag_id = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }
    
    TIFFSetField(tif, tag_id, 100);
    TIFFSetField(tif, tag_id, 200);

    // 5. TIFFWriteDirectory (on closed/NULL handle)
    TIFFWriteDirectory(tif);

    // 6. TIFFClose (again, safe with NULL)
    TIFFClose(tif);

    return 0;
}