#include <stdint.h>
#include <stddef.h>

namespace harness_000 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_002 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_003 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_005 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_006 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_007 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_009 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_010 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_011 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_012 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_013 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_014 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_016 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_017 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_018 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }
namespace harness_019 { int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size); }

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    uint8_t selector = Data[0] % 16;
    const uint8_t *NewData = Data + 1;
    size_t NewSize = Size - 1;

    switch (selector) {
        case 0: return harness_000::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 1: return harness_002::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 2: return harness_003::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 3: return harness_005::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 4: return harness_006::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 5: return harness_007::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 6: return harness_009::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 7: return harness_010::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 8: return harness_011::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 9: return harness_012::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 10: return harness_013::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 11: return harness_014::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 12: return harness_016::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 13: return harness_017::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 14: return harness_018::LLVMFuzzerTestOneInput(NewData, NewSize);
        case 15: return harness_019::LLVMFuzzerTestOneInput(NewData, NewSize);
    }
    return 0;
}
