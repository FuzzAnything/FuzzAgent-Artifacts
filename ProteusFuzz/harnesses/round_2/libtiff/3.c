// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
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

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal TIFF header if input is empty to ensure valid structure
            uint16_t header[8] = {0x002A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
            fwrite(header, sizeof(header), 1, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;
    const char *filename = NULL;
    const TIFFField *field = NULL;
    tmsize_t tile_size = 0;
    void *ptr = NULL;

    // Step 1: Prepare environment
    // Create a dummy file to open
    setup_dummy_file(Data, Size);

    // Open the file in read mode
    tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    // Step 2: Invoke target functions in order

    // 1. TIFFFileName
    filename = TIFFFileName(tif);
    // filename is const char*, no cleanup needed for the string itself

    // 2. TIFFFieldWithTag (Call 1)
    // Use a standard tag (e.g., TIFFTAG_IMAGEWIDTH = 256)
    field = TIFFFieldWithTag(tif, 256);

    // 3. TIFFFieldWithTag (Call 2)
    // Use another tag (e.g., TIFFTAG_IMAGELENGTH = 257)
    field = TIFFFieldWithTag(tif, 257);

    // 4. TIFFFieldWithTag (Call 3)
    // Use another tag (e.g., TIFFTAG_BITSPERSAMPLE = 258)
    field = TIFFFieldWithTag(tif, 258);

    // 5. TIFFFieldWithTag (Call 4)
    // Use another tag (e.g., TIFFTAG_COMPRESSION = 259)
    field = TIFFFieldWithTag(tif, 259);

    // 6. TIFFTileSize
    tile_size = TIFFTileSize(tif);
    // tile_size is tmsize_t, no cleanup needed

    // 7. TIFFFieldWithTag (Call 5)
    // Use another tag (e.g., TIFFTAG_PHOTOMETRIC = 262)
    field = TIFFFieldWithTag(tif, 262);

    // 8. _TIFFmalloc
    // Allocate a small buffer based on fuzz input size, capped to prevent OOM
    tmsize_t alloc_size = (Size > 0) ? Size : 1;
    if (alloc_size > 1024 * 1024) { // Cap at 1MB
        alloc_size = 1024 * 1024;
    }
    ptr = _TIFFmalloc(alloc_size);
    if (ptr != NULL) {
        // Optionally write some data to the buffer to exercise memory
        memset(ptr, 0xAA, alloc_size);
    }

    // 9. _TIFFfree
    if (ptr != NULL) {
        _TIFFfree(ptr);
        ptr = NULL;
    }

    // Step 3: Cleanup
    if (tif != NULL) {
        TIFFClose(tif);
    }

    // Clean up dummy file
    remove("./dummy_file");

    return 0;
}