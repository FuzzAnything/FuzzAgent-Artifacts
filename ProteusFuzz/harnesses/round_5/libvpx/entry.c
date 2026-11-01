// This is the entry of 68 fuzz drivers:
// 1, 3, 4, 6, 7, 9, 10, 11, 13, 14, 15, 17, 21, 23, 24, 25, 26, 32, 33, 34, 35, 36
// , 38, 44, 48, 49, 51, 52, 55, 60, 62, 63, 66, 76, 77, 78, 79, 82, 83, 84, 86, 87
// , 88, 91, 92, 95, 98, 107, 111, 113, 114, 116, 117, 118, 124, 125, 126, 128, 129
// , 130, 131, 136, 141, 142, 144, 145, 147, 149
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size);

// Entry function
int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
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
    switch (driverIndex % 68) {
        case 0:
            result = LLVMFuzzerTestOneInput_1(alignedData, remainSize);
            break;
        case 1:
            result = LLVMFuzzerTestOneInput_3(alignedData, remainSize);
            break;
        case 2:
            result = LLVMFuzzerTestOneInput_4(alignedData, remainSize);
            break;
        case 3:
            result = LLVMFuzzerTestOneInput_6(alignedData, remainSize);
            break;
        case 4:
            result = LLVMFuzzerTestOneInput_7(alignedData, remainSize);
            break;
        case 5:
            result = LLVMFuzzerTestOneInput_9(alignedData, remainSize);
            break;
        case 6:
            result = LLVMFuzzerTestOneInput_10(alignedData, remainSize);
            break;
        case 7:
            result = LLVMFuzzerTestOneInput_11(alignedData, remainSize);
            break;
        case 8:
            result = LLVMFuzzerTestOneInput_13(alignedData, remainSize);
            break;
        case 9:
            result = LLVMFuzzerTestOneInput_14(alignedData, remainSize);
            break;
        case 10:
            result = LLVMFuzzerTestOneInput_15(alignedData, remainSize);
            break;
        case 11:
            result = LLVMFuzzerTestOneInput_17(alignedData, remainSize);
            break;
        case 12:
            result = LLVMFuzzerTestOneInput_21(alignedData, remainSize);
            break;
        case 13:
            result = LLVMFuzzerTestOneInput_23(alignedData, remainSize);
            break;
        case 14:
            result = LLVMFuzzerTestOneInput_24(alignedData, remainSize);
            break;
        case 15:
            result = LLVMFuzzerTestOneInput_25(alignedData, remainSize);
            break;
        case 16:
            result = LLVMFuzzerTestOneInput_26(alignedData, remainSize);
            break;
        case 17:
            result = LLVMFuzzerTestOneInput_32(alignedData, remainSize);
            break;
        case 18:
            result = LLVMFuzzerTestOneInput_33(alignedData, remainSize);
            break;
        case 19:
            result = LLVMFuzzerTestOneInput_34(alignedData, remainSize);
            break;
        case 20:
            result = LLVMFuzzerTestOneInput_35(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_36(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_38(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_44(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_48(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_49(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_51(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_52(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_55(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_60(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_62(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_63(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_66(alignedData, remainSize);
            break;
        case 33:
            result = LLVMFuzzerTestOneInput_76(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_77(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_78(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_79(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_82(alignedData, remainSize);
            break;
        case 38:
            result = LLVMFuzzerTestOneInput_83(alignedData, remainSize);
            break;
        case 39:
            result = LLVMFuzzerTestOneInput_84(alignedData, remainSize);
            break;
        case 40:
            result = LLVMFuzzerTestOneInput_86(alignedData, remainSize);
            break;
        case 41:
            result = LLVMFuzzerTestOneInput_87(alignedData, remainSize);
            break;
        case 42:
            result = LLVMFuzzerTestOneInput_88(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_91(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_92(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_95(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_98(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_107(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_111(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_113(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_116(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_117(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_118(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_124(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_125(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_126(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_128(alignedData, remainSize);
            break;
        case 58:
            result = LLVMFuzzerTestOneInput_129(alignedData, remainSize);
            break;
        case 59:
            result = LLVMFuzzerTestOneInput_130(alignedData, remainSize);
            break;
        case 60:
            result = LLVMFuzzerTestOneInput_131(alignedData, remainSize);
            break;
        case 61:
            result = LLVMFuzzerTestOneInput_136(alignedData, remainSize);
            break;
        case 62:
            result = LLVMFuzzerTestOneInput_141(alignedData, remainSize);
            break;
        case 63:
            result = LLVMFuzzerTestOneInput_142(alignedData, remainSize);
            break;
        case 64:
            result = LLVMFuzzerTestOneInput_144(alignedData, remainSize);
            break;
        case 65:
            result = LLVMFuzzerTestOneInput_145(alignedData, remainSize);
            break;
        case 66:
            result = LLVMFuzzerTestOneInput_147(alignedData, remainSize);
            break;
        case 67:
            result = LLVMFuzzerTestOneInput_149(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
