// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFCreateEXIFDirectory at tif_dir.c:1732:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFMergeFieldInfo at tif_dirinfo.c:1147:5 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFGetTagListEntry at tif_extension.c:42:10 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        } else {
            // Write a minimal valid TIFF header if input is empty
            uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
            write(fd, header, sizeof(header));
        }
        close(fd);
    }
}

// Helper function to use va_list safely with variadic arguments
static void test_va_short(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    uint16_t dummy_short;
    va_start(ap, tag);
    TIFFVGetField(tif, tag, ap);
    va_end(ap);
}

static void test_va_long(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    uint32_t dummy_long;
    va_start(ap, tag);
    TIFFVGetField(tif, tag, ap);
    va_end(ap);
}

static void test_va_ascii(TIFF *tif, uint32_t tag, ...) {
    va_list ap;
    char dummy_str[64];
    va_start(ap, tag);
    TIFFVGetField(tif, tag, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input to a dummy file to simulate a TIFF file
    write_dummy_file(Data, Size);

    // Open the file for reading
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // Try opening for writing if read fails, to test write-path functions
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            return 0;
        }
    }

    // Define a custom field for testing TIFFMergeFieldInfo
    static const TIFFFieldInfo custom_fields[] = {
        {1000, 1, 1, TIFF_SHORT, 0, 1, 0, "CustomTag", "CustomTag"},
        {1001, 1, 1, TIFF_LONG, 0, 1, 0, "CustomTag2", "CustomTag2"},
        {1002, 1, 1, TIFF_ASCII, 0, 1, 0, "CustomTag3", "CustomTag3"}
    };
    uint32_t n_custom = sizeof(custom_fields) / sizeof(TIFFFieldInfo);

    // 1. Test TIFFMergeFieldInfo
    TIFFMergeFieldInfo(tif, custom_fields, n_custom);
    TIFFMergeFieldInfo(tif, custom_fields, 0);

    // 2. Test TIFFGetTagListEntry
    int indices[] = {-1, 0, 1, 2, 100, 1000};
    for (int i = 0; i < 6; i++) {
        TIFFGetTagListEntry(tif, indices[i]);
    }
    if (Size >= 4) {
        int idx = *(int32_t *)Data;
        TIFFGetTagListEntry(tif, idx);
    }

    // 3. Test TIFFFindField
    uint32_t tags[] = {0, 256, 257, 258, 1000, 1001, 1002, 0xFFFFFFFF};
    TIFFDataType types[] = {TIFF_ANY, TIFF_SHORT, TIFF_LONG, TIFF_ASCII, TIFF_BYTE};
    
    for (size_t i = 0; i < sizeof(tags)/sizeof(tags[0]); i++) {
        for (size_t j = 0; j < sizeof(types)/sizeof(types[0]); j++) {
            TIFFFindField(tif, tags[i], types[j]);
        }
    }
    if (Size >= 4) {
        uint32_t tag_id = *(uint32_t *)Data;
        TIFFFindField(tif, tag_id, TIFF_ANY);
    }

    // 4. Test TIFFVGetField
    uint32_t test_tag = (Size >= 4) ? *(uint32_t *)Data : 256;
    
    // Use helper functions to satisfy va_start requirements
    test_va_short(tif, test_tag);
    test_va_long(tif, test_tag);
    test_va_ascii(tif, test_tag);

    // 5. Test TIFFReadCustomDirectory
    // FIX: The function requires a non-NULL TIFFFieldArray. 
    // Since we cannot easily construct a valid TIFFFieldArray due to opaque/incomplete types 
    // in the public header, we skip this specific call to avoid the assertion failure.
    // Attempting to pass NULL triggers an assert() in libtiff.
    // Note: If internal headers were available, we would construct a minimal array here.
    // For robust fuzzing without internal access, skipping is the safe fallback.
    
    // 6. Test TIFFCreateEXIFDirectory
    TIFFCreateEXIFDirectory(tif);

    // Cleanup
    if (tif) {
        TIFFClose(tif);
    }

    return 0;
}