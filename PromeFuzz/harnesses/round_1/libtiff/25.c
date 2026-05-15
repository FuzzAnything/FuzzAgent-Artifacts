// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
// _TIFFmemcmp at tif_unix.c:360:5 in tiffio.h
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
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t width = 0;
        uint32_t height = 0;
        uint16_t samples_per_pixel = 0;
        
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
        
        size_t mem_size = Size > 1024 ? 1024 : Size;
        void *mem1 = malloc(mem_size);
        void *mem2 = malloc(mem_size);
        
        if (mem1 && mem2) {
            memset(mem1, 0xAA, mem_size);
            memset(mem2, 0xBB, mem_size);
            
            _TIFFmemcmp(mem1, mem2, (tmsize_t)mem_size);
            
            if (Size >= mem_size) {
                _TIFFmemcmp(Data, mem1, (tmsize_t)mem_size);
                _TIFFmemcmp(Data, Data, (tmsize_t)(mem_size/2));
            } else {
                _TIFFmemcmp(Data, mem1, (tmsize_t)Size);
                _TIFFmemcmp(Data, Data, (tmsize_t)Size);
            }
        }
        
        free(mem1);
        free(mem2);
        
        TIFFClose(tif);
    }
    
    remove_dummy_file();
    return 0;
}