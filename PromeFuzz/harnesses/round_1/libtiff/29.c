// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "libtiff/tiffio.h"

static int myTIFFVSetField(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    int ret;
    va_start(ap, tag);
    ret = TIFFVSetField(tif, tag, ap);
    va_end(ap);
    return ret;
}

static const TIFFFieldInfo customFieldInfo[] = {
    {65000, 1, 1, TIFF_LONG, 0, 0, 0, 0, "CustomTag"}
};

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }

    int tag_count = TIFFGetTagListCount(tif);
    if (tag_count > 0) {
        int idx = Data[0] % tag_count;
        uint32_t tag_id = TIFFGetTagListEntry(tif, idx);
        if (tag_id != (uint32_t)-1) {
            myTIFFVSetField(tif, tag_id, (uint32_t)Data[1]);
            TIFFUnsetField(tif, tag_id);
        }
    }

    uint32_t tag_to_find = ((uint32_t)Data[2] << 8) | Data[3];
    const TIFFField *field = TIFFFindField(tif, tag_to_find, TIFF_LONG);
    if (field) {
        TIFFUnsetField(tif, tag_to_find);
    }

    int merge_result = TIFFMergeFieldInfo(tif, customFieldInfo, 1);
    if (merge_result == 0) {
        myTIFFVSetField(tif, 65000, (uint32_t)Data[0]);
        TIFFUnsetField(tif, 65000);
    }

    TIFFClose(tif);
    remove("./dummy_file");
    return 0;
}