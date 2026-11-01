// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <stdarg.h>

static int dummy_va_func(TIFF *t, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int ret = TIFFVGetField(t, tag, ap);
    va_end(ap);
    return ret;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);

    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        // Try to create a file if open failed
        TIFF *w = TIFFOpen(filename, "w");
        if (w) {
            TIFFClose(w);
            tif = TIFFOpen(filename, "r");
        }
    }
    if (!tif) {
        return 0;
    }

    // Prepare custom field info for TIFFMergeFieldInfo
    static TIFFFieldInfo custom_fields[2];
    memset(custom_fields, 0, sizeof(custom_fields));
    custom_fields[0].field_tag = 0x1000;
    custom_fields[0].field_name = "CustomTag1";
    custom_fields[0].field_type = TIFF_SHORT;
    custom_fields[0].field_readcount = 1;
    custom_fields[0].field_writecount = 1;
    custom_fields[1].field_tag = 0x1001;
    custom_fields[1].field_name = "CustomTag2";
    custom_fields[1].field_type = TIFF_LONG;
    custom_fields[1].field_readcount = 1;
    custom_fields[1].field_writecount = 1;

    // 1. Test TIFFMergeFieldInfo
    TIFFMergeFieldInfo(tif, custom_fields, 2);

    // 2. Test TIFFVGetField
    uint32_t dummy_tag = 0x1000;
    dummy_va_func(tif, dummy_tag, (uint16_t *)0);

    // 3. Test TIFFGetTagListEntry
    int tag_index = (Size % 10) - 5; // Range: -5 to 4
    TIFFGetTagListEntry(tif, tag_index);

    // 4. Test TIFFReadCustomDirectory
    toff_t diroff = (Size > 4) ? *(uint32_t *)(Data) : 0;
    const TIFFFieldArray *infoarray = NULL;
    TIFFReadCustomDirectory(tif, diroff, infoarray);

    // 5. Test TIFFFindField
    uint32_t search_tag = (Size > 2) ? *(uint16_t *)(Data) : 0x1000;
    TIFFDataType search_type = (Size > 4) ? (TIFFDataType)(*(uint16_t *)(Data + 2)) : TIFF_ANY;
    TIFFFindField(tif, search_tag, search_type);

    // 6. Test TIFFCreateEXIFDirectory
    // This might fail if the file is read-only or not writable, which is fine for fuzzing
    TIFFCreateEXIFDirectory(tif);

    TIFFClose(tif);
    return 0;
}