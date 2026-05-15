// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write fuzzing data to a dummy file
    const char* filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    // Write at least some minimal valid TIFF header to make TIFFOpen happy
    // Big-endian TIFF header with minimal structure
    uint8_t header[8] = {0x4D, 0x4D, 0x00, 0x2A, 0x00, 0x00, 0x00, 0x08};
    write(fd, header, sizeof(header));
    
    // Append fuzzing data after header
    if (Size > 0) {
        write(fd, Data, Size);
    }
    close(fd);
    
    // Open the TIFF file
    TIFF* tif = TIFFOpen(filename, "r+");
    if (!tif) {
        // Try read-only mode if read-write fails
        tif = TIFFOpen(filename, "r");
    }
    
    if (tif) {
        // 1. TIFFCurrentDirOffset
        uint64_t current_offset = TIFFCurrentDirOffset(tif);
        (void)current_offset;
        
        // 2. TIFFCheckpointDirectory
        int checkpoint_result = TIFFCheckpointDirectory(tif);
        (void)checkpoint_result;
        
        // 3. TIFFSetWriteOffset - use various offsets
        // Use current_offset if valid, otherwise use derived values
        if (current_offset > 0) {
            TIFFSetWriteOffset(tif, current_offset);
        }
        // Try some other offsets based on fuzzing data
        if (Size >= sizeof(uint64_t)) {
            uint64_t offset;
            memcpy(&offset, Data, sizeof(uint64_t));
            TIFFSetWriteOffset(tif, offset);
        }
        
        // 4. TIFFCreateDirectory
        int create_result = TIFFCreateDirectory(tif);
        (void)create_result;
        
        // 5. TIFFSetSubDirectory - try various offsets
        if (current_offset > 0) {
            TIFFSetSubDirectory(tif, current_offset);
        }
        if (Size >= sizeof(uint64_t)) {
            uint64_t subdir_offset;
            memcpy(&subdir_offset, Data, sizeof(uint64_t));
            TIFFSetSubDirectory(tif, subdir_offset);
        }
        // Try offset derived from size
        TIFFSetSubDirectory(tif, (uint64_t)Size);
        
        // 6. TIFFWriteCustomDirectory
        uint64_t custom_dir_offset = 0;
        if (create_result == 1) {
            // Only try to write custom directory if we successfully created one
            TIFFWriteCustomDirectory(tif, &custom_dir_offset);
        }
        
        // Try to create and write another directory
        if (TIFFCreateDirectory(tif) == 1) {
            uint64_t another_offset = 0;
            TIFFWriteCustomDirectory(tif, &another_offset);
        }
        
        // Try more combinations
        // Reset to original directory if possible
        if (current_offset > 0) {
            TIFFSetSubDirectory(tif, current_offset);
            TIFFCheckpointDirectory(tif);
        }
        
        // Clean up
        TIFFClose(tif);
    }
    
    // Remove the dummy file
    unlink(filename);
    
    return 0;
}