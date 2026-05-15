// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenOptionsSetWarnAboutUnknownTags at tif_open.c:115:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (!opts) {
        remove_dummy_file();
        return 0;
    }
    
    // Use input bytes to determine option values
    // Ensure values are within reasonable bounds to avoid overflow
    tmsize_t max_cumulated = (tmsize_t)(Data[0] * 1024);
    tmsize_t max_single = (tmsize_t)(Data[1] * 1024);
    int warn_unknown = Data[2] & 1;
    
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, max_cumulated);
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_single);
    TIFFOpenOptionsSetWarnAboutUnknownTags(opts, warn_unknown);
    
    // Try different modes to explore more states
    const char *modes[] = {"r", "r+", "w", "w+"};
    int mode_idx = Data[3] % 4;
    
    TIFF *tif = TIFFOpenExt("./dummy_file", modes[mode_idx], opts);
    
    if (tif) {
        // Perform minimal operations to explore more states
        // Try to read basic directory information
        uint32_t dir_count = 0;
        if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &dir_count)) {
            // Field read successfully
        }
        
        // Try to set a simple tag
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        
        TIFFClose(tif);
    }
    
    TIFFOpenOptionsFree(opts);
    remove_dummy_file();
    
    return 0;
}