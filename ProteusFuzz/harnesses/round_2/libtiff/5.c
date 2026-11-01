// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFDeferStrileArrayWriting at tif_dirwrite.c:268:5 in tiffio.h
// TIFFWriteCheck at tif_write.c:605:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "tiffio.h"

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for testing
    const char *filename = "./dummy_file";
    
    // Write fuzzing data to the dummy file
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file for writing to allow directory operations
    TIFF *tif = TIFFOpen(filename, "w8");
    if (tif == NULL) {
        unlink(filename);
        return 0;
    }

    // 1. TIFFFreeDirectory
    // Free any existing directory (though it's a new file, this is safe)
    TIFFFreeDirectory(tif);

    // 2. TIFFCreateDirectory
    // Initialize a new directory
    if (TIFFCreateDirectory(tif) != 0) {
        TIFFClose(tif);
        unlink(filename);
        return 0;
    }

    // 3. TIFFSetField (Multiple calls to explore states)
    // We use data from the fuzz input to set various fields safely.
    // We need to ensure we don't pass invalid pointers or out-of-bounds values.
    
    // Set ImageWidth (TAG_IMAGEWIDTH)
    uint32_t width = 0;
    if (Size >= 4) {
        memcpy(&width, Data, 4);
        // Ensure width is reasonable to avoid immediate crashes or huge allocations
        if (width > 0 && width < 100000) {
            TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        }
    }

    // Set ImageLength (TAG_IMAGELENGTH)
    uint32_t length = 0;
    if (Size >= 8) {
        memcpy(&length, Data + 4, 4);
        if (length > 0 && length < 100000) {
            TIFFSetField(tif, TIFFTAG_IMAGELENGTH, length);
        }
    }

    // Set BitsPerSample (TAG_BITSPERSAMPLE)
    uint16_t bits = 8;
    if (Size >= 12) {
        bits = Data[12];
        if (bits > 0 && bits <= 32) {
            TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits);
        }
    }

    // Set Compression (TAG_COMPRESSION)
    uint16_t comp = 1; // Default to None
    if (Size >= 14) {
        comp = Data[14];
        // Use a valid compression value or default
        if (comp == 1 || comp == 2 || comp == 32773) {
            TIFFSetField(tif, TIFFTAG_COMPRESSION, comp);
        }
    }

    // Set PhotometricInterpretation (TAG_PHOTOMETRIC)
    uint16_t photo = 1;
    if (Size >= 16) {
        photo = Data[16];
        if (photo >= 1 && photo <= 6) {
            TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photo);
        }
    }

    // Set RowsPerStrip (TAG_ROWSPERSTRIP)
    uint32_t rows = 0;
    if (Size >= 20) {
        memcpy(&rows, Data + 16, 4);
        if (rows > 0) {
            TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rows);
        }
    }

    // Set PlanarConfig (TAG_PLANARCONFIG)
    uint16_t planar = 1;
    if (Size >= 24) {
        planar = Data[24];
        if (planar == 1 || planar == 2) {
            TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planar);
        }
    }

    // Set SampleFormat (TAG_SAMPLEFORMAT)
    uint16_t sample = 1;
    if (Size >= 26) {
        sample = Data[26];
        if (sample == 1 || sample == 2 || sample == 3) {
            TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, sample);
        }
    }

    // Set ExtraSamples (TAG_EXTRASAMPLES) - optional, might fail if not configured
    if (Size >= 28) {
        uint16_t extra_count = Data[28];
        if (extra_count > 0 && extra_count < 10) {
            uint16_t extra_info[10];
            for (int i = 0; i < extra_count && (29 + i * 2) < Size; i++) {
                extra_info[i] = Data[29 + i * 2];
            }
            TIFFSetField(tif, TIFFTAG_EXTRASAMPLES, extra_count, extra_info);
        }
    }

    // 4. TIFFDeferStrileArrayWriting
    // Attempt to defer writing strip/tile arrays
    TIFFDeferStrileArrayWriting(tif);

    // 5. TIFFWriteCheck
    // Check if the file is ready for writing
    // We pass 0 for the row/strip index and a dummy name
    TIFFWriteCheck(tif, 0, "FuzzingCheck");

    // Cleanup
    TIFFClose(tif);
    
    // Remove the dummy file
    unlink(filename);

    return 0;
}