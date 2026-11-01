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
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal valid TIFF header if input is empty to avoid immediate open failure
            uint16_t little_endian = 0x002A;
            uint16_t version = 42;
            uint32_t offset = 8;
            fwrite(&little_endian, 2, 1, f);
            fwrite(&version, 2, 1, f);
            fwrite(&offset, 4, 1, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
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
        tdir_t count = TIFFNumberOfDirectories(tif2);
        (void)count;
        TIFFClose(tif2);
    }

    // Sequence: TIFFOpen -> TIFFSetSubDirectory -> TIFFWriteDirectory -> TIFFNumberOfDirectories -> TIFFUnlinkDirectory -> TIFFClose
    TIFF *tif3 = TIFFOpen("./dummy_file", "r+");
    if (tif3) {
        uint64_t sub_offset = 0;
        if (Size > 0) {
            sub_offset = (uint64_t)Data[0];
        }
        TIFFSetSubDirectory(tif3, sub_offset);
        TIFFWriteDirectory(tif3);
        tdir_t count2 = TIFFNumberOfDirectories(tif3);
        if (count2 > 0) {
            tdir_t unlink_idx = 1;
            if (Size > 1) {
                unlink_idx = (tdir_t)Data[1];
                if (unlink_idx == 0) unlink_idx = 1;
            }
            if (unlink_idx <= count2) {
                TIFFUnlinkDirectory(tif3, unlink_idx);
            }
        }
        TIFFClose(tif3);
    }

    // Final TIFFClose call as per sequence (though tif3 is already closed, this handles the sequence requirement)
    // The sequence lists TIFFClose twice at the end. We can safely call it with NULL or a valid handle if we had one.
    // Since tif3 is closed, we just ensure the sequence logic is followed.
    // If the sequence implies closing a specific handle twice, libtiff handles NULL gracefully, 
    // but we don't have a dangling pointer. We will just call TIFFClose(NULL) to satisfy the count if needed,
    // or simply rely on the previous close. The prompt says "invoke the functions in the following order".
    // The last item is TIFFClose. We have closed tif3. 
    // To strictly follow the count if it implies a distinct operation, we can try to open again and close, 
    // but the sequence ends with TIFFClose. Let's assume the last TIFFClose refers to the handle from the previous block.
    // Since tif3 is closed, we do nothing more for the last TIFFClose to avoid double-free or invalid access,
    // unless we interpret the sequence as a list of calls on potentially different handles.
    // Given the flow, tif3 is the last handle. It is closed. 
    // If the sequence strictly requires another call, we can call TIFFClose(NULL) which is safe.
    TIFFClose(NULL);

    return 0;
}