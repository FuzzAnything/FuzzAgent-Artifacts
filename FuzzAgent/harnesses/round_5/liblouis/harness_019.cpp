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

namespace harness_019 {
/*
 * Fuzzing harness for liblouis targeting compileGrouping function via lou_compileString API
 * based on coverage analysis recommendations.
 * 
 * Target: compileGrouping identified as most critical uncovered function with 162 undiscovered branches.
 * Focus on internal branch coverage gaps in table compilation features (currently 58.43% branch coverage).
 * 
 * Specific focus areas:
 * 1. Direct targeting of compileGrouping function (lines 2352-2412 in compileTranslationTable.c)
 * 2. Complex group definitions beyond basic syntax
 * 3. Nested group scenarios and group hierarchy
 * 4. Advanced group validation and error handling in compilation
 * 5. Interaction between grouping rules and other table compilation features
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_018: Focused on CTO_Grouping operations with correct grouping syntax
 * - harness_019: Focus on complex group scenarios, nested definitions, advanced validation
 *                and edge cases in compileGrouping function internals
 * 
 * Primary API: lou_compileString (direct entry to compileRule -> compileGrouping)
 * 
 * Strategy: Generate diverse grouping scenarios targeting internal branch coverage:
 * 1. Complex group name patterns (unicode, special chars, long names)
 * 2. Nested group references and group dependency chains
 * 3. Group validation edge cases (boundary conditions, character ranges)
 * 4. Error recovery scenarios in group compilation
 * 5. Interaction with CTC_Math attribute and other character attributes
 */



// Log callback to avoid excessive logging during fuzzing
static void quiet_log(logLevels level, const char* msg) {
    (void)level;
    (void)msg;
}

// Global initialization flag
static int initialized = 0;

// Helper to generate complex group names with varied patterns
static std::string generate_complex_group_name(FuzzedDataProvider& fdp) {
    std::string name;
    int name_length = fdp.ConsumeIntegralInRange<int>(1, 50);
    
    for (int i = 0; i < name_length; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 6;
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
            case 3: // underscore
                name += '_';
                break;
            case 4: // hyphen
                name += '-';
                break;
            case 5: // extended ASCII (may cause validation issues)
                name += static_cast<char>(128 + (fdp.ConsumeIntegral<uint8_t>() % 128));
                break;
        }
    }
    
    return name;
}

// Helper to generate group characters with varied Unicode ranges
static std::string generate_group_characters(FuzzedDataProvider& fdp) {
    std::string chars;
    
    for (int i = 0; i < 2; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        switch (char_type) {
            case 0: // Basic Latin letter
                chars += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 1: // Basic Latin uppercase
                chars += static_cast<char>('A' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 2: // Digit
                chars += static_cast<char>('0' + (fdp.ConsumeIntegral<uint8_t>() % 10));
                break;
            case 3: // Punctuation
                chars += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15));
                break;
            case 4: // Space character (edge case)
                chars += ' ';
                break;
            case 5: // Tab character (edge case)
                chars += '\t';
                break;
            case 6: // Extended ASCII
                chars += static_cast<char>(128 + (fdp.ConsumeIntegral<uint8_t>() % 128));
                break;
            case 7: // Unicode surrogate/control character (edge case)
                chars += static_cast<char>(fdp.ConsumeIntegralInRange<uint8_t>(0, 31));
                break;
        }
        if (i == 0) chars += " ";
    }
    
    return chars;
}

// Helper to generate dots cells with varied patterns
static std::string generate_dots_cells(FuzzedDataProvider& fdp) {
    std::string dots;
    
    // First cell
    int cell1_len = fdp.ConsumeIntegralInRange<int>(0, 10);  // 0 length for error case
    if (cell1_len == 0) {
        dots = ",";
        int cell2_len = fdp.ConsumeIntegralInRange<int>(1, 6);
        for (int i = 0; i < cell2_len; i++) {
            dots += static_cast<char>('1' + (fdp.ConsumeIntegral<uint8_t>() % 6));
        }
        return dots;
    }
    
    for (int i = 0; i < cell1_len; i++) {
        uint8_t dot_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (dot_type) {
            case 0: // Valid dot (1-6)
                dots += static_cast<char>('1' + (fdp.ConsumeIntegral<uint8_t>() % 6));
                break;
            case 1: // Invalid dot (7-9)
                dots += static_cast<char>('7' + (fdp.ConsumeIntegral<uint8_t>() % 3));
                break;
            case 2: // Letter (invalid)
                dots += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 3: // Special character
                dots += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15));
                break;
        }
    }
    
    dots += ",";
    
    // Second cell
    int cell2_len = fdp.ConsumeIntegralInRange<int>(0, 10);  // 0 length for error case
    if (cell2_len == 0) {
        return dots;  // Missing second cell
    }
    
    for (int i = 0; i < cell2_len; i++) {
        uint8_t dot_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (dot_type) {
            case 0: // Valid dot (1-6)
                dots += static_cast<char>('1' + (fdp.ConsumeIntegral<uint8_t>() % 6));
                break;
            case 1: // Invalid dot (7-9)
                dots += static_cast<char>('7' + (fdp.ConsumeIntegral<uint8_t>() % 3));
                break;
            case 2: // Letter (invalid)
                dots += static_cast<char>('a' + (fdp.ConsumeIntegral<uint8_t>() % 26));
                break;
            case 3: // Special character
                dots += static_cast<char>('!' + (fdp.ConsumeIntegral<uint8_t>() % 15));
                break;
        }
    }
    
    return dots;
}

// Helper to generate grouping definition targeting specific compileGrouping branches
static std::string generate_targeted_grouping(FuzzedDataProvider& fdp, const std::string& group_name, int target_branch) {
    std::string grouping;
    
    grouping += "grouping ";
    grouping += group_name;
    grouping += " (";
    
    switch (target_branch % 12) {
        case 0: // Standard valid grouping
            grouping += generate_group_characters(fdp);
            grouping += ") @ ";
            grouping += generate_dots_cells(fdp);
            grouping += "\n";
            break;
            
        case 1: // Missing space between characters (compileGrouping line 2360: getRuleCharsText)
            grouping += "ab";  // No space
            grouping += ") @ 123,456\n";
            break;
            
        case 2: // Three characters instead of two (line 2369: groupChars.length != 2)
            grouping += "a b c";
            grouping += ") @ 123,456\n";
            break;
            
        case 3: // One character instead of two (line 2369)
            grouping += "a";
            grouping += ") @ 123,456\n";
            break;
            
        case 4: // No comma in dots operand (line 2363: k == groupDots.length)
            grouping += generate_group_characters(fdp);
            grouping += ") @ 123456\n";  // No comma
            break;
            
        case 5: // Multiple commas in dots (parseDots edge case)
            grouping += generate_group_characters(fdp);
            grouping += ") @ 123,,456\n";
            break;
            
        case 6: // Empty dots operand
            grouping += generate_group_characters(fdp);
            grouping += ") @ \n";
            break;
            
        case 7: // Dots operand with only comma
            grouping += generate_group_characters(fdp);
            grouping += ") @ ,\n";
            break;
            
        case 8: // Invalid dots parsing (parseDots failure path)
            grouping += generate_group_characters(fdp);
            grouping += ") @ abc,def\n";
            break;
            
        case 9: // Single dot cell (dotsParsed.length != 2 test)
            grouping += generate_group_characters(fdp);
            grouping += ") @ 123\n";
            break;
            
        case 10: // Three dot cells
            grouping += generate_group_characters(fdp);
            grouping += ") @ 123,456,789\n";
            break;
            
        case 11: // Empty parentheses (getRuleCharsText failure)
            grouping += ") @ 123,456\n";
            break;
    }
    
    return grouping;
}

// Helper to generate table with complex group interactions
static std::string generate_table_with_complex_groups(FuzzedDataProvider& fdp) {
    std::string table_content;
    
    // Start with table header
    table_content += "# Table targeting compileGrouping function coverage gaps\n";
    table_content += "locale en_US.UTF-8\n";
    table_content += "nocross\n\n";
    
    // Generate primary groups
    int num_primary_groups = fdp.ConsumeIntegralInRange<int>(1, 8);
    std::vector<std::string> group_names;
    
    table_content += "# Primary group definitions\n";
    for (int i = 0; i < num_primary_groups; i++) {
        std::string group_name = generate_complex_group_name(fdp);
        group_names.push_back(group_name);
        
        // Target specific branches in compileGrouping
        table_content += generate_targeted_grouping(fdp, group_name, fdp.ConsumeIntegral<int>());
    }
    table_content += "\n";
    
    // Generate dependent groups that reference other groups
    if (!group_names.empty() && fdp.ConsumeBool()) {
        table_content += "# Dependent group definitions\n";
        int num_dependent = fdp.ConsumeIntegralInRange<int>(1, 4);
        
        for (int i = 0; i < num_dependent; i++) {
            std::string dep_name = "dep_" + generate_complex_group_name(fdp);
            
            // Create dependent group that uses same characters as primary group
            table_content += "grouping ";
            table_content += dep_name;
            table_content += " (";
            
            // Reference characters from primary group
            int ref_idx = fdp.ConsumeIntegralInRange<int>(0, group_names.size() - 1);
            table_content += "x y";  // Different characters to avoid conflict
            table_content += ") @ ";
            table_content += generate_dots_cells(fdp);
            table_content += "\n";
        }
        table_content += "\n";
    }
    
    // Add math rules that interact with grouping (CTC_Math attribute setting)
    table_content += "# Math rules for group character attribute testing\n";
    int num_math = fdp.ConsumeIntegralInRange<int>(0, 10);
    for (int i = 0; i < num_math; i++) {
        table_content += "math ";
        table_content += static_cast<char>('+' + (fdp.ConsumeIntegral<uint8_t>() % 20));
        table_content += " ";
        table_content += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 123456));
        table_content += "\n";
    }
    table_content += "\n";
    
    // Add multipass rules using groups (tests group name lookup)
    if (!group_names.empty() && fdp.ConsumeBool()) {
        table_content += "# Multipass operations with groups\n";
        table_content += "multipass 2\n\n";
        
        // Use a random group
        int group_idx = fdp.ConsumeIntegralInRange<int>(0, group_names.size() - 1);
        
        table_content += "pass2 ";
        table_content += group_names[group_idx];
        table_content += " groupstart\n";
        
        table_content += "pass2 ";
        table_content += group_names[group_idx];
        table_content += " groupend\n\n";
    }
    
    // Add display rules (tests displayTable interaction)
    if (fdp.ConsumeBool()) {
        table_content += "# Display rules\n";
        table_content += "display\n";
        table_content += "dots123 @ 123\n";
        table_content += "dots456 @ 456\n\n";
    }
    
    // Add emphasis classes for additional table complexity
    if (fdp.ConsumeBool()) {
        table_content += "# Emphasis classes\n";
        table_content += "emphclass 1 bold\n";
        table_content += "emphclass 2 italic\n\n";
    }
    
    // Add invalid grouping to test error recovery
    if (fdp.ConsumeBool()) {
        table_content += "# Invalid grouping (error path testing)\n";
        table_content += "grouping invalid! (a b) @ 123,456\n";
        table_content += "grouping missing_dots (x y) @ \n";
        table_content += "grouping wrong_chars (single) @ 123,456\n\n";
    }
    
    return table_content;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for complex table generation
    if (size < 150) {
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
    
    // Generate table content with complex group scenarios
    std::string table_content = generate_table_with_complex_groups(fdp);
    
    // Create a unique temporary file name
    char temp_filename[] = "/tmp/table_complex_group_XXXXXX.ctb";
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
    
    // Compile the table using lou_compileString - this directly exercises compileGrouping
    // through the compilation chain: lou_compileString -> compileString -> compileRule -> compileGrouping
    int result = lou_compileString(filename, table_content.c_str());
    
    // Test edge cases: NULL input, empty string, very long string
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        // Test with NULL string (should fail but exercise error path)
        lou_compileString("dummy.ctb", NULL);
        
        // Test with empty string
        lou_compileString("empty.ctb", "");
        
        // Test with very long string (potential buffer overflow testing)
        std::string long_string(10000, 'x');
        lou_compileString("long.ctb", long_string.c_str());
    }
    
    // Clean up
    unlink(temp_filename);
    
    // Free any allocated tables
    lou_free();
    
    // Return value doesn't matter for fuzzing
    (void)result;
    return 0;
}

} // namespace harness_019
