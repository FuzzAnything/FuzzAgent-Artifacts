// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFCreateGPSDirectory at tif_dir.c:1742:5 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write fuzzing data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Attempt to open the file for reading
    TIFF *tif_read = TIFFOpen(filename, "rb");
    if (tif_read) {
        // Test TIFFGetTagListCount on read handle
        int count = TIFFGetTagListCount(tif_read);
        (void)count;

        // Test TIFFFreeDirectory on read handle
        TIFFFreeDirectory(tif_read);

        TIFFClose(tif_read);
    }

    // Attempt to open the file for writing
    TIFF *tif_write = TIFFOpen(filename, "wb");
    if (tif_write) {
        // Test TIFFCreateDirectory
        TIFFCreateDirectory(tif_write);

        // Test TIFFGetTagListCount on write handle
        int count1 = TIFFGetTagListCount(tif_write);
        (void)count1;

        // Test TIFFCreateEXIFDirectory
        int exif_res = TIFFCreateEXIFDirectory(tif_write);
        (void)exif_res;

        // Test TIFFGetTagListCount again
        int count2 = TIFFGetTagListCount(tif_write);
        (void)count2;

        // Test TIFFCreateGPSDirectory
        int gps_res = TIFFCreateGPSDirectory(tif_write);
        (void)gps_res;

        // Test TIFFGetTagListCount again
        int count3 = TIFFGetTagListCount(tif_write);
        (void)count3;

        // Removed TIFFCreateCustomDirectory(tif_write, NULL) call as it causes a segfault
        // when passing NULL to the field array parameter.

        // Test TIFFGetTagListCount again
        int count4 = TIFFGetTagListCount(tif_write);
        (void)count4;

        // Test TIFFFreeDirectory on write handle
        TIFFFreeDirectory(tif_write);

        // Test TIFFCreateDirectory again after free
        TIFFCreateDirectory(tif_write);

        // Close the write handle
        TIFFClose(tif_write);
    }

    return 0;
}