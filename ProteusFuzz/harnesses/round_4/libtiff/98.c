// Source: Fuzz driver for libtiff targeting _TIFFSetGetType
// This driver aims to cover all branches in _TIFFSetGetType by calling TIFFMergeFieldInfo
// with various combinations of field_type, field_writecount, and field_passcount.
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <tiffio.h>

// Dummy error and warning handlers to suppress output during fuzzing
static void dummy_error_handler(const char* module, const char* fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

static void dummy_warn_handler(const char* module, const char* fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
}

// Helper to create a TIFFFieldInfo entry with specific parameters
static TIFFFieldInfo create_field_info(uint16_t tag, TIFFDataType type, short writecount, unsigned char passcount) {
    TIFFFieldInfo info = {0};
    info.field_tag = tag;
    info.field_type = type;
    info.field_readcount = writecount; // Use writecount for readcount to avoid validation errors
    info.field_writecount = writecount;
    info.field_passcount = passcount;
    info.field_bit = FIELD_CUSTOM; // Use a custom bit to avoid conflicts
    info.field_oktochange = 0;
    info.field_name = "FuzzTag";
    return info;
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    const char *filename = "./dummy_file";
    
    // Write the fuzzing data to the file
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file in read/write mode
    TIFF *tif = TIFFOpen(filename, "rw+");
    if (!tif) {
        return 0;
    }

    // Set dummy error and warn handlers
    TIFFSetWarningHandler(dummy_warn_handler);
    TIFFSetErrorHandler(dummy_error_handler);

    // Extract parameters from input to drive coverage
    // We need to cover:
    // 1. type == TIFF_ASCII && count == TIFF_VARIABLE && passcount == 0
    // 2. count == 1 && passcount == 0 (with various types)
    // 3. count >= 1 && passcount == 0 (with various types)
    // 4. count == TIFF_VARIABLE && passcount == 1 (with various types)
    // 5. count == TIFF_VARIABLE2 && passcount == 1 (with various types)
    // 6. Default cases (uncovered lines 1028, 1065, 1102, 1139, 1144)

    // Use first 4 bytes to determine the scenario and parameters
    uint8_t scenario = Data[0] % 6;
    TIFFDataType type = (TIFFDataType)(Data[1] % 16); // 16 TIFF types
    short count = (short)(Data[2] % 100);
    unsigned char passcount = Data[3] % 2;

    // Adjust parameters to hit specific branches
    if (scenario == 0) {
        // Hit line 993: type == TIFF_ASCII && count == TIFF_VARIABLE && passcount == 0
        type = TIFF_ASCII;
        count = TIFF_VARIABLE;
        passcount = 0;
    } else if (scenario == 1) {
        // Hit lines 996-1030: count == 1 && passcount == 0
        count = 1;
        passcount = 0;
    } else if (scenario == 2) {
        // Hit lines 1033-1067: count >= 1 && passcount == 0 (but not 1)
        count = 2; // Ensure >= 1 and != 1
        passcount = 0;
    } else if (scenario == 3) {
        // Hit lines 1070-1104: count == TIFF_VARIABLE && passcount == 1
        count = TIFF_VARIABLE;
        passcount = 1;
    } else if (scenario == 4) {
        // Hit lines 1107-1141: count == TIFF_VARIABLE2 && passcount == 1
        count = TIFF_VARIABLE2;
        passcount = 1;
    } else {
        // Hit default cases (lines 1028, 1065, 1102, 1139, 1144)
        // Use invalid type or mismatched count/passcount
        type = (TIFFDataType)(100); // Invalid type
        count = 5;
        passcount = 0;
    }

    // Create a single field info entry
    TIFFFieldInfo info[1];
    info[0] = create_field_info(32768, type, count, passcount);

    // Call TIFFMergeFieldInfo to trigger _TIFFSetGetType
    TIFFMergeFieldInfo(tif, info, 1);

    // Close the TIFF file
    TIFFClose(tif);

    // Remove the temporary file
    unlink(filename);

    return 0;
}