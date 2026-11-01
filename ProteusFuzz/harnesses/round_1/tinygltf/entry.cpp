// This is the entry of 101 fuzz drivers:
// 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 16, 17, 19, 20, 23, 24, 25, 26, 28, 30, 3
// 1, 32, 33, 35, 37, 39, 40, 41, 43, 44, 47, 48, 49, 51, 52, 53, 55, 57, 58, 59, 6
// 0, 61, 62, 63, 65, 68, 71, 72, 73, 76, 77, 78, 81, 82, 83, 84, 85, 86, 88, 92, 9
// 4, 96, 97, 98, 100, 102, 103, 105, 106, 107, 108, 110, 111, 112, 113, 114, 116, 
// 120, 122, 125, 129, 132, 136, 137, 141, 143, 144, 145, 146, 147, 149, 150, 153, 
// 154, 155, 156, 157, 158, 162
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
extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_157(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size);
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
    switch (driverIndex % 101) {
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
            result = LLVMFuzzerTestOneInput_7(alignedData, remainSize);
            break;
        case 7:
            result = LLVMFuzzerTestOneInput_8(alignedData, remainSize);
            break;
        case 8:
            result = LLVMFuzzerTestOneInput_9(alignedData, remainSize);
            break;
        case 9:
            result = LLVMFuzzerTestOneInput_10(alignedData, remainSize);
            break;
        case 10:
            result = LLVMFuzzerTestOneInput_11(alignedData, remainSize);
            break;
        case 11:
            result = LLVMFuzzerTestOneInput_13(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_24(alignedData, remainSize);
            break;
        case 18:
            result = LLVMFuzzerTestOneInput_25(alignedData, remainSize);
            break;
        case 19:
            result = LLVMFuzzerTestOneInput_26(alignedData, remainSize);
            break;
        case 20:
            result = LLVMFuzzerTestOneInput_28(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_30(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_31(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_32(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_33(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_35(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_37(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_39(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_40(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_41(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_43(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_44(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_47(alignedData, remainSize);
            break;
        case 33:
            result = LLVMFuzzerTestOneInput_48(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_49(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_51(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_52(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_53(alignedData, remainSize);
            break;
        case 38:
            result = LLVMFuzzerTestOneInput_55(alignedData, remainSize);
            break;
        case 39:
            result = LLVMFuzzerTestOneInput_57(alignedData, remainSize);
            break;
        case 40:
            result = LLVMFuzzerTestOneInput_58(alignedData, remainSize);
            break;
        case 41:
            result = LLVMFuzzerTestOneInput_59(alignedData, remainSize);
            break;
        case 42:
            result = LLVMFuzzerTestOneInput_60(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_61(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_62(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_63(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_65(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_68(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_71(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_72(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_73(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_76(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_77(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_78(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_81(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_82(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_83(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_84(alignedData, remainSize);
            break;
        case 58:
            result = LLVMFuzzerTestOneInput_85(alignedData, remainSize);
            break;
        case 59:
            result = LLVMFuzzerTestOneInput_86(alignedData, remainSize);
            break;
        case 60:
            result = LLVMFuzzerTestOneInput_88(alignedData, remainSize);
            break;
        case 61:
            result = LLVMFuzzerTestOneInput_92(alignedData, remainSize);
            break;
        case 62:
            result = LLVMFuzzerTestOneInput_94(alignedData, remainSize);
            break;
        case 63:
            result = LLVMFuzzerTestOneInput_96(alignedData, remainSize);
            break;
        case 64:
            result = LLVMFuzzerTestOneInput_97(alignedData, remainSize);
            break;
        case 65:
            result = LLVMFuzzerTestOneInput_98(alignedData, remainSize);
            break;
        case 66:
            result = LLVMFuzzerTestOneInput_100(alignedData, remainSize);
            break;
        case 67:
            result = LLVMFuzzerTestOneInput_102(alignedData, remainSize);
            break;
        case 68:
            result = LLVMFuzzerTestOneInput_103(alignedData, remainSize);
            break;
        case 69:
            result = LLVMFuzzerTestOneInput_105(alignedData, remainSize);
            break;
        case 70:
            result = LLVMFuzzerTestOneInput_106(alignedData, remainSize);
            break;
        case 71:
            result = LLVMFuzzerTestOneInput_107(alignedData, remainSize);
            break;
        case 72:
            result = LLVMFuzzerTestOneInput_108(alignedData, remainSize);
            break;
        case 73:
            result = LLVMFuzzerTestOneInput_110(alignedData, remainSize);
            break;
        case 74:
            result = LLVMFuzzerTestOneInput_111(alignedData, remainSize);
            break;
        case 75:
            result = LLVMFuzzerTestOneInput_112(alignedData, remainSize);
            break;
        case 76:
            result = LLVMFuzzerTestOneInput_113(alignedData, remainSize);
            break;
        case 77:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
            break;
        case 78:
            result = LLVMFuzzerTestOneInput_116(alignedData, remainSize);
            break;
        case 79:
            result = LLVMFuzzerTestOneInput_120(alignedData, remainSize);
            break;
        case 80:
            result = LLVMFuzzerTestOneInput_122(alignedData, remainSize);
            break;
        case 81:
            result = LLVMFuzzerTestOneInput_125(alignedData, remainSize);
            break;
        case 82:
            result = LLVMFuzzerTestOneInput_129(alignedData, remainSize);
            break;
        case 83:
            result = LLVMFuzzerTestOneInput_132(alignedData, remainSize);
            break;
        case 84:
            result = LLVMFuzzerTestOneInput_136(alignedData, remainSize);
            break;
        case 85:
            result = LLVMFuzzerTestOneInput_137(alignedData, remainSize);
            break;
        case 86:
            result = LLVMFuzzerTestOneInput_141(alignedData, remainSize);
            break;
        case 87:
            result = LLVMFuzzerTestOneInput_143(alignedData, remainSize);
            break;
        case 88:
            result = LLVMFuzzerTestOneInput_144(alignedData, remainSize);
            break;
        case 89:
            result = LLVMFuzzerTestOneInput_145(alignedData, remainSize);
            break;
        case 90:
            result = LLVMFuzzerTestOneInput_146(alignedData, remainSize);
            break;
        case 91:
            result = LLVMFuzzerTestOneInput_147(alignedData, remainSize);
            break;
        case 92:
            result = LLVMFuzzerTestOneInput_149(alignedData, remainSize);
            break;
        case 93:
            result = LLVMFuzzerTestOneInput_150(alignedData, remainSize);
            break;
        case 94:
            result = LLVMFuzzerTestOneInput_153(alignedData, remainSize);
            break;
        case 95:
            result = LLVMFuzzerTestOneInput_154(alignedData, remainSize);
            break;
        case 96:
            result = LLVMFuzzerTestOneInput_155(alignedData, remainSize);
            break;
        case 97:
            result = LLVMFuzzerTestOneInput_156(alignedData, remainSize);
            break;
        case 98:
            result = LLVMFuzzerTestOneInput_157(alignedData, remainSize);
            break;
        case 99:
            result = LLVMFuzzerTestOneInput_158(alignedData, remainSize);
            break;
        case 100:
            result = LLVMFuzzerTestOneInput_162(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
