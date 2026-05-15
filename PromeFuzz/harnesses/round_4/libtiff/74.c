// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFUnsetField at tif_dir.c:1160:5 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Silent handler to suppress libtiff warnings/errors during fuzzing
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    // Write data to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    fwrite(data, 1, size, fp);
    fclose(fp);
    
    // Set error handlers to silent
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Try to open as TIFF
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // If can't open for reading, try to create new file
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            remove("./dummy_file");
            return NULL;
        }
        // Set some minimal required fields for a valid TIFF
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 1);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 1);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);
    }
    
    return tif;
}

// Helper function to call TIFFVSetField with variable arguments
static void call_tiffvsetfield(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    TIFFVSetField(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    TIFF* tif = create_dummy_tiff(Data, Size);
    if (!tif) return 0;
    
    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    // TIFFVSetField - using helper function
    if ((selector & 0x01) && (Size - data_offset >= sizeof(uint32_t) + sizeof(uint32_t))) {
        uint32_t tag;
        uint32_t value;
        memcpy(&tag, Data + data_offset, sizeof(uint32_t));
        memcpy(&value, Data + data_offset + sizeof(uint32_t), sizeof(uint32_t));
        data_offset += sizeof(uint32_t) * 2;
        
        call_tiffvsetfield(tif, tag, value);
    }
    
    // TIFFFindField
    if ((selector & 0x02) && (Size - data_offset >= sizeof(uint32_t) + sizeof(int))) {
        uint32_t tag;
        int type;
        memcpy(&tag, Data + data_offset, sizeof(uint32_t));
        memcpy(&type, Data + data_offset + sizeof(uint32_t), sizeof(int));
        data_offset += sizeof(uint32_t) + sizeof(int);
        
        TIFFFindField(tif, tag, (TIFFDataType)type);
    }
    
    // TIFFMergeFieldInfo - Note: We need to create dummy field info
    if ((selector & 0x04) && (Size - data_offset >= sizeof(uint32_t))) {
        uint32_t count;
        memcpy(&count, Data + data_offset, sizeof(uint32_t));
        data_offset += sizeof(uint32_t);
        
        // Create minimal dummy field info array
        if (count > 0 && count < 10) {
            // We'll create a simple array of TIFFFieldInfo structures
            // Note: TIFFFieldInfo is a deprecated type, but we need to use it
            // for TIFFMergeFieldInfo. We'll allocate it as an array of bytes.
            size_t field_info_size = 20; // Approximate size of TIFFFieldInfo
            void* field_info = malloc(count * field_info_size);
            if (field_info) {
                // Initialize with zeros
                memset(field_info, 0, count * field_info_size);
                // Cast to const TIFFFieldInfo* as expected by the function
                TIFFMergeFieldInfo(tif, (const TIFFFieldInfo*)field_info, count);
                free(field_info);
            }
        }
    }
    
    // TIFFPrintDirectory
    if (selector & 0x08) {
        long flags = 0;
        if (Size - data_offset >= sizeof(long)) {
            memcpy(&flags, Data + data_offset, sizeof(long));
            data_offset += sizeof(long);
        }
        
        FILE* out = fopen("./dummy_output", "w");
        if (out) {
            TIFFPrintDirectory(tif, out, flags);
            fclose(out);
            remove("./dummy_output");
        }
    }
    
    // TIFFUnsetField
    if ((selector & 0x10) && (Size - data_offset >= sizeof(uint32_t))) {
        uint32_t tag;
        memcpy(&tag, Data + data_offset, sizeof(uint32_t));
        data_offset += sizeof(uint32_t);
        
        TIFFUnsetField(tif, tag);
    }
    
    // TIFFGetTagListEntry
    if ((selector & 0x20) && (Size - data_offset >= sizeof(int))) {
        int index;
        memcpy(&index, Data + data_offset, sizeof(int));
        data_offset += sizeof(int);
        
        TIFFGetTagListEntry(tif, index);
    }
    
    // Cleanup
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}