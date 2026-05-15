#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <iomanip>

namespace harness_008 {
// Fuzzing harness for LittleCMS2 library
// harness_008.cpp - Advanced CGATS/IT8 file format processing targeting uncovered functions
// Targeting 250+ points of undiscovered complexity in CGATS/IT8 processing
// Focus on different aspects not covered by harness_003: property uncooked, find data format,
// more complex data manipulation, and advanced table operations


// Helper function to generate synthetic CGATS/IT8 data from fuzzer input
// Different from harness_003 to explore different data patterns
static std::string GenerateComplexCGATSData(FuzzedDataProvider& fdp) {
    std::stringstream ss;
    
    // Generate CGATS header with version
    int cgats_version = fdp.ConsumeIntegralInRange<int>(17, 21);
    ss << "CGATS." << cgats_version << "\n";
    
    // Add various header sections in different order
    std::vector<std::string> sections = {"DESCRIPTION", "ORIGINATOR", "CREATED", "MANUFACTURER", "MODEL", "DEVICE"};
    for (const auto& section : sections) {
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
            ss << section << "\t" << fdp.ConsumeRandomLengthString(32) << "\n";
        }
    }
    
    // DATA_FORMAT section
    ss << "BEGIN_DATA_FORMAT\n";
    
    int num_fields = fdp.ConsumeIntegralInRange<int>(1, 15);
    std::vector<std::string> field_types = {"SAMPLE_ID", "RGB_R", "RGB_G", "RGB_B", "XYZ_X", "XYZ_Y", "XYZ_Z", 
                                            "LAB_L", "LAB_A", "LAB_B", "STATUS", "TOLERANCE", "NOTES"};
    
    for (int i = 0; i < num_fields && i < (int)field_types.size(); i++) {
        ss << field_types[i];
        if (i < num_fields - 1) ss << "\t";
    }
    ss << "\nEND_DATA_FORMAT\n";
    
    // Properties
    ss << "NUMBER_OF_FIELDS\t" << num_fields << "\n";
    
    int num_sets = fdp.ConsumeIntegralInRange<int>(1, 50);
    ss << "NUMBER_OF_SETS\t" << num_sets << "\n";
    
    // Add some special properties
    if (fdp.ConsumeBool()) {
        ss << "SPECTRAL_BANDS\t" << fdp.ConsumeIntegralInRange<int>(10, 100) << "\n";
    }
    
    if (fdp.ConsumeBool()) {
        ss << "SPECTRAL_START_NM\t" << fdp.ConsumeIntegralInRange<int>(380, 780) << "\n";
    }
    
    if (fdp.ConsumeBool()) {
        ss << "SPECTRAL_END_NM\t" << fdp.ConsumeIntegralInRange<int>(380, 780) << "\n";
    }
    
    // DATA section
    ss << "BEGIN_DATA\n";
    
    for (int set = 0; set < num_sets; set++) {
        for (int field = 0; field < num_fields && field < (int)field_types.size(); field++) {
            // Generate appropriate data based on field type
            if (field_types[field].find("RGB") != std::string::npos || 
                field_types[field].find("XYZ") != std::string::npos ||
                field_types[field].find("LAB") != std::string::npos) {
                // Numeric data
                double val = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
                ss << val;
            } else if (field_types[field] == "SAMPLE_ID") {
                // Sample ID
                ss << "S" << std::setw(3) << std::setfill('0') << (set + 1);
            } else {
                // String data
                ss << fdp.ConsumeRandomLengthString(20);
            }
            
            if (field < num_fields - 1) ss << "\t";
        }
        ss << "\n";
    }
    
    ss << "END_DATA\n";
    
    // Add optional footer sections
    if (fdp.ConsumeBool()) {
        ss << "BEGIN_METADATA\n";
        int num_meta = fdp.ConsumeIntegralInRange<int>(1, 10);
        for (int i = 0; i < num_meta; i++) {
            ss << fdp.ConsumeRandomLengthString(10) << "\t" 
               << fdp.ConsumeRandomLengthString(20) << "\n";
        }
        ss << "END_METADATA\n";
    }
    
    return ss.str();
}

// Test advanced property operations not covered in harness_003
static void TestAdvancedProperties(cmsHANDLE hIT8, FuzzedDataProvider& fdp) {
    if (!hIT8) return;
    
    // Test cmsIT8SetPropertyUncooked - not covered in harness_003
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        std::string uncooked_key = fdp.ConsumeRandomLengthString(16);
        std::string uncooked_value = fdp.ConsumeRandomLengthString(32);
        cmsIT8SetPropertyUncooked(hIT8, uncooked_key.c_str(), uncooked_value.c_str());
        
        // Try to retrieve it
        const char* retrieved = cmsIT8GetProperty(hIT8, uncooked_key.c_str());
        if (retrieved) {
            // Just accessing for coverage
        }
    }
    
    // Test multiple properties with special characters
    std::vector<std::string> special_props = {
        "PROP_WITH_SPACES",
        "PROP-WITH-DASHES",
        "PROP.WITH.DOTS",
        "PROP_WITH_UNDERSCORES",
        "123NUMERIC_START"
    };
    
    for (const auto& prop : special_props) {
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
            std::string value = fdp.ConsumeRandomLengthString(20);
            cmsIT8SetPropertyStr(hIT8, prop.c_str(), value.c_str());
        }
    }
    
    // Test property enumeration with cleanup
    char** prop_list = NULL;
    cmsUInt32Number prop_count = cmsIT8EnumProperties(hIT8, &prop_list);
    
    // Access some properties from the list for coverage
    if (prop_list) {
        for (cmsUInt32Number i = 0; i < prop_count && i < 5; i++) {
            const char* prop_name = prop_list[i];
            if (prop_name) {
                const char* prop_value = cmsIT8GetProperty(hIT8, prop_name);
                if (prop_value) {
                    // Just accessing
                }
            }
        }
    }
    
    // Test cmsIT8EnumPropertyMulti - not thoroughly covered in harness_003
    if (prop_list && prop_count > 0) {
        for (cmsUInt32Number i = 0; i < prop_count && i < 3; i++) {
            const char* prop_name = prop_list[i];
            if (prop_name && fdp.ConsumeBool()) {
                const char** subprop_list = NULL;
                cmsUInt32Number subprop_count = cmsIT8EnumPropertyMulti(hIT8, prop_name, &subprop_list);
                
                if (subprop_list) {
                    // Just enumerating for coverage
                }
            }
        }
    }
}

// Test advanced data format operations
static void TestAdvancedDataFormat(cmsHANDLE hIT8, FuzzedDataProvider& fdp) {
    if (!hIT8) return;
    
    // Test cmsIT8FindDataFormat - not covered in harness_003
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
        // First enumerate existing data formats
        char** format_list = NULL;
        int format_count = cmsIT8EnumDataFormat(hIT8, &format_list);
        
        if (format_list && format_count > 0) {
            // Try to find some of them
            for (int i = 0; i < format_count && i < 3; i++) {
                const char* format_name = format_list[i];
                if (format_name) {
                    int found_idx = cmsIT8FindDataFormat(hIT8, format_name);
                    // Just calling for coverage
                }
            }
            
            // Try to find a non-existent format
            std::string non_existent = fdp.ConsumeRandomLengthString(16);
            int not_found = cmsIT8FindDataFormat(hIT8, non_existent.c_str());
            // Should return -1, just calling for coverage
        }
    }
    
    // Test setting data formats at specific indices
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        int format_idx = fdp.ConsumeIntegralInRange<int>(0, 10);
        std::string format_name = fdp.ConsumeRandomLengthString(16);
        cmsIT8SetDataFormat(hIT8, format_idx, format_name.c_str());
    }
    
    // Test data format enumeration with many formats
    char** all_formats = NULL;
    int all_format_count = cmsIT8EnumDataFormat(hIT8, &all_formats);
    if (all_formats) {
        // Just accessing for coverage
    }
}

// Test advanced patch operations
static void TestAdvancedPatchOperations(cmsHANDLE hIT8, FuzzedDataProvider& fdp) {
    if (!hIT8) return;
    
    // Test cmsIT8GetPatchByName with various patch names
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        // First get patch count
        char patch_buffer[256];
        
        // Get some patch names and try to find them
        int max_patches = 10;
        for (int i = 0; i < max_patches; i++) {
            const char* patch_name = cmsIT8GetPatchName(hIT8, i, patch_buffer);
            if (patch_name) {
                int patch_idx = cmsIT8GetPatchByName(hIT8, patch_name);
                // Just calling for coverage
                
                // Also test with modified name
                std::string modified_name = std::string(patch_name) + "_MODIFIED";
                int not_found = cmsIT8GetPatchByName(hIT8, modified_name.c_str());
                // Should return -1
            }
        }
    }
    
    // Test getting data by patch name with various sample names
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        // Get some patch names
        char patch_buffer[256];
        std::vector<std::string> patch_names;
        
        for (int i = 0; i < 5; i++) {
            const char* patch_name = cmsIT8GetPatchName(hIT8, i, patch_buffer);
            if (patch_name) {
                patch_names.push_back(patch_name);
            }
        }
        
        // Get data format names
        char** format_list = NULL;
        int format_count = cmsIT8EnumDataFormat(hIT8, &format_list);
        
        if (format_list && format_count > 0 && !patch_names.empty()) {
            // Try to get data for some combinations
            for (int i = 0; i < std::min(3, format_count); i++) {
                const char* sample_name = format_list[i];
                if (sample_name) {
                    for (const auto& patch_name : patch_names) {
                        const char* data = cmsIT8GetData(hIT8, patch_name.c_str(), sample_name);
                        if (data) {
                            // Try to get as double too
                            double dval = cmsIT8GetDataDbl(hIT8, patch_name.c_str(), sample_name);
                            // Just accessing
                        }
                    }
                }
            }
        }
    }
}

// Test table switching and table-specific operations
static void TestTableOperations(cmsHANDLE hIT8, FuzzedDataProvider& fdp) {
    if (!hIT8) return;
    
    // Get table count
    cmsUInt32Number table_count = cmsIT8TableCount(hIT8);
    
    if (table_count > 0) {
        // Switch between tables and perform operations
        for (cmsUInt32Number i = 0; i < table_count && i < 5; i++) {
            if (fdp.ConsumeBool()) {
                cmsIT8SetTable(hIT8, i);
                
                // Test operations in this table
                const char* sheet_type = cmsIT8GetSheetType(hIT8);
                if (sheet_type) {
                    // Just accessing
                }
                
                // Test setting sheet type
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
                    std::string new_type = fdp.ConsumeRandomLengthString(16);
                    cmsIT8SetSheetType(hIT8, new_type.c_str());
                }
                
                // Test comment setting
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
                    std::string comment = fdp.ConsumeRandomLengthString(64);
                    cmsIT8SetComment(hIT8, comment.c_str());
                }
            }
        }
    }
    
    // Test cmsIT8SetTableByLabel - more thoroughly than harness_003
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
        std::string set_label = fdp.ConsumeRandomLengthString(16);
        std::string field_label = fdp.ConsumeRandomLengthString(16);
        std::string expected_type = fdp.ConsumeRandomLengthString(16);
        
        int result = cmsIT8SetTableByLabel(hIT8, set_label.c_str(), field_label.c_str(), expected_type.c_str());
        // Just calling for coverage
    }
    
    // Test cmsIT8SetIndexColumn
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        std::string index_col = fdp.ConsumeRandomLengthString(16);
        cmsBool result = cmsIT8SetIndexColumn(hIT8, index_col.c_str());
        // Just calling for coverage
    }
    
    // Test cmsIT8DefineDblFormat with various formats
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        std::vector<std::string> formats = {
            "%.10g", "%.6f", "%.2f", "%.0f", "%e", "%E", "%g", "%G"
        };
        
        for (const auto& format : formats) {
            if (fdp.ConsumeBool()) {
                cmsIT8DefineDblFormat(hIT8, format.c_str());
            }
        }
    }
}

// Test data manipulation with various data types
static void TestDataManipulation(cmsHANDLE hIT8, FuzzedDataProvider& fdp) {
    if (!hIT8) return;
    
    // Get table dimensions
    char** format_list = NULL;
    int format_count = cmsIT8EnumDataFormat(hIT8, &format_list);
    
    if (!format_list || format_count <= 0) return;
    
    // Try setting and getting data at various positions
    int max_rows = 10;
    int max_cols = std::min(format_count, 10);
    
    for (int row = 0; row < max_rows; row++) {
        for (int col = 0; col < max_cols; col++) {
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 5) {
                // Get current value
                const char* current = cmsIT8GetDataRowCol(hIT8, row, col);
                
                // Set new value
                std::string new_value = fdp.ConsumeRandomLengthString(20);
                cmsBool set_result = cmsIT8SetDataRowCol(hIT8, row, col, new_value.c_str());
                
                // Try to get as double
                double dval = cmsIT8GetDataRowColDbl(hIT8, row, col);
                
                // Set as double
                if (fdp.ConsumeBool()) {
                    double new_dval = fdp.ConsumeFloatingPointInRange<double>(-1000.0, 1000.0);
                    cmsIT8SetDataRowColDbl(hIT8, row, col, new_dval);
                }
            }
        }
    }
}

// Test saving and loading with different options
static void TestSaveLoadOperations(cmsHANDLE hIT8, FuzzedDataProvider& fdp) {
    if (!hIT8) return;
    
    // Test saving to memory
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        cmsUInt32Number bytes_needed = 0;
        
        // First get size
        cmsBool size_result = cmsIT8SaveToMem(hIT8, NULL, &bytes_needed);
        
        if (size_result && bytes_needed > 0 && bytes_needed < 1024 * 1024) {
            // Allocate buffer and save
            std::vector<char> buffer(bytes_needed);
            cmsBool save_result = cmsIT8SaveToMem(hIT8, buffer.data(), &bytes_needed);
            
            // Try to load the saved data
            if (save_result) {
                cmsHANDLE hLoaded = cmsIT8LoadFromMem(0, buffer.data(), bytes_needed);
                if (hLoaded) {
                    // Do some operations on loaded data
                    const char* sheet_type = cmsIT8GetSheetType(hLoaded);
                    if (sheet_type) {
                        // Just accessing
                    }
                    cmsIT8Free(hLoaded);
                }
            }
        }
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need enough data for meaningful testing
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Generate complex CGATS data
    std::string cgats_data = GenerateComplexCGATSData(fdp);
    
    // Load the CGATS data
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(0, cgats_data.c_str(), (cmsUInt32Number)cgats_data.size());
    
    if (!hIT8) {
        // Try with empty context
        hIT8 = cmsIT8Alloc(0);
        if (!hIT8) return 0;
        
        // Set some basic properties
        cmsIT8SetSheetType(hIT8, "CGATS.17");
        
        // Add some data
        if (fdp.remaining_bytes() > 20) {
            cmsIT8SetPropertyStr(hIT8, "DESCRIPTION", fdp.ConsumeRandomLengthString(32).c_str());
        }
    }
    
    // Test advanced property operations (targeting uncovered functions)
    TestAdvancedProperties(hIT8, fdp);
    
    // Test advanced data format operations
    TestAdvancedDataFormat(hIT8, fdp);
    
    // Test advanced patch operations
    TestAdvancedPatchOperations(hIT8, fdp);
    
    // Test table operations
    TestTableOperations(hIT8, fdp);
    
    // Test data manipulation
    TestDataManipulation(hIT8, fdp);
    
    // Test save/load operations
    TestSaveLoadOperations(hIT8, fdp);
    
    // Clean up
    cmsIT8Free(hIT8);
    
    return 0;
}

} // namespace harness_008
