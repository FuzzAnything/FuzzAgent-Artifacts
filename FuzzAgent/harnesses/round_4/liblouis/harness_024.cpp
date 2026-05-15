#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "liblouis.h"

namespace harness_024 {
// liblouis compileRule deep error path fuzzing harness (Phase 2)
// Target: compileRule function with 580 blocked branches - focused error path exploration
// Focus: Deep internal error paths, complex validation failures, macro expansion errors,
//        pattern compilation failures, and attribute/class validation logic
// Based on coverage analysis guidance targeting remaining 580 blocked branches in compileRule
// Semantic differentiation from harness_023: Focus on error paths rather than opcode patterns


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Helper to generate malformed table content targeting specific compileError calls
static std::string generate_error_path_content(FuzzedDataProvider& fdp, int error_path_id) {
    std::string content;
    
    switch (error_path_id % 12) {
        case 0: // Token too long errors
            content = "table ";
            // Generate very long table name (> MAXSTRING)
            for (int i = 0; i < 300; i++) {
                content += "x";
            }
            content += "\n";
            break;
            
        case 1: // Macro definition errors
            content = "table test\n";
            content += "macro ";
            // Generate macro with invalid name (containing numbers)
            content += "macro123 ";
            content += "eom\n";
            content += "  letter a 1\n";
            content += "eom\n";
            break;
            
        case 2: // Macro without eom termination
            content = "table test\n";
            content += "macro incomplete\n";
            content += "  letter a 1\n";
            content += "  letter b 12\n";
            // Missing eom terminator
            break;
            
        case 3: // Character specification errors
            content = "table test\n";
            content += "space ";  // Missing character specification
            content += "\n";
            content += "digit invalid-hex\n";  // Invalid hex specification
            break;
            
        case 4: // Dot pattern errors
            content = "table test\n";
            content += "letter a ";  // Missing dot pattern
            content += "\n";
            content += "letter b 9\n";  // Invalid dot (9 doesn't exist, only 1-8)
            break;
            
        case 5: // Rule length errors
            content = "table test\n";
            content += "display ";  // Missing both character and cell
            content += "\n";
            content += "display ab 123\n";  // Too many characters
            break;
            
        case 6: // Attribute and class errors  
            content = "table test\n";
            content += "class ";  // Missing class name
            content += "\n";
            content += "attribute invalid-attr-name\n";  // Invalid attribute name
            break;
            
        case 7: // Multipass operand errors
            content = "table test\n";
            content += "pass2\n";
            content += "multipass ";
            // Generate invalid multipass operand
            content += "[invalid!@#$%]\n";
            break;
            
        case 8: // Match pattern errors
            content = "table test\n";
            content += "match ";
            // Invalid match pattern
            content += "[!@#$%] ";
            content += "ab 12\n";
            break;
            
        case 9: // Emphasis and mode errors
            content = "table test\n";
            content += "emphclass ";  // Missing class name
            content += "\n";
            content += "emphletter ";  // Missing character
            content += "\n";
            break;
            
        case 10: // Control directive conflicts
            content = "table test\n";
            content += "noback\n";
            content += "nofor\n";  // This should cause conflict
            content += "noback\n";  // Duplicate
            break;
            
        case 11: // Complex nested errors with macros and includes
            content = "table test\n";
            content += "include ";  // Missing file name
            content += "\n";
            content += "macro m1 eom\n";
            content += "  include missing-file.ctb\n";  // Include inside macro
            content += "eom\n";
            content += "m1\n";
            break;
            
        default:
            content = "table test\n";
            content += "finish\n";
    }
    
    // Add some random content to explore more paths
    if (fdp.ConsumeBool()) {
        content += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 256));
    }
    
    return content;
}

// Generate table with complex attribute chains to test attribute validation
static std::string generate_attribute_chain_content(FuzzedDataProvider& fdp) {
    std::string content = "table attribute-test\n";
    
    // Create circular attribute references
    content += "attribute attr1\n";
    content += "  after attr2\n";
    content += "attribute attr2\n";
    content += "  after attr1\n";  // Circular reference
    
    // Invalid attribute combinations
    content += "attribute attr3\n";
    content += "  after invalid-attr\n";  // Non-existent attribute
    
    // Self-referential attribute
    content += "attribute attr4\n";
    content += "  after attr4\n";  // Self-reference
    
    // Complex chain
    content += "attribute attr5\n";
    content += "  before attr1\n";
    content += "  after attr2\n";
    content += "  before attr3\n";
    
    content += "finish\n";
    return content;
}

// Generate table with pattern compilation failures
static std::string generate_pattern_failure_content(FuzzedDataProvider& fdp) {
    std::string content = "table pattern-test\n";
    
    // Invalid pattern syntax
    content += "match [invalid(!] ab 12\n";
    
    // Pattern with undefined characters
    content += "match [xyz] ab 12\n";  // x,y,z not defined
    
    // Complex invalid pattern
    content += "match [a-z(!@#] ab 12\n";
    
    // Backmatch with issues
    content += "backmatch [invalid] ba 21\n";
    
    // Pattern with escape sequence errors
    content += "match [\\xgg] ab 12\n";  // Invalid hex
    
    content += "finish\n";
    return content;
}

// Generate table with macro expansion failures
static std::string generate_macro_expansion_failure_content(FuzzedDataProvider& fdp) {
    std::string content = "table macro-test\n";
    
    // Macro with wrong argument count
    content += "macro m1 arg1 eom\n";
    content += "  letter arg1 1\n";
    content += "eom\n";
    content += "m1\n";  // Missing argument
    
    // Nested macro definition (should fail)
    content += "macro outer eom\n";
    content += "  macro inner eom\n";
    content += "    letter a 1\n";
    content += "  eom\n";
    content += "eom\n";
    
    // Macro call with too many arguments
    content += "macro m2 eom\n";
    content += "  letter a 1\n";
    content += "eom\n";
    content += "m2 extra-arg\n";  // Extra argument
    
    // Macro with syntax errors inside
    content += "macro m3 eom\n";
    content += "  letter   \n";  // Incomplete rule
    content += "  invalid-opcode\n";
    content += "eom\n";
    
    content += "finish\n";
    return content;
}

// Generate table with character class validation failures
static std::string generate_class_validation_failure_content(FuzzedDataProvider& fdp) {
    std::string content = "table class-test\n";
    
    // Duplicate character definitions
    content += "letter a 1\n";
    content += "letter a 12\n";  // Duplicate
    
    // Character in multiple classes
    content += "letter b 123\n";
    content += "digit b\n";  // b already in letter class
    
    // Invalid character ranges
    content += "punctuation z-a\n";  // Wrong order
    
    // Hexadecimal errors
    content += "space 0xXYZ\n";  // Invalid hex
    
    // Character class overflow attempts
    for (int i = 0; i < 50; i++) {
        content += "letter " + std::to_string(i + 1000) + " 1\n";
    }
    
    content += "finish\n";
    return content;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        initialized = true;
    }
    
    // Select error path testing strategy
    uint8_t strategy = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    std::string table_name;
    std::string table_content;
    
    switch (strategy) {
        case 0: // Specific compileError paths
            table_name = "error-paths.ctb";
            table_content = generate_error_path_content(fdp, fdp.ConsumeIntegral<int>());
            break;
            
        case 1: // Attribute chain validation failures
            table_name = "attribute-chain.ctb";
            table_content = generate_attribute_chain_content(fdp);
            break;
            
        case 2: // Pattern compilation failures
            table_name = "pattern-failures.ctb";
            table_content = generate_pattern_failure_content(fdp);
            break;
            
        case 3: // Macro expansion failures
            table_name = "macro-expansion.ctb";
            table_content = generate_macro_expansion_failure_content(fdp);
            break;
            
        case 4: // Character class validation failures
            table_name = "class-validation.ctb";
            table_content = generate_class_validation_failure_content(fdp);
            break;
            
        case 5: // Combined complex failures
            table_name = "combined-failures.ctb";
            table_content = "table combined\n";
            
            // Mix multiple error types
            if (fdp.ConsumeBool()) {
                table_content += "macro bad1 eom\n";
                table_content += "  letter \n";  // Incomplete
                table_content += "eom\n";
            }
            
            if (fdp.ConsumeBool()) {
                table_content += "attribute bad-attr\n";
                table_content += "  after non-existent\n";
            }
            
            if (fdp.ConsumeBool()) {
                table_content += "match [invalid] xy 13\n";
            }
            
            if (fdp.ConsumeBool()) {
                table_content += "letter x 9\n";  // Invalid dot
            }
            
            table_content += "finish\n";
            break;
    }
    
    // Add random mutations to explore nearby code paths
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 10) {
        size_t insert_pos = fdp.ConsumeIntegralInRange<size_t>(0, table_content.length());
        std::string mutation = fdp.ConsumeRandomLengthString(
            fdp.ConsumeIntegralInRange<size_t>(1, 64));
        table_content.insert(insert_pos, mutation);
    }
    
    // Attempt compilation - focus is on triggering error paths
    int compile_result = lou_compileString(table_name.c_str(), table_content.c_str());
    (void)compile_result; // Result is not important, we're testing error paths
    
    // Test incremental compilation on same table
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 20) {
        std::string additional_content;
        
        // Try to add more error-prone content
        uint8_t add_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (add_type) {
            case 0:
                additional_content = "noback\nnofor\n";  // Conflict
                break;
            case 1:
                additional_content = "letter   \n";  // Incomplete
                break;
            case 2:
                additional_content = "include \n";  // Missing file
                break;
            case 3:
                additional_content = fdp.ConsumeRandomLengthString(32);
                break;
        }
        
        int add_result = lou_compileString(table_name.c_str(), additional_content.c_str());
        (void)add_result;
    }
    
    // Test with different table names to trigger different initialization paths
    if (fdp.remaining_bytes() > 30) {
        std::string alt_table_name = "alt-" + std::to_string(fdp.ConsumeIntegral<uint32_t>()) + ".ctb";
        std::string alt_content = "table alt-test\n";
        
        // Generate content focusing on less common opcodes
        if (fdp.ConsumeBool()) {
            alt_content += "swapcc a b\n";
        }
        if (fdp.ConsumeBool()) {
            alt_content += "swapcd c d\n";
        }
        if (fdp.ConsumeBool()) {
            alt_content += "swapdd e f\n";
        }
        if (fdp.ConsumeBool()) {
            alt_content += "compdots g 123\n";
        }
        if (fdp.ConsumeBool()) {
            alt_content += "comp6\n";
        }
        
        alt_content += "finish\n";
        
        int alt_result = lou_compileString(alt_table_name.c_str(), alt_content.c_str());
        (void)alt_result;
    }
    
    // Clean up
    lou_free();
    
    return 0;
}

} // namespace harness_024
