#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "re2/re2.h"

namespace harness_006 {
// RE2 program analysis functions harness
// Targets: RE2::ProgramSize, RE2::ProgramFanout, RE2::ReverseProgramSize, RE2::ReverseProgramFanout
// These APIs currently have 0% coverage and represent 12+ branches of undiscovered complexity
// Based on coverage guidance: /root/FuzzAgent/output/re2/coverage_guidance/harness_006_guidance.md



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Minimum size check: need enough data for pattern and test operations
  if (size < 16) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // 1. Create pattern from fuzzer input
  std::string pattern = fdp.ConsumeRandomLengthString(256);
  
  // 2. Create RE2 object with the pattern
  re2::RE2 re(pattern);
  
  // Only proceed if pattern compiled successfully
  if (!re.ok()) {
    return 0;
  }

  // 3. Test ProgramSize() - get forward program size
  int program_size = re.ProgramSize();
  // program_size should be >= 0 for valid compiled regex

  // 4. Test ProgramFanout() - analyze forward fanout distribution
  // Determine histogram size from fuzzer input
  size_t histogram_size = fdp.ConsumeIntegralInRange<size_t>(0, 100);
  std::vector<int> forward_histogram;
  
  // Resize histogram vector if needed (will be filled by ProgramFanout)
  if (histogram_size > 0) {
    forward_histogram.resize(histogram_size);
  }
  
  // Call ProgramFanout with the histogram vector
  int forward_fanout_bucket = re.ProgramFanout(&forward_histogram);
  // forward_fanout_bucket indicates the largest non-empty bucket
  // negative value indicates error

  // 5. Test ReverseProgramSize() - get reverse program size
  int reverse_program_size = re.ReverseProgramSize();
  // reverse_program_size should be >= 0 for valid compiled regex

  // 6. Test ReverseProgramFanout() - analyze reverse fanout distribution
  // Determine histogram size from remaining fuzzer input
  size_t reverse_histogram_size = fdp.ConsumeIntegralInRange<size_t>(0, 100);
  std::vector<int> reverse_histogram;
  
  // Resize histogram vector if needed (will be filled by ReverseProgramFanout)
  if (reverse_histogram_size > 0) {
    reverse_histogram.resize(reverse_histogram_size);
  }
  
  // Call ReverseProgramFanout with the histogram vector
  int reverse_fanout_bucket = re.ReverseProgramFanout(&reverse_histogram);
  // reverse_fanout_bucket indicates the largest non-empty bucket
  // negative value indicates error

  // 7. Additional testing with different histogram configurations
  // Test with empty histogram (nullptr should be allowed according to documentation)
  if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
    std::vector<int> empty_histogram;
    re.ProgramFanout(&empty_histogram);
    re.ReverseProgramFanout(&empty_histogram);
  }

  // 8. Test with pre-initialized histogram values
  if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
    size_t preinit_size = fdp.ConsumeIntegralInRange<size_t>(1, 50);
    std::vector<int> preinit_histogram(preinit_size);
    
    // Initialize with some values from fuzzer input
    for (size_t i = 0; i < preinit_size && fdp.remaining_bytes() > 0; i++) {
      preinit_histogram[i] = fdp.ConsumeIntegral<int>();
    }
    
    re.ProgramFanout(&preinit_histogram);
    // Note: histogram contents will be overwritten by the function
  }

  // 9. Test with very large histogram to exercise boundary conditions
  if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
    std::vector<int> large_histogram(1000);  // Fixed large size
    re.ProgramFanout(&large_histogram);
    re.ReverseProgramFanout(&large_histogram);
  }

  // 10. Test relationship between forward and reverse program sizes
  // For some regex patterns, forward and reverse program sizes may differ
  // This is implementation-specific behavior worth observing

  // RE2 object automatically cleaned up when it goes out of scope
  // All vector objects also automatically cleaned up

  return 0;
}

} // namespace harness_006
