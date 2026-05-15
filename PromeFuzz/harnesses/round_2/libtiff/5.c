// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
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

static int write_data_to_file(const char* filename, const uint8_t* data, size_t size) {
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    const char* filename = "./dummy_file";
    
    // Write fuzzer input to file
    if (write_data_to_file(filename, Data, Size) != 0) {
        return 0;
    }
    
    // Open TIFF file for writing
    TIFF* tif = TIFFOpen(filename, "w");
    if (!tif) {
        return 0;
    }
    
    // 1. TIFFFreeDirectory
    TIFFFreeDirectory(tif);
    
    // 2. TIFFCreateCustomDirectory - use NULL for field array since we can't access internal structure
    int create_result = TIFFCreateCustomDirectory(tif, NULL);
    
    // 3. TIFFSetField - set some basic tags
    if (Size >= 4) {
        uint32_t width = *(uint32_t*)Data;
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    }
    
    // 4. TIFFSetField - set another tag
    if (Size >= 8) {
        uint32_t height = *((uint32_t*)(Data + 4));
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    }
    
    // 5. TIFFWriteCustomDirectory
    uint64_t dir_offset = 0;
    int write_result = TIFFWriteCustomDirectory(tif, &dir_offset);
    
    // 6. TIFFSetDirectory
    if (Size >= 12) {
        uint16_t dir_num = *((uint16_t*)(Data + 8));
        TIFFSetDirectory(tif, dir_num);
    }
    
    // 7. TIFFSetField - set resolution
    if (Size >= 20) {
        float xres = *((float*)(Data + 12));
        TIFFSetField(tif, TIFFTAG_XRESOLUTION, xres);
    }
    
    // 8. TIFFSetField - set another resolution
    if (Size >= 24) {
        float yres = *((float*)(Data + 16));
        TIFFSetField(tif, TIFFTAG_YRESOLUTION, yres);
    }
    
    // 9. TIFFClose - close the writing TIFF
    TIFFClose(tif);
    
    // 10. TIFFOpen - open for reading
    tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }
    
    // 11. TIFFGetField - get width
    uint32_t retrieved_width = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &retrieved_width);
    
    // 12. TIFFGetField - get height
    uint32_t retrieved_height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &retrieved_height);
    
    // 13. TIFFReadEXIFDirectory
    if (Size >= 28) {
        uint64_t exif_offset = *((uint64_t*)(Data + 20));
        TIFFReadEXIFDirectory(tif, exif_offset);
    }
    
    // 14. TIFFGetField - get resolution
    float retrieved_xres = 0.0;
    TIFFGetField(tif, TIFFTAG_XRESOLUTION, &retrieved_xres);
    
    // 15. TIFFReadCustomDirectory - use NULL for field array
    if (Size >= 36) {
        uint64_t custom_offset = *((uint64_t*)(Data + 28));
        TIFFReadCustomDirectory(tif, custom_offset, NULL);
    }
    
    // 16. TIFFGetField - get another resolution
    float retrieved_yres = 0.0;
    TIFFGetField(tif, TIFFTAG_YRESOLUTION, &retrieved_yres);
    
    // 17. TIFFGetField - get samples per pixel
    uint16_t samples = 0;
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples);
    
    // 18. TIFFClose - close reading TIFF
    TIFFClose(tif);
    
    // 19. TIFFClose - extra close (should handle NULL gracefully)
    TIFFClose(NULL);
    
    // Clean up dummy file
    unlink(filename);
    
    return 0;
}