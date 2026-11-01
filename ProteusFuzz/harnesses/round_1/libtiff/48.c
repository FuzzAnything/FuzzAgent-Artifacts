// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFSetMode at tif_open.c:848:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetupStrips at tif_write.c:553:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFForceStrileArrayWriting at tif_flush.c:76:5 in tiffio.h
// TIFFFlush at tif_flush.c:30:5 in tiffio.h
// TIFFWriteDirectory at tif_dirwrite.c:238:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid-ish TIFF header if input is empty
            uint16_t header[8] = {0x002A, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
            fwrite(header, sizeof(uint16_t), 8, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare the environment
    setup_dummy_file(Data, Size);

    // Open the file for reading to test read-related checks
    TIFF *tif_read = TIFFOpen("./dummy_file", "r");
    if (tif_read) {
        // Test TIFFSetMode on read handle
        TIFFSetMode(tif_read, O_RDONLY);
        TIFFSetMode(tif_read, O_RDWR); // Might fail or switch, but safe to call

        // Test TIFFWriteCheck on read handle (should fail/return 0)
        TIFFWriteCheck(tif_read, 0, "test_read");

        // Test TIFFFlush on read handle
        TIFFFlush(tif_read);

        // Test TIFFForceStrileArrayWriting on read handle (should fail)
        TIFFForceStrileArrayWriting(tif_read);

        // Test TIFFDeferStrileArrayWriting on read handle (should fail)
        TIFFDeferStrileArrayWriting(tif_read);

        // Test TIFFSetupStrips on read handle
        TIFFSetupStrips(tif_read);

        TIFFClose(tif_read);
    }

    // Open the file for writing to test write-related logic
    // We use "w" to create a new file or truncate existing
    TIFF *tif_write = TIFFOpen("./dummy_file", "w");
    if (tif_write) {
        // Test TIFFSetMode on write handle
        TIFFSetMode(tif_write, O_RDWR);
        TIFFSetMode(tif_write, O_RDONLY); // Attempt to switch to read

        // Test TIFFDeferStrileArrayWriting before directory write
        TIFFDeferStrileArrayWriting(tif_write);

        // Test TIFFSetupStrips
        TIFFSetupStrips(tif_write);

        // Test TIFFWriteCheck
        TIFFWriteCheck(tif_write, 0, "test_write");

        // Test TIFFForceStrileArrayWriting
        // Note: This might fail if directory hasn't been written yet, which is expected
        TIFFForceStrileArrayWriting(tif_write);

        // Test TIFFFlush
        TIFFFlush(tif_write);

        // Try to set some basic tags to make the directory more "real" for subsequent calls
        uint16_t bits = 8;
        TIFFSetField(tif_write, TIFFTAG_BITSPERSAMPLE, bits);
        uint32_t width = (Size > 0) ? (Data[0] % 100) + 1 : 10;
        uint32_t height = (Size > 1) ? (Data[1] % 100) + 1 : 10;
        TIFFSetField(tif_write, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif_write, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif_write, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif_write, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

        // Re-test functions after setting fields
        TIFFSetupStrips(tif_write);
        TIFFWriteCheck(tif_write, 0, "test_write_post_setup");
        TIFFForceStrileArrayWriting(tif_write);
        TIFFFlush(tif_write);

        // Try writing a dummy directory to trigger state changes
        TIFFWriteDirectory(tif_write);

        // Test functions after directory write
        TIFFForceStrileArrayWriting(tif_write);
        TIFFDeferStrileArrayWriting(tif_write); // Should fail now
        TIFFFlush(tif_write);

        TIFFClose(tif_write);
    }

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}