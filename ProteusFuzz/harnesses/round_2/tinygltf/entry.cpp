// This is the entry of 33 fuzz drivers:
// 1, 2, 3, 4, 5, 6, 23, 25, 28, 29, 31, 32, 34, 37, 49, 51, 53, 57, 59, 65, 66, 67
// , 71, 76, 81, 92, 93, 98, 105, 109, 110, 114, 134
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 33) {
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
            result = LLVMFuzzerTestOneInput_4(alignedData, remainSize);
            break;
        case 4:
            result = LLVMFuzzerTestOneInput_5(alignedData, remainSize);
            break;
        case 5:
            result = LLVMFuzzerTestOneInput_6(alignedData, remainSize);
            break;
        case 6:
            result = LLVMFuzzerTestOneInput_23(alignedData, remainSize);
            break;
        case 7:
            result = LLVMFuzzerTestOneInput_25(alignedData, remainSize);
            break;
        case 8:
            result = LLVMFuzzerTestOneInput_28(alignedData, remainSize);
            break;
        case 9:
            result = LLVMFuzzerTestOneInput_29(alignedData, remainSize);
            break;
        case 10:
            result = LLVMFuzzerTestOneInput_31(alignedData, remainSize);
            break;
        case 11:
            result = LLVMFuzzerTestOneInput_32(alignedData, remainSize);
            break;
        case 12:
            result = LLVMFuzzerTestOneInput_34(alignedData, remainSize);
            break;
        case 13:
            result = LLVMFuzzerTestOneInput_37(alignedData, remainSize);
            break;
        case 14:
            result = LLVMFuzzerTestOneInput_49(alignedData, remainSize);
            break;
        case 15:
            result = LLVMFuzzerTestOneInput_51(alignedData, remainSize);
            break;
        case 16:
            result = LLVMFuzzerTestOneInput_53(alignedData, remainSize);
            break;
        case 17:
            result = LLVMFuzzerTestOneInput_57(alignedData, remainSize);
            break;
        case 18:
            result = LLVMFuzzerTestOneInput_59(alignedData, remainSize);
            break;
        case 19:
            result = LLVMFuzzerTestOneInput_65(alignedData, remainSize);
            break;
        case 20:
            result = LLVMFuzzerTestOneInput_66(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_67(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_71(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_76(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_81(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_92(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_93(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_98(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_105(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_109(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_110(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_134(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
