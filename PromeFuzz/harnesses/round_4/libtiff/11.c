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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif1 = TIFFOpen("./dummy_file", "r+");
    if (tif1) {
        TIFFWriteDirectory(tif1);
        TIFFClose(tif1);
    }
    
    TIFF *tif2 = TIFFOpen("./dummy_file", "r+");
    if (tif2) {
        TIFFRewriteDirectory(tif2);
        TIFFCreateDirectory(tif2);
        TIFFWriteDirectory(tif2);
        TIFFNumberOfDirectories(tif2);
        TIFFClose(tif2);
    }
    
    TIFF *tif3 = TIFFOpen("./dummy_file", "r+");
    if (tif3) {
        uint64_t offset = 0;
        if (Size >= sizeof(uint64_t)) {
            memcpy(&offset, Data, sizeof(uint64_t));
        }
        TIFFSetSubDirectory(tif3, offset);
        TIFFWriteDirectory(tif3);
        TIFFNumberOfDirectories(tif3);
        
        tdir_t dir_num = 0;
        if (Size >= sizeof(tdir_t) + sizeof(uint64_t)) {
            memcpy(&dir_num, Data + sizeof(uint64_t), sizeof(tdir_t));
        }
        TIFFUnlinkDirectory(tif3, dir_num);
        
        TIFFClose(tif3);
    }
    
    cleanup_dummy_file();
    return 0;
}