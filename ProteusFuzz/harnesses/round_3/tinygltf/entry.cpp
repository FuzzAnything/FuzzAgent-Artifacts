// This is the entry of 38 fuzz drivers:
// 1, 2, 4, 5, 8, 12, 14, 15, 16, 18, 19, 23, 40, 43, 44, 51, 66, 70, 72, 77, 78, 9
// 3, 103, 106, 108, 112, 114, 120, 126, 128, 131, 136, 138, 143, 149, 150, 151, 15
// 3
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size);

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

    // The selector prefix can make remainData unaligned. Copy the payload to
    // malloc-aligned storage before passing it to generated C/C++ harnesses.
    uint8_t *alignedData = (uint8_t *)malloc(remainSize);
    if (alignedData == NULL) {
        return 0;
    }
    memcpy(alignedData, remainData, remainSize);

    // Select driver
    int result = 0;
    switch (driverIndex % 38) {
        case 0:
            result = LLVMFuzzerTestOneInput_1(alignedData, remainSize);
            break;
        case 1:
            result = LLVMFuzzerTestOneInput_2(alignedData, remainSize);
            break;
        case 2:
            result = LLVMFuzzerTestOneInput_4(alignedData, remainSize);
            break;
        case 3:
            result = LLVMFuzzerTestOneInput_5(alignedData, remainSize);
            break;
        case 4:
            result = LLVMFuzzerTestOneInput_8(alignedData, remainSize);
            break;
        case 5:
            result = LLVMFuzzerTestOneInput_12(alignedData, remainSize);
            break;
        case 6:
            result = LLVMFuzzerTestOneInput_14(alignedData, remainSize);
            break;
        case 7:
            result = LLVMFuzzerTestOneInput_15(alignedData, remainSize);
            break;
        case 8:
            result = LLVMFuzzerTestOneInput_16(alignedData, remainSize);
            break;
        case 9:
            result = LLVMFuzzerTestOneInput_18(alignedData, remainSize);
            break;
        case 10:
            result = LLVMFuzzerTestOneInput_19(alignedData, remainSize);
            break;
        case 11:
            result = LLVMFuzzerTestOneInput_23(alignedData, remainSize);
            break;
        case 12:
            result = LLVMFuzzerTestOneInput_40(alignedData, remainSize);
            break;
        case 13:
            result = LLVMFuzzerTestOneInput_43(alignedData, remainSize);
            break;
        case 14:
            result = LLVMFuzzerTestOneInput_44(alignedData, remainSize);
            break;
        case 15:
            result = LLVMFuzzerTestOneInput_51(alignedData, remainSize);
            break;
        case 16:
            result = LLVMFuzzerTestOneInput_66(alignedData, remainSize);
            break;
        case 17:
            result = LLVMFuzzerTestOneInput_70(alignedData, remainSize);
            break;
        case 18:
            result = LLVMFuzzerTestOneInput_72(alignedData, remainSize);
            break;
        case 19:
            result = LLVMFuzzerTestOneInput_77(alignedData, remainSize);
            break;
        case 20:
            result = LLVMFuzzerTestOneInput_78(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_93(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_103(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_106(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_108(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_112(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_120(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_126(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_128(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_131(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_136(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_138(alignedData, remainSize);
            break;
        case 33:
            result = LLVMFuzzerTestOneInput_143(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_149(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_150(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_151(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_153(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
