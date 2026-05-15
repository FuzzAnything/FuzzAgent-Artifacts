// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdarg.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static void call_vsetfield(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write fuzz data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }
    
    // Try to read directory to populate internal structures
    TIFFReadDirectory(tif);
    
    // Test TIFFPrintDirectory
    FILE *out = fopen("./dummy_out", "w");
    if (out) {
        // Use fuzz data to determine flags (mask to reasonable range)
        long flags = (Size > 0) ? (Data[0] & 0xFF) : 0;
        TIFFPrintDirectory(tif, out, flags);
        fclose(out);
        remove("./dummy_out");
    }
    
    // Test TIFFGetTagListEntry
    if (Size > 1) {
        int tag_index = Data[1] % 256; // Limit to reasonable range
        TIFFGetTagListEntry(tif, tag_index);
    }
    
    // Test TIFFVSetField using helper function
    if (Size > 4) {
        uint32_t tag = (Data[2] << 24) | (Data[3] << 16) | (Data[4] << 8) | Data[5];
        int dummy_value = (Size > 5) ? Data[6] : 0;
        call_vsetfield(tif, tag, dummy_value);
    }
    
    // Test TIFFUnsetField
    if (Size > 8) {
        uint32_t tag = (Data[6] << 24) | (Data[7] << 16) | (Data[8] << 8) | Data[9];
        TIFFUnsetField(tif, tag);
    }
    
    // Test TIFFFindField
    if (Size > 10) {
        uint32_t tag = (Data[10] << 24) | (Data[11] << 16) | (Data[12] << 8) | Data[13];
        TIFFDataType dtype = (Size > 14) ? (Data[14] % 12) : TIFF_NOTYPE; // 12 is max enum value
        TIFFFindField(tif, tag, dtype);
    }
    
    // Test TIFFMergeFieldInfo
    if (Size > 20) {
        // Create a dummy field info array on the stack (not static)
        TIFFFieldInfo field_info[2];
        
        // Initialize first field info
        field_info[0].field_tag = (Data[15] << 24) | (Data[16] << 16) | (Data[17] << 8) | Data[18];
        field_info[0].field_type = (Size > 19) ? (Data[19] % 12) : TIFF_BYTE;
        field_info[0].field_readcount = 1;
        field_info[0].field_writecount = 1;
        
        // Initialize second field info
        field_info[1].field_tag = (Data[20] << 24) | (Data[21] << 16) | (Data[22] << 8) | Data[23];
        field_info[1].field_type = (Size > 24) ? (Data[24] % 12) : TIFF_BYTE;
        field_info[1].field_readcount = 1;
        field_info[1].field_writecount = 1;
        
        uint32_t num_fields = (Size > 25) ? (Data[25] % 3) : 1; // 0, 1, or 2 fields
        TIFFMergeFieldInfo(tif, field_info, num_fields);
    }
    
    // Cleanup
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}