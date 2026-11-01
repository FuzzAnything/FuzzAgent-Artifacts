// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
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

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal valid-ish header if input is empty to avoid immediate crash on open
            // BigTIFF or standard TIFF header could be written, but empty is often handled gracefully by libtiff
            // We write a small dummy to ensure the file exists.
            const char dummy[] = "II*";
            fwrite(dummy, 1, sizeof(dummy), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // 1. Prepare the environment
    setup_dummy_file(Data, Size);

    TIFF *tif = NULL;
    int ret = 0;
    tdir_t dir_idx = 0;
    uint32_t tag = TIFFTAG_IMAGEWIDTH;
    uint32_t val = 100;

    // Sequence: TIFFOpen -> TIFFCheckpointDirectory -> TIFFSetField -> TIFFWriteDirectory -> TIFFSetDirectory -> TIFFCreateDirectory -> TIFFClose
    // Attempt 1: Open for writing (create new file)
    tif = TIFFOpen("./dummy_file", "w");
    if (tif) {
        ret = TIFFCheckpointDirectory(tif);
        (void)ret;
        ret = TIFFSetField(tif, tag, val);
        (void)ret;
        ret = TIFFWriteDirectory(tif);
        (void)ret;
        ret = TIFFSetDirectory(tif, dir_idx);
        (void)ret;
        ret = TIFFCreateDirectory(tif);
        (void)ret;
        TIFFClose(tif);
        tif = NULL;
    }

    // Sequence: TIFFOpen -> TIFFSetField -> TIFFCheckpointDirectory -> TIFFSetField -> TIFFCheckpointDirectory -> TIFFWriteDirectory -> TIFFClose
    // Attempt 2: Open existing file for writing
    tif = TIFFOpen("./dummy_file", "w");
    if (tif) {
        ret = TIFFSetField(tif, tag, val);
        (void)ret;
        ret = TIFFCheckpointDirectory(tif);
        (void)ret;
        ret = TIFFSetField(tif, tag, val + 1);
        (void)ret;
        ret = TIFFCheckpointDirectory(tif);
        (void)ret;
        ret = TIFFWriteDirectory(tif);
        (void)ret;
        TIFFClose(tif);
        tif = NULL;
    }

    // Sequence: TIFFOpen -> TIFFSetDirectory -> TIFFSetDirectory -> TIFFClose
    // Attempt 3: Open for reading to test directory switching
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        ret = TIFFSetDirectory(tif, dir_idx);
        (void)ret;
        ret = TIFFSetDirectory(tif, dir_idx + 1);
        (void)ret;
        TIFFClose(tif);
        tif = NULL;
    }

    // Sequence: TIFFClose (on NULL to test robustness)
    TIFFClose(NULL);

    // Cleanup
    // The file "./dummy_file" is left on disk, which is acceptable for fuzzing environments
    // or can be removed if strict cleanup is required. For standard fuzzing, leaving it is fine.
    // unlink("./dummy_file"); 

    return 0;
}