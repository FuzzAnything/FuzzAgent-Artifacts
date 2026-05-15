// This is the entry of 38 fuzz drivers:
// 4, 6, 28, 33, 42, 88, 96, 102, 107, 119, 120, 124, 133, 136, 141, 144, 148, 151,
//  231, 259, 285, 286, 293, 299, 309, 318, 320, 323, 324, 341, 350, 370, 371, 398,
//  404, 414, 419, 423
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_231(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_259(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_285(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_286(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_293(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_299(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_309(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_318(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_320(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_323(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_324(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_341(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_350(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_370(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_371(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_398(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_404(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_414(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_419(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_423(const uint8_t *Data, size_t Size);

// Entry function
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Driver selector
    if (Size < 1) {
        return 0;
    }
    const uint8_t *selector = Data;
    unsigned int driverIndex = 0;
    memcpy(&driverIndex, selector, 1);

    // Remain data and size
    const uint8_t *remainData = Data + 1;
    size_t remainSize = Size - 1;
    if (remainSize == 0) {
        return 0;
    }

    // Select driver
    switch (driverIndex % 38) {
        case 0:
            return LLVMFuzzerTestOneInput_4(remainData, remainSize);
        case 1:
            return LLVMFuzzerTestOneInput_6(remainData, remainSize);
        case 2:
            return LLVMFuzzerTestOneInput_28(remainData, remainSize);
        case 3:
            return LLVMFuzzerTestOneInput_33(remainData, remainSize);
        case 4:
            return LLVMFuzzerTestOneInput_42(remainData, remainSize);
        case 5:
            return LLVMFuzzerTestOneInput_88(remainData, remainSize);
        case 6:
            return LLVMFuzzerTestOneInput_96(remainData, remainSize);
        case 7:
            return LLVMFuzzerTestOneInput_102(remainData, remainSize);
        case 8:
            return LLVMFuzzerTestOneInput_107(remainData, remainSize);
        case 9:
            return LLVMFuzzerTestOneInput_119(remainData, remainSize);
        case 10:
            return LLVMFuzzerTestOneInput_120(remainData, remainSize);
        case 11:
            return LLVMFuzzerTestOneInput_124(remainData, remainSize);
        case 12:
            return LLVMFuzzerTestOneInput_133(remainData, remainSize);
        case 13:
            return LLVMFuzzerTestOneInput_136(remainData, remainSize);
        case 14:
            return LLVMFuzzerTestOneInput_141(remainData, remainSize);
        case 15:
            return LLVMFuzzerTestOneInput_144(remainData, remainSize);
        case 16:
            return LLVMFuzzerTestOneInput_148(remainData, remainSize);
        case 17:
            return LLVMFuzzerTestOneInput_151(remainData, remainSize);
        case 18:
            return LLVMFuzzerTestOneInput_231(remainData, remainSize);
        case 19:
            return LLVMFuzzerTestOneInput_259(remainData, remainSize);
        case 20:
            return LLVMFuzzerTestOneInput_285(remainData, remainSize);
        case 21:
            return LLVMFuzzerTestOneInput_286(remainData, remainSize);
        case 22:
            return LLVMFuzzerTestOneInput_293(remainData, remainSize);
        case 23:
            return LLVMFuzzerTestOneInput_299(remainData, remainSize);
        case 24:
            return LLVMFuzzerTestOneInput_309(remainData, remainSize);
        case 25:
            return LLVMFuzzerTestOneInput_318(remainData, remainSize);
        case 26:
            return LLVMFuzzerTestOneInput_320(remainData, remainSize);
        case 27:
            return LLVMFuzzerTestOneInput_323(remainData, remainSize);
        case 28:
            return LLVMFuzzerTestOneInput_324(remainData, remainSize);
        case 29:
            return LLVMFuzzerTestOneInput_341(remainData, remainSize);
        case 30:
            return LLVMFuzzerTestOneInput_350(remainData, remainSize);
        case 31:
            return LLVMFuzzerTestOneInput_370(remainData, remainSize);
        case 32:
            return LLVMFuzzerTestOneInput_371(remainData, remainSize);
        case 33:
            return LLVMFuzzerTestOneInput_398(remainData, remainSize);
        case 34:
            return LLVMFuzzerTestOneInput_404(remainData, remainSize);
        case 35:
            return LLVMFuzzerTestOneInput_414(remainData, remainSize);
        case 36:
            return LLVMFuzzerTestOneInput_419(remainData, remainSize);
        case 37:
            return LLVMFuzzerTestOneInput_423(remainData, remainSize);
        default:
            return 0;
    }
    return 0;
}
