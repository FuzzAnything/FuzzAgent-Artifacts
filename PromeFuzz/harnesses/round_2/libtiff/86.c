// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    
    return (written == (ssize_t)size) ? 0 : -1;
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    int result;
    
    result = TIFFIsUpSampled(tif);
    
    int dir_count = 0;
    while (TIFFReadDirectory(tif) == 1) {
        dir_count++;
        if (dir_count > 10) break;
        
        TIFFIsUpSampled(tif);
    }
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    TIFFRewriteDirectory(tif);
    
    TIFFCreateDirectory(tif);
    
    TIFFCheckpointDirectory(tif);
    
    TIFFWriteDirectory(tif);
    
    TIFFSetDirectory(tif, 0);
    if (TIFFReadDirectory(tif) == 1) {
        TIFFIsUpSampled(tif);
        TIFFRewriteDirectory(tif);
        TIFFCheckpointDirectory(tif);
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}