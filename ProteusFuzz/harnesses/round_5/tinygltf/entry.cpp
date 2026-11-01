// This is the entry of 91 fuzz drivers:
// 1, 2, 3, 4, 5, 7, 8, 9, 12, 13, 14, 15, 16, 17, 19, 20, 23, 25, 28, 31, 32, 34, 
// 37, 40, 41, 42, 43, 46, 47, 55, 56, 58, 59, 62, 63, 65, 67, 68, 70, 71, 72, 73, 
// 74, 75, 76, 77, 79, 80, 84, 85, 89, 90, 93, 94, 95, 97, 98, 102, 105, 106, 107, 
// 108, 109, 110, 111, 112, 113, 115, 116, 118, 122, 123, 124, 129, 130, 131, 133, 
// 134, 135, 136, 137, 138, 141, 142, 145, 146, 150, 153, 154, 157, 164
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
extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_157(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 91) {
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
            result = LLVMFuzzerTestOneInput_7(alignedData, remainSize);
            break;
        case 6:
            result = LLVMFuzzerTestOneInput_8(alignedData, remainSize);
            break;
        case 7:
            result = LLVMFuzzerTestOneInput_9(alignedData, remainSize);
            break;
        case 8:
            result = LLVMFuzzerTestOneInput_12(alignedData, remainSize);
            break;
        case 9:
            result = LLVMFuzzerTestOneInput_13(alignedData, remainSize);
            break;
        case 10:
            result = LLVMFuzzerTestOneInput_14(alignedData, remainSize);
            break;
        case 11:
            result = LLVMFuzzerTestOneInput_15(alignedData, remainSize);
            break;
        case 12:
            result = LLVMFuzzerTestOneInput_16(alignedData, remainSize);
            break;
        case 13:
            result = LLVMFuzzerTestOneInput_17(alignedData, remainSize);
            break;
        case 14:
            result = LLVMFuzzerTestOneInput_19(alignedData, remainSize);
            break;
        case 15:
            result = LLVMFuzzerTestOneInput_20(alignedData, remainSize);
            break;
        case 16:
            result = LLVMFuzzerTestOneInput_23(alignedData, remainSize);
            break;
        case 17:
            result = LLVMFuzzerTestOneInput_25(alignedData, remainSize);
            break;
        case 18:
            result = LLVMFuzzerTestOneInput_28(alignedData, remainSize);
            break;
        case 19:
            result = LLVMFuzzerTestOneInput_31(alignedData, remainSize);
            break;
        case 20:
            result = LLVMFuzzerTestOneInput_32(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_34(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_37(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_40(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_41(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_42(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_43(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_46(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_47(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_55(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_56(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_58(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_59(alignedData, remainSize);
            break;
        case 33:
            result = LLVMFuzzerTestOneInput_62(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_63(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_65(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_67(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_68(alignedData, remainSize);
            break;
        case 38:
            result = LLVMFuzzerTestOneInput_70(alignedData, remainSize);
            break;
        case 39:
            result = LLVMFuzzerTestOneInput_71(alignedData, remainSize);
            break;
        case 40:
            result = LLVMFuzzerTestOneInput_72(alignedData, remainSize);
            break;
        case 41:
            result = LLVMFuzzerTestOneInput_73(alignedData, remainSize);
            break;
        case 42:
            result = LLVMFuzzerTestOneInput_74(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_75(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_76(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_77(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_79(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_80(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_84(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_85(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_89(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_90(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_93(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_94(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_95(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_97(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_98(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_102(alignedData, remainSize);
            break;
        case 58:
            result = LLVMFuzzerTestOneInput_105(alignedData, remainSize);
            break;
        case 59:
            result = LLVMFuzzerTestOneInput_106(alignedData, remainSize);
            break;
        case 60:
            result = LLVMFuzzerTestOneInput_107(alignedData, remainSize);
            break;
        case 61:
            result = LLVMFuzzerTestOneInput_108(alignedData, remainSize);
            break;
        case 62:
            result = LLVMFuzzerTestOneInput_109(alignedData, remainSize);
            break;
        case 63:
            result = LLVMFuzzerTestOneInput_110(alignedData, remainSize);
            break;
        case 64:
            result = LLVMFuzzerTestOneInput_111(alignedData, remainSize);
            break;
        case 65:
            result = LLVMFuzzerTestOneInput_112(alignedData, remainSize);
            break;
        case 66:
            result = LLVMFuzzerTestOneInput_113(alignedData, remainSize);
            break;
        case 67:
            result = LLVMFuzzerTestOneInput_115(alignedData, remainSize);
            break;
        case 68:
            result = LLVMFuzzerTestOneInput_116(alignedData, remainSize);
            break;
        case 69:
            result = LLVMFuzzerTestOneInput_118(alignedData, remainSize);
            break;
        case 70:
            result = LLVMFuzzerTestOneInput_122(alignedData, remainSize);
            break;
        case 71:
            result = LLVMFuzzerTestOneInput_123(alignedData, remainSize);
            break;
        case 72:
            result = LLVMFuzzerTestOneInput_124(alignedData, remainSize);
            break;
        case 73:
            result = LLVMFuzzerTestOneInput_129(alignedData, remainSize);
            break;
        case 74:
            result = LLVMFuzzerTestOneInput_130(alignedData, remainSize);
            break;
        case 75:
            result = LLVMFuzzerTestOneInput_131(alignedData, remainSize);
            break;
        case 76:
            result = LLVMFuzzerTestOneInput_133(alignedData, remainSize);
            break;
        case 77:
            result = LLVMFuzzerTestOneInput_134(alignedData, remainSize);
            break;
        case 78:
            result = LLVMFuzzerTestOneInput_135(alignedData, remainSize);
            break;
        case 79:
            result = LLVMFuzzerTestOneInput_136(alignedData, remainSize);
            break;
        case 80:
            result = LLVMFuzzerTestOneInput_137(alignedData, remainSize);
            break;
        case 81:
            result = LLVMFuzzerTestOneInput_138(alignedData, remainSize);
            break;
        case 82:
            result = LLVMFuzzerTestOneInput_141(alignedData, remainSize);
            break;
        case 83:
            result = LLVMFuzzerTestOneInput_142(alignedData, remainSize);
            break;
        case 84:
            result = LLVMFuzzerTestOneInput_145(alignedData, remainSize);
            break;
        case 85:
            result = LLVMFuzzerTestOneInput_146(alignedData, remainSize);
            break;
        case 86:
            result = LLVMFuzzerTestOneInput_150(alignedData, remainSize);
            break;
        case 87:
            result = LLVMFuzzerTestOneInput_153(alignedData, remainSize);
            break;
        case 88:
            result = LLVMFuzzerTestOneInput_154(alignedData, remainSize);
            break;
        case 89:
            result = LLVMFuzzerTestOneInput_157(alignedData, remainSize);
            break;
        case 90:
            result = LLVMFuzzerTestOneInput_164(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
