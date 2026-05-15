// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFCheckpointDirectory at tif_dirwrite.c:292:5 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetWriteOffset at tif_write.c:961:6 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
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

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return -1;
    fwrite(data, 1, size, fp);
    fclose(fp);
    return 0;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    if (write_dummy_file(Data, Size) != 0) return 0;
    
    TIFF *tif = TIFFOpen("./dummy_file", "r+");
    if (!tif) {
        tif = TIFFOpen("./dummy_file", "r");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
    }
    
    uint64_t offset;
    memcpy(&offset, Data, sizeof(uint64_t));
    
    TIFFSetSubDirectory(tif, offset);
    TIFFCheckpointDirectory(tif);
    TIFFCreateDirectory(tif);
    uint64_t current_offset = TIFFCurrentDirOffset(tif);
    TIFFSetWriteOffset(tif, (toff_t)offset);
    TIFFWriteCustomDirectory(tif, &current_offset);
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}