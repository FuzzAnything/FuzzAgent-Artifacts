// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
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

static tmsize_t dummy_read(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return -1;
}

static tmsize_t dummy_write(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    (void)size;
    return -1;
}

static toff_t dummy_seek(thandle_t handle, toff_t offset, int whence) {
    (void)handle;
    (void)offset;
    (void)whence;
    return -1;
}

static int dummy_close(thandle_t handle) {
    (void)handle;
    return 0;
}

static toff_t dummy_size(thandle_t handle) {
    (void)handle;
    return 0;
}

static int dummy_map(thandle_t handle, void** base, toff_t* size) {
    (void)handle;
    (void)base;
    (void)size;
    return 0;
}

static void dummy_unmap(thandle_t handle, void* base, toff_t size) {
    (void)handle;
    (void)base;
    (void)size;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    const char* filename = "./dummy_file";
    
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    uint8_t mode_selector = Data[0] % 3;
    const char* mode;
    switch (mode_selector) {
        case 0: mode = "r"; break;
        case 1: mode = "w"; break;
        default: mode = "a"; break;
    }
    
    TIFF* tif = TIFFOpen(filename, mode);
    if (tif) {
        const char* name = TIFFFileName(tif);
        (void)name;
        
        const char* new_name = "test_name";
        const char* old_name = TIFFSetFileName(tif, new_name);
        (void)old_name;
        
        TIFFClose(tif);
    }
    
    TIFFOpenOptions* opts = NULL;
    
    tif = TIFFOpenExt(filename, mode, opts);
    if (tif) {
        TIFFClose(tif);
    }
    
    int fd = open(filename, O_RDONLY);
    if (fd >= 0) {
        tif = TIFFFdOpenExt(fd, filename, mode, opts);
        if (tif) {
            TIFFClose(tif);
        } else {
            close(fd);
        }
    }
    
    thandle_t dummy_handle = (thandle_t)12345;
    tif = TIFFClientOpenExt(filename, mode, dummy_handle,
                           dummy_read, dummy_write,
                           dummy_seek, dummy_close,
                           dummy_size, dummy_map,
                           dummy_unmap, opts);
    if (tif) {
        TIFFClose(tif);
    }
    
    unlink(filename);
    return 0;
}