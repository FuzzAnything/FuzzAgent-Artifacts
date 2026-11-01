// This is the entry of 88 fuzz drivers:
// 1, 2, 6, 7, 8, 10, 11, 16, 17, 18, 22, 25, 26, 27, 32, 33, 35, 36, 37, 38, 39, 4
// 0, 41, 43, 45, 47, 48, 51, 52, 54, 55, 56, 57, 59, 60, 64, 66, 71, 75, 81, 82, 8
// 3, 84, 85, 86, 90, 93, 94, 95, 97, 98, 99, 101, 105, 106, 107, 109, 110, 111, 11
// 2, 113, 118, 119, 125, 130, 131, 132, 135, 137, 138, 139, 141, 143, 149, 151, 15
// 6, 157, 158, 159, 160, 162, 163, 164, 165, 166, 167, 170, 171
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_157(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_165(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_166(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_167(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_170(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_171(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 88) {
        case 0:
            result = LLVMFuzzerTestOneInput_1(alignedData, remainSize);
            break;
        case 1:
            result = LLVMFuzzerTestOneInput_2(alignedData, remainSize);
            break;
        case 2:
            result = LLVMFuzzerTestOneInput_6(alignedData, remainSize);
            break;
        case 3:
            result = LLVMFuzzerTestOneInput_7(alignedData, remainSize);
            break;
        case 4:
            result = LLVMFuzzerTestOneInput_8(alignedData, remainSize);
            break;
        case 5:
            result = LLVMFuzzerTestOneInput_10(alignedData, remainSize);
            break;
        case 6:
            result = LLVMFuzzerTestOneInput_11(alignedData, remainSize);
            break;
        case 7:
            result = LLVMFuzzerTestOneInput_16(alignedData, remainSize);
            break;
        case 8:
            result = LLVMFuzzerTestOneInput_17(alignedData, remainSize);
            break;
        case 9:
            result = LLVMFuzzerTestOneInput_18(alignedData, remainSize);
            break;
        case 10:
            result = LLVMFuzzerTestOneInput_22(alignedData, remainSize);
            break;
        case 11:
            result = LLVMFuzzerTestOneInput_25(alignedData, remainSize);
            break;
        case 12:
            result = LLVMFuzzerTestOneInput_26(alignedData, remainSize);
            break;
        case 13:
            result = LLVMFuzzerTestOneInput_27(alignedData, remainSize);
            break;
        case 14:
            result = LLVMFuzzerTestOneInput_32(alignedData, remainSize);
            break;
        case 15:
            result = LLVMFuzzerTestOneInput_33(alignedData, remainSize);
            break;
        case 16:
            result = LLVMFuzzerTestOneInput_35(alignedData, remainSize);
            break;
        case 17:
            result = LLVMFuzzerTestOneInput_36(alignedData, remainSize);
            break;
        case 18:
            result = LLVMFuzzerTestOneInput_37(alignedData, remainSize);
            break;
        case 19:
            result = LLVMFuzzerTestOneInput_38(alignedData, remainSize);
            break;
        case 20:
            result = LLVMFuzzerTestOneInput_39(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_40(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_41(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_43(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_45(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_47(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_48(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_51(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_52(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_54(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_55(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_56(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_57(alignedData, remainSize);
            break;
        case 33:
            result = LLVMFuzzerTestOneInput_59(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_60(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_64(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_66(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_71(alignedData, remainSize);
            break;
        case 38:
            result = LLVMFuzzerTestOneInput_75(alignedData, remainSize);
            break;
        case 39:
            result = LLVMFuzzerTestOneInput_81(alignedData, remainSize);
            break;
        case 40:
            result = LLVMFuzzerTestOneInput_82(alignedData, remainSize);
            break;
        case 41:
            result = LLVMFuzzerTestOneInput_83(alignedData, remainSize);
            break;
        case 42:
            result = LLVMFuzzerTestOneInput_84(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_85(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_86(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_90(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_93(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_94(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_95(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_97(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_98(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_99(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_101(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_105(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_106(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_107(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_109(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_110(alignedData, remainSize);
            break;
        case 58:
            result = LLVMFuzzerTestOneInput_111(alignedData, remainSize);
            break;
        case 59:
            result = LLVMFuzzerTestOneInput_112(alignedData, remainSize);
            break;
        case 60:
            result = LLVMFuzzerTestOneInput_113(alignedData, remainSize);
            break;
        case 61:
            result = LLVMFuzzerTestOneInput_118(alignedData, remainSize);
            break;
        case 62:
            result = LLVMFuzzerTestOneInput_119(alignedData, remainSize);
            break;
        case 63:
            result = LLVMFuzzerTestOneInput_125(alignedData, remainSize);
            break;
        case 64:
            result = LLVMFuzzerTestOneInput_130(alignedData, remainSize);
            break;
        case 65:
            result = LLVMFuzzerTestOneInput_131(alignedData, remainSize);
            break;
        case 66:
            result = LLVMFuzzerTestOneInput_132(alignedData, remainSize);
            break;
        case 67:
            result = LLVMFuzzerTestOneInput_135(alignedData, remainSize);
            break;
        case 68:
            result = LLVMFuzzerTestOneInput_137(alignedData, remainSize);
            break;
        case 69:
            result = LLVMFuzzerTestOneInput_138(alignedData, remainSize);
            break;
        case 70:
            result = LLVMFuzzerTestOneInput_139(alignedData, remainSize);
            break;
        case 71:
            result = LLVMFuzzerTestOneInput_141(alignedData, remainSize);
            break;
        case 72:
            result = LLVMFuzzerTestOneInput_143(alignedData, remainSize);
            break;
        case 73:
            result = LLVMFuzzerTestOneInput_149(alignedData, remainSize);
            break;
        case 74:
            result = LLVMFuzzerTestOneInput_151(alignedData, remainSize);
            break;
        case 75:
            result = LLVMFuzzerTestOneInput_156(alignedData, remainSize);
            break;
        case 76:
            result = LLVMFuzzerTestOneInput_157(alignedData, remainSize);
            break;
        case 77:
            result = LLVMFuzzerTestOneInput_158(alignedData, remainSize);
            break;
        case 78:
            result = LLVMFuzzerTestOneInput_159(alignedData, remainSize);
            break;
        case 79:
            result = LLVMFuzzerTestOneInput_160(alignedData, remainSize);
            break;
        case 80:
            result = LLVMFuzzerTestOneInput_162(alignedData, remainSize);
            break;
        case 81:
            result = LLVMFuzzerTestOneInput_163(alignedData, remainSize);
            break;
        case 82:
            result = LLVMFuzzerTestOneInput_164(alignedData, remainSize);
            break;
        case 83:
            result = LLVMFuzzerTestOneInput_165(alignedData, remainSize);
            break;
        case 84:
            result = LLVMFuzzerTestOneInput_166(alignedData, remainSize);
            break;
        case 85:
            result = LLVMFuzzerTestOneInput_167(alignedData, remainSize);
            break;
        case 86:
            result = LLVMFuzzerTestOneInput_170(alignedData, remainSize);
            break;
        case 87:
            result = LLVMFuzzerTestOneInput_171(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
