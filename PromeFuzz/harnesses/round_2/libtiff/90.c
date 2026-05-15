// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress error messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress warning messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

// Helper function to call TIFFVSetField with variable arguments
static void call_tiffvsetfield(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    // Write input data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    
    // Write minimal valid TIFF header plus any additional data
    uint8_t minimal_tiff[] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
    fwrite(minimal_tiff, 1, sizeof(minimal_tiff), fp);
    if (Size > 0) {
        fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
    }
    fclose(fp);
    
    // Open the dummy file
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try read-only mode if read-write fails
        tif = TIFFOpen("./dummy_file", "r");
        if (!tif) {
            // Clean up and exit if we can't open the file
            remove("./dummy_file");
            return 0;
        }
    }
    
    // Set error and warning handlers to suppress output
    TIFFSetErrorHandler(dummy_error_handler);
    TIFFSetWarningHandler(dummy_warn_handler);
    
    // Use input data to generate parameters for API calls
    // We'll use the first few bytes to seed parameters
    
    // 1. Test TIFFMergeFieldInfo
    if (Size >= sizeof(uint32_t) * 2) {
        // Create some dummy field info structures
        TIFFFieldInfo field_info[2];
        memset(&field_info[0], 0, sizeof(TIFFFieldInfo));
        memset(&field_info[1], 0, sizeof(TIFFFieldInfo));
        
        // Use input data to populate tag values safely
        uint32_t tag1 = 0, tag2 = 0;
        memcpy(&tag1, Data, sizeof(uint32_t));
        if (Size >= sizeof(uint32_t) * 2) {
            memcpy(&tag2, Data + sizeof(uint32_t), sizeof(uint32_t));
        }
        
        field_info[0].field_tag = tag1;
        field_info[1].field_tag = tag2;
        
        TIFFMergeFieldInfo(tif, field_info, 2);
    }
    
    // 2. Test TIFFFindField
    if (Size >= sizeof(uint32_t) * 2) {
        uint32_t tag = 0;
        uint32_t dtype_val = 0;
        memcpy(&tag, Data, sizeof(uint32_t));
        memcpy(&dtype_val, Data + sizeof(uint32_t), sizeof(uint32_t));
        
        TIFFDataType dtype = (TIFFDataType)(dtype_val % 13); // 13 data types in enum
        const TIFFField *field = TIFFFindField(tif, tag, dtype);
        (void)field; // Result may be NULL, that's OK
    }
    
    // 3. Test TIFFPrintDirectory - fix: use uint32_t instead of long
    if (Size >= sizeof(uint32_t)) {
        uint32_t flags_val = 0;
        memcpy(&flags_val, Data, sizeof(uint32_t));
        long flags = (long)(flags_val & 0xFFFF); // Limit flag range
        
        FILE *out = fopen("/dev/null", "w");
        if (out) {
            TIFFPrintDirectory(tif, out, flags);
            fclose(out);
        }
    }
    
    // 4. Test TIFFGetTagListEntry
    if (Size >= sizeof(int)) {
        int index = 0;
        memcpy(&index, Data, sizeof(int));
        index = index % 100; // Limit index range
        uint32_t tag = TIFFGetTagListEntry(tif, index);
        (void)tag; // Result may be -1, that's OK
    }
    
    // 5. Test TIFFUnsetField
    if (Size >= sizeof(uint32_t)) {
        uint32_t tag = 0;
        memcpy(&tag, Data, sizeof(uint32_t));
        TIFFUnsetField(tif, tag);
    }
    
    // 6. Test TIFFVSetField via helper function
    if (Size >= sizeof(uint32_t) * 3) {
        uint32_t tag = 0, value = 0;
        memcpy(&tag, Data + sizeof(uint32_t) * 2, sizeof(uint32_t));
        memcpy(&value, Data + sizeof(uint32_t), sizeof(uint32_t));
        
        // Call through helper function with variable arguments
        call_tiffvsetfield(tif, tag, value);
    }
    
    // Clean up
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}