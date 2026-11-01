// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFLastDirectory at tif_dir.c:2229:5 in tiffio.h
// TIFFSetSubDirectory at tif_dir.c:2153:5 in tiffio.h
// TIFFReadEXIFDirectory at tif_dirread.c:5462:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        } else {
            // Write a minimal valid TIFF header if input is empty
            // Byte order (II), Magic number (42), Offset to first IFD (8)
            uint8_t header[] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            fwrite(header, 1, sizeof(header), fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    TIFF *tif = NULL;
    int ret;
    uint64_t diroff = 0;
    tdir_t dir_index = 0;
    toff_t custom_offset = 0;

    // Ensure we have some data to write to the dummy file
    if (Size == 0) {
        setup_dummy_file(NULL, 0);
    } else {
        setup_dummy_file(Data, Size);
    }

    // Attempt to open the file for reading
    tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // 1. Test TIFFSetDirectory
        // Try setting directory 0, and potentially others if fuzz data suggests
        if (Size > 0) {
            dir_index = Data[0] % 10; // Limit to reasonable range
        }
        ret = TIFFSetDirectory(tif, dir_index);
        (void)ret;

        // 2. Test TIFFLastDirectory
        ret = TIFFLastDirectory(tif);
        (void)ret;

        // 3. Test TIFFSetSubDirectory
        // Use fuzz data to generate an offset
        if (Size >= 8) {
            custom_offset = *(const uint64_t *)(Data + 8);
        } else if (Size > 0) {
            custom_offset = Data[0];
        }
        ret = TIFFSetSubDirectory(tif, custom_offset);
        (void)ret;

        // 4. Test TIFFReadEXIFDirectory
        // Use the same offset logic
        ret = TIFFReadEXIFDirectory(tif, custom_offset);
        (void)ret;

        // 5. Test TIFFReadCustomDirectory
        // We pass NULL for infoarray to test error handling or default behavior
        // Note: Passing NULL might cause a crash in some versions, which is good for fuzzing
        ret = TIFFReadCustomDirectory(tif, custom_offset, NULL);
        (void)ret;

        // Close and reopen for writing to test write functions
        TIFFClose(tif);
        tif = NULL;

        // Reopen for writing
        tif = TIFFOpen("./dummy_file", "w");
        if (tif) {
            // Set some basic fields to make the directory valid for writing
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

            // 6. Test TIFFWriteCustomDirectory
            ret = TIFFWriteCustomDirectory(tif, &diroff);
            (void)ret;

            // Try to set a subdirectory and write again
            if (Size >= 8) {
                diroff = *(const uint64_t *)(Data + 8);
            }
            ret = TIFFSetSubDirectory(tif, diroff);
            (void)ret;
            
            // Attempt another write
            ret = TIFFWriteCustomDirectory(tif, &diroff);
            (void)ret;
            
            TIFFClose(tif);
            tif = NULL;
        }
    }

    // Cleanup
    if (tif) {
        TIFFClose(tif);
    }

    return 0;
}