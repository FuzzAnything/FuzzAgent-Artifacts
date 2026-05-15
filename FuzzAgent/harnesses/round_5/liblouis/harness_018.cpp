#include <fuzzer/FuzzedDataProvider.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <memory>
#include <liblouis.h>

namespace harness_018 {
/*
 * Fuzzing harness for liblouis targeting CTO_Grouping operations (braille grouping functionality)
 * based on coverage analysis recommendations.
 * 
 * Target: CTO_Grouping opcode compilation identified as most critical coverage gap
 *         with 0% coverage and 526 blocked branches in compileRule function.
 * 
 * Specific focus areas:
 * 1. CTO_Grouping opcode compilation (line 2955 in compileTranslationTable.c)
 * 2. compileGrouping function (lines 2352-2412) with 0% coverage
 * 3. Group definition syntax: "grouping name (char1 char2) @ dots1,dots2"
 * 4. Group name lookup for pass_groupstart/pass_groupend operations (lines 2095, 2256)
 * 5. Error paths in compileGrouping: comma validation, length checks, parse failures
 * 6. Interaction with CTC_Math attribute setting (lines 2379-2388)
 * 7. Additional CTO_Math rules for group boundaries (lines 2402-2409)
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_009: Basic macro and grouping operations with incorrect grouping syntax
 * - harness_014: Macro compilation features
 * - harness_016: CTO_BackMatch compilation
 * - harness_017: CTO_BackMatch error paths
 * - harness_018: Deep CTO_Grouping compilation, correct grouping syntax,
 *                group name usage in multipass, error path testing (current)
 * 
 * Primary API: lou_compileString (direct entry to compileRule)
 * 
 * Strategy: Generate diverse grouping definitions with focus on:
 * 1. Correct grouping syntax per compileGrouping requirements
 * 2. Group name generation for lookup in multipass operations
 * 3. Error scenarios: missing comma, wrong character count, invalid dots
 * 4. Math attribute propagation for grouped characters
 * 5. Multipass operations using defined groups (pass_groupstart/pass_groupend)
 */



// Log callback to avoid excessive logging during fuzzing
static void quiet_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Helper to generate valid group name (letters and digits only)
static std::string generate_group_name(FuzzedDataProvider& fdp) {
    std::string name;
    int name_length = fdp.ConsumeIntegralInRange<int>(1, 20);
    
    for (int i = 0; i < name_length; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (char_type) {
            case 0: // lowercase letter
                name += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 1: // uppercase letter
                name += static_cast<char>('A' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 2: // digit (but not as first character)
                if (i > 0) {
                    name += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
                } else {
                    name += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                }
                break;
        }
    }
    
    return name;
}

// Helper to generate grouping definition with correct syntax
static std::string generate_grouping_definition(FuzzedDataProvider& fdp, const std::string& group_name) {
    std::string grouping;
    
    // Correct grouping format: grouping name (char1 char2) @ dots1,dots2
    grouping += "grouping ";
    grouping += group_name;
    grouping += " (";
    
    // Generate two Unicode characters (can be letters, digits, or special chars)
    for (int i = 0; i < 2; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (char_type) {
            case 0: // lowercase letter
                grouping += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 1: // uppercase letter
                grouping += static_cast<char>('A' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 2: // digit
                grouping += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
                break;
            case 3: // special character (ASCII range 33-126)
                grouping += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 90));
                break;
        }
        if (i == 0) grouping += " ";
    }
    
    grouping += ") @ ";
    
    // Generate two cells separated by comma
    std::string cell1, cell2;
    
    // First cell (1-6 digits)
    int cell1_len = fdp.ConsumeIntegralInRange<int>(1, 6);
    for (int i = 0; i < cell1_len; i++) {
        cell1 += static_cast<char>('1' + (fdp.ConsumeIntegral<uint8_t>() % 6));
    }
    
    // Second cell (1-6 digits)
    int cell2_len = fdp.ConsumeIntegralInRange<int>(1, 6);
    for (int i = 0; i < cell2_len; i++) {
        cell2 += static_cast<char>('1' + (fdp.ConsumeIntegral<uint8_t>() % 6));
    }
    
    grouping += cell1;
    grouping += ",";
    grouping += cell2;
    grouping += "\n";
    
    return grouping;
}

// Helper to generate grouping definition with error (to test error paths)
static std::string generate_grouping_with_error(FuzzedDataProvider& fdp, const std::string& group_name, int error_type) {
    std::string grouping;
    
    grouping += "grouping ";
    grouping += group_name;
    grouping += " (";
    
    switch (error_type % 8) {
        case 0: // Wrong character count (1 character instead of 2)
            grouping += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            grouping += ") @ 123,456\n";
            break;
            
        case 1: // Wrong character count (3 characters instead of 2)
            for (int i = 0; i < 3; i++) {
                grouping += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                if (i < 2) grouping += " ";
            }
            grouping += ") @ 123,456\n";
            break;
            
        case 2: // Missing comma in dots operand
            grouping += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            grouping += " ";
            grouping += static_cast<char>('b' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            grouping += ") @ 123456\n";  // No comma
            break;
            
        case 3: // Invalid dots (non-digit characters)
            grouping += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            grouping += " ";
            grouping += static_cast<char>('b' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            grouping += ") @ abc,def\n";
            break;
            
        case 4: // Missing second cell after comma
            grouping += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            grouping += " ";
            grouping += static_cast<char>('b' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            grouping += ") @ 123,\n";
            break;
            
        case 5: // Missing first cell before comma
            grouping += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            grouping += " ";
            grouping += static_cast<char>('b' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            grouping += ") @ ,456\n";
            break;
            
        case 6: // Empty parentheses
            grouping += ") @ 123,456\n";
            break;
            
        case 7: // Invalid character in name
            grouping += "invalid-name! (";
            grouping += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            grouping += " ";
            grouping += static_cast<char>('b' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            grouping += ") @ 123,456\n";
            break;
    }
    
    return grouping;
}

// Helper to generate multipass rule using a group
static std::string generate_multipass_with_group(FuzzedDataProvider& fdp, const std::string& group_name) {
    std::string multipass;
    
    multipass += "multipass 1\n\n";
    
    // Test part using groupstart/groupend
    uint8_t pass_op = fdp.ConsumeIntegral<uint8_t>() % 3;
    
    switch (pass_op) {
        case 0: // groupstart
            multipass += "pass2 ";
            multipass += group_name;
            multipass += " groupstart\n";
            break;
            
        case 1: // groupend
            multipass += "pass2 ";
            multipass += group_name;
            multipass += " groupend\n";
            break;
            
        case 2: // groupreplace (if supported)
            multipass += "pass2 ";
            multipass += group_name;
            multipass += " groupreplace @ 123\n";
            break;
    }
    
    multipass += "\n";
    
    return multipass;
}

// Helper to generate complete table with grouping features
static std::string generate_table_with_grouping_focus(FuzzedDataProvider& fdp) {
    std::string table_content;
    
    // Start with table header
    table_content += "# Table with grouping operations targeting CTO_Grouping coverage\n";
    table_content += "locale en_US.UTF-8\n\n";
    
    // Generate some groups first
    int num_groups = fdp.ConsumeIntegralInRange<int>(1, 5);
    std::vector<std::string> group_names;
    
    table_content += "# Group definitions\n";
    for (int i = 0; i < num_groups; i++) {
        std::string group_name = generate_group_name(fdp);
        group_names.push_back(group_name);
        
        // Decide whether to generate valid or error grouping
        if (fdp.ConsumeBool()) {
            table_content += generate_grouping_definition(fdp, group_name);
        } else {
            table_content += generate_grouping_with_error(fdp, group_name, fdp.ConsumeIntegral<int>());
        }
    }
    table_content += "\n";
    
    // Add some standard math characters (since grouping sets CTC_Math attribute)
    if (fdp.ConsumeBool()) {
        table_content += "# Math character definitions\n";
        int num_math = fdp.ConsumeIntegralInRange<int>(1, 10);
        for (int i = 0; i < num_math; i++) {
            table_content += "math ";
            table_content += static_cast<char>('+' + (fdp.ConsumeIntegral<uint8_t>() % 10));
            table_content += " ";
            table_content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            table_content += "\n";
        }
        table_content += "\n";
    }
    
    // Add some regular translation rules
    if (fdp.ConsumeBool()) {
        table_content += "# Standard translation rules\n";
        int num_rules = fdp.ConsumeIntegralInRange<int>(5, 20);
        for (int i = 0; i < num_rules; i++) {
            table_content += "letter ";
            table_content += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
            table_content += " @ ";
            table_content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 1234567));
            table_content += "\n";
        }
        table_content += "\n";
    }
    
    // Add multipass rules using the groups (if any valid groups were created)
    if (!group_names.empty() && fdp.ConsumeBool()) {
        table_content += "# Multipass operations using groups\n";
        
        // Use a random group from the ones we defined
        int group_idx = fdp.ConsumeIntegralInRange<int>(0, group_names.size() - 1);
        table_content += generate_multipass_with_group(fdp, group_names[group_idx]);
    }
    
    // Add display rules
    if (fdp.ConsumeBool()) {
        table_content += "# Display rules\n";
        table_content += "display\n";
        table_content += "dots46 @ 46\n";
        table_content += "dots123 @ 123\n\n";
    }
    
    // Add emphasis classes
    if (fdp.ConsumeBool()) {
        table_content += "# Emphasis classes\n";
        table_content += "emphclass 1 bold\n";
        table_content += "emphclass 2 italic\n\n";
    }
    
    return table_content;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for meaningful table generation
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    if (!initialized) {
        lou_registerLogCallback(quiet_log);
        initialized = 1;
    }
    
    // Set data path to point to tables directory (required for table resolution)
    std::string data_path = "/root/src/liblouis/tables";
    char* path_result = lou_setDataPath(data_path.c_str());
    (void)path_result; // Result may be NULL, that's okay
    
    // Generate table content focusing on grouping operations
    std::string table_content = generate_table_with_grouping_focus(fdp);
    
    // Create a unique temporary file name
    char temp_filename[] = "/tmp/table_grouping_XXXXXX.ctb";
    int fd = mkstemps(temp_filename, 4);
    if (fd == -1) {
        return 0;
    }
    
    // Write table content to file
    FILE* temp_file = fdopen(fd, "w");
    if (temp_file == NULL) {
        close(fd);
        return 0;
    }
    
    fwrite(table_content.c_str(), 1, table_content.length(), temp_file);
    fclose(temp_file);
    
    // Extract just the filename without path for compilation
    const char* filename = strrchr(temp_filename, '/');
    if (filename == NULL) {
        filename = temp_filename;
    } else {
        filename++; // Skip the '/'
    }
    
    // Compile the table using lou_compileString
    // Note: lou_compileString expects the table name without path when dataPath is set
    int result = lou_compileString(filename, table_content.c_str());
    
    // Also test compilation with NULL content (should fail but test error path)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        lou_compileString("dummy.ctb", NULL);
    }
    
    // Clean up
    unlink(temp_filename);
    
    // Free any allocated tables
    lou_free();
    
    // Return value doesn't matter for fuzzing
    (void)result;
    return 0;
}

} // namespace harness_018
