// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove_dummy_file();
        return 0;
    }

    tmsize_t max_alloc = *(tmsize_t*)Data;
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_alloc);

    const char *mode = "w";
    TIFF *tif = TIFFOpenExt("./dummy_file", mode, opts);
    
    TIFFOpenOptionsFree(opts);
    
    if (!tif) {
        remove_dummy_file();
        return 0;
    }

    TIFFClose(tif);

    tif = TIFFOpenExt("./dummy_file", mode, NULL);
    if (!tif) {
        remove_dummy_file();
        return 0;
    }

    uint32_t tag1 = *(uint32_t*)Data;
    uint32_t value1 = *(uint32_t*)(Data + 4);
    TIFFSetField(tif, tag1, value1);

    uint32_t tag2 = *(uint32_t*)(Data + 8) % 65536;
    uint32_t value2 = *(uint32_t*)(Data + 12);
    TIFFSetField(tif, tag2, value2);

    TIFFWriteDirectory(tif);
    TIFFClose(tif);

    remove_dummy_file();
    return 0;
}