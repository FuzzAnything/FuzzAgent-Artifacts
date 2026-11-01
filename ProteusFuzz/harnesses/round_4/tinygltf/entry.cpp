// This is the entry of 51 fuzz drivers:
// 1, 2, 3, 5, 6, 15, 17, 18, 19, 25, 27, 31, 33, 38, 39, 47, 50, 51, 54, 60, 75, 7
// 7, 80, 81, 83, 85, 86, 87, 88, 96, 97, 102, 105, 107, 112, 114, 116, 119, 120, 1
// 22, 123, 128, 134, 137, 138, 140, 147, 148, 149, 155, 162
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 51) {
        case 0:
            result = LLVMFuzzerTestOneInput_1(alignedData, remainSize);
            break;
        case 1:
            result = LLVMFuzzerTestOneInput_2(alignedData, remainSize);
            break;
        case 2:
            result = LLVMFuzzerTestOneInput_3(alignedData, remainSize);
            break;
        case 3:
            result = LLVMFuzzerTestOneInput_5(alignedData, remainSize);
            break;
        case 4:
            result = LLVMFuzzerTestOneInput_6(alignedData, remainSize);
            break;
        case 5:
            result = LLVMFuzzerTestOneInput_15(alignedData, remainSize);
            break;
        case 6:
            result = LLVMFuzzerTestOneInput_17(alignedData, remainSize);
            break;
        case 7:
            result = LLVMFuzzerTestOneInput_18(alignedData, remainSize);
            break;
        case 8:
            result = LLVMFuzzerTestOneInput_19(alignedData, remainSize);
            break;
        case 9:
            result = LLVMFuzzerTestOneInput_25(alignedData, remainSize);
            break;
        case 10:
            result = LLVMFuzzerTestOneInput_27(alignedData, remainSize);
            break;
        case 11:
            result = LLVMFuzzerTestOneInput_31(alignedData, remainSize);
            break;
        case 12:
            result = LLVMFuzzerTestOneInput_33(alignedData, remainSize);
            break;
        case 13:
            result = LLVMFuzzerTestOneInput_38(alignedData, remainSize);
            break;
        case 14:
            result = LLVMFuzzerTestOneInput_39(alignedData, remainSize);
            break;
        case 15:
            result = LLVMFuzzerTestOneInput_47(alignedData, remainSize);
            break;
        case 16:
            result = LLVMFuzzerTestOneInput_50(alignedData, remainSize);
            break;
        case 17:
            result = LLVMFuzzerTestOneInput_51(alignedData, remainSize);
            break;
        case 18:
            result = LLVMFuzzerTestOneInput_54(alignedData, remainSize);
            break;
        case 19:
            result = LLVMFuzzerTestOneInput_60(alignedData, remainSize);
            break;
        case 20:
            result = LLVMFuzzerTestOneInput_75(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_77(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_80(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_81(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_83(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_85(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_86(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_87(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_88(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_96(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_97(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_102(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_105(alignedData, remainSize);
            break;
        case 33:
            result = LLVMFuzzerTestOneInput_107(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_112(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_116(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_119(alignedData, remainSize);
            break;
        case 38:
            result = LLVMFuzzerTestOneInput_120(alignedData, remainSize);
            break;
        case 39:
            result = LLVMFuzzerTestOneInput_122(alignedData, remainSize);
            break;
        case 40:
            result = LLVMFuzzerTestOneInput_123(alignedData, remainSize);
            break;
        case 41:
            result = LLVMFuzzerTestOneInput_128(alignedData, remainSize);
            break;
        case 42:
            result = LLVMFuzzerTestOneInput_134(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_137(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_138(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_140(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_147(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_148(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_149(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_155(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_162(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
