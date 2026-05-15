#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include "liblouis.h"

namespace harness_014 {
// liblouis emphasis and sequence opcode compilation harness
// Target: compileRule function in compileTranslationTable.c (568 blocked branches)
// Focus on emphasis, numeric, and sequence opcode paths that are less exercised
// Based on coverage analysis: table compilation subsystem needs more diverse opcode testing
// Primary API: lou_compileString (directly exercises compileRule)
// Secondary APIs: lou_checkTable (validation), lou_freeTableFile (cleanup)
// Target opcodes: CTO_EmphClass, CTO_EmphLetter, CTO_NumberSign, CTO_SeqDelimiter, 
//                 CTO_SeqBeforeChars, CTO_SeqAfterChars, CTO_CapsModeChars, CTO_NumericModeChars
// Semantic differentiation from harness_013: Focuses on emphasis, numeric, and sequence opcodes
//                                            rather than macro and grouping opcodes


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Helper to generate table content with emphasis and sequence opcodes
static std::string generate_emphasis_sequence_table(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Start with basic table metadata
    content += "table emphasis-sequence-test\n";
    content += "locale en_US\n";
    content += "author fuzzer-generated\n";
    
    // Add emphasis class definitions
    if (fdp.ConsumeBool()) {
        content += "emphclass ";
        size_t class_name_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        for (size_t i = 0; i < class_name_len; i++) {
            char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
            content += c;
        }
        content += "\n";
    }
    
    // Add emphasis letter definitions
    if (fdp.ConsumeBool()) {
        size_t num_emph_letters = fdp.ConsumeIntegralInRange<size_t>(1, 5);
        for (size_t i = 0; i < num_emph_letters; i++) {
            content += "emphletter ";
            char letter = fdp.ConsumeIntegralInRange<char>('a', 'z');
            content += letter;
            content += " ";
            content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
            content += "\n";
        }
    }
    
    // Add number sign definitions
    if (fdp.ConsumeBool()) {
        content += "numbersign ";
        char sign_char = fdp.ConsumeIntegralInRange<char>('0', '9');
        content += sign_char;
        content += " ";
        content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
        content += "\n";
    }
    
    // Add sequence delimiter
    if (fdp.ConsumeBool()) {
        content += "seqdelimiter ";
        char delim = fdp.ConsumeIntegralInRange<char>('!', '~');
        content += delim;
        content += "\n";
    }
    
    // Add sequence before/after characters
    if (fdp.ConsumeBool()) {
        content += "seqbeforechars ";
        size_t before_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
        for (size_t i = 0; i < before_len; i++) {
            content += fdp.ConsumeIntegralInRange<char>('a', 'z');
        }
        content += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        content += "seqafterchars ";
        size_t after_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
        for (size_t i = 0; i < after_len; i++) {
            content += fdp.ConsumeIntegralInRange<char>('a', 'z');
        }
        content += "\n";
    }
    
    // Add caps mode characters
    if (fdp.ConsumeBool()) {
        content += "capsmodechars ";
        size_t caps_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
        for (size_t i = 0; i < caps_len; i++) {
            content += fdp.ConsumeIntegralInRange<char>('A', 'Z');
        }
        content += "\n";
    }
    
    // Add numeric mode characters
    if (fdp.ConsumeBool()) {
        content += "numericmodechars ";
        size_t num_len = fdp.ConsumeIntegralInRange<size_t>(1, 8);
        for (size_t i = 0; i < num_len; i++) {
            content += fdp.ConsumeIntegralInRange<char>('0', '9');
        }
        content += "\n";
    }
    
    // Add character definitions with various attributes
    size_t num_char_defs = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    for (size_t i = 0; i < num_char_defs; i++) {
        uint8_t char_type = fdp.ConsumeIntegral<uint8_t>() % 8;
        
        switch (char_type) {
            case 0:
                content += "space ";
                content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
                content += "\n";
                break;
                
            case 1:
                content += "digit ";
                content += fdp.ConsumeIntegralInRange<char>('0', '9');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
                content += "\n";
                break;
                
            case 2:
                content += "punctuation ";
                content += fdp.ConsumeIntegralInRange<char>('!', '/');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
                content += "\n";
                break;
                
            case 3:
                content += "letter ";
                content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
                content += "\n";
                break;
                
            case 4:
                content += "uppercase ";
                content += fdp.ConsumeIntegralInRange<char>('A', 'Z');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
                content += "\n";
                break;
                
            case 5:
                content += "lowercase ";
                content += fdp.ConsumeIntegralInRange<char>('a', 'z');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
                content += "\n";
                break;
                
            case 6:
                content += "sign ";
                content += fdp.ConsumeIntegralInRange<char>('$', '&');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
                content += "\n";
                break;
                
            case 7:
                content += "math ";
                content += fdp.ConsumeIntegralInRange<char>('+', '/');
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 8));
                content += "\n";
                break;
        }
    }
    
    return content;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for table generation parameters
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        initialized = true;
    }
    
    // Generate table name from fuzzer input
    std::string table_name = "fuzz-emph-seq-";
    size_t name_suffix_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
    for (size_t i = 0; i < name_suffix_len; i++) {
        char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
        table_name += c;
    }
    table_name += ".ctb";
    
    // Generate table content with emphasis and sequence opcodes
    std::string table_content = generate_emphasis_sequence_table(fdp);
    
    // Call lou_compileString to compile the generated table
    // This directly exercises compileRule function with diverse opcodes
    const char* table_ptr = table_name.c_str();
    const char* content_ptr = table_content.c_str();
    
    int compile_result = lou_compileString(table_ptr, content_ptr);
    
    // If compilation succeeded, we can optionally validate the table
    if (compile_result) {
        // Try to check the table for validation
        int check_result = lou_checkTable(table_ptr);
        
        // Free the table file if it was successfully compiled
        lou_freeTableFile(const_cast<char*>(table_ptr));
    }
    
    // Note: lou_compileString returns 0 on success, 1 on failure
    // This is opposite of typical C conventions, but we handle both cases
    
    return 0;
}

} // namespace harness_014
