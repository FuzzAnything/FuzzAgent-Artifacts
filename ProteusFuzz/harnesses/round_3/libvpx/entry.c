// This is the entry of 67 fuzz drivers:
// 1, 6, 12, 13, 14, 15, 17, 18, 19, 20, 21, 22, 24, 27, 28, 29, 30, 32, 35, 36, 39
// , 41, 42, 43, 44, 45, 49, 51, 59, 60, 63, 64, 65, 67, 69, 71, 72, 75, 76, 78, 79
// , 88, 89, 90, 91, 94, 96, 97, 100, 101, 104, 106, 108, 112, 114, 116, 117, 119, 
// 120, 122, 126, 127, 129, 130, 131, 134, 137
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 67) {
        case 0:
            result = LLVMFuzzerTestOneInput_1(alignedData, remainSize);
            break;
        case 1:
            result = LLVMFuzzerTestOneInput_6(alignedData, remainSize);
            break;
        case 2:
            result = LLVMFuzzerTestOneInput_12(alignedData, remainSize);
            break;
        case 3:
            result = LLVMFuzzerTestOneInput_13(alignedData, remainSize);
            break;
        case 4:
            result = LLVMFuzzerTestOneInput_14(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_20(alignedData, remainSize);
            break;
        case 10:
            result = LLVMFuzzerTestOneInput_21(alignedData, remainSize);
            break;
        case 11:
            result = LLVMFuzzerTestOneInput_22(alignedData, remainSize);
            break;
        case 12:
            result = LLVMFuzzerTestOneInput_24(alignedData, remainSize);
            break;
        case 13:
            result = LLVMFuzzerTestOneInput_27(alignedData, remainSize);
            break;
        case 14:
            result = LLVMFuzzerTestOneInput_28(alignedData, remainSize);
            break;
        case 15:
            result = LLVMFuzzerTestOneInput_29(alignedData, remainSize);
            break;
        case 16:
            result = LLVMFuzzerTestOneInput_30(alignedData, remainSize);
            break;
        case 17:
            result = LLVMFuzzerTestOneInput_32(alignedData, remainSize);
            break;
        case 18:
            result = LLVMFuzzerTestOneInput_35(alignedData, remainSize);
            break;
        case 19:
            result = LLVMFuzzerTestOneInput_36(alignedData, remainSize);
            break;
        case 20:
            result = LLVMFuzzerTestOneInput_39(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_41(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_42(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_43(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_44(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_45(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_49(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_51(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_59(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_60(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_63(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_64(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_65(alignedData, remainSize);
            break;
        case 33:
            result = LLVMFuzzerTestOneInput_67(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_69(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_71(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_72(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_75(alignedData, remainSize);
            break;
        case 38:
            result = LLVMFuzzerTestOneInput_76(alignedData, remainSize);
            break;
        case 39:
            result = LLVMFuzzerTestOneInput_78(alignedData, remainSize);
            break;
        case 40:
            result = LLVMFuzzerTestOneInput_79(alignedData, remainSize);
            break;
        case 41:
            result = LLVMFuzzerTestOneInput_88(alignedData, remainSize);
            break;
        case 42:
            result = LLVMFuzzerTestOneInput_89(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_90(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_91(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_94(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_96(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_97(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_100(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_101(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_104(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_106(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_108(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_112(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_116(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_117(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_119(alignedData, remainSize);
            break;
        case 58:
            result = LLVMFuzzerTestOneInput_120(alignedData, remainSize);
            break;
        case 59:
            result = LLVMFuzzerTestOneInput_122(alignedData, remainSize);
            break;
        case 60:
            result = LLVMFuzzerTestOneInput_126(alignedData, remainSize);
            break;
        case 61:
            result = LLVMFuzzerTestOneInput_127(alignedData, remainSize);
            break;
        case 62:
            result = LLVMFuzzerTestOneInput_129(alignedData, remainSize);
            break;
        case 63:
            result = LLVMFuzzerTestOneInput_130(alignedData, remainSize);
            break;
        case 64:
            result = LLVMFuzzerTestOneInput_131(alignedData, remainSize);
            break;
        case 65:
            result = LLVMFuzzerTestOneInput_134(alignedData, remainSize);
            break;
        case 66:
            result = LLVMFuzzerTestOneInput_137(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
