#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>
#include "magic.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace harness_022 {
/*
 * Fuzzing harness for libmagic library targeting magic_check API with diverse
 * magic file patterns to address coverage bottlenecks in parse and magiccheck functions.
 * 
 * BACKGROUND: After 22 harnesses, parse function (apprentice.c) has 178 blocked 
 * branches (61% blockage) and magiccheck function (softmagic.c) has 208 blocked 
 * branches (87% blockage). Analysis shows parse is a strategic bottleneck that 
 * limits generation of diverse magic patterns needed to exercise magiccheck.
 * 
 * TARGET: magic_check API with diverse magic file content to directly cover 
 * parse's blocked branches and indirectly target magiccheck's blocked branches.
 * 
 * SPECIFIC PATTERN VARIATIONS TO INCLUDE (from coverage guidance):
 * 1. Continuation patterns (multiple `>` levels)
 * 2. Indirect modifiers (`&`, `(` syntax)
 * 3. All magic type specifiers (FILE_BYTE through FILE_GUID)
 * 4. String modifiers with various flags
 * 5. Edge case patterns that trigger default/error paths
 * 6. Invalid relation operators for float/double types
 * 7. FILE_BESTRING16 vs FILE_LESTRING16 patterns
 * 8. Regex patterns with error conditions
 * 9. Search patterns with range limits
 * 10. Type combinations that hit the default switch case
 * 11. Continuation level transitions (diff > 1)
 * 12. Indirect offset at level 0 (should trigger warnings)
 * 
 * APIs to target:
 * - magic_open (Required Helper: Initialization)
 * - magic_check (Target API: Magic file validation)
 * - magic_close (Required Helper: Cleanup)
 * 
 * INVOCATION SEQUENCE:
 * 1. magic_open() to create magic_set context
 * 2. Generate diverse magic file content including all pattern variations above
 * 3. magic_check() with the generated magic file
 * 4. magic_close() to clean up resources
 * 
 * Project: $SRC (/root/src/libmagic)
 * Output directory: $OUTPUT/harnesses
 * Existing harnesses: 000-021 (analyze for semantic diversity)
 * 
 * Generate harness_022.cpp that focuses on magic file validation with comprehensive
 * pattern diversity, addressing the parse bottleneck to unlock magiccheck coverage.
 */

// Include libmagic headers

// Include necessary system headers for POSIX functions
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient input for diverse pattern generation
    // Minimum: flags + multiple pattern values
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================================
    // PHASE 1: Initialize magic context with CHECK flag
    // ============================================================
    // Use MAGIC_CHECK flag to enable validation checks during parsing
    magic_t ms = magic_open(MAGIC_CHECK);
    if (ms == nullptr) {
        return 0;
    }
    std::ostringstream magic_content;
    magic_content << "# Comprehensive magic file for parse coverage\n";
    magic_content << "# Targeting 178 blocked branches in apprentice.c parse()\n\n";
    
    // Helper function to generate random offset
    auto gen_offset = [&fdp]() -> std::string {
        int offset = fdp.ConsumeIntegralInRange<int>(-100, 100);
        return std::to_string(offset);
    };
    
    // Helper function to generate random value
    auto gen_value = [&fdp]() -> std::string {
        return std::to_string(fdp.ConsumeIntegral<int32_t>());
    };
    
    auto gen_float_value = [&fdp]() -> std::string {
        return std::to_string(fdp.ConsumeFloatingPoint<float>());
    };
    
    auto gen_double_value = [&fdp]() -> std::string {
        return std::to_string(fdp.ConsumeFloatingPoint<double>());
    };
    
    auto gen_string = [&fdp]() -> std::string {
        std::string str = fdp.ConsumeRandomLengthString(20);
        // Escape special characters
        std::string escaped;
        for (char c : str) {
            if (c == '\\' || c == '\t' || c == '\n') {
                escaped.push_back('\\');
                if (c == '\n') c = 'n';
                else if (c == '\t') c = 't';
            }
            escaped.push_back(c);
        }
        return escaped;
    };
    
    // ============================================================
    // PATTERN VARIATION 1: Continuation patterns (multiple `>` levels)
    // ============================================================
    magic_content << "\n# 1. Continuation patterns (multiple > levels)\n";
    magic_content << gen_offset() << "  byte = " << gen_value() << "  Root level\n";
    magic_content << ">" << gen_offset() << "  short = " << gen_value() << "  Level 1 continuation\n";
    magic_content << ">>" << gen_offset() << "  long = " << gen_value() << "  Level 2 continuation\n";
    magic_content << ">>>" << gen_offset() << "  belong = " << gen_value() << "  Level 3 continuation\n";
    magic_content << ">>>>" << gen_offset() << "  lelong = " << gen_value() << "  Level 4 continuation\n";
    
    // ============================================================
    // PATTERN VARIATION 2 & 3: Indirect modifiers and all type specifiers
    // ============================================================
    magic_content << "\n# 2. Indirect modifiers (&, ( syntax) and all type specifiers\n";
    
    // Indirect offset with various types
    magic_content << "&" << gen_offset() << "  byte = " << gen_value() << "  Relative offset with byte\n";
    magic_content << "(" << gen_offset() << "  short = " << gen_value() << "  Indirect offset with short\n";
    magic_content << "(&" << gen_offset() << "  long = " << gen_value() << "  Indirect relative offset with long\n";
    
    // Complete type coverage from FILE_BYTE through FILE_GUID
    magic_content << "\n# 3. All magic type specifiers\n";
    magic_content << gen_offset() << "  byte = " << gen_value() << "  FILE_BYTE\n";
    magic_content << gen_offset() << "  short = " << gen_value() << "  FILE_SHORT\n";
    magic_content << gen_offset() << "  long = " << gen_value() << "  FILE_LONG\n";
    magic_content << gen_offset() << "  string = " << gen_string() << "  FILE_STRING\n";
    magic_content << gen_offset() << "  date = " << gen_value() << "  FILE_DATE\n";
    magic_content << gen_offset() << "  beshort = " << gen_value() << "  FILE_BESHORT\n";
    magic_content << gen_offset() << "  belong = " << gen_value() << "  FILE_BELONG\n";
    magic_content << gen_offset() << "  bedate = " << gen_value() << "  FILE_BEDATE\n";
    magic_content << gen_offset() << "  leshort = " << gen_value() << "  FILE_LESHORT\n";
    magic_content << gen_offset() << "  lelong = " << gen_value() << "  FILE_LELONG\n";
    magic_content << gen_offset() << "  ledate = " << gen_value() << "  FILE_LEDATE\n";
    magic_content << gen_offset() << "  pstring = " << gen_string() << "  FILE_PSTRING\n";
    magic_content << gen_offset() << "  ldate = " << gen_value() << "  FILE_LDATE\n";
    magic_content << gen_offset() << "  beldate = " << gen_value() << "  FILE_BELDATE\n";
    magic_content << gen_offset() << "  leldate = " << gen_value() << "  FILE_LELDATE\n";
    magic_content << gen_offset() << "  regex = " << gen_string() << "  FILE_REGEX\n";
    magic_content << gen_offset() << "  bestring16 = " << gen_string() << "  FILE_BESTRING16\n";
    magic_content << gen_offset() << "  lestring16 = " << gen_string() << "  FILE_LESTRING16\n";
    magic_content << gen_offset() << "  search = " << gen_string() << "  FILE_SEARCH\n";
    magic_content << gen_offset() << "  medate = " << gen_value() << "  FILE_MEDATE\n";
    magic_content << gen_offset() << "  meldate = " << gen_value() << "  FILE_MELDATE\n";
    magic_content << gen_offset() << "  melong = " << gen_value() << "  FILE_MELONG\n";
    magic_content << gen_offset() << "  quad = " << gen_value() << "  FILE_QUAD\n";
    magic_content << gen_offset() << "  lequad = " << gen_value() << "  FILE_LEQUAD\n";
    magic_content << gen_offset() << "  bequad = " << gen_value() << "  FILE_BEQUAD\n";
    magic_content << gen_offset() << "  qdate = " << gen_value() << "  FILE_QDATE\n";
    magic_content << gen_offset() << "  leqdate = " << gen_value() << "  FILE_LEQDATE\n";
    magic_content << gen_offset() << "  beqdate = " << gen_value() << "  FILE_BEQDATE\n";
    magic_content << gen_offset() << "  qldate = " << gen_value() << "  FILE_QLDATE\n";
    magic_content << gen_offset() << "  leqldate = " << gen_value() << "  FILE_LEQLDATE\n";
    magic_content << gen_offset() << "  beqldate = " << gen_value() << "  FILE_BEQLDATE\n";
    magic_content << gen_offset() << "  float = " << gen_float_value() << "  FILE_FLOAT\n";
    magic_content << gen_offset() << "  befloat = " << gen_float_value() << "  FILE_BEFLOAT\n";
    magic_content << gen_offset() << "  lefloat = " << gen_float_value() << "  FILE_LEFLOAT\n";
    magic_content << gen_offset() << "  double = " << gen_double_value() << "  FILE_DOUBLE\n";
    magic_content << gen_offset() << "  bedouble = " << gen_double_value() << "  FILE_BEDOUBLE\n";
    magic_content << gen_offset() << "  ledouble = " << gen_double_value() << "  FILE_LEDOUBLE\n";
    magic_content << gen_offset() << "  leid3 = " << gen_value() << "  FILE_LEID3\n";
    magic_content << gen_offset() << "  beid3 = " << gen_value() << "  FILE_BEID3\n";
    magic_content << gen_offset() << "  indirect = " << gen_value() << "  FILE_INDIRECT\n";
    magic_content << gen_offset() << "  qwdate = " << gen_value() << "  FILE_QWDATE\n";
    magic_content << gen_offset() << "  leqwdate = " << gen_value() << "  FILE_LEQWDATE\n";
    magic_content << gen_offset() << "  beqwdate = " << gen_value() << "  FILE_BEQWDATE\n";
    magic_content << gen_offset() << "  name = " << gen_string() << "  FILE_NAME\n";
    magic_content << gen_offset() << "  use = " << gen_string() << "  FILE_USE\n";
    magic_content << gen_offset() << "  clear = " << gen_value() << "  FILE_CLEAR\n";
    magic_content << gen_offset() << "  der = " << gen_string() << "  FILE_DER\n";
    magic_content << gen_offset() << "  guid = " << gen_string() << "  FILE_GUID\n";
    
    // ============================================================
    // PATTERN VARIATION 4: String modifiers with various flags
    // ============================================================
    magic_content << "\n# 4. String modifiers with various flags\n";
    // String with B flag (treat as binary)
    magic_content << gen_offset() << "  string/b = " << gen_string() << "  String with /b flag\n";
    // String with W flag (wide characters)
    magic_content << gen_offset() << "  string/w = " << gen_string() << "  String with /w flag\n";
    // String with c flag (case insensitive)
    magic_content << gen_offset() << "  string/c = " << gen_string() << "  String with /c flag\n";
    // String with multiple flags
    magic_content << gen_offset() << "  string/bcw = " << gen_string() << "  String with /bcw flags\n";
    
    // ============================================================
    // PATTERN VARIATION 5: Edge case patterns that trigger default/error paths
    // ============================================================
    magic_content << "\n# 5. Edge case patterns for error paths\n";
    // Invalid type name to trigger default switch case
    magic_content << gen_offset() << "  invalidtype = " << gen_value() << "  Invalid type name\n";
    // Malformed offset
    magic_content << "abc  byte = " << gen_value() << "  Non-numeric offset\n";
    // Missing offset
    magic_content << "  byte = " << gen_value() << "  Missing offset\n";
    
    // ============================================================
    // PATTERN VARIATION 6: Invalid relation operators for float/double types
    // ============================================================
    magic_content << "\n# 6. Invalid relations for float/double\n";
    // Float with '&' relation (invalid for float)
    magic_content << gen_offset() << "  float & " << gen_float_value() << "  Float with & relation\n";
    // Double with '^' relation (invalid for double)
    magic_content << gen_offset() << "  double ^ " << gen_double_value() << "  Double with ^ relation\n";
    
    // ============================================================
    // PATTERN VARIATION 7: FILE_BESTRING16 vs FILE_LESTRING16 patterns
    // ============================================================
    magic_content << "\n# 7. BESTRING16 vs LESTRING16 patterns\n";
    // Test both with different relations
    magic_content << gen_offset() << "  bestring16 = " << gen_string() << "  BE string16 equal\n";
    magic_content << gen_offset() << "  bestring16 ! " << gen_string() << "  BE string16 not equal\n";
    magic_content << gen_offset() << "  lestring16 = " << gen_string() << "  LE string16 equal\n";
    magic_content << gen_offset() << "  lestring16 ! " << gen_string() << "  LE string16 not equal\n";
    
    // ============================================================
    // PATTERN VARIATION 8: Regex patterns with error conditions
    // ============================================================
    magic_content << "\n# 8. Regex patterns with potential errors\n";
    // Valid regex
    magic_content << gen_offset() << "  regex = ^test[0-9]+$  Valid regex pattern\n";
    // Invalid regex (unbalanced brackets)
    magic_content << gen_offset() << "  regex = test[0-9  Invalid regex (unbalanced)\n";
    // Regex with special characters
    magic_content << gen_offset() << "  regex = .*\\\\.txt$  Regex with escaped dot\n";
    
    // ============================================================
    // PATTERN VARIATION 9: Search patterns with range limits
    // ============================================================
    magic_content << "\n# 9. Search patterns with range limits\n";
    magic_content << gen_offset() << "  search/4096 = " << gen_string() << "  Search with 4K limit\n";
    magic_content << gen_offset() << "  search/1 = " << gen_string() << "  Search with 1 byte limit\n";
    
    // ============================================================
    // PATTERN VARIATION 10: Type combinations for default switch case
    // ============================================================
    magic_content << "\n# 10. Type combinations for default case\n";
    // Unknown type with various relations
    magic_content << gen_offset() << "  unknown = " << gen_value() << "  Unknown type = relation\n";
    magic_content << gen_offset() << "  unknown ! " << gen_value() << "  Unknown type ! relation\n";
    magic_content << gen_offset() << "  unknown > " << gen_value() << "  Unknown type > relation\n";
    
    // ============================================================
    // PATTERN VARIATION 11: Continuation level transitions (diff > 1)
    // ============================================================
    magic_content << "\n# 11. Continuation level jumps (diff > 1)\n";
    magic_content << gen_offset() << "  byte = " << gen_value() << "  Level 0\n";
    magic_content << ">" << gen_offset() << "  short = " << gen_value() << "  Level 1\n";
    magic_content << ">>>" << gen_offset() << "  long = " << gen_value() << "  Jump to Level 3 (diff=2)\n";
    
    // ============================================================
    // PATTERN VARIATION 12: Indirect offset at level 0
    // ============================================================
    magic_content << "\n# 12. Indirect offset at level 0 (should warn)\n";
    magic_content << "&" << gen_offset() << "  byte = " << gen_value() << "  & at level 0\n";
    magic_content << "(" << gen_offset() << "  short = " << gen_value() << "  ( at level 0\n";
    
    // ============================================================
    // PHASE 3: Write magic content to temporary file
    // ============================================================
    
    std::string magic_str = magic_content.str();
    
    // Create temporary file
    char magic_template[] = "/tmp/magic_check_fuzz_XXXXXX";
    int magic_fd = mkstemp(magic_template);
    if (magic_fd == -1) {
        magic_close(ms);
        return 0;
    }
    
    // Write magic content
    ssize_t written = write(magic_fd, magic_str.c_str(), magic_str.size());
    if (written < 0) {
        close(magic_fd);
        unlink(magic_template);
        magic_close(ms);
        return 0;
    }
    fsync(magic_fd);
    close(magic_fd);
    
    // ============================================================
    // PHASE 4: Call magic_check with the generated file
    // ============================================================
    
    int result = magic_check(ms, magic_template);
    
    // Clean up temporary file
    unlink(magic_template);
    
    // ============================================================
    // PHASE 5: Cleanup
    // ============================================================
    
    magic_close(ms);
    
    // Return 0 regardless of magic_check result - we're testing the parsing,
    // not expecting valid magic files
    return 0;
}

} // namespace harness_022
