// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
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

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Create or truncate the dummy file
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    // Write some data to the file to make it non-empty
    ssize_t written = write(fd, Data, Size);
    close(fd);
    
    if (written < 0) {
        return 0;
    }

    // Open the file for writing to test write-related functions
    TIFF *tif = TIFFOpen(filename, "w");
    if (tif == NULL) {
        return 0;
    }

    // Set some basic directory fields to ensure TIFFWriteCheck might pass
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 10);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 10);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    // Fuzz TIFFSetupStrips
    // This function is typically called implicitly, but we can call it directly
    // to test its internal logic with the current directory state.
    TIFFSetupStrips(tif);

    // Fuzz TIFFSetMode
    // Try different modes: O_RDONLY, O_WRONLY, O_RDWR
    int prev_mode = TIFFSetMode(tif, O_WRONLY);
    (void)prev_mode; // Suppress unused warning
    
    // Try setting to read-only (might fail if write state is active, but safe to call)
    prev_mode = TIFFSetMode(tif, O_RDONLY);
    (void)prev_mode;

    // Fuzz TIFFDeferStrileArrayWriting
    // This should succeed if the file is open for writing and directory not written yet
    int defer_result = TIFFDeferStrileArrayWriting(tif);
    (void)defer_result;

    // Fuzz TIFFWriteCheck
    // Check if we can write scanlines (0) or tiles (1)
    int write_check_strip = TIFFWriteCheck(tif, 0, "scanline");
    (void)write_check_strip;
    
    int write_check_tile = TIFFWriteCheck(tif, 1, "tile");
    (void)write_check_tile;

    // Fuzz TIFFFlush
    // Force buffered data to disk
    int flush_result = TIFFFlush(tif);
    (void)flush_result;

    // Fuzz TIFFForceStrileArrayWriting
    // This requires the directory to have been written and no other pending changes
    // It might fail depending on the state, but calling it is safe for fuzzing
    int force_result = TIFFForceStrileArrayWriting(tif);
    (void)force_result;

    // Cleanup
    cleanup_tiff(tif);

    // Remove the dummy file
    unlink(filename);

    return 0;
}