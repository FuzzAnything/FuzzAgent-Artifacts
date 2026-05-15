#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include "liblouis.h"

namespace harness_008 {
// liblouis deep table compilation exploration harness
// Target APIs: lou_compileString (Primary), lou_freeTableFile (Secondary)
// Required helpers: lou_setDataPath, lou_getDataPath (optional)
// Based on Phase 2 analysis: API coverage 100% but branch coverage only 56.09%
// Major blocker: compileRule function (550 blocked branches)
// Focus: Generating complex table definitions with diverse opcodes, error injection,
//        macro feature testing, include file directives, character class definitions
// Invocation sequence: 1. Generate complex table definitions with diverse opcodes
//                      2. Call lou_compileString with generated table strings
//                      3. Handle compilation errors (expect some inputs to fail)
//                      4. Call lou_freeTableFile to clean up successfully compiled tables
// Semantic differentiation from harness_003 and harness_005: Focuses on opcode diversity
// and complex table patterns to directly exercise compileRule function


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Helper to generate diverse opcode-based table content
static std::string generate_table_with_diverse_opcodes(FuzzedDataProvider& fdp) {
    std::string content;
    
    // Start with table name and locale
    if (fdp.ConsumeBool()) {
        content += "table ";
        content += fdp.ConsumeRandomLengthString(32);
        content += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        content += "locale ";
        content += fdp.ConsumeRandomLengthString(16);
        content += "\n";
    }
    
    if (fdp.ConsumeBool()) {
        content += "author ";
        content += fdp.ConsumeRandomLengthString(32);
        content += "\n";
    }
    
    // Generate diverse opcode sequences based on fuzzer input
    size_t num_opcodes = fdp.ConsumeIntegralInRange<size_t>(1, 20);
    
    for (size_t i = 0; i < num_opcodes; i++) {
        uint8_t opcode_type = fdp.ConsumeIntegral<uint8_t>() % 20;
        
        switch (opcode_type) {
            case 0: { // CTO_IncludeFile
                if (fdp.ConsumeBool()) {
                    content += "include ";
                    content += fdp.ConsumeRandomLengthString(64);
                    content += "\n";
                }
                break;
            }
            case 1: { // CTO_Macro (if enabled)
                if (fdp.ConsumeBool()) {
                    content += "macro ";
                    content += fdp.ConsumeRandomLengthString(16);
                    content += " eom\n";
                    // Add some macro content
                    size_t macro_lines = fdp.ConsumeIntegralInRange<size_t>(1, 5);
                    for (size_t j = 0; j < macro_lines; j++) {
                        content += fdp.ConsumeRandomLengthString(32);
                        content += "\n";
                    }
                    content += "eom\n";
                }
                break;
            }
            case 2: { // CTO_NoBack and CTO_NoFor
                if (fdp.ConsumeBool()) {
                    content += "noback\n";
                }
                if (fdp.ConsumeBool()) {
                    content += "nofor\n";
                }
                // Add a character definition after these
                content += "space ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 3: { // Character class definitions
                const char* char_classes[] = {
                    "space", "digit", "punctuation", "math", "sign", 
                    "letter", "uppercase", "lowercase", "litdigit", "grouping"
                };
                const size_t num_classes = sizeof(char_classes) / sizeof(char_classes[0]);
                uint8_t class_idx = fdp.ConsumeIntegral<uint8_t>() % num_classes;
                content += char_classes[class_idx];
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 4: { // Display rules
                content += "display ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 5: { // Replace rules
                content += "replace ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 6: { // Context rules
                content += "context ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 7: { // Correct rules
                content += "correct ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 8: { // Contraction rules
                content += "contraction ";
                content += fdp.ConsumeRandomLengthString(8);
                content += "\n";
                break;
            }
            case 9: { // Word rules (various types)
                const char* word_types[] = {
                    "word", "sufword", "prefword", "midword", "midendword",
                    "begword", "endword", "wholeword", "partword"
                };
                const size_t num_types = sizeof(word_types) / sizeof(word_types[0]);
                uint8_t type_idx = fdp.ConsumeIntegral<uint8_t>() % num_types;
                content += word_types[type_idx];
                content += " ";
                content += fdp.ConsumeRandomLengthString(12);
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 10: { // Always rules
                content += "always ";
                content += fdp.ConsumeRandomLengthString(8);
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 11: { // Class definitions
                content += "class ";
                content += fdp.ConsumeRandomLengthString(8);
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 12: { // After/Before rules
                if (fdp.ConsumeBool()) {
                    content += "after ";
                } else {
                    content += "before ";
                }
                content += fdp.ConsumeRandomLengthString(8);
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 13: { // Emphasis class definitions
                content += "emphclass ";
                content += fdp.ConsumeRandomLengthString(8);
                content += "\n";
                break;
            }
            case 14: { // Letter sign and number sign rules
                if (fdp.ConsumeBool()) {
                    content += "letsign ";
                } else {
                    content += "numsign ";
                }
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 15: { // No letsign/number sign rules
                if (fdp.ConsumeBool()) {
                    content += "noletsign ";
                } else {
                    content += "nonumsign ";
                }
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 16: { // Capitalization rules
                const char* caps_rules[] = {
                    "capsletter", "begcaps", "endcaps", "begcapsphrase", "endcapsphrase"
                };
                const size_t num_caps = sizeof(caps_rules) / sizeof(caps_rules[0]);
                uint8_t caps_idx = fdp.ConsumeIntegral<uint8_t>() % num_caps;
                content += caps_rules[caps_idx];
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 17: { // Mode rules
                content += "modeletter ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 18: { // Large sign rules
                content += "largesign ";
                content += fdp.ConsumeRandomLengthString(4);
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
            case 19: { // Multipass indicator
                content += "multind ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += " ";
                content += fdp.ConsumeRandomLengthString(8);
                content += "\n";
                break;
            }
            default: {
                // Generate random opcode-like text (might trigger error paths)
                content += fdp.ConsumeRandomLengthString(12);
                content += " ";
                content += std::to_string(fdp.ConsumeIntegralInRange<uint8_t>(1, 255));
                content += "\n";
                break;
            }
        }
        
        // Occasionally add comments
        if (fdp.ConsumeBool()) {
            content += "# ";
            content += fdp.ConsumeRandomLengthString(40);
            content += "\n";
        }
        
        // Occasionally add blank lines
        if (fdp.ConsumeBool()) {
            content += "\n";
        }
    }
    
    return content;
}

// Helper to generate table names
static std::string generate_table_name(FuzzedDataProvider& fdp) {
    std::string table_name;
    
    // Choose language prefix
    const char* prefixes[] = {"en", "fr", "de", "es", "ar", "zh", "ja", "ko", "test", "invalid"};
    const size_t num_prefixes = sizeof(prefixes) / sizeof(prefixes[0]);
    uint8_t prefix_idx = fdp.ConsumeIntegral<uint8_t>() % num_prefixes;
    table_name += prefixes[prefix_idx];
    table_name += "-";
    
    // Choose region
    if (fdp.ConsumeBool()) {
        const char* regions[] = {"us", "gb", "ca", "au", "fr", "de", "es", "cn", "jp"};
        const size_t num_regions = sizeof(regions) / sizeof(regions[0]);
        uint8_t region_idx = fdp.ConsumeIntegral<uint8_t>() % num_regions;
        table_name += regions[region_idx];
        table_name += "-";
    }
    
    // Choose grade/type
    const char* types[] = {"g1", "g2", "g3", "comp", "comp8", "compbrl", "ascii", "unicode"};
    const size_t num_types = sizeof(types) / sizeof(types[0]);
    uint8_t type_idx = fdp.ConsumeIntegral<uint8_t>() % num_types;
    table_name += types[type_idx];
    
    // Choose extension
    if (fdp.ConsumeBool()) {
        table_name += ".ctb";
    } else {
        table_name += ".utb";
    }
    
    return table_name;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for various parameters and complex table data
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize liblouis once
    static bool initialized = false;
    if (!initialized) {
        lou_registerLogCallback(avoid_log_callback);
        initialized = true;
    }
    
    // 1. Call lou_setDataPath to configure table search path
    // Use actual tables directory for more realistic testing
    char* path_result = lou_setDataPath("/root/src/liblouis/tables");
    // Return value can be NULL if path is invalid, which is OK for fuzzing
    
    // 2. Generate multiple table strings with diverse opcodes
    size_t num_table_strings = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    std::vector<std::string> table_strings;
    std::vector<std::string> table_names;
    
    for (size_t i = 0; i < num_table_strings; i++) {
        if (fdp.remaining_bytes() < 100) break;
        
        // Generate table name
        std::string table_name = generate_table_name(fdp);
        table_names.push_back(table_name);
        
        // Generate table content with diverse opcodes
        std::string table_content = generate_table_with_diverse_opcodes(fdp);
        table_strings.push_back(table_content);
    }
    
    if (table_strings.empty()) {
        return 0;
    }
    
    // 3. Test compilation of each table string
    for (size_t i = 0; i < table_strings.size(); i++) {
        if (fdp.remaining_bytes() < 50) break;
        
        // Choose whether to use table name or NULL
        const char* table_to_use = NULL;
        if (fdp.ConsumeBool() && !table_names[i].empty()) {
            // Create a modifiable copy for lou_freeTableFile
            char* table_copy = strdup(table_names[i].c_str());
            if (table_copy) {
                table_to_use = table_copy;
            }
        }
        
        // Call lou_compileString with the generated table string
        int compile_result = lou_compileString(table_to_use ? table_to_use : "test-table", 
                                              table_strings[i].c_str());
        
        // Handle compilation result
        if (compile_result == 1) {
            // Compilation succeeded - table was added to chain
            // We could potentially do more operations here, but for fuzzing
            // we mainly want to exercise the compilation paths
        } else {
            // Compilation failed - this is expected for many fuzzer inputs
            // and helps test error handling paths in compileRule
        }
        
        // Clean up if we allocated a table name
        if (table_to_use) {
            lou_freeTableFile(const_cast<char*>(table_to_use));
        }
    }
    
    // 4. Optionally test lou_getDataPath
    if (fdp.ConsumeBool()) {
        char* data_path = lou_getDataPath();
        // data_path may be NULL, which is OK
        (void)data_path;
    }
    
    // Note: We don't call lou_free() here because it would remove all
    // compiled tables, which might affect other harnesses running
    // in the same process. The tables will be cleaned up when the
    // fuzzing process ends.
    
    return 0;
}

} // namespace harness_008
