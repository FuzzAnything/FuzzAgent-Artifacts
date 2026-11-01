// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "tiffio.h"

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        } else {
            // Write minimal valid TIFF header if input is empty
            uint16_t header[] = {42, 0, 0, 0}; // Little endian, offset 8
            fwrite(header, 1, 8, fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Open file for writing to test directory creation functions
    TIFF *tif_write = TIFFOpen("./dummy_file", "w8");
    if (tif_write) {
        // Test TIFFCreateGPSDirectory
        TIFFCreateGPSDirectory(tif_write);

        // Test TIFFCreateEXIFDirectory
        TIFFCreateEXIFDirectory(tif_write);

        // Test TIFFCreateCustomDirectory with NULL removed to avoid crash
        // The API requires a non-NULL pointer to TIFFFieldArray
        // We skip this call or would need to construct a valid array which is complex
        // TIFFCreateCustomDirectory(tif_write, NULL); // Removed

        // Test TIFFFreeDirectory
        TIFFFreeDirectory(tif_write);

        TIFFClose(tif_write);
    }

    // Open file for reading to test read functions
    // Ensure file exists and has some content
    if (Size > 0) {
        TIFF *tif_read = TIFFOpen("./dummy_file", "r");
        if (tif_read) {
            // Test TIFFSetDirectory with various indices
            tdir_t dir_index = 0;
            if (Size > 0) {
                dir_index = Data[0];
            }
            TIFFSetDirectory(tif_read, dir_index);

            // Test TIFFReadCustomDirectory
            // Using 0 as offset might be valid for the first IFD or invalid.
            // Passing NULL for infoarray to test error handling.
            TIFFReadCustomDirectory(tif_read, 0, NULL);

            TIFFClose(tif_read);
        }
    }

    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}