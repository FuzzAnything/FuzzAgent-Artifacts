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
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) return 0;
    }
    
    TIFFFreeDirectory(tif);
    
    const TIFFFieldArray *dummyFieldArray = NULL;
    int create_result = TIFFCreateCustomDirectory(tif, dummyFieldArray);
    
    uint32_t tag1 = *((uint32_t*)Data);
    uint32_t tag2 = *((uint32_t*)(Data + 4));
    uint32_t value1 = *((uint32_t*)(Data + 8));
    uint32_t value2 = *((uint32_t*)(Data + 12));
    
    if (create_result == 1) {
        TIFFSetField(tif, tag1, value1);
        TIFFSetField(tif, tag2, value2);
        
        uint64_t offset = 0;
        TIFFWriteCustomDirectory(tif, &offset);
    }
    
    tdir_t dir_num = 0;
    TIFFSetDirectory(tif, dir_num);
    
    TIFFSetField(tif, tag1, value2);
    TIFFSetField(tif, tag2, value1);
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t get_value1, get_value2;
    TIFFGetField(tif, tag1, &get_value1);
    TIFFGetField(tif, tag2, &get_value2);
    
    toff_t diroff = *((toff_t*)(Data + 16));
    TIFFReadEXIFDirectory(tif, diroff);
    
    TIFFGetField(tif, tag1, &get_value1);
    
    TIFFReadCustomDirectory(tif, diroff, dummyFieldArray);
    
    TIFFGetField(tif, tag1, &get_value1);
    TIFFGetField(tif, tag2, &get_value2);
    
    TIFFClose(tif);
    
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        TIFFClose(tif);
    }
    
    unlink("./dummy_file");
    return 0;
}