// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "libtiff/tiffio.h"

static tmsize_t customRead(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    return (tmsize_t)fread(buf, 1, size, file);
}

static tmsize_t customWrite(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    return (tmsize_t)fwrite(buf, 1, size, file);
}

static toff_t customSeek(thandle_t handle, toff_t offset, int whence) {
    FILE* file = (FILE*)handle;
    if (fseek(file, (long)offset, whence) == 0) {
        return ftell(file);
    }
    return (toff_t)-1;
}

static int customClose(thandle_t handle) {
    FILE* file = (FILE*)handle;
    return fclose(file);
}

static toff_t customSize(thandle_t handle) {
    FILE* file = (FILE*)handle;
    long current = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, current, SEEK_SET);
    return (toff_t)size;
}

static int customMap(thandle_t handle, void** base, toff_t* size) {
    (void)handle;
    (void)base;
    (void)size;
    return 0;
}

static void customUnmap(thandle_t handle, void* base, toff_t size) {
    (void)handle;
    (void)base;
    (void)size;
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    const char* filename = "./dummy_file";
    FILE* file = fopen(filename, "wb");
    if (!file) {
        return 0;
    }
    fwrite(Data, 1, Size, file);
    fclose(file);

    TIFFOpenOptions* opts = NULL;
    
    TIFF* tif = TIFFOpenExt(filename, "r", opts);
    if (tif) {
        const char* name = TIFFFileName(tif);
        if (name) {
            const char* new_name = "test_name.tiff";
            const char* old_name = TIFFSetFileName(tif, new_name);
            (void)old_name;
        }
        TIFFClose(tif);
    }

    tif = TIFFOpen(filename, "r");
    if (tif) {
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

    file = fopen(filename, "rb");
    if (file) {
        tif = TIFFClientOpenExt(filename, "r", (thandle_t)file,
                                customRead, customWrite,
                                customSeek, customClose, customSize,
                                customMap, customUnmap,
                                opts);
        if (tif) {
            TIFFClose(tif);
        } else {
            fclose(file);
        }
    }

    unlink(filename);
    return 0;
}