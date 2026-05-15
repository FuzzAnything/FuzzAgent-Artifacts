// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static TIFF* open_dummy_tiff(const uint8_t* data, size_t size) {
    // Write fuzzer data to dummy file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    write(fd, data, size);
    close(fd);
    
    // Open TIFF file
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    return TIFFOpen("./dummy_file", "r");
}

int LLVMFuzzerTestOneInput_64(const uint8_t* data, size_t size) {
    if (size < 4) return 0;
    
    TIFF* tif = open_dummy_tiff(data, size);
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Extract some parameters from fuzzer data
    uint32_t param1 = *(uint32_t*)data;
    uint32_t param2 = *(uint32_t*)(data + 4 % size);
    
    // Call target functions with various parameters
    uint64_t result;
    
    // 1. TIFFVStripSize64
    result = TIFFVStripSize64(tif, param1 % 1000);
    
    // 2. TIFFTileSize64
    result = TIFFTileSize64(tif);
    
    // 3. TIFFStripSize64
    result = TIFFStripSize64(tif);
    
    // 4. TIFFRawStripSize64
    result = TIFFRawStripSize64(tif, param1 % 100);
    
    // 5. TIFFGetStrileByteCount
    result = TIFFGetStrileByteCount(tif, param2 % 100);
    
    // 6. TIFFVTileSize64
    result = TIFFVTileSize64(tif, param1 % 1000);
    
    // Try to explore more states by calling with different parameters
    for (int i = 0; i < 3; i++) {
        uint32_t rows = (param1 + i * 100) % 5000;
        TIFFVStripSize64(tif, rows);
        TIFFVTileSize64(tif, rows);
        
        uint32_t strip = (param2 + i * 10) % 200;
        TIFFRawStripSize64(tif, strip);
        TIFFGetStrileByteCount(tif, strip);
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}