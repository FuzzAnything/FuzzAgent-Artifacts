// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
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

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal dummy data if input is empty to ensure file exists
            const char *dummy = "II*\0";
            fwrite(dummy, 1, strlen(dummy), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    // Open file for writing (mode "w" creates/truncates, "a" appends)
    // We try both modes to explore different states
    TIFF *tif = NULL;
    
    // Attempt to open for writing
    tif = TIFFOpen("./dummy_file", "w");
    
    if (tif != NULL) {
        // Set some basic required tags to make the directory valid for writing checks
        // This helps trigger more code paths in TIFFWriteCheck and TIFFWriteDirectory
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 100);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 100);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        
        // Fuzz target 1: TIFFDeferStrileArrayWriting
        // Must be called after opening but before writing directory
        (void)TIFFDeferStrileArrayWriting(tif);

        // Fuzz target 2: TIFFSetupStrips
        // Configures strip/tile counts
        (void)TIFFSetupStrips(tif);

        // Fuzz target 4: TIFFWriteCheck
        // Validates write readiness. 
        // We pass 0 for scanline/tile index and a dummy function name
        (void)TIFFWriteCheck(tif, 0, "TestFunction");

        // Fuzz target 6: TIFFWriteDirectory
        // Writes the current directory tags
        (void)TIFFWriteDirectory(tif);

        // Fuzz target 3: TIFFFlush
        // Forces buffered data to disk
        (void)TIFFFlush(tif);

        // Fuzz target 5: TIFFForceStrileArrayWriting
        // Forces writing of strip arrays without rewriting directory
        // Only valid after initial directory write and no other pending changes
        (void)TIFFForceStrileArrayWriting(tif);

        // Cleanup
        TIFFClose(tif);
    }

    // Also test opening in append mode to test different flags
    tif = TIFFOpen("./dummy_file", "a");
    if (tif != NULL) {
        // In append mode, some operations might behave differently
        (void)TIFFWriteCheck(tif, 0, "TestFunctionAppend");
        (void)TIFFFlush(tif);
        (void)TIFFForceStrileArrayWriting(tif);
        
        // Try writing a new directory
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 50);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 50);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        (void)TIFFWriteDirectory(tif);

        TIFFClose(tif);
    }

    return 0;
}