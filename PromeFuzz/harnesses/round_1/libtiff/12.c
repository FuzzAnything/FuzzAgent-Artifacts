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

static void cleanup_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    write_dummy_file(Data, Size);
    
    // First sequence: Open, WriteDirectory, Close
    TIFF* tif1 = TIFFOpen("./dummy_file", "w");
    if (tif1) {
        TIFFWriteDirectory(tif1);
        TIFFClose(tif1);
    }
    
    // Second sequence: Open, RewriteDirectory, CreateDirectory, WriteDirectory, NumberOfDirectories, Close
    TIFF* tif2 = TIFFOpen("./dummy_file", "r+");
    if (tif2) {
        TIFFRewriteDirectory(tif2);
        TIFFCreateDirectory(tif2);
        TIFFWriteDirectory(tif2);
        TIFFNumberOfDirectories(tif2);
        TIFFClose(tif2);
    }
    
    // Third sequence: Open, SetSubDirectory, WriteDirectory, NumberOfDirectories, UnlinkDirectory, Close, Close
    TIFF* tif3 = TIFFOpen("./dummy_file", "r+");
    if (tif3) {
        // Use part of fuzzer input for offset if available
        uint64_t offset = 0;
        if (Size >= sizeof(uint64_t)) {
            memcpy(&offset, Data, sizeof(uint64_t));
        }
        TIFFSetSubDirectory(tif3, offset);
        
        TIFFWriteDirectory(tif3);
        TIFFNumberOfDirectories(tif3);
        
        // Use part of fuzzer input for directory number if available
        tdir_t dir_num = 0;
        if (Size >= sizeof(uint64_t) + sizeof(tdir_t)) {
            memcpy(&dir_num, Data + sizeof(uint64_t), sizeof(tdir_t));
        }
        TIFFUnlinkDirectory(tif3, dir_num);
        
        TIFFClose(tif3);
        // Second close as per specification (though unusual)
        TIFFClose(tif3);
    }
    
    // Cleanup
    cleanup_file();
    
    return 0;
}