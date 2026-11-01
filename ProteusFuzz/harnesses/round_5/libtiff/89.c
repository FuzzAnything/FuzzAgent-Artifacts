// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenOptionsSetWarnAboutUnknownTags at tif_open.c:115:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have at least a small valid header to avoid immediate crashes on empty files
    // We write the fuzzed data to a dummy file
    write_dummy_file(Data, Size);

    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    // Fuzz the options based on input data
    if (Size >= 2) {
        tmsize_t max_single = ((tmsize_t)Data[0] << 8) | Data[1];
        TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_single);
    }

    if (Size >= 4) {
        tmsize_t max_cumulated = ((tmsize_t)Data[2] << 8) | Data[3];
        TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, max_cumulated);
    }

    if (Size >= 5) {
        int warn = Data[4] % 2; // 0 or 1
        TIFFOpenOptionsSetWarnAboutUnknownTags(opts, warn);
    }

    // Try to open the file with the options
    // We try both read ("r") and write ("w") modes based on data
    const char *mode = (Size > 6 && Data[6] % 2 == 0) ? "r" : "w";
    
    TIFF *tif = TIFFOpenExt("./dummy_file", mode, opts);
    
    // If opened successfully, do minimal operations to exercise state
    if (tif != NULL) {
        // Attempt to set a directory to trigger internal logic
        if (TIFFSetDirectory(tif, 0) != 0) {
            // Just a no-op to ensure the state is active
        }
        TIFFClose(tif);
    }

    // Cleanup options
    TIFFOpenOptionsFree(opts);

    return 0;
}