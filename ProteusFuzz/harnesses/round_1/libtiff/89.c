// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetClientdata at tif_open.c:833:11 in tiffio.h
// TIFFClientdata at tif_open.c:828:11 in tiffio.h
// TIFFSetClientInfo at tif_extension.c:78:6 in tiffio.h
// TIFFPrintDirectory at tif_print.c:244:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFCleanup at tif_close.c:44:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

typedef struct {
    uint8_t *base;
    size_t size;
    uint8_t *current;
} DummyClientData;

static DummyClientData g_client_data;

static tmsize_t
DummyReadProc(thandle_t cd, tdata_t buf, tmsize_t size) {
    if (!cd || size <= 0) return 0;
    DummyClientData *data = (DummyClientData *)cd;
    tmsize_t remaining = data->size - (data->current - data->base);
    if (size > remaining) size = remaining;
    if (size <= 0) return 0;
    memcpy(buf, data->current, size);
    data->current += size;
    return size;
}

static tmsize_t
DummyWriteProc(thandle_t cd, const tdata_t buf, tmsize_t size) {
    return size;
}

static toff_t
DummySeekProc(thandle_t cd, toff_t off, int whence) {
    if (!cd) return -1;
    DummyClientData *data = (DummyClientData *)cd;
    if (whence == SEEK_SET) {
        if (off < 0 || (size_t)off > data->size) return -1;
        data->current = data->base + off;
        return 0;
    } else if (whence == SEEK_CUR) {
        if (off < 0 && (size_t)(-off) > (data->current - data->base)) return -1;
        if (off > 0 && (size_t)off > (data->size - (data->current - data->base))) return -1;
        data->current += off;
        return 0;
    } else if (whence == SEEK_END) {
        if (off > 0) return -1;
        size_t new_pos = data->size + off;
        data->current = data->base + new_pos;
        return 0;
    }
    return -1;
}

static int
DummyCloseProc(thandle_t cd) {
    return 0;
}

static tmsize_t
DummySizeProc(thandle_t cd) {
    if (!cd) return 0;
    DummyClientData *data = (DummyClientData *)cd;
    return data->size;
}

static tdata_t
DummyMapFileProc(thandle_t cd, toff_t *pfileoff) {
    return NULL;
}

static void
DummyUnmapFileProc(thandle_t cd, tdata_t base, toff_t off) {
}

int
LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    g_client_data.base = (uint8_t *)Data;
    g_client_data.size = Size;
    g_client_data.current = (uint8_t *)Data;

    // Initialize options
    TIFFOpenOptions *opts = NULL;

    // 1. Test TIFFClientOpenExt
    TIFF *tif = TIFFClientOpenExt(
        "dummy_file", 
        "r", 
        (thandle_t)&g_client_data,
        DummyReadProc,
        DummyWriteProc,
        DummySeekProc,
        DummyCloseProc,
        (TIFFSizeProc)DummySizeProc,
        (TIFFMapFileProc)DummyMapFileProc,
        (TIFFUnmapFileProc)DummyUnmapFileProc,
        opts
    );

    if (tif) {
        // 2. Test TIFFSetClientdata
        void *old_data = TIFFSetClientdata(tif, (thandle_t)Data);
        (void)old_data;

        // 3. Test TIFFClientdata
        thandle_t retrieved = TIFFClientdata(tif);
        (void)retrieved;

        // 4. Test TIFFSetClientInfo
        char *info_name = (char *)malloc(Size > 10 ? 10 : Size);
        if (info_name) {
            strncpy(info_name, "fuzz_info", Size > 10 ? 10 : Size);
            info_name[Size > 10 ? 9 : Size-1] = '\0';
            TIFFSetClientInfo(tif, (void *)Data, info_name);
            free(info_name);
        }

        // 5. Test TIFFPrintDirectory
        FILE *stream = stderr;
        long flags = 0;
        if (Size > 0) {
            flags = (long)Data[0];
        }
        TIFFPrintDirectory(tif, stream, flags);

        // Cleanup
        TIFFClose(tif);
        TIFFCleanup(tif);
    }

    return 0;
}