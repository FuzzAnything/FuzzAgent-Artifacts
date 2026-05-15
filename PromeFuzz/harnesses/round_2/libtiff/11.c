// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFUnlinkDirectory at tif_dir.c:2237:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    write_dummy_file(Data, Size);
    
    // First sequence
    TIFF *tif1 = TIFFOpen("./dummy_file", "w");
    if (tif1) {
        TIFFWriteDirectory(tif1);
        TIFFClose(tif1);
    }
    
    // Second sequence
    TIFF *tif2 = TIFFOpen("./dummy_file", "r+");
    if (tif2) {
        TIFFRewriteDirectory(tif2);
        TIFFCreateDirectory(tif2);
        TIFFWriteDirectory(tif2);
        TIFFNumberOfDirectories(tif2);
        TIFFClose(tif2);
    }
    
    // Third sequence
    TIFF *tif3 = TIFFOpen("./dummy_file", "r+");
    if (tif3) {
        // Use first 8 bytes as offset for TIFFSetSubDirectory
        uint64_t offset = 0;
        if (Size >= 8) {
            memcpy(&offset, Data, sizeof(offset));
        }
        TIFFSetSubDirectory(tif3, offset);
        
        TIFFWriteDirectory(tif3);
        TIFFNumberOfDirectories(tif3);
        
        // Use next 4 bytes as directory number for TIFFUnlinkDirectory
        tdir_t dir_num = 0;
        if (Size >= 12) {
            memcpy(&dir_num, Data + 8, sizeof(dir_num));
        }
        TIFFUnlinkDirectory(tif3, dir_num);
        
        TIFFClose(tif3);
    }
    
    // Final TIFFClose (duplicate as specified)
    // Note: This would normally be an error, but following specification
    TIFFClose(NULL);
    
    // Clean up
    unlink("./dummy_file");
    
    return 0;
}