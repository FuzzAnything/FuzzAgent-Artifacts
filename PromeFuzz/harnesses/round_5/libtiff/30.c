// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVGetField at tif_dir.c:1599:5 in tiffio.h
// TIFFVGetFieldDefaulted at tif_aux.c:217:5 in tiffio.h
// TIFFVSetField at tif_dir.c:1205:5 in tiffio.h
// TIFFFindField at tif_dirinfo.c:795:18 in tiffio.h
// TIFFFieldWithTag at tif_dirinfo.c:845:18 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdarg.h>

static TIFF* create_test_tiff(const uint8_t* data, size_t size) {
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return NULL;
    
    // Write minimal TIFF header (BigTIFF to avoid 4GB limit issues)
    uint8_t header[] = {
        0x49, 0x49, 0x2B, 0x00, 0x08, 0x00, 0x00, 0x00,  // BigTIFF magic
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // First IFD offset (0 for now)
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00   // Reserved
    };
    fwrite(header, 1, sizeof(header), fp);
    
    // Write fuzzer data
    if (size > 0) {
        fwrite(data, 1, size > 1024 ? 1024 : size, fp);
    }
    
    fclose(fp);
    
    return TIFFOpen("./dummy_file", "r+");
}

static void cleanup_test_tiff(TIFF* tif) {
    if (tif) {
        TIFFClose(tif);
    }
    remove("./dummy_file");
}

// Helper function to test TIFFVGetField with a specific argument
static int test_vgetfield_with_arg(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int result = TIFFVGetField(tif, tag, ap);
    va_end(ap);
    return result;
}

// Helper function to test TIFFVGetFieldDefaulted with a specific argument
static int test_vgetfielddefaulted_with_arg(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int result = TIFFVGetFieldDefaulted(tif, tag, ap);
    va_end(ap);
    return result;
}

// Helper function to test TIFFVSetField with a specific argument
static int test_vsetfield_with_arg(TIFF* tif, uint32_t tag, ...) {
    va_list ap;
    va_start(ap, tag);
    int result = TIFFVSetField(tif, tag, ap);
    va_end(ap);
    return result;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    TIFF* tif = create_test_tiff(Data, Size);
    if (!tif) return 0;
    
    // Extract some values from fuzzer input for parameters
    uint32_t tag;
    memcpy(&tag, Data, sizeof(uint32_t));
    
    TIFFDataType dtype = (TIFFDataType)(Data[0] % 13); // 13 TIFFDataType values
    
    // Test TIFFFindField
    const TIFFField* field1 = TIFFFindField(tif, tag, dtype);
    
    // Test TIFFFieldWithTag
    const TIFFField* field2 = TIFFFieldWithTag(tif, tag);
    
    // Test TIFFVGetField with various argument types
    if (field1 || field2) {
        // Prepare arguments based on data type
        switch (dtype) {
            case TIFF_BYTE:
            case TIFF_SBYTE:
            case TIFF_UNDEFINED: {
                uint8_t byte_val = Data[0];
                test_vgetfield_with_arg(tif, tag, &byte_val);
                test_vgetfielddefaulted_with_arg(tif, tag, &byte_val);
                
                // Test TIFFGetFieldDefaulted
                uint8_t out_byte;
                TIFFGetFieldDefaulted(tif, tag, &out_byte);
                break;
            }
            case TIFF_ASCII: {
                // Create a null-terminated string from fuzzer data
                char str_val[101] = {0};
                size_t str_len = Size > 4 ? Size - 4 : 0;
                if (str_len > 100) str_len = 100;
                if (str_len > 0) {
                    memcpy(str_val, Data + 4, str_len);
                    str_val[str_len] = '\0';
                }
                
                test_vgetfield_with_arg(tif, tag, str_val);
                test_vgetfielddefaulted_with_arg(tif, tag, str_val);
                
                // Test TIFFGetFieldDefaulted
                char* out_str;
                TIFFGetFieldDefaulted(tif, tag, &out_str);
                break;
            }
            case TIFF_SHORT:
            case TIFF_SSHORT: {
                uint16_t short_val = 0;
                if (Size >= 6) {
                    memcpy(&short_val, Data + 4, sizeof(uint16_t));
                }
                
                test_vgetfield_with_arg(tif, tag, &short_val);
                test_vgetfielddefaulted_with_arg(tif, tag, &short_val);
                
                // Test TIFFGetFieldDefaulted
                uint16_t out_short;
                TIFFGetFieldDefaulted(tif, tag, &out_short);
                break;
            }
            case TIFF_LONG:
            case TIFF_SLONG:
            case TIFF_IFD: {
                uint32_t long_val = 0;
                if (Size >= 8) {
                    memcpy(&long_val, Data + 4, sizeof(uint32_t));
                }
                
                test_vgetfield_with_arg(tif, tag, &long_val);
                test_vgetfielddefaulted_with_arg(tif, tag, &long_val);
                
                // Test TIFFGetFieldDefaulted
                uint32_t out_long;
                TIFFGetFieldDefaulted(tif, tag, &out_long);
                break;
            }
            case TIFF_LONG8:
            case TIFF_SLONG8:
            case TIFF_IFD8: {
                uint64_t long8_val = 0;
                if (Size >= 12) {
                    memcpy(&long8_val, Data + 4, sizeof(uint64_t));
                }
                
                test_vgetfield_with_arg(tif, tag, &long8_val);
                test_vgetfielddefaulted_with_arg(tif, tag, &long8_val);
                
                // Test TIFFGetFieldDefaulted
                uint64_t out_long8;
                TIFFGetFieldDefaulted(tif, tag, &out_long8);
                break;
            }
            case TIFF_RATIONAL:
            case TIFF_SRATIONAL: {
                uint32_t rational[2] = {0, 1};
                if (Size >= 12) {
                    memcpy(&rational[0], Data + 4, sizeof(uint32_t));
                    memcpy(&rational[1], Data + 8, sizeof(uint32_t));
                }
                
                test_vgetfield_with_arg(tif, tag, &rational);
                test_vgetfielddefaulted_with_arg(tif, tag, &rational);
                
                // Test TIFFGetFieldDefaulted
                float out_float;
                TIFFGetFieldDefaulted(tif, tag, &out_float);
                break;
            }
            case TIFF_FLOAT: {
                float float_val = 0.0f;
                if (Size >= 8) {
                    memcpy(&float_val, Data + 4, sizeof(float));
                }
                
                test_vgetfield_with_arg(tif, tag, &float_val);
                test_vgetfielddefaulted_with_arg(tif, tag, &float_val);
                
                // Test TIFFGetFieldDefaulted
                float out_float;
                TIFFGetFieldDefaulted(tif, tag, &out_float);
                break;
            }
            case TIFF_DOUBLE: {
                double double_val = 0.0;
                if (Size >= 12) {
                    memcpy(&double_val, Data + 4, sizeof(double));
                }
                
                test_vgetfield_with_arg(tif, tag, &double_val);
                test_vgetfielddefaulted_with_arg(tif, tag, &double_val);
                
                // Test TIFFGetFieldDefaulted
                double out_double;
                TIFFGetFieldDefaulted(tif, tag, &out_double);
                break;
            }
            default:
                // Unknown type, use generic approach
                test_vgetfield_with_arg(tif, tag, NULL);
                test_vgetfielddefaulted_with_arg(tif, tag, NULL);
                break;
        }
        
        // Test TIFFVSetField with the same data
        if (Size > 8) {
            switch (dtype) {
                case TIFF_BYTE:
                case TIFF_SBYTE:
                case TIFF_UNDEFINED: {
                    uint8_t set_byte = Data[8];
                    test_vsetfield_with_arg(tif, tag, set_byte);
                    break;
                }
                case TIFF_ASCII: {
                    char set_str[101] = {0};
                    size_t str_len = Size > 8 ? Size - 8 : 0;
                    if (str_len > 100) str_len = 100;
                    if (str_len > 0) {
                        memcpy(set_str, Data + 8, str_len);
                        set_str[str_len] = '\0';
                    }
                    test_vsetfield_with_arg(tif, tag, set_str);
                    break;
                }
                case TIFF_SHORT:
                case TIFF_SSHORT: {
                    uint16_t set_short = 0;
                    if (Size >= 10) {
                        memcpy(&set_short, Data + 8, sizeof(uint16_t));
                    }
                    test_vsetfield_with_arg(tif, tag, set_short);
                    break;
                }
                case TIFF_LONG:
                case TIFF_SLONG:
                case TIFF_IFD: {
                    uint32_t set_long = 0;
                    if (Size >= 12) {
                        memcpy(&set_long, Data + 8, sizeof(uint32_t));
                    }
                    test_vsetfield_with_arg(tif, tag, set_long);
                    break;
                }
                default:
                    // Try generic set
                    test_vsetfield_with_arg(tif, tag, NULL);
                    break;
            }
        }
    }
    
    // Try some common TIFF tags with different data types
    uint32_t common_tags[] = {
        256,  // ImageWidth
        257,  // ImageLength
        258,  // BitsPerSample
        259,  // Compression
        262,  // PhotometricInterpretation
        273,  // StripOffsets
        277,  // SamplesPerPixel
        278,  // RowsPerStrip
        279,  // StripByteCounts
        282,  // XResolution
        283,  // YResolution
        296,  // ResolutionUnit
        306,  // DateTime
        315,  // Artist
        316,  // HostComputer
        317,  // Predictor
        320,  // ColorMap
        338,  // ExtraSamples
        339,  // SampleFormat
    };
    
    for (size_t i = 0; i < sizeof(common_tags)/sizeof(common_tags[0]) && i < 5; i++) {
        uint32_t common_tag = common_tags[i];
        
        // Test with various data types
        for (int dt = 0; dt < 5; dt++) {
            TIFFDataType test_dtype = (TIFFDataType)(dt);
            const TIFFField* field = TIFFFindField(tif, common_tag, test_dtype);
            if (field) {
                // Test with a simple integer argument
                uint32_t test_val = 42;
                test_vgetfield_with_arg(tif, common_tag, &test_val);
                test_vgetfielddefaulted_with_arg(tif, common_tag, &test_val);
                
                // Try to set if we have enough data
                if (Size > 12) {
                    uint32_t set_val;
                    memcpy(&set_val, Data + (i * 4) % Size, sizeof(uint32_t));
                    test_vsetfield_with_arg(tif, common_tag, set_val);
                }
            }
        }
    }
    
    cleanup_test_tiff(tif);
    return 0;
}