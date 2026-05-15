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
#include "tiffio.h"
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

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    uint64_t offset1 = TIFFCurrentDirOffset(tif);
    
    tdir_t dir1 = (tdir_t)(Data[0] & 0x0F);
    TIFFSetDirectory(tif, dir1);
    
    uint64_t offset2 = TIFFCurrentDirOffset(tif);
    
    int read_result = TIFFReadDirectory(tif);
    (void)read_result;
    
    uint64_t offset3 = TIFFCurrentDirOffset(tif);
    
    tdir_t dir2 = (tdir_t)((Data[0] >> 4) & 0x0F);
    TIFFSetDirectory(tif, dir2);
    
    tdir_t dir3 = (tdir_t)((Size > 1 ? Data[1] : 0) & 0x0F);
    TIFFSetDirectory(tif, dir3);
    
    tdir_t dir4 = (tdir_t)((Size > 1 ? Data[1] >> 4 : 0) & 0x0F);
    TIFFSetDirectory(tif, dir4);
    
    uint64_t subdir1 = (uint64_t)(Size > 2 ? Data[2] : 0);
    TIFFSetSubDirectory(tif, subdir1);
    
    uint64_t subdir2 = (uint64_t)(Size > 3 ? Data[3] : 0);
    TIFFSetSubDirectory(tif, subdir2);
    
    uint64_t subdir3 = (uint64_t)(Size > 4 ? Data[4] : 0);
    TIFFSetSubDirectory(tif, subdir3);
    
    uint64_t subdir4 = (uint64_t)(Size > 5 ? Data[5] : 0);
    TIFFSetSubDirectory(tif, subdir4);
    
    int is_big_endian = TIFFIsBigEndian(tif);
    (void)is_big_endian;
    
    TIFFClose(tif);
    remove_dummy_file();
    
    return 0;
}