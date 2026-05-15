// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff error/warning messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    // Suppress libtiff error/warning messages
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the dummy file as TIFF
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        // Try read-only mode if read-write fails
        tif = TIFFOpen("./dummy_file", "r");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }
    
    // Use fuzzer data to generate parameters
    // We'll use the first few bytes to determine which operations to perform
    // and generate parameters from the remaining data
    
    if (Size > 0) {
        uint8_t op_selector = Data[0];
        size_t data_offset = 1;
        
        // Generate parameters from fuzzer data
        uint64_t dir_offset = 0;
        tdir_t dir_index = 0;
        
        if (Size - data_offset >= sizeof(uint64_t)) {
            memcpy(&dir_offset, Data + data_offset, sizeof(uint64_t));
            data_offset += sizeof(uint64_t);
        } else if (Size - data_offset > 0) {
            memcpy(&dir_offset, Data + data_offset, Size - data_offset);
        }
        
        if (Size - data_offset >= sizeof(tdir_t)) {
            memcpy(&dir_index, Data + data_offset, sizeof(tdir_t));
        }
        
        // Perform operations based on selector
        // We'll try different sequences to explore more states
        
        // Operation 1: TIFFSetSubDirectory
        if (op_selector & 0x01) {
            TIFFSetSubDirectory(tif, dir_offset);
        }
        
        // Operation 2: TIFFCreateDirectory
        if (op_selector & 0x02) {
            TIFFCreateDirectory(tif);
        }
        
        // Operation 3: TIFFRewriteDirectory
        if (op_selector & 0x04) {
            TIFFRewriteDirectory(tif);
        }
        
        // Operation 4: TIFFSetDirectory
        if (op_selector & 0x08) {
            TIFFSetDirectory(tif, dir_index);
        }
        
        // Operation 5: TIFFLastDirectory
        if (op_selector & 0x10) {
            TIFFLastDirectory(tif);
        }
        
        // Operation 6: TIFFReadDirectory
        if (op_selector & 0x20) {
            TIFFReadDirectory(tif);
        }
        
        // Try some additional sequences
        
        // Sequence A: Create directory, then set it
        if (op_selector & 0x40) {
            TIFFCreateDirectory(tif);
            TIFFSetDirectory(tif, 0);
        }
        
        // Sequence B: Read multiple directories if available
        if (op_selector & 0x80) {
            int read_result;
            do {
                read_result = TIFFReadDirectory(tif);
            } while (read_result > 0 && (op_selector & 0x80));
        }
    }
    
    // Clean up
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}