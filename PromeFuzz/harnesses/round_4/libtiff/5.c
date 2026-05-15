// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    uint32_t dir_index = 0;
    if (Size >= 8) {
        memcpy(&dir_index, Data, sizeof(dir_index));
    }
    
    TIFFSetDirectory(tif, dir_index % 10);
    
    if (Size >= 12) {
        uint32_t dir_index2 = 0;
        memcpy(&dir_index2, Data + 4, sizeof(dir_index2));
        TIFFSetDirectory(tif, dir_index2 % 10);
    }
    
    uint64_t offset = TIFFCurrentDirOffset(tif);
    (void)offset;
    
    TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
    (void)seek_proc;
    
    int swapped = TIFFIsByteSwapped(tif);
    (void)swapped;
    
    uint32_t long_val = 0;
    if (Size >= 16) {
        memcpy(&long_val, Data + 8, sizeof(long_val));
    }
    TIFFSwabLong(&long_val);
    
    TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
    (void)write_proc;
    
    tdir_t num_dirs = TIFFNumberOfDirectories(tif);
    (void)num_dirs;
    
    if (Size >= 20) {
        uint32_t dir_index3 = 0;
        memcpy(&dir_index3, Data + 12, sizeof(dir_index3));
        TIFFSetDirectory(tif, dir_index3 % 10);
    }
    
    if (Size >= 24) {
        uint32_t dir_index4 = 0;
        memcpy(&dir_index4, Data + 16, sizeof(dir_index4));
        TIFFSetDirectory(tif, dir_index4 % 10);
    }
    
    TIFFClose(tif);
    
    TIFF *tif2 = TIFFOpen("./dummy_file", "r");
    if (tif2) {
        TIFFClose(tif2);
    }
    
    remove_dummy_file();
    return 0;
}