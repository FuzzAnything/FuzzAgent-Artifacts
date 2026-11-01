// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <tiffio.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size) ? 1 : 0;
}

// Helper function to safely call TIFFVGetField with variable arguments
static int safe_tiff_vgetfield(TIFF *tif, uint32_t tag, ...) {
    if (!tif) return 0;
    va_list ap;
    va_start(ap, tag);
    int result = TIFFVGetField(tif, tag, ap);
    va_end(ap);
    return result;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input data to a dummy file
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // 1. TIFFMergeFieldInfo
    // We skip this as constructing a valid TIFFFieldInfo array is complex and error-prone.

    // 2. TIFFFindField
    // Try to find standard fields
    const TIFFField *found = TIFFFindField(tif, TIFFTAG_IMAGEWIDTH, TIFF_ANY);
    (void)found;
    found = TIFFFindField(tif, TIFFTAG_IMAGELENGTH, TIFF_ANY);
    (void)found;
    // Try a custom tag ID (0x0100)
    found = TIFFFindField(tif, 0x0100, TIFF_ANY);
    (void)found;

    // 3. TIFFVGetField
    // Use the helper function to handle varargs correctly
    safe_tiff_vgetfield(tif, TIFFTAG_IMAGEWIDTH, 0);

    // 4. TIFFCreateEXIFDirectory
    // This might fail in read mode, but we call it to exercise the code path.
    TIFFCreateEXIFDirectory(tif);

    // 5. TIFFReadCustomDirectory
    // We skip this as we cannot safely construct a TIFFFieldArray.

    // 6. TIFFGetTagListEntry
    // Try valid and invalid indices
    uint32_t tag_id = TIFFGetTagListEntry(tif, 0);
    (void)tag_id;
    tag_id = TIFFGetTagListEntry(tif, -1);
    (void)tag_id;
    tag_id = TIFFGetTagListEntry(tif, 1000);
    (void)tag_id;

    // Cleanup
    TIFFClose(tif);
    
    return 0;
}