#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <vector>
#include <string>

namespace harness_013 {
/*
 * Fuzzing harness for libtiff library focusing on field management APIs with high undiscovered complexity
 * Targets field management APIs: TIFFFindField, TIFFFieldWithTag, TIFFFieldWithName, TIFFFieldTag,
 * TIFFFieldName, TIFFFieldDataType, TIFFFieldPassCount, TIFFFieldReadCount, TIFFFieldWriteCount
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Basic TIFF file operations and directory reading
 * - harness_001: TIFF writing operations
 * - harness_002: RGBA image reading
 * - harness_003: Directory manipulation basics  
 * - harness_004: Advanced directory management APIs
 * - harness_012: EXIF and GPS directory reading
 * - harness_013: Specifically targets field metadata querying and management APIs
 * 
 * Testing strategy:
 * 1. Create a TIFF file with various field configurations
 * 2. Test field lookup APIs with valid and invalid tags/names
 * 3. Test field metadata querying APIs
 * 4. Test field count and type information APIs
 * 5. Use FuzzedDataProvider for structured input processing
 */


// Error handler to suppress library error messages during fuzzing
extern "C" void handle_error(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    return;
}

// Helper function to create a TIFF file with various field configurations
static TIFF* create_field_test_tiff(FuzzedDataProvider& fdp, const char* filename) {
    // Open TIFF file for writing
    TIFF* tif = TIFFOpen(filename, "w");
    if (!tif) {
        return nullptr;
    }
    
    // Set error handlers
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
    
    // Create main directory with basic image and varied field configurations
    uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(1, 512);
    uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(1, 512);
    uint16_t samples_per_pixel = fdp.ConsumeIntegralInRange<uint16_t>(1, 4);
    uint16_t bits_per_sample = fdp.ConsumeIntegralInRange<uint16_t>(1, 32);
    uint16_t photometric = fdp.ConsumeIntegral<uint16_t>() % 6;
    uint16_t compression = fdp.ConsumeIntegral<uint16_t>() % 7;
    
    // Set basic TIFF directory fields
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samples_per_pixel);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits_per_sample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    
    // Add some optional fields based on fuzzed input
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
        TIFFSetField(tif, TIFFTAG_ORIENTATION, fdp.ConsumeIntegral<uint16_t>() % 9);
    }
    
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
        TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, fdp.ConsumeIntegral<uint16_t>() % 3);
    }
    
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 8) {
        float xres = fdp.ConsumeFloatingPoint<float>();
        float yres = fdp.ConsumeFloatingPoint<float>();
        TIFFSetField(tif, TIFFTAG_XRESOLUTION, xres);
        TIFFSetField(tif, TIFFTAG_YRESOLUTION, yres);
    }
    
    // Add some string fields if we have enough input
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
        std::string software = fdp.ConsumeRandomLengthString(50);
        TIFFSetField(tif, TIFFTAG_SOFTWARE, software.c_str());
    }
    
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
        std::string datetime = fdp.ConsumeRandomLengthString(20);
        TIFFSetField(tif, TIFFTAG_DATETIME, datetime.c_str());
    }
    
    // Write directory
    if (!TIFFWriteDirectory(tif)) {
        TIFFClose(tif);
        return nullptr;
    }
    
    return tif;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need sufficient data for meaningful testing
    if (size < 300) {
        return 0;
    }
    
    // Initialize FuzzedDataProvider for structured input processing
    FuzzedDataProvider fdp(data, size);
    
    // Create a temporary TIFF file for testing
    const char* filename = "/tmp/fuzzed_field_test_tiff.tif";
    
    // Create the test TIFF file
    TIFF* tif = create_field_test_tiff(fdp, filename);
    if (!tif) {
        // Clean up temp file if it was created
        remove(filename);
        return 0;
    }
    
    // Close and reopen the file for reading to ensure proper field initialization
    TIFFClose(tif);
    tif = TIFFOpen(filename, "r");
    if (!tif) {
        remove(filename);
        return 0;
    }
    
    // Set error handlers for reading
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
    
    // Test field lookup APIs with various tags
    // Common TIFF tags to test
    uint32_t test_tags[] = {
        TIFFTAG_IMAGEWIDTH,
        TIFFTAG_IMAGELENGTH,
        TIFFTAG_SAMPLESPERPIXEL,
        TIFFTAG_BITSPERSAMPLE,
        TIFFTAG_PHOTOMETRIC,
        TIFFTAG_COMPRESSION,
        TIFFTAG_PLANARCONFIG,
        TIFFTAG_ORIENTATION,
        TIFFTAG_RESOLUTIONUNIT,
        TIFFTAG_XRESOLUTION,
        TIFFTAG_YRESOLUTION,
        TIFFTAG_SOFTWARE,
        TIFFTAG_DATETIME,
        TIFFTAG_ARTIST,
        TIFFTAG_HOSTCOMPUTER,
        TIFFTAG_MAKE,
        TIFFTAG_MODEL
    };
    
    // Test TIFFFieldWithTag for each test tag
    for (size_t i = 0; i < sizeof(test_tags)/sizeof(test_tags[0]); i++) {
        const TIFFField* field = TIFFFieldWithTag(tif, test_tags[i]);
        if (field) {
            // Test field metadata APIs on found fields
            uint32_t tag = TIFFFieldTag(field);
            const char* name = TIFFFieldName(field);
            TIFFDataType data_type = TIFFFieldDataType(field);
            int pass_count = TIFFFieldPassCount(field);
            int read_count = TIFFFieldReadCount(field);
            int write_count = TIFFFieldWriteCount(field);
            
            // Use the values to avoid compiler warnings
            (void)tag;
            (void)name;
            (void)data_type;
            (void)pass_count;
            (void)read_count;
            (void)write_count;
        }
    }
    
    // Test TIFFFindField with various data types
    TIFFDataType test_types[] = {
        TIFF_NOTYPE,
        TIFF_BYTE,
        TIFF_ASCII,
        TIFF_SHORT,
        TIFF_LONG,
        TIFF_RATIONAL,
        TIFF_SBYTE,
        TIFF_UNDEFINED,
        TIFF_SSHORT,
        TIFF_SLONG,
        TIFF_SRATIONAL,
        TIFF_FLOAT,
        TIFF_DOUBLE,
        TIFF_IFD,
        TIFF_LONG8,
        TIFF_SLONG8,
        TIFF_IFD8
    };
    
    // Test a few random tags with different data types
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 4; i++) {
        uint32_t random_tag = fdp.ConsumeIntegral<uint32_t>();
        TIFFDataType random_type = test_types[fdp.ConsumeIntegralInRange<size_t>(0, sizeof(test_types)/sizeof(test_types[0]) - 1)];
        
        const TIFFField* field = TIFFFindField(tif, random_tag, random_type);
        if (field) {
            // Test field metadata on found field
            const char* field_name = TIFFFieldName(field);
            TIFFDataType found_type = TIFFFieldDataType(field);
            
            (void)field_name;
            (void)found_type;
        }
    }
    
    // Test TIFFFieldWithName with common field names
    const char* test_names[] = {
        "ImageWidth",
        "ImageLength",
        "BitsPerSample",
        "Compression",
        "PhotometricInterpretation",
        "SamplesPerPixel",
        "RowsPerStrip",
        "XResolution",
        "YResolution",
        "Software",
        "DateTime",
        "Artist",
        "Make",
        "Model"
    };
    
    for (size_t i = 0; i < sizeof(test_names)/sizeof(test_names[0]); i++) {
        const TIFFField* field = TIFFFieldWithName(tif, test_names[i]);
        if (field) {
            // Test field metadata
            uint32_t tag = TIFFFieldTag(field);
            const char* name = TIFFFieldName(field);
            
            (void)tag;
            (void)name;
        }
    }
    
    // Test TIFFFieldWithName with random strings from fuzzed input
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 10; i++) {
        std::string random_name = fdp.ConsumeRandomLengthString(20);
        const TIFFField* field = TIFFFieldWithName(tif, random_name.c_str());
        if (field) {
            // Unexpected but valid - test metadata
            uint32_t tag = TIFFFieldTag(field);
            (void)tag;
        }
    }
    
    // Test field count and size APIs
    // Iterate through a subset of tags and test their properties
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 4; i++) {
        uint32_t tag_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(test_tags)/sizeof(test_tags[0]) - 1);
        const TIFFField* field = TIFFFieldWithTag(tif, test_tags[tag_idx]);
        
        if (field) {
            // Test TIFFFieldSetGetSize - internal storage size
            int setget_size = TIFFFieldSetGetSize(field);
            
            // Test TIFFFieldSetGetCountSize - count parameter size
            int count_size = TIFFFieldSetGetCountSize(field);
            
            // Test TIFFFieldIsAnonymous
            int is_anonymous = TIFFFieldIsAnonymous(field);
            
            (void)setget_size;
            (void)count_size;
            (void)is_anonymous;
            
            // Test field pass count variations
            int pass_count = TIFFFieldPassCount(field);
            if (pass_count == TIFF_VARIABLE || pass_count == TIFF_VARIABLE2 || pass_count == TIFF_SPP) {
                // Test handling of special count values
                (void)pass_count;
            }
        }
    }
    
    // Test field operations on different directories if multiple exist
    if (TIFFNumberOfDirectories(tif) > 1) {
        // Switch to a different directory and test field APIs again
        uint16_t dir_num = fdp.ConsumeIntegralInRange<uint16_t>(0, TIFFNumberOfDirectories(tif) - 1);
        TIFFSetDirectory(tif, dir_num);
        
        // Test a few field APIs in the new directory
        for (int i = 0; i < 3; i++) {
            uint32_t tag_idx = fdp.ConsumeIntegralInRange<size_t>(0, sizeof(test_tags)/sizeof(test_tags[0]) - 1);
            const TIFFField* field = TIFFFieldWithTag(tif, test_tags[tag_idx]);
            
            if (field) {
                uint32_t tag = TIFFFieldTag(field);
                const char* name = TIFFFieldName(field);
                
                (void)tag;
                (void)name;
            }
        }
    }
    
    // Clean up
    TIFFClose(tif);
    remove(filename);
    
    return 0;
}

} // namespace harness_013
