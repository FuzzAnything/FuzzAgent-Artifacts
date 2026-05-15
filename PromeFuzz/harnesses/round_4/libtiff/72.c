// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetMode at tif_open.c:843:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFRewriteDirectory at tif_dirwrite.c:483:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
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

#include "tiffio.h"

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return 0;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return written == size;
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }
    
    TIFF* tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        tif = TIFFOpen("./dummy_file", "r");
    }
    
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    int mode = TIFFGetMode(tif);
    
    if (mode & O_RDWR || mode & O_WRONLY) {
        TIFFSetupStrips(tif);
        
        TIFFDeferStrileArrayWriting(tif);
        
        TIFFCheckpointDirectory(tif);
        
        TIFFForceStrileArrayWriting(tif);
        
        TIFFRewriteDirectory(tif);
        
        TIFFFlush(tif);
    } else {
        TIFFFlush(tif);
        
        TIFFCheckpointDirectory(tif);
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}