// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFUnlinkDirectory at tif_dir.c:2237:5 in tiffio.h
// TIFFUnlinkDirectory at tif_dir.c:2237:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFUnlinkDirectory at tif_dir.c:2237:5 in tiffio.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    if (Size > 0 && Data != NULL) {
        fwrite(Data, 1, Size, f);
    } else {
        // Write a minimal valid TIFF header if input is empty or null
        // Little-endian, version 42, first IFD at offset 8
        uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
        fwrite(header, 1, 8, f);
    }
    fclose(f);
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    // Open in read mode first to test read-related functions
    TIFF *tif_read = TIFFOpen("./dummy_file", "r");
    if (tif_read) {
        // Test TIFFGetMode
        int mode = TIFFGetMode(tif_read);
        (void)mode;

        // Test TIFFLastDirectory
        int is_last = TIFFLastDirectory(tif_read);
        (void)is_last;

        // Test TIFFSetDirectory with various indices
        // Try index 0 and potentially 1 if file has directories
        TIFFSetDirectory(tif_read, 0);
        TIFFSetDirectory(tif_read, 1);
        TIFFSetDirectory(tif_read, 2);

        // Test TIFFRewriteDirectory (likely fails on read-only, but tests the path)
        TIFFRewriteDirectory(tif_read);

        TIFFClose(tif_read);
    }

    // Open in write mode to test write-related functions
    // Note: Overwriting the file.
    TIFF *tif_write = TIFFOpen("./dummy_file", "w");
    if (tif_write) {
        // Test TIFFGetMode
        int mode = TIFFGetMode(tif_write);
        (void)mode;

        // Test TIFFCreateEXIFDirectory
        int exif_ok = TIFFCreateEXIFDirectory(tif_write);
        (void)exif_ok;

        // Test TIFFRewriteDirectory (might fail if no directory written yet)
        TIFFRewriteDirectory(tif_write);

        // Test TIFFUnlinkDirectory
        // Index must be 1-based, so try 1.
        TIFFUnlinkDirectory(tif_write, 1);
        TIFFUnlinkDirectory(tif_write, 0); // Should fail

        // Test TIFFSetDirectory
        TIFFSetDirectory(tif_write, 0);
        TIFFSetDirectory(tif_write, 1);

        TIFFClose(tif_write);
    }

    // Open in append mode if supported
    TIFF *tif_append = TIFFOpen("./dummy_file", "a");
    if (tif_append) {
        int mode = TIFFGetMode(tif_append);
        (void)mode;
        TIFFLastDirectory(tif_append);
        TIFFSetDirectory(tif_append, 0);
        TIFFRewriteDirectory(tif_append);
        TIFFUnlinkDirectory(tif_append, 1);
        TIFFClose(tif_append);
    }

    return 0;
}