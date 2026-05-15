// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "tiffio.h"

typedef struct {
    const uint8_t* data;
    size_t size;
    size_t pos;
} mem_buffer;

static tmsize_t dummy_read(thandle_t handle, void* buf, tmsize_t size) {
    mem_buffer* mb = (mem_buffer*)handle;
    tmsize_t to_read = size;
    if (mb->pos >= mb->size) return 0;
    if (to_read > mb->size - mb->pos) to_read = mb->size - mb->pos;
    memcpy(buf, mb->data + mb->pos, to_read);
    mb->pos += to_read;
    return to_read;
}

static tmsize_t dummy_write(thandle_t handle, void* buf, tmsize_t size) {
    return 0;
}

static toff_t dummy_seek(thandle_t handle, toff_t offset, int whence) {
    mem_buffer* mb = (mem_buffer*)handle;
    switch (whence) {
        case SEEK_SET:
            mb->pos = offset;
            break;
        case SEEK_CUR:
            mb->pos += offset;
            break;
        case SEEK_END:
            mb->pos = mb->size + offset;
            break;
        default:
            return (toff_t)-1;
    }
    if (mb->pos < 0 || mb->pos > mb->size) return (toff_t)-1;
    return mb->pos;
}

static int dummy_close(thandle_t handle) {
    return 0;
}

static toff_t dummy_size(thandle_t handle) {
    mem_buffer* mb = (mem_buffer*)handle;
    return mb->size;
}

static int dummy_map(thandle_t handle, void** base, toff_t* size) {
    return 0;
}

static void dummy_unmap(thandle_t handle, void* base, toff_t size) {
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const char* filename = "./dummy_file";
    
    FILE* f = fopen(filename, "wb");
    if (!f) return 0;
    fwrite(Data, 1, Size, f);
    fclose(f);
    
    TIFF* tif = NULL;
    
    if (Size % 2 == 0) {
        tif = TIFFOpen(filename, "r");
    } else {
        tif = TIFFOpenExt(filename, "r", NULL);
    }
    
    if (tif) {
        const char* name = TIFFFileName(tif);
        if (name) {
            TIFFSetFileName(tif, "new_name");
        }
        TIFFClose(tif);
    }
    
    int fd = open(filename, O_RDONLY);
    if (fd >= 0) {
        tif = TIFFFdOpen(fd, filename, "r");
        if (tif) {
            TIFFClose(tif);
        } else {
            close(fd);
        }
    }
    
    mem_buffer mb = {Data, Size, 0};
    
    tif = TIFFClientOpenExt("memory", "r", 
                           (thandle_t)&mb,
                           dummy_read, dummy_write,
                           dummy_seek, dummy_close, dummy_size,
                           dummy_map, dummy_unmap,
                           NULL);
    if (tif) {
        TIFFClose(tif);
    }
    
    unlink(filename);
    return 0;
}