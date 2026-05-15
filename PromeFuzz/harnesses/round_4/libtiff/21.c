// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFIsBigEndian at tif_open.c:899:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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
    // Suppress libtiff error/warning messages during fuzzing
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write fuzzer data to a dummy file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    write(fd, Data, Size);
    close(fd);
    
    // Open the TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Call functions in the specified order
    uint64_t offset1 = TIFFCurrentDirOffset(tif);
    
    // Use first 4 bytes for directory index if available
    tdir_t dir1 = 0;
    if (Size >= 4) {
        dir1 = *(tdir_t*)Data;
    }
    int setdir1 = TIFFSetDirectory(tif, dir1);
    
    uint64_t offset2 = TIFFCurrentDirOffset(tif);
    
    int readdir = TIFFReadDirectory(tif);
    
    uint64_t offset3 = TIFFCurrentDirOffset(tif);
    
    // Use next bytes for directory indices if available
    tdir_t dir2 = 0, dir3 = 0, dir4 = 0;
    if (Size >= 8) {
        dir2 = *((tdir_t*)(Data + 4));
    }
    if (Size >= 12) {
        dir3 = *((tdir_t*)(Data + 8));
    }
    if (Size >= 16) {
        dir4 = *((tdir_t*)(Data + 12));
    }
    
    int setdir2 = TIFFSetDirectory(tif, dir2);
    int setdir3 = TIFFSetDirectory(tif, dir3);
    int setdir4 = TIFFSetDirectory(tif, dir4);
    
    // Use remaining bytes for subdirectory offsets if available
    uint64_t subdir1 = 0, subdir2 = 0, subdir3 = 0, subdir4 = 0;
    size_t offset = 16;
    if (Size >= offset + 8) {
        subdir1 = *((uint64_t*)(Data + offset));
        offset += 8;
    }
    if (Size >= offset + 8) {
        subdir2 = *((uint64_t*)(Data + offset));
        offset += 8;
    }
    if (Size >= offset + 8) {
        subdir3 = *((uint64_t*)(Data + offset));
        offset += 8;
    }
    if (Size >= offset + 8) {
        subdir4 = *((uint64_t*)(Data + offset));
    }
    
    int setsubdir1 = TIFFSetSubDirectory(tif, subdir1);
    int setsubdir2 = TIFFSetSubDirectory(tif, subdir2);
    int setsubdir3 = TIFFSetSubDirectory(tif, subdir3);
    int setsubdir4 = TIFFSetSubDirectory(tif, subdir4);
    
    int is_big_endian = TIFFIsBigEndian(tif);
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    // Use variables to avoid unused variable warnings
    (void)offset1;
    (void)offset2;
    (void)offset3;
    (void)setdir1;
    (void)setdir2;
    (void)setdir3;
    (void)setdir4;
    (void)readdir;
    (void)setsubdir1;
    (void)setsubdir2;
    (void)setsubdir3;
    (void)setsubdir4;
    (void)is_big_endian;
    
    return 0;
}