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

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    uint64_t offset1 = TIFFCurrentDirOffset(tif);
    
    tdir_t dir_idx = 0;
    if (Size >= 12) {
        memcpy(&dir_idx, Data + 8, sizeof(tdir_t));
    }
    TIFFSetDirectory(tif, dir_idx);
    
    uint64_t offset2 = TIFFCurrentDirOffset(tif);
    
    int read_result = TIFFReadDirectory(tif);
    
    uint64_t offset3 = TIFFCurrentDirOffset(tif);
    
    tdir_t dir_idx2 = 0;
    tdir_t dir_idx3 = 0;
    tdir_t dir_idx4 = 0;
    if (Size >= 24) {
        memcpy(&dir_idx2, Data + 12, sizeof(tdir_t));
        memcpy(&dir_idx3, Data + 16, sizeof(tdir_t));
        memcpy(&dir_idx4, Data + 20, sizeof(tdir_t));
    }
    TIFFSetDirectory(tif, dir_idx2);
    TIFFSetDirectory(tif, dir_idx3);
    TIFFSetDirectory(tif, dir_idx4);
    
    uint64_t subdir_offset1 = 0;
    uint64_t subdir_offset2 = 0;
    uint64_t subdir_offset3 = 0;
    uint64_t subdir_offset4 = 0;
    if (Size >= 56) {
        memcpy(&subdir_offset1, Data + 24, sizeof(uint64_t));
        memcpy(&subdir_offset2, Data + 32, sizeof(uint64_t));
        memcpy(&subdir_offset3, Data + 40, sizeof(uint64_t));
        memcpy(&subdir_offset4, Data + 48, sizeof(uint64_t));
    }
    TIFFSetSubDirectory(tif, subdir_offset1);
    TIFFSetSubDirectory(tif, subdir_offset2);
    TIFFSetSubDirectory(tif, subdir_offset3);
    TIFFSetSubDirectory(tif, subdir_offset4);
    
    int is_big_endian = TIFFIsBigEndian(tif);
    
    TIFFClose(tif);
    remove_dummy_file();
    
    return 0;
}