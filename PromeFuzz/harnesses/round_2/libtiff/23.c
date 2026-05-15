// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "w8");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    TIFFWriteDirectory(tif);
    
    tdir_t dir_index = (tdir_t)(Data[0] % 10);
    TIFFSetDirectory(tif, dir_index);
    
    uint64_t offset = TIFFCurrentDirOffset(tif);
    (void)offset;
    
    if (Size > 1) {
        dir_index = (tdir_t)(Data[1] % 10);
        TIFFSetDirectory(tif, dir_index);
    }
    
    TIFFForceStrileArrayWriting(tif);
    
    TIFFFlushData(tif);
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}