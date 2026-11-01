// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFUnlinkDirectory at tif_dir.c:2237:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tiffio.h"

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    // Sequence: TIFFOpen -> TIFFWriteDirectory -> TIFFClose
    TIFF *tif1 = TIFFOpen("./dummy_file", "w");
    if (tif1) {
        TIFFWriteDirectory(tif1);
        TIFFClose(tif1);
    }

    // Sequence: TIFFOpen -> TIFFRewriteDirectory -> TIFFCreateDirectory -> TIFFWriteDirectory -> TIFFNumberOfDirectories -> TIFFClose
    TIFF *tif2 = TIFFOpen("./dummy_file", "r+");
    if (tif2) {
        TIFFRewriteDirectory(tif2);
        TIFFCreateDirectory(tif2);
        TIFFWriteDirectory(tif2);
        TIFFNumberOfDirectories(tif2);
        TIFFClose(tif2);
    }

    // Sequence: TIFFOpen -> TIFFSetSubDirectory -> TIFFWriteDirectory -> TIFFNumberOfDirectories -> TIFFUnlinkDirectory -> TIFFClose
    TIFF *tif3 = TIFFOpen("./dummy_file", "r+");
    if (tif3) {
        // Use fuzzed data to determine offset, ensuring it's within reasonable bounds or 0
        uint64_t offset = 0;
        if (Size >= 8) {
            memcpy(&offset, Data, 8);
            // Mask to avoid huge invalid offsets that might cause immediate crashes unrelated to logic
            offset &= 0x0000FFFFFFFFFFFFFFULL; 
        }
        TIFFSetSubDirectory(tif3, offset);
        
        TIFFWriteDirectory(tif3);
        TIFFNumberOfDirectories(tif3);
        
        // Try to unlink directory 1 if it exists
        TIFFUnlinkDirectory(tif3, 1);
        
        TIFFClose(tif3);
    }

    // Final TIFFClose as per sequence (though tif3 is already closed, this handles the pattern)
    TIFFClose(NULL); // Safe to call with NULL

    return 0;
}