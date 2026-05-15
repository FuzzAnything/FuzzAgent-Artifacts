#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include "liblouis.h"

namespace harness_009 {
// liblouis translation opcode path exploration harness
// Target: Exercise blocked opcode paths in for_selectRule (222 blocked branches)
// Primary API: lou_translateString
// Secondary APIs: lou_getTable (table loading), lou_free (cleanup)
// Focus opcodes: CTO_RepWord, CTO_RepEndWord, CTO_ExactDots, CTO_JoinNum, CTC_EmpMatch
// Based on coverage analysis: API coverage 100% but branch coverage low (56.42%)
// Major blocker: for_selectRule function with 222 blocked branches
// Invocation sequence: 1. Call lou_getTable to load translation tables containing specific opcodes
//                      2. Call lou_translateString with inputs designed to trigger blocked paths
//                      3. Call lou_free to clean up
// Semantic differentiation: Targets specific translation opcode paths not covered in previous harnesses


// Include liblouis public header
extern "C" {
}

// Log callback to suppress logs during fuzzing
static void avoid_log_callback(logLevels level, const char *msg) {
    (void)level;
    (void)msg;
}

// Table selection helper - tables likely to contain target opcodes
static const char* select_table_for_opcode_testing(FuzzedDataProvider& fdp) {
    // Tables with emphasis markers and various opcodes
    const char* emphasis_tables[] = {
        "da-dk-g16.ctb",      // Danish tables with emphasis markers
        "da-dk-g18.ctb",      // Danish tables with emphasis
        "da-dk-g26.ctb",      // Danish tables with emphasis
        "en-ueb-g2.ctb",      // UEB grade 2 with contractions
        "en-us-comp8.ctb",    // US computer braille
        "en-us-comp6.ctb",    // US computer braille 6-dot
        "zhcn-g2.ctb",        // Chinese grade 2
        "sv-g1.ctb",          // Swedish grade 1
        "de-g0.utb",          // German grade 0
    };
    const size_t num_tables = sizeof(emphasis_tables) / sizeof(emphasis_tables[0]);
    
    size_t table_idx = fdp.ConsumeIntegralInRange<size_t>(0, num_tables - 1);
    return emphasis_tables[table_idx];
}

// Generate input strings designed to trigger specific opcode paths
static std::string generate_opcode_targeting_input(FuzzedDataProvider& fdp) {
    std::string input;
    uint8_t opcode_target = fdp.ConsumeIntegral<uint8_t>() % 6;
    
    switch (opcode_target) {
        case 0: { // CTO_RepWord, CTO_RepEndWord - repeated words
            size_t num_repeats = fdp.ConsumeIntegralInRange<size_t>(2, 5);
            std::string word = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 10));
            
            for (size_t i = 0; i < num_repeats; i++) {
                if (i > 0) input += " ";
                input += word;
            }
            break;
        }
        
        case 1: { // CTO_ExactDots - specific character sequences
            // Generate sequences that might trigger exact dot pattern matching
            size_t seq_len = fdp.ConsumeIntegralInRange<size_t>(3, 10);
            for (size_t i = 0; i < seq_len; i++) {
                char c = fdp.ConsumeIntegralInRange<char>('a', 'z');
                input += c;
                
                // Occasionally add numbers or special chars that might need exact dot patterns
                if (fdp.ConsumeBool() && i < seq_len - 1) {
                    input += fdp.ConsumeIntegralInRange<char>('0', '9');
                }
            }
            break;
        }
        
        case 2: { // CTO_JoinNum - number sequences
            size_t num_count = fdp.ConsumeIntegralInRange<size_t>(2, 6);
            for (size_t i = 0; i < num_count; i++) {
                if (i > 0) {
                    // Add separators that might trigger join operations
                    if (fdp.ConsumeBool()) input += " ";
                    if (fdp.ConsumeBool()) input += ",";
                }
                input += fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 4));
            }
            break;
        }
        
        case 3: { // Emphasis markers - text with potential emphasis
            // Generate text that might trigger emphasis matching
            size_t text_len = fdp.ConsumeIntegralInRange<size_t>(5, 50);
            input = fdp.ConsumeRandomLengthString(text_len);
            
            // Occasionally add punctuation that might affect emphasis
            if (fdp.ConsumeBool()) {
                size_t insert_pos = fdp.ConsumeIntegralInRange<size_t>(0, input.length());
                if (insert_pos < input.length()) {
                    input.insert(insert_pos, 1, fdp.ConsumeIntegralInRange<char>('!', '?'));
                }
            }
            break;
        }
        
        case 4: { // Pass rule search operations - complex patterns
            // Generate patterns that might trigger pass rule searches
            size_t pattern_len = fdp.ConsumeIntegralInRange<size_t>(5, 30);
            input = fdp.ConsumeRandomLengthString(pattern_len);
            
            // Add some structure that might trigger search operations
            if (fdp.ConsumeBool() && input.length() > 3) {
                size_t pos = fdp.ConsumeIntegralInRange<size_t>(1, input.length() - 2);
                input.insert(pos, "ing");
            }
            break;
        }
        
        case 5: { // Mixed opcode testing
            // Mix different patterns to test multiple opcodes
            size_t sections = fdp.ConsumeIntegralInRange<size_t>(2, 4);
            for (size_t s = 0; s < sections; s++) {
                if (s > 0) input += " ";
                
                uint8_t section_type = fdp.ConsumeIntegral<uint8_t>() % 4;
                switch (section_type) {
                    case 0: // Repeated word section
                        input += fdp.ConsumeRandomLengthString(3);
                        input += " ";
                        input += fdp.ConsumeRandomLengthString(3);
                        break;
                    case 1: { // Number section
                        size_t num_len = fdp.ConsumeIntegralInRange<size_t>(1, 4);
                        for (size_t j = 0; j < num_len; j++) {
                            input += static_cast<char>('0' + fdp.ConsumeIntegralInRange<uint8_t>(0, 9));
                        }
                        break;
                    }
                    case 2: // Mixed alphanumeric
                        input += fdp.ConsumeRandomLengthString(4);
                        break;
                    case 3: // With punctuation
                        input += fdp.ConsumeRandomLengthString(3);
                        input += fdp.ConsumeIntegralInRange<char>('!', '?');
                        break;
                }
            }
            break;
        }
    }
    
    return input;
}

// Simple ASCII to widechar conversion
static size_t ascii_to_widechar(const char* ascii, widechar* wide, size_t max_len) {
    size_t i;
    for (i = 0; i < max_len && ascii[i] != '\0'; i++) {
        wide[i] = ascii[i];
    }
    return i;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough for table selection and input generation
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
    
    // Step 1: Call lou_getTable to load translation tables containing specific opcodes
    const char* table = select_table_for_opcode_testing(fdp);
    
    // Load the table (lou_translateString will call lou_getTable internally)
    const void* loaded_table = lou_getTable(table);
    if (loaded_table == NULL) {
        // Table loading failed - continue anyway as some tables might not be available
        // lou_translateString will handle this
    }
    
    // Step 2: Call lou_translateString with inputs designed to trigger blocked paths
    std::string text = generate_opcode_targeting_input(fdp);
    
    if (text.empty()) {
        // If no text generated, try a simple fallback
        text = fdp.ConsumeRandomLengthString(fdp.ConsumeIntegralInRange<size_t>(1, 50));
    }
    
    if (text.empty()) {
        return 0;
    }
    
    // Convert to widechar
    std::vector<widechar> inbuf(text.size() + 1);
    size_t inlen = ascii_to_widechar(text.c_str(), inbuf.data(), text.size());
    int inlen_int = static_cast<int>(inlen);
    
    // Allocate output buffer - braille can expand, so allocate more space
    int outlen_int = inlen_int * 16;
    std::vector<widechar> outbuf(outlen_int + 1);
    
    // Select mode based on fuzzed input
    int mode = 0; // plain_text
    uint8_t mode_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
    switch (mode_choice) {
        case 0:
            mode = plain_text;
            break;
        case 1:
            mode = emph_1; // italic
            break;
        case 2:
            mode = emph_2; // underline
            break;
        case 3:
            mode = emph_3; // bold
            break;
        case 4:
            mode = computer_braille;
            break;
        case 5:
            mode = no_translate;
            break;
        case 6:
            mode = no_contract;
            break;
        case 7:
            // Combined mode
            mode = emph_1 | emph_2 | computer_braille;
            break;
    }
    
    // Optional: typeform array for emphasis testing
    std::vector<formtype> typeform;
    bool use_typeform = fdp.ConsumeBool();
    if (use_typeform && inlen_int > 0) {
        typeform.resize(inlen_int);
        for (int i = 0; i < inlen_int; i++) {
            typeform[i] = fdp.ConsumeIntegral<formtype>();
        }
    }
    
    // Optional: spacing array
    std::vector<char> spacing;
    bool use_spacing = fdp.ConsumeBool();
    if (use_spacing && inlen_int > 0) {
        spacing.resize(inlen_int);
        for (int i = 0; i < inlen_int; i++) {
            spacing[i] = fdp.ConsumeIntegral<char>();
        }
    }
    
    // Call lou_translateString with the generated input
    int result = lou_translateString(
        table,
        inbuf.data(),
        &inlen_int,
        outbuf.data(),
        &outlen_int,
        use_typeform ? typeform.data() : NULL,
        use_spacing ? spacing.data() : NULL,
        mode
    );
    
    // Result is ignored - we're interested in code path coverage, not successful translation
    
    // Step 3: Call lou_free to clean up
    // Note: lou_free frees all memory allocated by liblouis
    // We might want to call it periodically, but for now we'll rely on cleanup at end
    
    return 0;
}

} // namespace harness_009
