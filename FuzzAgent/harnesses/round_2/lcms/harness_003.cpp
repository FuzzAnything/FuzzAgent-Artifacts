#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>
#include <cstdio>

namespace harness_003 {
// Fuzzing harness for LittleCMS2 library
// Targets CGATS/IT8 file and cube file handling APIs in cmscgats.c module
// harness_003.cpp - Focus on CGATS/IT8 file handling functions
// This module currently has 0% coverage, so this harness aims to maximize coverage


// Helper function to generate synthetic CGATS/IT8 data from fuzzer input
static std::string GenerateCGATSData(FuzzedDataProvider& fdp) {
    std::string cgats_data;
    
    // CGATS header
    cgats_data += "CGATS.17\n";
    cgats_data += "BEGIN_DATA_FORMAT\n";
    
    // Generate random number of fields (1-10)
    int num_fields = fdp.ConsumeIntegralInRange<int>(1, 10);
    std::vector<std::string> field_names;
    
    for (int i = 0; i < num_fields; i++) {
        std::string field_name = "FIELD_" + std::to_string(i);
        if (fdp.ConsumeBool()) {
            field_name = fdp.ConsumeRandomLengthString(16);
        }
        field_names.push_back(field_name);
        cgats_data += field_name;
        if (i < num_fields - 1) cgats_data += "\t";
    }
    cgats_data += "\nEND_DATA_FORMAT\n";
    
    // Add NUMBER_OF_FIELDS property
    cgats_data += "NUMBER_OF_FIELDS\t" + std::to_string(num_fields) + "\n";
    
    // Generate random number of patches/sets (1-20)
    int num_patches = fdp.ConsumeIntegralInRange<int>(1, 20);
    cgats_data += "NUMBER_OF_SETS\t" + std::to_string(num_patches) + "\n";
    
    // Add BEGIN_DATA section
    cgats_data += "BEGIN_DATA\n";
    
    // Generate patch data
    for (int patch = 0; patch < num_patches; patch++) {
        for (int field = 0; field < num_fields; field++) {
            // Generate random data values
            if (fdp.ConsumeBool()) {
                // Numeric value
                double val = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
                cgats_data += std::to_string(val);
            } else {
                // String value
                std::string str_val = fdp.ConsumeRandomLengthString(16);
                cgats_data += str_val;
            }
            if (field < num_fields - 1) cgats_data += "\t";
        }
        cgats_data += "\n";
    }
    cgats_data += "END_DATA\n";
    
    // Add some optional properties
    if (fdp.ConsumeBool()) {
        cgats_data += "DESCRIPTION\t" + fdp.ConsumeRandomLengthString(32) + "\n";
    }
    if (fdp.ConsumeBool()) {
        cgats_data += "ORIGINATOR\t" + fdp.ConsumeRandomLengthString(32) + "\n";
    }
    if (fdp.ConsumeBool()) {
        cgats_data += "CREATED\t" + fdp.ConsumeRandomLengthString(32) + "\n";
    }
    
    return cgats_data;
}

// Helper function to generate synthetic cube data from fuzzer input
static std::string GenerateCubeData(FuzzedDataProvider& fdp) {
    std::string cube_data;
    
    // Cube header
    cube_data += "TITLE \"" + fdp.ConsumeRandomLengthString(32) + "\"\n";
    
    // Generate LUT sizes
    int lut1d_size = fdp.ConsumeIntegralInRange<int>(2, 20);
    int lut3d_size = fdp.ConsumeIntegralInRange<int>(2, 10);
    
    cube_data += "LUT_1D_SIZE " + std::to_string(lut1d_size) + "\n";
    cube_data += "LUT_3D_SIZE " + std::to_string(lut3d_size) + "\n";
    
    // Domain ranges
    cube_data += "DOMAIN_MIN 0.0 0.0 0.0\n";
    cube_data += "DOMAIN_MAX 1.0 1.0 1.0\n";
    
    // Input ranges
    cube_data += "LUT_1D_INPUT_RANGE 0.0 1.0\n";
    cube_data += "LUT_3D_INPUT_RANGE 0.0 1.0\n";
    
    // 1D LUT data
    cube_data += "\n# 1D LUT\n";
    for (int i = 0; i < lut1d_size; i++) {
        for (int c = 0; c < 3; c++) {
            double val = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
            cube_data += std::to_string(val);
            if (c < 2) cube_data += " ";
        }
        cube_data += "\n";
    }
    
    // 3D LUT data  
    cube_data += "\n# 3D LUT\n";
    int total_3d_points = lut3d_size * lut3d_size * lut3d_size;
    for (int i = 0; i < total_3d_points; i++) {
        for (int c = 0; c < 3; c++) {
            double val = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
            cube_data += std::to_string(val);
            if (c < 2) cube_data += " ";
        }
        cube_data += "\n";
    }
    
    return cube_data;
}

// Test IT8/CGATS property manipulation functions
static void TestIT8Properties(cmsHANDLE hIT8, FuzzedDataProvider& fdp) {
    if (!hIT8) return;
    
    // Set and get various properties
    const char* prop_names[] = {"DESCRIPTION", "ORIGINATOR", "CREATED", "MANUFACTURER", "MODEL"};
    const int num_props = sizeof(prop_names) / sizeof(prop_names[0]);
    
    for (int i = 0; i < num_props && fdp.remaining_bytes() > 10; i++) {
        if (fdp.ConsumeBool()) {
            std::string prop_value = fdp.ConsumeRandomLengthString(32);
            cmsIT8SetPropertyStr(hIT8, prop_names[i], prop_value.c_str());
            
            // Verify property was set
            const char* retrieved = cmsIT8GetProperty(hIT8, prop_names[i]);
            if (retrieved) {
                // Just accessing is enough for coverage
            }
        }
    }
    
    // Test numeric properties
    if (fdp.ConsumeBool()) {
        double num_val = fdp.ConsumeFloatingPointInRange<double>(0.0, 1000.0);
        cmsIT8SetPropertyDbl(hIT8, "NUMERIC_PROP", num_val);
        
        double retrieved = cmsIT8GetPropertyDbl(hIT8, "NUMERIC_PROP");
        // Just accessing is enough
    }
    
    // Test hex properties
    if (fdp.ConsumeBool()) {
        uint32_t hex_val = fdp.ConsumeIntegral<uint32_t>();
        cmsIT8SetPropertyHex(hIT8, "HEX_PROP", hex_val);
    }
    
    // Test multi properties
    if (fdp.ConsumeBool()) {
        std::string key = fdp.ConsumeRandomLengthString(16);
        std::string subkey = fdp.ConsumeRandomLengthString(16);
        std::string value = fdp.ConsumeRandomLengthString(32);
        cmsIT8SetPropertyMulti(hIT8, key.c_str(), subkey.c_str(), value.c_str());
        
        const char* retrieved = cmsIT8GetPropertyMulti(hIT8, key.c_str(), subkey.c_str());
        if (retrieved) {
            // Just accessing
        }
    }
    
    // Test property enumeration
    char** prop_list = NULL;
    cmsUInt32Number prop_count = cmsIT8EnumProperties(hIT8, &prop_list);
    if (prop_list) {
        // Just enumerating is enough for coverage
    }
}

// Test IT8/CGATS data manipulation functions
static void TestIT8Data(cmsHANDLE hIT8, FuzzedDataProvider& fdp) {
    if (!hIT8) return;
    
    // Get table count and set different tables
    cmsUInt32Number table_count = cmsIT8TableCount(hIT8);
    if (table_count > 0) {
        for (cmsUInt32Number i = 0; i < table_count && i < 5; i++) {
            if (fdp.ConsumeBool()) {
                cmsIT8SetTable(hIT8, i);
                
                // Test getting sheet type
                const char* sheet_type = cmsIT8GetSheetType(hIT8);
                if (sheet_type) {
                    // Just accessing
                }
                
                // Test data format enumeration
                char** format_list = NULL;
                int format_count = cmsIT8EnumDataFormat(hIT8, &format_list);
                if (format_list) {
                    // Just enumerating
                }
                
                // Test getting data by row/col
                int max_rows = fdp.ConsumeIntegralInRange<int>(0, 10);
                int max_cols = fdp.ConsumeIntegralInRange<int>(0, 10);
                
                for (int r = 0; r < max_rows && r < 10; r++) {
                    for (int c = 0; c < max_cols && c < 10; c++) {
                        if (fdp.ConsumeBool()) {
                            const char* cell = cmsIT8GetDataRowCol(hIT8, r, c);
                            if (cell) {
                                // Just accessing
                            }
                            
                            // Try to get as double
                            double dval = cmsIT8GetDataRowColDbl(hIT8, r, c);
                            // Just accessing
                        }
                    }
                }
                
                // Test setting data
                if (fdp.remaining_bytes() > 20) {
                    int set_row = fdp.ConsumeIntegralInRange<int>(0, 5);
                    int set_col = fdp.ConsumeIntegralInRange<int>(0, 5);
                    std::string set_value = fdp.ConsumeRandomLengthString(16);
                    cmsIT8SetDataRowCol(hIT8, set_row, set_col, set_value.c_str());
                    
                    // Set as double
                    if (fdp.ConsumeBool()) {
                        double dval = fdp.ConsumeFloatingPoint<double>();
                        cmsIT8SetDataRowColDbl(hIT8, set_row, set_col, dval);
                    }
                }
                
                // Test patch name functions
                int max_patches = fdp.ConsumeIntegralInRange<int>(0, 10);
                for (int p = 0; p < max_patches && p < 10; p++) {
                    char patch_buffer[256];
                    const char* patch_name = cmsIT8GetPatchName(hIT8, p, patch_buffer);
                    if (patch_name) {
                        // Just accessing
                        
                        // Test getting patch by name
                        int patch_idx = cmsIT8GetPatchByName(hIT8, patch_name);
                        // Just accessing
                    }
                }
                
                // Test getting/setting data by patch/sample name
                if (fdp.remaining_bytes() > 30) {
                    std::string patch_name = "PATCH_" + std::to_string(fdp.ConsumeIntegral<int>());
                    std::string sample_name = "SAMPLE_" + std::to_string(fdp.ConsumeIntegral<int>());
                    
                    const char* data = cmsIT8GetData(hIT8, patch_name.c_str(), sample_name.c_str());
                    if (data) {
                        // Just accessing
                        
                        double dval = cmsIT8GetDataDbl(hIT8, patch_name.c_str(), sample_name.c_str());
                        // Just accessing
                    }
                    
                    // Set data
                    if (fdp.ConsumeBool()) {
                        std::string set_val = fdp.ConsumeRandomLengthString(16);
                        cmsIT8SetData(hIT8, patch_name.c_str(), sample_name.c_str(), set_val.c_str());
                        
                        // Set as double
                        if (fdp.ConsumeBool()) {
                            double dval = fdp.ConsumeFloatingPoint<double>();
                            cmsIT8SetDataDbl(hIT8, patch_name.c_str(), sample_name.c_str(), dval);
                        }
                    }
                }
            }
        }
    }
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 128) {
        return 0;  // Need minimum data for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input into three parts: for CGATS data, cube data, and property/data testing
    size_t part_size = size / 3;
    if (part_size < 50) part_size = 50;
    
    // Create CGATS data from first part
    std::string cgats_input = fdp.ConsumeBytesAsString(part_size);
    
    // Try to load as CGATS/IT8
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(0, cgats_input.c_str(), (cmsUInt32Number)cgats_input.size());
    
    if (hIT8) {
        // Test property manipulation
        TestIT8Properties(hIT8, fdp);
        
        // Test data manipulation  
        TestIT8Data(hIT8, fdp);
        
        // Test saving to memory
        if (fdp.ConsumeBool()) {
            cmsUInt32Number bytes_needed = 0;
            cmsIT8SaveToMem(hIT8, NULL, &bytes_needed);
            
            if (bytes_needed > 0 && bytes_needed < 1024 * 1024) {
                std::vector<char> save_buffer(bytes_needed);
                cmsIT8SaveToMem(hIT8, save_buffer.data(), &bytes_needed);
            }
        }
        
        // Test setting sheet type
        if (fdp.ConsumeBool()) {
            std::string sheet_type = fdp.ConsumeRandomLengthString(16);
            cmsIT8SetSheetType(hIT8, sheet_type.c_str());
        }
        
        // Test setting comment
        if (fdp.ConsumeBool()) {
            std::string comment = fdp.ConsumeRandomLengthString(64);
            cmsIT8SetComment(hIT8, comment.c_str());
        }
        
        // Test setting index column
        if (fdp.ConsumeBool()) {
            std::string index_col = fdp.ConsumeRandomLengthString(16);
            cmsIT8SetIndexColumn(hIT8, index_col.c_str());
        }
        
        // Test defining double format
        if (fdp.ConsumeBool()) {
            std::string format = fdp.ConsumeRandomLengthString(16);
            cmsIT8DefineDblFormat(hIT8, format.c_str());
        }
        
        // Test setting table by label
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 30) {
            std::string set_label = fdp.ConsumeRandomLengthString(16);
            std::string field_label = fdp.ConsumeRandomLengthString(16);
            std::string expected_type = fdp.ConsumeRandomLengthString(16);
            cmsIT8SetTableByLabel(hIT8, set_label.c_str(), field_label.c_str(), expected_type.c_str());
        }
        
        // Clean up
        cmsIT8Free(hIT8);
    }
    
    // Test cube file parsing if we have enough remaining data
    if (fdp.remaining_bytes() > 100) {
        std::string cube_input = fdp.ConsumeRemainingBytesAsString();
        
        // The cube parsing is internal to cmscgats.c, but we can test by trying to load it
        // Cube files should also be valid CGATS/IT8 files
        cmsHANDLE hCube = cmsIT8LoadFromMem(0, cube_input.c_str(), (cmsUInt32Number)cube_input.size());
        if (hCube) {
            // Cube-specific tests
            const char* sheet_type = cmsIT8GetSheetType(hCube);
            if (sheet_type && strstr(sheet_type, "CUBE")) {
                // This is a cube file - test some cube-specific functions
                // Note: Cube parsing functions are internal, so we just test the IT8 interface
                TestIT8Properties(hCube, fdp);
                TestIT8Data(hCube, fdp);
            }
            cmsIT8Free(hCube);
        }
    }
    
    return 0;
}

} // namespace harness_003
