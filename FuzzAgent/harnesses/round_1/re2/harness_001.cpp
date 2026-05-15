#include <fuzzer/FuzzedDataProvider.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include "re2/re2.h"

namespace harness_001 {
// Minimal RE2 fuzzing harness


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size == 0 || size > 4096)
    return 0;

  FuzzedDataProvider fdp(data, size);

  // Create pattern and text from fuzzer input
  std::string pattern = fdp.ConsumeRandomLengthString(256);
  std::string text = fdp.ConsumeRandomLengthString(256);

  // Try to compile and match pattern
  re2::RE2 re(pattern);
  
  // Only test matching if pattern compiled successfully
  if (re.ok()) {
    // Test basic matching
    re2::RE2::FullMatch(text, re);
    re2::RE2::PartialMatch(text, re);
  }

  return 0;
}

} // namespace harness_001
