// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetMapFileProc at tif_open.c:937:17 in tiffio.h
// TIFFGetUnmapFileProc at tif_open.c:942:19 in tiffio.h
// TIFFGetWriteProc at tif_open.c:917:19 in tiffio.h
// TIFFGetSeekProc at tif_open.c:922:14 in tiffio.h
// TIFFGetReadProc at tif_open.c:912:19 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
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

/* Custom I/O handlers for TIFFClientOpen */
static tmsize_t customRead(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    return fread(buf, 1, size, file);
}

static tmsize_t customWrite(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    return fwrite(buf, 1, size, file);
}

static toff_t customSeek(thandle_t handle, toff_t offset, int whence) {
    FILE* file = (FILE*)handle;
    if (fseek(file, offset, whence) == 0) {
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
    return size;
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

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Write fuzzer data to dummy file */
    FILE* dummy = fopen("./dummy_file", "wb");
    if (!dummy) return 0;
    fwrite(Data, 1, Size, dummy);
    fclose(dummy);
    
    /* Test with TIFFOpen first */
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        /* Test the getter functions */
        TIFFMapFileProc mapProc = TIFFGetMapFileProc(tif);
        TIFFUnmapFileProc unmapProc = TIFFGetUnmapFileProc(tif);
        TIFFReadWriteProc writeProc = TIFFGetWriteProc(tif);
        TIFFSeekProc seekProc = TIFFGetSeekProc(tif);
        TIFFReadWriteProc readProc = TIFFGetReadProc(tif);
        
        /* Use the function pointers to avoid unused variable warnings */
        (void)mapProc;
        (void)unmapProc;
        (void)writeProc;
        (void)seekProc;
        (void)readProc;
        
        TIFFClose(tif);
    }
    
    /* Test with TIFFClientOpen */
    dummy = fopen("./dummy_file", "rb");
    if (dummy) {
        TIFF* clientTif = TIFFClientOpen("client", "r", 
                                         (thandle_t)dummy,
                                         customRead,
                                         customWrite,
                                         customSeek,
                                         customClose,
                                         customSize,
                                         customMap,
                                         customUnmap);
        
        if (clientTif) {
            /* Test the getter functions on client-opened TIFF */
            TIFFMapFileProc clientMapProc = TIFFGetMapFileProc(clientTif);
            TIFFUnmapFileProc clientUnmapProc = TIFFGetUnmapFileProc(clientTif);
            TIFFReadWriteProc clientWriteProc = TIFFGetWriteProc(clientTif);
            TIFFSeekProc clientSeekProc = TIFFGetSeekProc(clientTif);
            TIFFReadWriteProc clientReadProc = TIFFGetReadProc(clientTif);
            
            /* Use the function pointers */
            (void)clientMapProc;
            (void)clientUnmapProc;
            (void)clientWriteProc;
            (void)clientSeekProc;
            (void)clientReadProc;
            
            TIFFClose(clientTif);
        } else {
            fclose(dummy);
        }
    }
    
    /* Clean up dummy file */
    remove("./dummy_file");
    
    return 0;
}