// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenOptionsSetWarnAboutUnknownTags at tif_open.c:115:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
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

static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {}
static void dummy_warn_handler(const char* module, const char* fmt, va_list ap) {}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const char* filename = "./dummy_file";
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return 0;
    
    write(fd, Data, Size);
    close(fd);
    
    TIFFOpenOptions* opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        unlink(filename);
        return 0;
    }
    
    uint8_t selector = Data[0];
    
    if (Size > 1) {
        tmsize_t max_single = (tmsize_t)Data[1] * 1024;
        TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_single);
    }
    
    if (Size > 2) {
        tmsize_t max_cumulated = (tmsize_t)Data[2] * 1024;
        TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, max_cumulated);
    }
    
    if (Size > 3) {
        int warn_unknown = Data[3] & 1;
        TIFFOpenOptionsSetWarnAboutUnknownTags(opts, warn_unknown);
    }
    
    const char* mode = "r";
    if (selector & 0x20) {
        mode = "r+";
    }
    
    TIFF* tif = TIFFOpenExt(filename, mode, opts);
    
    if (tif) {
        TIFFClose(tif);
    }
    
    TIFFOpenOptionsFree(opts);
    unlink(filename);
    
    return 0;
}