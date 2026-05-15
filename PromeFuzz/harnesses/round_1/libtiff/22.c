// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetTagExtender at tif_dir.c:1675:16 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
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

static void dummy_tag_extender(TIFF* tif)
{
    /* Dummy tag extender that does nothing */
    (void)tif;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size)
{
    if (Size == 0) {
        return 0;
    }

    /* Write fuzzer input to dummy file */
    const char* filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    ssize_t written = write(fd, Data, Size);
    close(fd);
    
    if (written != (ssize_t)Size) {
        return 0;
    }

    /* Open the dummy file */
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        unlink(filename);
        return 0;
    }

    /* Set tag extender */
    TIFFExtendProc prev_extender = TIFFSetTagExtender(dummy_tag_extender);
    (void)prev_extender; /* Unused but keep for completeness */

    /* Call TIFFGetField four times with different tags */
    uint32_t width = 0;
    uint32_t height = 0;
    uint16_t bits_per_sample = 0;
    uint16_t compression = 0;
    
    /* First TIFFGetField - try to get image width */
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    
    /* Second TIFFGetField - try to get image length */
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    /* Third TIFFGetField - try to get bits per sample */
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    
    /* Fourth TIFFGetField - try to get compression type */
    TIFFGetField(tif, TIFFTAG_COMPRESSION, &compression);

    /* Clean up */
    TIFFClose(tif);
    unlink(filename);

    return 0;
}