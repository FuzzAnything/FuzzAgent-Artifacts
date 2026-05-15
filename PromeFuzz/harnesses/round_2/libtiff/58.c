// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenOptionsSetWarnAboutUnknownTags at tif_open.c:115:6 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        return 0;
    }

    uint8_t selector = Data[0];
    size_t data_index = 1;

    if (selector & 0x01) {
        tmsize_t max_cumulated = 0;
        if (data_index + sizeof(tmsize_t) <= Size) {
            memcpy(&max_cumulated, Data + data_index, sizeof(tmsize_t));
            data_index += sizeof(tmsize_t);
        }
        TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, max_cumulated);
    }

    if (selector & 0x02) {
        int warn_unknown = 0;
        if (data_index + sizeof(int) <= Size) {
            memcpy(&warn_unknown, Data + data_index, sizeof(int));
            data_index += sizeof(int);
        }
        TIFFOpenOptionsSetWarnAboutUnknownTags(opts, warn_unknown & 1);
    }

    if (selector & 0x04) {
        tmsize_t max_single = 0;
        if (data_index + sizeof(tmsize_t) <= Size) {
            memcpy(&max_single, Data + data_index, sizeof(tmsize_t));
            data_index += sizeof(tmsize_t);
        }
        TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_single);
    }

    const char *mode = "r";
    if (selector & 0x08) {
        mode = "w";
    }

    TIFF *tif = TIFFOpenExt("./dummy_file", mode, opts);
    if (tif) {
        TIFFClose(tif);
    }

    TIFFOpenOptionsFree(opts);
    
    unlink("./dummy_file");
    return 0;
}