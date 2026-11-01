// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tiffio.h>

/* 
 * Fuzz driver for libtiff API functions:
 * TIFFFreeDirectory, TIFFCreateEXIFDirectory, TIFFCreateGPSDirectory,
 * TIFFSetDirectory, TIFFCreateCustomDirectory, TIFFWriteDirectory.
 */

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            /* Write a minimal valid TIFF header if input is empty to avoid immediate open failure */
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;
    int ret = 0;
    tdir_t dir_idx;
    
    /* Prepare environment: create a dummy file */
    setup_dummy_file(Data, Size);

    /* Open the file in write mode to allow directory creation and writing */
    tif = TIFFOpen("./dummy_file", "w8");
    if (!tif) {
        return 0;
    }

    /* 
     * Fuzzing logic:
     * 1. Attempt to set directory index (TIFFSetDirectory)
     * 2. Create custom, EXIF, or GPS directories
     * 3. Write directory
     * 4. Free directory
     * We use data from the input to drive decisions where possible.
     */

    /* Extract a directory index from input if available */
    if (Size >= 4) {
        dir_idx = (tdir_t)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
        /* Limit index to a reasonable range to avoid extreme values causing issues */
        if (dir_idx > 1000) {
            dir_idx = 1000;
        }
        TIFFSetDirectory(tif, dir_idx);
    } else {
        dir_idx = 0;
    }

    /* Try creating EXIF directory */
    if (Size > 0 && Data[0] % 3 == 0) {
        TIFFCreateEXIFDirectory(tif);
    }

    /* Try creating GPS directory */
    if (Size > 1 && Data[1] % 3 == 1) {
        TIFFCreateGPSDirectory(tif);
    }

    /* Try creating a custom directory */
    /* Note: TIFFCreateCustomDirectory requires a valid TIFFFieldArray. 
       Since constructing a valid one is complex and depends on internal libtiff structures,
       and the prompt says "If custom directory creation is not required, omit the call",
       we will skip this to avoid undefined behavior or crashes due to invalid structures.
       However, to explore state, we could attempt if we had a static valid array, 
       but without specific field definitions, it's safer to omit as per instructions.
    */

    /* Write directory */
    if (Size > 2 && Data[2] % 2 == 0) {
        TIFFWriteDirectory(tif);
    }

    /* Free directory */
    TIFFFreeDirectory(tif);

    /* Cleanup */
    if (tif) {
        TIFFClose(tif);
    }

    return 0;
}