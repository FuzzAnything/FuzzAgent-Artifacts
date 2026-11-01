// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFGetTagListCount at tif_extension.c:34:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static void dummy_cleanup(TIFF *tif) {
    if (tif) {
        TIFFClose(tif);
    }
}

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    if (size > 0 && data != NULL) {
        ssize_t written = write(fd, data, size);
        if (written != (ssize_t)size) {
            close(fd);
            return -1;
        }
    }
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a dummy file to simulate a TIFF file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }

    // Attempt to open the file in read mode first to test read-related APIs
    TIFF *tif_read = TIFFOpen("./dummy_file", "r");
    if (tif_read) {
        // Test TIFFGetTagListCount on a potentially valid or invalid file
        int count = TIFFGetTagListCount(tif_read);
        (void)count; // Suppress unused warning

        // Test TIFFSetDirectory with various indices derived from input
        tdir_t dir_idx = 0;
        if (Size >= 4) {
            dir_idx = *(const uint32_t *)(Data);
        } else if (Size >= 2) {
            dir_idx = *(const uint16_t *)(Data);
        } else {
            dir_idx = Data[0];
        }
        // Limit index to a reasonable range to avoid excessive resource usage
        if (dir_idx > 10000) dir_idx = 10000;
        int set_dir_res = TIFFSetDirectory(tif_read, dir_idx);
        (void)set_dir_res;

        // Test TIFFFreeDirectory
        TIFFFreeDirectory(tif_read);

        // Close read handle
        TIFFClose(tif_read);
    }

    // Attempt to open the file in write mode to test write-related APIs
    // We use "w+" to allow overwriting for fuzzing purposes
    TIFF *tif_write = TIFFOpen("./dummy_file", "w+");
    if (tif_write) {
        // Test TIFFCreateDirectory
        int create_dir_res = TIFFCreateDirectory(tif_write);
        (void)create_dir_res;

        // Test TIFFCreateEXIFDirectory
        int create_exif_res = TIFFCreateEXIFDirectory(tif_write);
        (void)create_exif_res;

        // Test TIFFCreateCustomDirectory
        // FIX: The library crashes if NULL is passed to TIFFCreateCustomDirectory
        // because it dereferences the pointer without checking.
        // We skip this specific call with NULL to prevent the crash.
        // A valid fuzzing strategy would require constructing a valid TIFFFieldArray,
        // which is complex due to opaque structures, so we omit this specific test case.
        // int create_custom_res = TIFFCreateCustomDirectory(tif_write, NULL); 

        // Test TIFFGetTagListCount on write handle
        int count_write = TIFFGetTagListCount(tif_write);
        (void)count_write;

        // Test TIFFSetDirectory on write handle
        tdir_t dir_idx_write = 0;
        if (Size >= 4) {
            dir_idx_write = *(const uint32_t *)(Data);
        } else if (Size >= 2) {
            dir_idx_write = *(const uint16_t *)(Data);
        } else {
            dir_idx_write = Data[0];
        }
        if (dir_idx_write > 10000) dir_idx_write = 10000;
        int set_dir_res_write = TIFFSetDirectory(tif_write, dir_idx_write);
        (void)set_dir_res_write;

        // Test TIFFFreeDirectory on write handle
        TIFFFreeDirectory(tif_write);

        // Close write handle
        TIFFClose(tif_write);
    }

    return 0;
}