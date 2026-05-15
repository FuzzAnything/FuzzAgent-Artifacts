// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tiffio.h"

static tsize_t dummy_read(thandle_t handle, tdata_t buf, tsize_t size) {
    FILE* file = (FILE*)handle;
    return fread(buf, 1, size, file);
}

static tsize_t dummy_write(thandle_t handle, tdata_t buf, tsize_t size) {
    FILE* file = (FILE*)handle;
    return fwrite(buf, 1, size, file);
}

static toff_t dummy_seek(thandle_t handle, toff_t offset, int whence) {
    FILE* file = (FILE*)handle;
    if (fseek(file, offset, whence) != 0) {
        return -1;
    }
    return ftell(file);
}

static int dummy_close(thandle_t handle) {
    FILE* file = (FILE*)handle;
    return fclose(file);
}

static toff_t dummy_size(thandle_t handle) {
    FILE* file = (FILE*)handle;
    long current = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, current, SEEK_SET);
    return size;
}

static int dummy_map(thandle_t handle, tdata_t* base, toff_t* size) {
    return 0;
}

static void dummy_unmap(thandle_t handle, tdata_t base, toff_t size) {
    return;
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    FILE* file = fopen("./dummy_file", "wb");
    if (!file) {
        return 0;
    }
    fwrite(Data, 1, Size, file);
    fclose(file);

    file = fopen("./dummy_file", "rb");
    if (!file) {
        return 0;
    }

    TIFF* tif = TIFFClientOpen(
        "dummy", 
        "r", 
        (thandle_t)file,
        dummy_read,
        dummy_write,
        dummy_seek,
        dummy_close,
        dummy_size,
        dummy_map,
        dummy_unmap
    );

    if (tif) {
        TIFFSeekProc seek_proc = TIFFGetSeekProc(tif);
        if (seek_proc) {
            seek_proc((thandle_t)file, Size % 100, SEEK_SET);
        }

        TIFFReadWriteProc read_proc = TIFFGetReadProc(tif);
        if (read_proc) {
            char buf[10];
            read_proc((thandle_t)file, buf, sizeof(buf));
        }

        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        if (write_proc) {
            char buf[10] = {0};
            write_proc((thandle_t)file, buf, sizeof(buf));
        }

        TIFFMapFileProc map_proc = TIFFGetMapFileProc(tif);
        if (map_proc) {
            tdata_t base;
            toff_t size;
            map_proc((thandle_t)file, &base, &size);
        }

        TIFFUnmapFileProc unmap_proc = TIFFGetUnmapFileProc(tif);
        if (unmap_proc) {
            unmap_proc((thandle_t)file, NULL, 0);
        }

        TIFFClose(tif);
    } else {
        fclose(file);
    }

    remove("./dummy_file");
    return 0;
}