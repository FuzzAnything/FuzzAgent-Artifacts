// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
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

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        cleanup_dummy_file();
        return 0;
    }
    
    tmsize_t max_mem = (tmsize_t)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, max_mem);
    
    TIFF* tif = TIFFOpenExt("./dummy_file", "w", opts);
    TIFFOpenOptionsFree(opts);
    
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    TIFFClose(tif);
    
    tif = TIFFOpenExt("./dummy_file", "w", NULL);
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint32_t tag1 = (uint32_t)(Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24));
    uint32_t value1 = (uint32_t)(Data[4 % Size] | (Data[(4 + 1) % Size] << 8) | 
                                 (Data[(4 + 2) % Size] << 16) | (Data[(4 + 3) % Size] << 24));
    TIFFSetField(tif, tag1, value1);
    
    uint32_t tag2 = (uint32_t)(Data[8 % Size] | (Data[(8 + 1) % Size] << 8) | 
                               (Data[(8 + 2) % Size] << 16) | (Data[(8 + 3) % Size] << 24));
    uint32_t value2 = (uint32_t)(Data[12 % Size] | (Data[(12 + 1) % Size] << 8) | 
                                 (Data[(12 + 2) % Size] << 16) | (Data[(12 + 3) % Size] << 24));
    TIFFSetField(tif, tag2, value2);
    
    TIFFWriteDirectory(tif);
    TIFFClose(tif);
    
    cleanup_dummy_file();
    return 0;
}