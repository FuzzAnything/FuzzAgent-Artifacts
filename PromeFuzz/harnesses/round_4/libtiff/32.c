// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libtiff/tiffio.h"

static void error_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress error messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

static void warning_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress warning messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(error_handler);
    TIFFSetWarningHandler(warning_handler);

    // Create a dummy TIFF file
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    
    // Write fuzzer data to file
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Try to open the file as TIFF
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        // Clean up and exit if we can't open as TIFF
        remove(filename);
        return 0;
    }
    
    // Use fuzzer data to generate parameters for TIFFFindField
    // We'll use the first 8 bytes for tag and type
    if (Size >= 8) {
        uint32_t tag;
        TIFFDataType type;
        
        // Extract tag from first 4 bytes
        memcpy(&tag, Data, 4);
        
        // Extract type from next 4 bytes (but use only valid enum values)
        uint32_t type_raw;
        memcpy(&type_raw, Data + 4, 4);
        type = (TIFFDataType)(type_raw % (TIFF_IFD8 + 1)); // TIFF_IFD8 is the last enum value
        
        // Call TIFFFindField with fuzzer-generated parameters
        const TIFFField* field = TIFFFindField(tif, tag, type);
        
        // If we found a field, call the other functions
        if (field) {
            // Call all the target API functions
            int count_size = TIFFFieldSetGetCountSize(field);
            int set_get_size = TIFFFieldSetGetSize(field);
            int write_count = TIFFFieldWriteCount(field);
            int read_count = TIFFFieldReadCount(field);
            int pass_count = TIFFFieldPassCount(field);
            
            // Use the results to avoid compiler optimizations
            (void)count_size;
            (void)set_get_size;
            (void)write_count;
            (void)read_count;
            (void)pass_count;
        }
    }
    
    // Try to find some common TIFF tags to exercise more code paths
    if (Size >= 4) {
        uint32_t common_tag;
        memcpy(&common_tag, Data, 4);
        
        // Try with different data types
        const TIFFField* field1 = TIFFFindField(tif, common_tag, TIFF_BYTE);
        const TIFFField* field2 = TIFFFindField(tif, common_tag, TIFF_SHORT);
        const TIFFField* field3 = TIFFFindField(tif, common_tag, TIFF_LONG);
        const TIFFField* field4 = TIFFFindField(tif, common_tag, TIFF_RATIONAL);
        
        if (field1) {
            TIFFFieldSetGetCountSize(field1);
            TIFFFieldSetGetSize(field1);
            TIFFFieldWriteCount(field1);
            TIFFFieldReadCount(field1);
            TIFFFieldPassCount(field1);
        }
        
        if (field2) {
            TIFFFieldSetGetCountSize(field2);
            TIFFFieldSetGetSize(field2);
            TIFFFieldWriteCount(field2);
            TIFFFieldReadCount(field2);
            TIFFFieldPassCount(field2);
        }
        
        if (field3) {
            TIFFFieldSetGetCountSize(field3);
            TIFFFieldSetGetSize(field3);
            TIFFFieldWriteCount(field3);
            TIFFFieldReadCount(field3);
            TIFFFieldPassCount(field3);
        }
        
        if (field4) {
            TIFFFieldSetGetCountSize(field4);
            TIFFFieldSetGetSize(field4);
            TIFFFieldWriteCount(field4);
            TIFFFieldReadCount(field4);
            TIFFFieldPassCount(field4);
        }
    }
    
    // Clean up
    TIFFClose(tif);
    remove(filename);
    
    return 0;
}