// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Write fuzzing data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open TIFF file for reading/writing
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try opening as read-only if r+ fails
        tif = TIFFOpen("./dummy_file", "r");
        if (!tif) {
            remove("./dummy_file");
            return 0;
        }
    }

    // Try reading directories first
    int read_result;
    do {
        read_result = TIFFReadDirectory(tif);
    } while (read_result > 0);

    // Reset to first directory if possible
    if (read_result == 0) {
        TIFFSetDirectory(tif, 0);
    }

    // Test TIFFLastDirectory
    (void)TIFFLastDirectory(tif);

    // Test TIFFCreateDirectory
    (void)TIFFCreateDirectory(tif);

    // Test TIFFCheckpointDirectory
    (void)TIFFCheckpointDirectory(tif);

    // Test TIFFRewriteDirectory
    (void)TIFFRewriteDirectory(tif);

    // Test TIFFSetSubDirectory with various offsets
    // Use fuzzing data to generate offsets if available
    uint64_t offset = 0;
    if (Size >= sizeof(uint64_t)) {
        memcpy(&offset, Data, sizeof(uint64_t));
    }
    (void)TIFFSetSubDirectory(tif, offset);

    // Try reading directories again after operations
    if (read_result >= 0) {
        do {
            read_result = TIFFReadDirectory(tif);
        } while (read_result > 0);
    }

    // Clean up
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}