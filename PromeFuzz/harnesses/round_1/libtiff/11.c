// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
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

static int write_to_file(const char* filename, const uint8_t* data, size_t size) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    ssize_t written = write(fd, data, size);
    close(fd);
    return written == (ssize_t)size ? 0 : -1;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    const char* filename = "./dummy_file";
    if (write_to_file(filename, Data, Size) != 0) {
        return 0;
    }
    
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        unlink(filename);
        return 0;
    }
    
    tmsize_t strip_size = TIFFStripSize(tif);
    void* buffer = _TIFFmalloc(strip_size);
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    
    if (num_strips > 0 && buffer) {
        for (uint32_t strip = 0; strip < num_strips && strip < 10; strip++) {
            TIFFReadEncodedStrip(tif, strip, buffer, strip_size);
        }
    }
    
    if (buffer) {
        _TIFFfree(buffer);
    }
    _TIFFfree(NULL);
    
    TIFFClose(tif);
    unlink(filename);
    
    return 0;
}