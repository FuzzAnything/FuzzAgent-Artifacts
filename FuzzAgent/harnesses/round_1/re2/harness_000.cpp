#include <stddef.h>
#include <stdint.h>

namespace harness_000 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  // Simple harness that does nothing but exercises the fuzzer infrastructure
  if (size > 0 && data[0] == 'X') {
    return 0;
  }
  return 0;
}

} // namespace harness_000
