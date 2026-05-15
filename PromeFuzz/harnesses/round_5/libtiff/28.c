// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
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
#include <fcntl.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static int write_to_file(const char* filename, const uint8_t* data, size_t size) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write input data to dummy file
    if (write_to_file("./dummy_file", Data, Size) != 0) {
        return 0;
    }
    
    // Try to open the file as TIFF in different modes
    const char* modes[] = {"r", "w", "a", "r+"};
    for (size_t m = 0; m < sizeof(modes)/sizeof(modes[0]); m++) {
        TIFF* tif = TIFFOpen("./dummy_file", modes[m]);
        if (!tif) continue;
        
        // Try to read directory if in read mode
        if (modes[m][0] == 'r') {
            TIFFReadDirectory(tif);
        }
        
        // Call target functions with various parameters
        // Use input data to influence parameters
        
        // 1. TIFFCheckpointDirectory
        TIFFCheckpointDirectory(tif);
        
        // 2. TIFFCurrentDirOffset
        uint64_t offset = TIFFCurrentDirOffset(tif);
        
        // 3. TIFFCreateDirectory
        TIFFCreateDirectory(tif);
        
        // 4. TIFFSetSubDirectory - use offset from current or derived from input
        if (Size >= sizeof(uint64_t)) {
            uint64_t subdir_offset;
            memcpy(&subdir_offset, Data, sizeof(uint64_t));
            TIFFSetSubDirectory(tif, subdir_offset);
            
            // Also try with the current offset
            TIFFSetSubDirectory(tif, offset);
        }
        
        // 5. TIFFWriteCustomDirectory
        if (Size >= sizeof(uint64_t)) {
            uint64_t custom_offset;
            memcpy(&custom_offset, Data, sizeof(uint64_t));
            TIFFWriteCustomDirectory(tif, &custom_offset);
        }
        
        // 6. TIFFSetWriteOffset
        if (Size >= sizeof(toff_t)) {
            toff_t write_offset;
            memcpy(&write_offset, Data, sizeof(toff_t));
            TIFFSetWriteOffset(tif, write_offset);
        }
        
        // Try to create another directory and write it
        TIFFCreateDirectory(tif);
        TIFFWriteDirectory(tif);
        
        // Try to set some basic fields to create valid directory
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)256);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32_t)256);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, (uint16_t)8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16_t)1);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, (uint32_t)256);
        
        // Try checkpoint again after setting fields
        TIFFCheckpointDirectory(tif);
        
        // Try to write custom directory again
        if (Size >= sizeof(uint64_t)) {
            uint64_t another_offset;
            memcpy(&another_offset, Data + (Size > 8 ? 8 : 0), 
                   sizeof(uint64_t) < Size ? sizeof(uint64_t) : Size);
            TIFFWriteCustomDirectory(tif, &another_offset);
        }
        
        TIFFClose(tif);
    }
    
    // Clean up
    unlink("./dummy_file");
    
    return 0;
}