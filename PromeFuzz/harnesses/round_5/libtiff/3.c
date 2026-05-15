// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
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

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            return 0;
        }
    }
    
    TIFFFreeDirectory(tif);
    
    const TIFFFieldArray *infoarray = NULL;
    TIFFCreateCustomDirectory(tif, infoarray);
    
    uint32_t tag1 = *(uint32_t*)Data;
    uint32_t value1 = *(uint32_t*)(Data + 4);
    TIFFSetField(tif, tag1, value1);
    
    uint32_t tag2 = *(uint32_t*)(Data + 8);
    uint32_t value2 = *(uint32_t*)(Data + 12);
    TIFFSetField(tif, tag2, value2);
    
    uint64_t offset = 0;
    TIFFWriteCustomDirectory(tif, &offset);
    
    tdir_t dir_num = *(tdir_t*)(Data + 16) % 10;
    TIFFSetDirectory(tif, dir_num);
    
    uint32_t tag3 = *(uint32_t*)(Data + 18);
    uint32_t value3 = *(uint32_t*)(Data + 22);
    TIFFSetField(tif, tag3, value3);
    
    uint32_t tag4 = *(uint32_t*)(Data + 26);
    uint32_t value4 = *(uint32_t*)(Data + 30);
    TIFFSetField(tif, tag4, value4);
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }
    
    uint32_t get_val1, get_val2;
    TIFFGetField(tif, tag1, &get_val1);
    TIFFGetField(tif, tag2, &get_val2);
    
    toff_t diroff = *(toff_t*)(Data + 34);
    TIFFReadEXIFDirectory(tif, diroff);
    
    uint32_t get_val3;
    TIFFGetField(tif, tag3, &get_val3);
    
    toff_t custom_diroff = *(toff_t*)(Data + 42);
    TIFFReadCustomDirectory(tif, custom_diroff, infoarray);
    
    uint32_t get_val4, get_val5;
    TIFFGetField(tif, tag4, &get_val4);
    TIFFGetField(tif, tag1, &get_val5);
    
    TIFFClose(tif);
    
    tif = NULL;
    TIFFClose(tif);
    
    unlink("./dummy_file");
    return 0;
}