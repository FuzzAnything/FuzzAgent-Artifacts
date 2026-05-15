// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldPassCount at tif_dirinfo.c:873:5 in tiffio.h
// TIFFFieldReadCount at tif_dirinfo.c:875:5 in tiffio.h
// TIFFFieldSetGetSize at tif_dirinfo.c:673:5 in tiffio.h
// TIFFFieldSetGetCountSize at tif_dirinfo.c:757:5 in tiffio.h
// TIFFFieldWriteCount at tif_dirinfo.c:877:5 in tiffio.h
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

static void create_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    create_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }

    uint32_t tag = *(const uint32_t *)Data;
    TIFFDataType type = (TIFFDataType)(*(const uint32_t *)(Data + 4) % 14);
    
    const TIFFField *field = TIFFFindField(tif, tag, type);
    
    if (field) {
        int pass_count = TIFFFieldPassCount(field);
        int read_count = TIFFFieldReadCount(field);
        int set_get_size = TIFFFieldSetGetSize(field);
        int set_get_count_size = TIFFFieldSetGetCountSize(field);
        int write_count = TIFFFieldWriteCount(field);
        
        (void)pass_count;
        (void)read_count;
        (void)set_get_size;
        (void)set_get_count_size;
        (void)write_count;
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}