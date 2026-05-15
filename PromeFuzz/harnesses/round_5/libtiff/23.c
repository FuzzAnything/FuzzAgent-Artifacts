// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFCurrentDirOffset at tif_dir.c:2223:10 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFFlushData at tif_flush.c:146:5 in tiffio.h
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

static void dummy_cleanup(const char* filename) {
    if (filename) {
        unlink(filename);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    const char* filename = "./dummy_file";
    
    // Write fuzzing data to dummy file
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    // Write at least some minimal TIFF header data
    // We need enough data for TIFF to parse something
    if (Size > 0) {
        write(fd, Data, Size > 4096 ? 4096 : Size);
    } else {
        // Write minimal TIFF header if no data
        uint8_t minimal_header[] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
        write(fd, minimal_header, sizeof(minimal_header));
    }
    close(fd);
    
    // Open TIFF file for writing
    TIFF* tif = TIFFOpen(filename, "w");
    if (!tif) {
        dummy_cleanup(filename);
        return 0;
    }
    
    // Set some minimal required tags for a valid TIFF directory
    // These are needed for TIFFWriteDirectory to work
    uint32_t width = 1;
    uint32_t height = 1;
    uint16_t samples_per_pixel = 1;
    uint16_t bits_per_sample = 8;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar_config);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height);
    
    // 1. TIFFWriteDirectory
    TIFFWriteDirectory(tif);
    
    // 2. TIFFSetDirectory (set to first directory)
    TIFFSetDirectory(tif, 0);
    
    // 3. TIFFCurrentDirOffset
    uint64_t offset = TIFFCurrentDirOffset(tif);
    (void)offset; // Use offset to avoid unused variable warning
    
    // 4. TIFFSetDirectory (set to first directory again)
    TIFFSetDirectory(tif, 0);
    
    // 5. TIFFForceStrileArrayWriting
    TIFFForceStrileArrayWriting(tif);
    
    // 6. TIFFFlushData
    TIFFFlushData(tif);
    
    // Cleanup
    TIFFClose(tif);
    dummy_cleanup(filename);
    
    return 0;
}