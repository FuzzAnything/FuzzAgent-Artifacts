// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdarg.h>

static void helper_TIFFVSetField(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    FILE *file = fopen("./dummy_file", "wb");
    if (!file) return 0;
    
    fwrite(Data, 1, Size, file);
    fclose(file);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }
    
    uint8_t *data_ptr = (uint8_t *)Data;
    size_t data_remaining = Size;
    
    if (data_remaining >= 1) {
        int tag_index = data_ptr[0] % 100;
        TIFFGetTagListEntry(tif, tag_index);
        data_ptr++;
        data_remaining--;
    }
    
    if (data_remaining >= 3) {
        uint32_t tag = (data_ptr[0] << 8) | data_ptr[1];
        TIFFDataType dtype = data_ptr[2] % 13;
        TIFFFindField(tif, tag, dtype);
        data_ptr += 3;
        data_remaining -= 3;
    }
    
    if (data_remaining >= 5) {
        uint32_t tag = (data_ptr[0] << 8) | data_ptr[1];
        int value = (data_ptr[2] << 24) | (data_ptr[3] << 16) | (data_ptr[4] << 8);
        
        helper_TIFFVSetField(tif, tag, value);
        
        data_ptr += 5;
        data_remaining -= 5;
    }
    
    if (data_remaining >= 2) {
        uint32_t tag = (data_ptr[0] << 8) | data_ptr[1];
        TIFFUnsetField(tif, tag);
        data_ptr += 2;
        data_remaining -= 2;
    }
    
    if (data_remaining >= 3) {
        uint32_t num_fields = data_ptr[0] % 10 + 1;
        TIFFFieldInfo *field_infos = (TIFFFieldInfo *)malloc(num_fields * sizeof(TIFFFieldInfo));
        if (field_infos) {
            for (uint32_t i = 0; i < num_fields; i++) {
                field_infos[i].field_tag = (data_ptr[1] << 8) | data_ptr[2];
                field_infos[i].field_type = data_ptr[i % 3] % 13;
                field_infos[i].field_readcount = 1;
                field_infos[i].field_writecount = 1;
                field_infos[i].field_bit = 0;
                field_infos[i].field_oktochange = 1;
                field_infos[i].field_passcount = 0;
                field_infos[i].field_name = "TestField";
            }
            TIFFMergeFieldInfo(tif, field_infos, num_fields);
            free(field_infos);
        }
        data_ptr += 3;
        data_remaining -= 3;
    }
    
    FILE *out = fopen("./dummy_out", "w");
    if (out) {
        long flags = 0;
        if (data_remaining >= 1) {
            flags = data_ptr[0];
        }
        TIFFPrintDirectory(tif, out, flags);
        fclose(out);
        remove("./dummy_out");
    }
    
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}