#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <lcms2.h>

// Minimal valid IT8 template to ensure parser entry into DataSection.
// Contains: Header with 2 sets, 2 samples, DataFormat, and Data with ID, String, and Numeric values.
// This structure is designed to trigger lines 2159 (SIDENT), 2164 (SSTRING), 2169 (default/numeric),
// and potentially 2191 (mismatch) if the fuzzer alters the data count.
static const char g_minimal_it8[] = 
    "IT8.1\n"
    "NUMBER_OF_SETS 2\n"
    "NUMBER_OF_SAMPLES 2\n"
    "BEGIN_DATA_FORMAT\n"
    "SAMPLE 1\n"
    "SAMPLE 2\n"
    "END_DATA_FORMAT\n"
    "BEGIN_DATA\n"
    "ID1 \"STR1\" 100\n"
    "ID2 \"STR2\" 200\n"
    "END_DATA\n";

int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const uint8_t *input_data = Data;
    size_t input_len = Size;

    // If input is too small to be a valid IT8 file, use the template.
    // This ensures we can reach the DataSection logic even with small fuzz inputs.
    if (Size < 20) {
        input_data = (const uint8_t*)g_minimal_it8;
        input_len = sizeof(g_minimal_it8) - 1;
    }

    // Load IT8 from memory. This triggers the call chain:
    // cmsIT8LoadFromMem -> ParseIT8 -> DataSection
    // The template ensures the parser enters DataSection and exercises:
    // - Line 2141-2142: Allocation check
    // - Line 2145: While loop entry
    // - Line 2147: Field wrap-around (if data is long enough)
    // - Line 2159: SIDENT case (ID1, ID2)
    // - Line 2164: SSTRING case ("STR1", "STR2")
    // - Line 2169/2171/2174: Default case (100, 200)
    // - Line 2191: Mismatch check (if fuzzer modifies set count vs data lines)
    cmsHANDLE hIT8 = cmsIT8LoadFromMem(NULL, input_data, (cmsUInt32Number)input_len);

    // Cleanup
    if (hIT8) {
        cmsIT8Free(hIT8);
    }

    return 0;
}