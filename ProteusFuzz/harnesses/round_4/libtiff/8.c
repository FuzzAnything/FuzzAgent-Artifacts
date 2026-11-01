// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>
#include <tiff.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

/* 
 * Helper to safely invoke TIFFSetField with fuzzed data.
 * We use a static buffer to avoid passing pointers to the fuzzer input buffer directly,
 * which can cause issues if libtiff reads beyond the provided length or if the buffer is freed.
 */
static int safe_TIFFSetField(TIFF *t, uint32_t tag, const uint8_t *buf, size_t len) {
    if (!t || !buf || len == 0) return 0;
    
    /* Limit the length to prevent excessive memory usage or buffer overflows */
    if (len > 1024) len = 1024;

    /* Create a local copy of the data to ensure safety */
    uint8_t *safe_buf = (uint8_t *)malloc(len);
    if (!safe_buf) return 0;
    
    memcpy(safe_buf, buf, len);
    
    /* 
     * TIFFSetField expects specific types based on the tag. 
     * Passing a pointer directly can be risky if the tag expects a primitive type.
     * However, for fuzzing, we want to trigger internal logic.
     * We cast to void* to pass the pointer, but this is only safe for tags that accept pointers (e.g., TIFFTAG_ARTIST).
     * For other tags, this might trigger undefined behavior, which is what we want to catch.
     * To mitigate the specific crash (heap-buffer-overflow), we ensure the buffer is valid and large enough.
     */
    int ret = 0;
    
    /* 
     * Attempt to set the field. We pass the pointer. 
     * If the tag expects a different type, libtiff might misinterpret the pointer.
     * This is intentional for fuzzing to expose type confusion bugs.
     */
    ret = TIFFSetField(t, tag, (void*)safe_buf);
    
    free(safe_buf);
    return ret;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "w");
    if (!tif) {
        return 0;
    }

    /* Sequence: 20 TIFFSetField calls */
    for (int i = 0; i < 20; i++) {
        uint32_t tag = 0;
        size_t offset = i * 4;
        if (Size >= offset + 4) {
            tag = *(uint32_t*)(Data + offset);
        }
        
        size_t val_len = Size - (offset + 4);
        if (val_len > 0) {
            /* Ensure we don't read past the buffer */
            if (offset + 4 < Size) {
                safe_TIFFSetField(tif, tag, Data + offset + 4, val_len);
            }
        }
    }

    /* TIFFFindField */
    uint32_t find_tag = 0;
    if (Size >= 84) find_tag = *(uint32_t*)(Data + 84);
    TIFFFindField(tif, find_tag, TIFF_ANY);

    /* TIFFWriteDirectory */
    TIFFWriteDirectory(tif);

    /* TIFFCreateGPSDirectory */
    TIFFCreateGPSDirectory(tif);

    /* Sequence: 20 more TIFFSetField calls */
    for (int i = 0; i < 20; i++) {
        uint32_t tag = 0;
        size_t offset = 88 + (size_t)(i * 4);
        if (Size >= offset + 4) {
            tag = *(uint32_t*)(Data + offset);
        }
        
        size_t val_len = Size - (offset + 4);
        if (val_len > 0) {
            if (offset + 4 < Size) {
                safe_TIFFSetField(tif, tag, Data + offset + 4, val_len);
            }
        }
    }

    /* TIFFSetDirectory */
    tdir_t dir_idx = 0;
    if (Size >= 168) dir_idx = *(uint32_t*)(Data + 168);
    TIFFSetDirectory(tif, dir_idx);

    /* TIFFWriteDirectory */
    TIFFWriteDirectory(tif);

    /* TIFFCreateEXIFDirectory */
    TIFFCreateEXIFDirectory(tif);

    /* One more TIFFSetField */
    uint32_t exif_tag = 0;
    if (Size >= 172) exif_tag = *(uint32_t*)(Data + 172);
    size_t exif_val_len = Size - 172;
    if (exif_val_len > 0) {
        if (172 < Size) {
            safe_TIFFSetField(tif, exif_tag, Data + 172, exif_val_len);
        }
    }

    /* TIFFSetDirectory */
    tdir_t dir_idx2 = 0;
    if (Size >= 176) dir_idx2 = *(uint32_t*)(Data + 176);
    TIFFSetDirectory(tif, dir_idx2);

    /* TIFFWriteDirectory */
    TIFFWriteDirectory(tif);

    /* TIFFClose */
    TIFFClose(tif);
    tif = NULL;

    /* TIFFOpen (read mode) */
    TIFF *tif2 = TIFFOpen("./dummy_file", "r");
    if (tif2) {
        /* TIFFSetDirectory */
        tdir_t dir_idx3 = 0;
        if (Size >= 180) dir_idx3 = *(uint32_t*)(Data + 180);
        TIFFSetDirectory(tif2, dir_idx3);

        /* TIFFFindField */
        uint32_t find_tag2 = 0;
        if (Size >= 184) find_tag2 = *(uint32_t*)(Data + 184);
        TIFFFindField(tif2, find_tag2, TIFF_ANY);

        /* TIFFClose */
        TIFFClose(tif2);
        tif2 = NULL;
    }

    /* Final TIFFClose (on potentially NULL or already closed handle, handled gracefully) */
    if (tif) {
        TIFFClose(tif);
    }

    return 0;
}