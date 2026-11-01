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

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        } else {
            // Write minimal valid TIFF header if input is empty
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: Prepare environment
    setup_dummy_file(Data, Size);

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
        // Use data from fuzz input to determine subdirectory offset if possible, otherwise 0
        uint64_t sub_off = 0;
        if (Size >= 8) {
            memcpy(&sub_off, Data, 8);
        }
        TIFFSetSubDirectory(tif3, sub_off);
        
        TIFFWriteDirectory(tif3);
        tdir_t count = TIFFNumberOfDirectories(tif3);
        
        // Try to unlink a directory if count > 0
        // TIFFUnlinkDirectory requires 1-based index
        if (count > 0) {
            tdir_t unlink_idx = 1;
            // Fuzz the index slightly based on input, but ensure it's valid
            if (Size >= 1) {
                unlink_idx = (Data[0] % count) + 1;
            }
            TIFFUnlinkDirectory(tif3, unlink_idx);
        }
        TIFFClose(tif3);
    }

    // Final cleanup: TIFFClose (redundant but follows sequence)
    // The sequence ends with two TIFFClose calls. 
    // Since we closed tif3 above, we just ensure no leaks.
    // If the sequence implies closing the same handle twice, libtiff handles NULL gracefully.
    TIFFClose(NULL); 

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}