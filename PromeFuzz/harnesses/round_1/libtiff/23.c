// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void cleanup_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_file();
        return 0;
    }
    
    uint64_t offset1 = TIFFCurrentDirOffset(tif);
    
    tdir_t dir_index = (tdir_t)(Data[0] % 16);
    TIFFSetDirectory(tif, dir_index);
    
    uint64_t offset2 = TIFFCurrentDirOffset(tif);
    
    TIFFReadDirectory(tif);
    
    uint64_t offset3 = TIFFCurrentDirOffset(tif);
    
    tdir_t dir_index2 = (tdir_t)((Size > 1 ? Data[1] : 0) % 16);
    TIFFSetDirectory(tif, dir_index2);
    
    tdir_t dir_index3 = (tdir_t)((Size > 2 ? Data[2] : 0) % 16);
    TIFFSetDirectory(tif, dir_index3);
    
    tdir_t dir_index4 = (tdir_t)((Size > 3 ? Data[3] : 0) % 16);
    TIFFSetDirectory(tif, dir_index4);
    
    uint64_t subdir_offset1 = (Size > 4 ? Data[4] : 0);
    TIFFSetSubDirectory(tif, subdir_offset1);
    
    uint64_t subdir_offset2 = (Size > 5 ? Data[5] : 0);
    TIFFSetSubDirectory(tif, subdir_offset2);
    
    uint64_t subdir_offset3 = (Size > 6 ? Data[6] : 0);
    TIFFSetSubDirectory(tif, subdir_offset3);
    
    uint64_t subdir_offset4 = (Size > 7 ? Data[7] : 0);
    TIFFSetSubDirectory(tif, subdir_offset4);
    
    TIFFIsBigEndian(tif);
    
    TIFFClose(tif);
    cleanup_file();
    
    return 0;
}