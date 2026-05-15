// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        uint32_t tag = 0;
        TIFFDataType dtype = TIFF_NOTYPE;
        
        if (Size >= sizeof(uint32_t) + sizeof(TIFFDataType)) {
            memcpy(&tag, Data, sizeof(uint32_t));
            memcpy(&dtype, Data + sizeof(uint32_t), sizeof(TIFFDataType));
        } else if (Size >= sizeof(uint32_t)) {
            memcpy(&tag, Data, sizeof(uint32_t));
        }
        
        const TIFFField *found_field = TIFFFindField(tif, tag, dtype);
        
        if (found_field) {
            int size1 = TIFFFieldSetGetSize(found_field);
            int size2 = TIFFFieldSetGetCountSize(found_field);
            int write_count = TIFFFieldWriteCount(found_field);
            int pass_count = TIFFFieldPassCount(found_field);
            int read_count = TIFFFieldReadCount(found_field);
            
            (void)size1;
            (void)size2;
            (void)write_count;
            (void)pass_count;
            (void)read_count;
        }
        
        TIFFClose(tif);
    }
    
    unlink("./dummy_file");
    
    TIFFFieldSetGetSize(NULL);
    TIFFFieldSetGetCountSize(NULL);
    
    return 0;
}