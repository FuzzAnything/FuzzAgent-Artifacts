// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
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
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal valid TIFF header if data is empty
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Step 1: Prepare environment
    write_dummy_file(Data, Size);

    // Sequence: TIFFOpen -> TIFFWriteDirectory -> TIFFClose
    TIFF *tif1 = TIFFOpen("./dummy_file", "w");
    if (tif1) {
        TIFFCreateDirectory(tif1);
        TIFFWriteDirectory(tif1);
        TIFFClose(tif1);
    }

    // Sequence: TIFFOpen -> TIFFRewriteDirectory -> TIFFCreateDirectory -> TIFFWriteDirectory -> TIFFNumberOfDirectories -> TIFFClose
    TIFF *tif2 = TIFFOpen("./dummy_file", "r+");
    if (tif2) {
        TIFFRewriteDirectory(tif2);
        TIFFCreateDirectory(tif2);
        TIFFWriteDirectory(tif2);
        (void)TIFFNumberOfDirectories(tif2);
        TIFFClose(tif2);
    }

    // Sequence: TIFFOpen -> TIFFSetSubDirectory -> TIFFWriteDirectory -> TIFFNumberOfDirectories -> TIFFUnlinkDirectory -> TIFFClose
    TIFF *tif3 = TIFFOpen("./dummy_file", "r+");
    if (tif3) {
        // Try to set subdirectory, might fail if no subifds, but safe to call
        (void)TIFFSetSubDirectory(tif3, 0);
        TIFFWriteDirectory(tif3);
        tdir_t count = TIFFNumberOfDirectories(tif3);
        if (count > 0) {
            // Attempt to unlink the last directory (1-based index)
            (void)TIFFUnlinkDirectory(tif3, count);
        }
        TIFFClose(tif3);
    }

    // Final cleanup: TIFFClose (idempotent for NULL, but we ensure handle is valid if needed)
    // The sequence ends with TIFFClose, which we already called for tif3.
    // If we had another open handle, we would close it here.
    // The requirement lists two TIFFClose at the end, likely implying closing the last handle again or a second handle.
    // Since we closed tif3, we just ensure no leaks.
    
    return 0;
}