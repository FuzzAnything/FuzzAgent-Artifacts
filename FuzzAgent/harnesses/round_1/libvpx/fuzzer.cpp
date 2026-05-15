#include <stdint.h>
#include <stddef.h>

namespace harness_004 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_006 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_010 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    uint8_t selector = Data[0] % 3;
    const uint8_t *NewData = Data + 1;
    size_t NewSize = Size - 1;

    switch (selector) {
        case 0: return harness_004::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 1: return harness_006::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 2: return harness_010::LLVMFuzzerTestOneInput(NewData, NewSize);
    }
    return 0;
}
