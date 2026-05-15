// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF* tif = TIFFOpen("./dummy_file", "w");
    if (!tif) return 0;
    
    uint32_t width = *(uint32_t*)Data;
    uint32_t height = *(uint32_t*)(Data + 4);
    uint16_t bits_per_sample = *(uint16_t*)(Data + 8);
    uint16_t samples_per_pixel = *(uint16_t*)(Data + 10);
    uint16_t compression = *(uint16_t*)(Data + 12);
    uint16_t photometric = *(uint16_t*)(Data + 14);
    uint16_t planar_config = *(uint16_t*)(Data + 16);
    uint32_t rows_per_strip = *(uint32_t*)(Data + 18);
    uint16_t orientation = *(uint16_t*)(Data + 22);
    uint16_t fill_order = *(uint16_t*)(Data + 24);
    uint16_t extra_samples = *(uint16_t*)(Data + 26);
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows_per_strip);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, orientation);
    TIFFSetField(tif, TIFFTAG_FILLORDER, fill_order);
    TIFFSetField(tif, TIFFTAG_EXTRASAMPLES, extra_samples, (uint16_t*)Data);
    
    tmsize_t buf_size = width * samples_per_pixel * (bits_per_sample / 8);
    if (buf_size <= 0) buf_size = 100;
    
    void* buffer = _TIFFmalloc(buf_size);
    if (!buffer) {
        TIFFClose(tif);
        unlink("./dummy_file");
        return 0;
    }
    
    size_t copy_size = Size - 28;
    if (copy_size > buf_size) copy_size = buf_size;
    
    _TIFFmemcpy(buffer, Data + 28, copy_size);
    
    TIFFWriteScanline(tif, buffer, 0, 0);
    
    _TIFFfree(buffer);
    TIFFClose(tif);
    
    unlink("./dummy_file");
    return 0;
}