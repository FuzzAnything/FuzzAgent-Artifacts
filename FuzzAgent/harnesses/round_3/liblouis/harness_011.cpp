#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <liblouis/liblouis.h>
#include <assert.h>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>

namespace harness_011 {
// liblouis fuzzing harness - Deep compilation logic targeting compileRule function
// Targets: lou_compileString (primary), lou_checkTable (validation), lou_getTable (retrieval), 
//          lou_freeTableFiles (cleanup), lou_setDataPath (setup)
// Focus: Exercise deep compilation logic and blocked branches in compileRule function (339/910 branches blocked)
//        All opcode types (CTO_Macro, CTO_Letter, CTO_Grouping, CTO_Display, etc.)
//        Complex rule combinations with noback/nofor/nocross modifiers
//        Nested includes and macro definitions
//        Character class definitions with various attributes
//        Emphasis class specifications
// Based on coverage guidance: /root/FuzzAgent/output/liblouis/coverage_guidance/harness_011_guidance.md
// Uses FuzzedDataProvider for structured input processing


// Log callback to suppress output during fuzzing
void avoid_log(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Cleanup function
static void free_resources(void) {
    lou_free();
}

// Generate a character for various opcode definitions (Unicode-aware)
static std::string generate_unicode_char(FuzzedDataProvider& fdp) {
    uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (char_type) {
        case 0:
            // Basic ASCII letter
            return std::string(1, static_cast<char>(fdp.ConsumeIntegralInRange<int>('a', 'z')));
        case 1:
            // ASCII uppercase letter  
            return std::string(1, static_cast<char>(fdp.ConsumeIntegralInRange<int>('A', 'Z')));
        case 2:
            // Unicode character using \uXXXX escape (4 hex digits)
            {
                int unicode_val = fdp.ConsumeIntegralInRange<int>(0x0041, 0x024F);
                char hex_buf[7];
                snprintf(hex_buf, sizeof(hex_buf), "\\u%04X", unicode_val);
                return std::string(hex_buf);
            }
        case 3:
            // Hex character representation using \xXX escape (2 hex digits)
            {
                int hex_val = fdp.ConsumeIntegralInRange<int>(0x20, 0x7E);
                char hex_buf[5];
                snprintf(hex_buf, sizeof(hex_buf), "\\x%02X", hex_val);
                return std::string(hex_buf);
            }
        default:
            return "a";
    }
}

// Generate braille dots pattern with more diversity
static std::string generate_braille_dots(FuzzedDataProvider& fdp) {
    std::string dots;
    int dot_count = fdp.ConsumeIntegralInRange<int>(1, 12); // Up to 12 dots for extended braille
    
    for (int i = 0; i < dot_count && i < 8; i++) {
        int dot = fdp.ConsumeIntegralInRange<int>(1, 8);
        dots += std::to_string(dot);
    }
    
    // Add extended dots (9-16 as a-f) if we have more than 8
    if (dot_count > 8 && fdp.ConsumeBool()) {
        for (int i = 8; i < dot_count && i < 16; i++) {
            int ext_dot = fdp.ConsumeIntegralInRange<int>(0, 5);
            dots += static_cast<char>('a' + ext_dot);
        }
    }
    
    // Sometimes use hyphen for no dots
    if (dots.empty() || fdp.ConsumeBool()) {
        return "-";
    }
    
    return dots;
}

// Generate emphasis class definitions (targeting emphasis class specifications)
static std::string generate_emphasis_class(FuzzedDataProvider& fdp) {
    std::string emph_class;
    uint8_t emph_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (emph_type) {
        case 0:
            emph_class = "bold";
            break;
        case 1:
            emph_class = "italic";
            break;
        case 2:
            emph_class = "underline";
            break;
        case 3: {
            // Random emphasis class name
            int name_len = fdp.ConsumeIntegralInRange<int>(3, 12);
            for (int i = 0; i < name_len; i++) {
                emph_class += static_cast<char>(fdp.ConsumeIntegralInRange<int>('a', 'z'));
            }
            break;
        }
    }
    
    return emph_class;
}

// Generate table content with comprehensive opcode coverage for compileRule function
static std::string generate_comprehensive_table(FuzzedDataProvider& fdp) {
    std::string table;
    
    // Add optional ISO/UTF-8 header to test header processing paths
    if (fdp.ConsumeBool()) {
        uint8_t header_type = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (header_type) {
            case 0:
                table += "ISO 8859-1\n";
                break;
            case 1:
                table += "UTF-8\n";
                break;
            case 2:
                // No header to test default path
                break;
        }
    }
    
    // CTO_Letter opcode definitions (targeting CTO_Letter path)
    int letter_count = fdp.ConsumeIntegralInRange<int>(1, 8);
    for (int i = 0; i < letter_count && fdp.remaining_bytes() > 2; i++) {
        std::string letter_char = generate_unicode_char(fdp);
        std::string dots = generate_braille_dots(fdp);
        
        table += "letter \"";
        table += letter_char;
        table += "\" ";
        table += dots;
        table += "\n";
    }
    
    // CTO_Display opcode (targeting display rule compilation)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 2) {
        int display_count = fdp.ConsumeIntegralInRange<int>(1, 4);
        for (int i = 0; i < display_count && fdp.remaining_bytes() > 2; i++) {
            std::string display_char = generate_unicode_char(fdp);
            std::string display_dots = generate_braille_dots(fdp);
            
            table += "display \"";
            table += display_char;
            table += "\" ";
            table += display_dots;
            table += "\n";
        }
    }
    
    // CTO_Grouping opcode with various modifiers
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 3) {
        int grouping_count = fdp.ConsumeIntegralInRange<int>(1, 3);
        for (int i = 0; i < grouping_count && fdp.remaining_bytes() > 3; i++) {
            table += "grouping \"";
            
            int group_len = fdp.ConsumeIntegralInRange<int>(1, 4);
            for (int j = 0; j < group_len && fdp.remaining_bytes() > 0; j++) {
                table += generate_unicode_char(fdp);
            }
            
            table += "\" \"";
            table += generate_braille_dots(fdp);
            table += "\" ";
            
            // Add noback/nofor/nocross modifiers
            uint8_t modifier = fdp.ConsumeIntegral<uint8_t>() % 4;
            switch (modifier) {
                case 0:
                    table += "noback";
                    break;
                case 1:
                    table += "nofor";
                    break;
                case 2:
                    table += "nocross";
                    break;
                case 3:
                    table += "noback nofor";
                    break;
            }
            table += "\n";
        }
    }
    
    // Character class definitions (CTO_Space, CTO_Digit, CTO_Punctuation, etc.)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 2) {
        uint8_t class_type = fdp.ConsumeIntegral<uint8_t>() % 6;
        std::string class_opcode;
        
        switch (class_type) {
            case 0: class_opcode = "space"; break;
            case 1: class_opcode = "digit"; break;
            case 2: class_opcode = "litdigit"; break;
            case 3: class_opcode = "punctuation"; break;
            case 4: class_opcode = "math"; break;
            case 5: class_opcode = "sign"; break;
        }
        
        if (!class_opcode.empty()) {
            table += class_opcode;
            table += " \"";
            table += generate_unicode_char(fdp);
            table += "\" ";
            table += generate_braille_dots(fdp);
            table += "\n";
        }
    }
    
    // CTO_UpperCase and CTO_LowerCase
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 2) {
        if (fdp.ConsumeBool()) {
            table += "uppercase \"";
        } else {
            table += "lowercase \"";
        }
        table += generate_unicode_char(fdp);
        table += "\" ";
        table += generate_braille_dots(fdp);
        table += "\n";
    }
    
    // Emphasis class specifications (CTO_EmphClass)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 2) {
        std::string emph_class = generate_emphasis_class(fdp);
        table += "emphclass ";
        table += emph_class;
        table += " \"";
        table += generate_braille_dots(fdp);
        table += "\"\n";
    }
    
    // CTO_Macro definitions (if macros are enabled)
#ifdef ENABLE_MACROS
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        table += "macro test_macro_";
        table += std::to_string(fdp.ConsumeIntegral<uint32_t>() % 5);
        table += " \"";
        table += generate_braille_dots(fdp);
        table += "\"\n";
    }
#endif
    
    // Include directives (nested includes)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 2) {
        table += "include \"test_include_";
        table += std::to_string(fdp.ConsumeIntegral<uint32_t>() % 3);
        table += ".ctb\"\n";
    }
    
    // CTO_NoBack, CTO_NoFor, CTO_NoCross standalone modifiers
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        uint8_t standalone_mod = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (standalone_mod) {
            case 0:
                table += "noback\n";
                break;
            case 1:
                table += "nofor\n";
                break;
            case 2:
                table += "nocross\n";
                break;
        }
    }
    
    // Complex rule combinations with multiple modifiers
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
        table += "letter \"";
        table += generate_unicode_char(fdp);
        table += "\" ";
        table += generate_braille_dots(fdp);
        
        // Add multiple modifiers
        int mod_count = fdp.ConsumeIntegralInRange<int>(0, 3);
        for (int i = 0; i < mod_count; i++) {
            uint8_t mod_type = fdp.ConsumeIntegral<uint8_t>() % 3;
            switch (mod_type) {
                case 0:
                    table += " noback";
                    break;
                case 1:
                    table += " nofor";
                    break;
                case 2:
                    table += " nocross";
                    break;
            }
        }
        table += "\n";
    }
    
    // CTO_Attribute and CTO_Class opcodes
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 3) {
        if (fdp.ConsumeBool()) {
            table += "attribute \"";
        } else {
            table += "class \"";
        }
        table += generate_unicode_char(fdp);
        table += "\" ";
        table += std::to_string(fdp.ConsumeIntegralInRange<int>(1, 64));
        table += "\n";
    }
    
    // CTO_EmphLetter and related emphasis opcodes
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 2) {
        uint8_t emph_opcode = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (emph_opcode) {
            case 0:
                table += "emphletter \"";
                break;
            case 1:
                table += "begemphword \"";
                break;
            case 2:
                table += "endemphword \"";
                break;
            case 3:
                table += "begemph \"";
                break;
        }
        table += generate_braille_dots(fdp);
        table += "\"\n";
    }
    
    // Terminator
    table += "LOU_ENDSEGMENT\n";
    
    return table;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need enough data for comprehensive table generation
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Configure logging
    lou_registerLogCallback(avoid_log);
    
    // Set log level based on input
    uint8_t log_level_choice = fdp.ConsumeIntegral<uint8_t>() % 6;
    logLevels log_level = LOU_LOG_ALL;
    switch (log_level_choice) {
        case 0: log_level = LOU_LOG_ALL; break;
        case 1: log_level = LOU_LOG_DEBUG; break;
        case 2: log_level = LOU_LOG_INFO; break;
        case 3: log_level = LOU_LOG_WARN; break;
        case 4: log_level = LOU_LOG_ERROR; break;
        case 5: log_level = LOU_LOG_FATAL; break;
    }
    lou_setLogLevel(log_level);
    
    // Step 1a: Configure custom table directory using lou_setDataPath
    const char* temp_path = "/tmp/liblouis-fuzz-compilerule";
    mkdir(temp_path, 0755);
    char* old_path = lou_setDataPath(temp_path);
    (void)old_path; // Old path ignored
    
    // Step 1b: Generate table name from input
    std::string table_name = fdp.ConsumeRandomLengthString(48);
    if (table_name.empty() && fdp.remaining_bytes() > 0) {
        table_name = "fuzz-compile-table-" + std::to_string(fdp.ConsumeIntegral<uint32_t>());
    }
    
    // Step 2: Generate comprehensive table content targeting compileRule function
    std::string table_content = generate_comprehensive_table(fdp);
    if (table_content.empty()) {
        lou_free(); // Cleanup before returning
        return 0;
    }
    
    // Step 3: Compile table with lou_compileString (primary target)
    int compile_result = lou_compileString(table_name.c_str(), table_content.c_str());
    (void)compile_result; // Result ignored for fuzzing
    
    // Step 4: Validate compilation with lou_checkTable (validation)
    int check_result = lou_checkTable(table_name.c_str());
    (void)check_result;
    
    // Step 5: Retrieve compiled table with lou_getTable (retrieval)
    const void* table_ptr = lou_getTable(table_name.c_str());
    (void)table_ptr; // Table pointer ignored for fuzzing
    
    // Step 6: Test table listing and cleanup functions
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
        // Test lou_findTables to find tables matching query
        std::string query = fdp.ConsumeRandomLengthString(32);
        if (!query.empty()) {
            char** found_tables = lou_findTables(query.c_str());
            if (found_tables) {
                // Clean up with lou_freeTableFiles (cleanup target)
                lou_freeTableFiles(found_tables);
            }
        }
    }
    
    // Step 7: Test lou_listTables and lou_freeTableFiles
    if (fdp.ConsumeBool()) {
        char** all_tables = lou_listTables();
        if (all_tables) {
            lou_freeTableFiles(all_tables);
        }
    }
    
    // Step 8: Test emphasis class retrieval functions
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 2) {
        const char** emph_classes = lou_getEmphClasses(table_name.c_str());
        if (emph_classes) {
            lou_freeEmphClasses(emph_classes);
        }
    }
    
    // Step 9: Test lou_getTypeformForEmphClass
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 2) {
        std::string emph_class_name = fdp.ConsumeRandomLengthString(16);
        if (!emph_class_name.empty()) {
            formtype typeform = lou_getTypeformForEmphClass(table_name.c_str(), emph_class_name.c_str());
            (void)typeform;
        }
    }
    
    // Step 10: Cleanup - lou_free will be called by atexit
    // but we also explicitly test lou_freeTableFiles if tables were listed
    
    // Return 0 to indicate successful execution
    return 0;
}

} // namespace harness_011
