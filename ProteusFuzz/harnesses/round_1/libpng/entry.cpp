// This is the entry of 106 fuzz drivers:
// 1, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
//  28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 40, 41, 42, 43, 44, 45, 46, 47, 48,
//  50, 51, 53, 54, 55, 56, 61, 62, 63, 64, 65, 66, 67, 69, 70, 71, 72, 73, 74, 75,
//  76, 77, 78, 79, 80, 81, 82, 84, 85, 86, 87, 89, 90, 91, 92, 93, 94, 95, 96, 98,
//  99, 101, 102, 103, 104, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 
// 117, 118, 119, 120, 121, 122, 123, 125, 126
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size);
extern "C" int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 106) {
        case 0:
            result = LLVMFuzzerTestOneInput_1(alignedData, remainSize);
            break;
        case 1:
            result = LLVMFuzzerTestOneInput_8(alignedData, remainSize);
            break;
        case 2:
            result = LLVMFuzzerTestOneInput_9(alignedData, remainSize);
            break;
        case 3:
            result = LLVMFuzzerTestOneInput_10(alignedData, remainSize);
            break;
        case 4:
            result = LLVMFuzzerTestOneInput_11(alignedData, remainSize);
            break;
        case 5:
            result = LLVMFuzzerTestOneInput_12(alignedData, remainSize);
            break;
        case 6:
            result = LLVMFuzzerTestOneInput_13(alignedData, remainSize);
            break;
        case 7:
            result = LLVMFuzzerTestOneInput_14(alignedData, remainSize);
            break;
        case 8:
            result = LLVMFuzzerTestOneInput_15(alignedData, remainSize);
            break;
        case 9:
            result = LLVMFuzzerTestOneInput_16(alignedData, remainSize);
            break;
        case 10:
            result = LLVMFuzzerTestOneInput_17(alignedData, remainSize);
            break;
        case 11:
            result = LLVMFuzzerTestOneInput_18(alignedData, remainSize);
            break;
        case 12:
            result = LLVMFuzzerTestOneInput_19(alignedData, remainSize);
            break;
        case 13:
            result = LLVMFuzzerTestOneInput_20(alignedData, remainSize);
            break;
        case 14:
            result = LLVMFuzzerTestOneInput_21(alignedData, remainSize);
            break;
        case 15:
            result = LLVMFuzzerTestOneInput_22(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_27(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_28(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_29(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_30(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_31(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_32(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_33(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_34(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_35(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_36(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_37(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_38(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_40(alignedData, remainSize);
            break;
        case 33:
            result = LLVMFuzzerTestOneInput_41(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_42(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_43(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_44(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_45(alignedData, remainSize);
            break;
        case 38:
            result = LLVMFuzzerTestOneInput_46(alignedData, remainSize);
            break;
        case 39:
            result = LLVMFuzzerTestOneInput_47(alignedData, remainSize);
            break;
        case 40:
            result = LLVMFuzzerTestOneInput_48(alignedData, remainSize);
            break;
        case 41:
            result = LLVMFuzzerTestOneInput_50(alignedData, remainSize);
            break;
        case 42:
            result = LLVMFuzzerTestOneInput_51(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_53(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_54(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_55(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_56(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_61(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_62(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_63(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_64(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_65(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_66(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_67(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_69(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_70(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_71(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_72(alignedData, remainSize);
            break;
        case 58:
            result = LLVMFuzzerTestOneInput_73(alignedData, remainSize);
            break;
        case 59:
            result = LLVMFuzzerTestOneInput_74(alignedData, remainSize);
            break;
        case 60:
            result = LLVMFuzzerTestOneInput_75(alignedData, remainSize);
            break;
        case 61:
            result = LLVMFuzzerTestOneInput_76(alignedData, remainSize);
            break;
        case 62:
            result = LLVMFuzzerTestOneInput_77(alignedData, remainSize);
            break;
        case 63:
            result = LLVMFuzzerTestOneInput_78(alignedData, remainSize);
            break;
        case 64:
            result = LLVMFuzzerTestOneInput_79(alignedData, remainSize);
            break;
        case 65:
            result = LLVMFuzzerTestOneInput_80(alignedData, remainSize);
            break;
        case 66:
            result = LLVMFuzzerTestOneInput_81(alignedData, remainSize);
            break;
        case 67:
            result = LLVMFuzzerTestOneInput_82(alignedData, remainSize);
            break;
        case 68:
            result = LLVMFuzzerTestOneInput_84(alignedData, remainSize);
            break;
        case 69:
            result = LLVMFuzzerTestOneInput_85(alignedData, remainSize);
            break;
        case 70:
            result = LLVMFuzzerTestOneInput_86(alignedData, remainSize);
            break;
        case 71:
            result = LLVMFuzzerTestOneInput_87(alignedData, remainSize);
            break;
        case 72:
            result = LLVMFuzzerTestOneInput_89(alignedData, remainSize);
            break;
        case 73:
            result = LLVMFuzzerTestOneInput_90(alignedData, remainSize);
            break;
        case 74:
            result = LLVMFuzzerTestOneInput_91(alignedData, remainSize);
            break;
        case 75:
            result = LLVMFuzzerTestOneInput_92(alignedData, remainSize);
            break;
        case 76:
            result = LLVMFuzzerTestOneInput_93(alignedData, remainSize);
            break;
        case 77:
            result = LLVMFuzzerTestOneInput_94(alignedData, remainSize);
            break;
        case 78:
            result = LLVMFuzzerTestOneInput_95(alignedData, remainSize);
            break;
        case 79:
            result = LLVMFuzzerTestOneInput_96(alignedData, remainSize);
            break;
        case 80:
            result = LLVMFuzzerTestOneInput_98(alignedData, remainSize);
            break;
        case 81:
            result = LLVMFuzzerTestOneInput_99(alignedData, remainSize);
            break;
        case 82:
            result = LLVMFuzzerTestOneInput_101(alignedData, remainSize);
            break;
        case 83:
            result = LLVMFuzzerTestOneInput_102(alignedData, remainSize);
            break;
        case 84:
            result = LLVMFuzzerTestOneInput_103(alignedData, remainSize);
            break;
        case 85:
            result = LLVMFuzzerTestOneInput_104(alignedData, remainSize);
            break;
        case 86:
            result = LLVMFuzzerTestOneInput_106(alignedData, remainSize);
            break;
        case 87:
            result = LLVMFuzzerTestOneInput_107(alignedData, remainSize);
            break;
        case 88:
            result = LLVMFuzzerTestOneInput_108(alignedData, remainSize);
            break;
        case 89:
            result = LLVMFuzzerTestOneInput_109(alignedData, remainSize);
            break;
        case 90:
            result = LLVMFuzzerTestOneInput_110(alignedData, remainSize);
            break;
        case 91:
            result = LLVMFuzzerTestOneInput_111(alignedData, remainSize);
            break;
        case 92:
            result = LLVMFuzzerTestOneInput_112(alignedData, remainSize);
            break;
        case 93:
            result = LLVMFuzzerTestOneInput_113(alignedData, remainSize);
            break;
        case 94:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
            break;
        case 95:
            result = LLVMFuzzerTestOneInput_115(alignedData, remainSize);
            break;
        case 96:
            result = LLVMFuzzerTestOneInput_116(alignedData, remainSize);
            break;
        case 97:
            result = LLVMFuzzerTestOneInput_117(alignedData, remainSize);
            break;
        case 98:
            result = LLVMFuzzerTestOneInput_118(alignedData, remainSize);
            break;
        case 99:
            result = LLVMFuzzerTestOneInput_119(alignedData, remainSize);
            break;
        case 100:
            result = LLVMFuzzerTestOneInput_120(alignedData, remainSize);
            break;
        case 101:
            result = LLVMFuzzerTestOneInput_121(alignedData, remainSize);
            break;
        case 102:
            result = LLVMFuzzerTestOneInput_122(alignedData, remainSize);
            break;
        case 103:
            result = LLVMFuzzerTestOneInput_123(alignedData, remainSize);
            break;
        case 104:
            result = LLVMFuzzerTestOneInput_125(alignedData, remainSize);
            break;
        case 105:
            result = LLVMFuzzerTestOneInput_126(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
