// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 0) {
            write(fd, Data, Size);
        } else {
            // Write a minimal valid TIFF header if input is empty to ensure open succeeds
            // BigTIFF or ClassicTIFF minimal header
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            write(fd, header, sizeof(header));
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    // 1. Test TIFFReadDirectory
    // This is often the first step to parse the IFD.
    // It returns 1 on success, 0 on EOF, -1 on error.
    int dir_result = TIFFReadDirectory(tif);
    (void)dir_result;

    // 2. Test TIFFIsUpSampled
    // Checks if up-sampling is required.
    int is_upsampled = TIFFIsUpSampled(tif);
    (void)is_upsampled;

    // 3. Test TIFFIsMSB2LSB
    // Checks bit order.
    int is_msb2lsb = TIFFIsMSB2LSB(tif);
    (void)is_msb2lsb;

    // 4. Test TIFFReadRGBAImage
    // Requires width, height, and buffer.
    // We attempt to read with a small buffer to trigger potential bounds checks or errors.
    // We use a static buffer to avoid allocation issues in fuzzing context.
    static uint32_t rgba_buffer[1024];
    uint32_t width = 0;
    uint32_t height = 0;
    
    // Try to get dimensions if directory was read successfully
    if (dir_result > 0) {
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    }
    
    // Fuzz with potentially invalid or small dimensions to test error handling
    if (width == 0) width = 1;
    if (height == 0) height = 1;
    
    // Limit buffer usage to avoid OOM
    if (width > 32 || height > 32) {
        width = 1;
        height = 1;
    }

    int read_rgba_result = TIFFReadRGBAImage(tif, width, height, rgba_buffer, 0);
    (void)read_rgba_result;

    // 5. Test TIFFReadRGBAStrip
    // Reads a single strip.
    // We try row 0.
    int read_strip_result = TIFFReadRGBAStrip(tif, 0, rgba_buffer);
    (void)read_strip_result;

    // 6. Test TIFFReadRGBAStripExt
    // Similar to above but with stop_on_error flag.
    int read_strip_ext_result = TIFFReadRGBAStripExt(tif, 0, rgba_buffer, 0);
    (void)read_strip_ext_result;

    // Cleanup
    TIFFClose(tif);
    
    // Remove the dummy file
    unlink("./dummy_file");

    return 0;
}