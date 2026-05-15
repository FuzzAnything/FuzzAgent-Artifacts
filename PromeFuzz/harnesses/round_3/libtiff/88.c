// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static tmsize_t dummy_read(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    return size;
}

static tmsize_t dummy_write(thandle_t handle, void* buf, tmsize_t size) {
    (void)handle;
    (void)buf;
    return size;
}

static toff_t dummy_seek(thandle_t handle, toff_t offset, int whence) {
    (void)handle;
    (void)offset;
    (void)whence;
    return 0;
}

static int dummy_close(thandle_t handle) {
    (void)handle;
    return 0;
}

static toff_t dummy_size(thandle_t handle) {
    (void)handle;
    return 0;
}

static int dummy_map(thandle_t handle, void** base, toff_t* offset) {
    (void)handle;
    (void)base;
    (void)offset;
    return 0;
}

static void dummy_unmap(thandle_t handle, void* base, toff_t offset) {
    (void)handle;
    (void)base;
    (void)offset;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    FILE* dummy = fopen("./dummy_file", "wb");
    if (!dummy) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy);
    fclose(dummy);

    TIFF* tif = TIFFClientOpen(
        "./dummy_file",
        "r",
        (thandle_t)1,
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
        TIFFUnmapFileProc unmap_proc = TIFFGetUnmapFileProc(tif);
        TIFFReadWriteProc write_proc = TIFFGetWriteProc(tif);
        TIFFReadWriteProc read_proc = TIFFGetReadProc(tif);
        TIFFMapFileProc map_proc = TIFFGetMapFileProc(tif);

        if (seek_proc) {
            seek_proc((thandle_t)1, 0, SEEK_SET);
        }

        if (unmap_proc) {
            unmap_proc((thandle_t)1, NULL, 0);
        }

        if (write_proc) {
            char buf[1] = {0};
            write_proc((thandle_t)1, buf, 1);
        }

        if (read_proc) {
            char buf[1];
            read_proc((thandle_t)1, buf, 1);
        }

        if (map_proc) {
            void* base = NULL;
            toff_t offset = 0;
            map_proc((thandle_t)1, &base, &offset);
        }

        TIFFClose(tif);
    }

    remove("./dummy_file");
    return 0;
}