// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFIsMSB2LSB at tif_open.c:894:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
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
#include <sys/stat.h>
#include <tiffio.h>

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    ssize_t written = write(fd, Data, Size);
    close(fd);
    if (written != (ssize_t)Size) {
        return 0;
    }

    TIFF *tif = TIFFOpen(filename, "rb");
    if (tif == NULL) {
        return 0;
    }

    // Fuzz TIFFReadDirectory
    (void)TIFFReadDirectory(tif);

    // Fuzz TIFFIsMSB2LSB
    int is_msb = TIFFIsMSB2LSB(tif);
    (void)is_msb;

    // Fuzz TIFFIsUpSampled
    int is_upsampled = TIFFIsUpSampled(tif);
    (void)is_upsampled;

    // Fuzz TIFFGetField with various common tags
    uint32_t width, height;
    uint16_t bitspersample, samplesperpixel, photometric, compression;
    uint16_t planarconfig;
    uint32_t rowstride;

    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) == 1) {
        if (TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height) == 1) {
            if (TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample) == 1) {
                if (TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel) == 1) {
                    if (TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric) == 1) {
                        if (TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression) == 1) {
                            if (TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig) == 1) {
                                // Calculate rowstride if needed for buffer allocation
                                rowstride = width * samplesperpixel * ((bitspersample + 7) / 8);
                            }
                        }
                    }
                }
            }
        }
    }

    // Fuzz TIFFReadRGBAImage
    if (width > 0 && height > 0) {
        uint32_t *rgba_buffer = (uint32_t *)malloc(width * height * sizeof(uint32_t));
        if (rgba_buffer != NULL) {
            int stop_on_error = 1;
            (void)TIFFReadRGBAImage(tif, width, height, rgba_buffer, stop_on_error);
            free(rgba_buffer);
        }
    }

    // Fuzz TIFFReadRGBAStrip
    if (width > 0 && height > 0) {
        uint32_t *strip_buffer = (uint32_t *)malloc(width * height * sizeof(uint32_t));
        if (strip_buffer != NULL) {
            // Try reading the first strip (index 0)
            (void)TIFFReadRGBAStrip(tif, 0, strip_buffer);
            // Try reading a potentially invalid strip index based on fuzz data
            if (Size >= 4) {
                uint32_t strip_idx = *(const uint32_t *)Data;
                // Limit to a reasonable range to avoid excessive allocation or invalid access
                if (strip_idx < 1000) {
                    (void)TIFFReadRGBAStrip(tif, strip_idx, strip_buffer);
                }
            }
            free(strip_buffer);
        }
    }

    cleanup_tiff(tif);
    unlink(filename);

    return 0;
}