// This is the entry of 131 fuzz drivers:
// 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 2
// 3, 24, 25, 26, 27, 28, 29, 30, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 4
// 4, 45, 46, 47, 49, 50, 51, 53, 54, 55, 56, 57, 58, 59, 60, 61, 63, 65, 67, 68, 7
// 0, 71, 74, 75, 76, 77, 79, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 9
// 5, 98, 100, 102, 103, 104, 105, 106, 108, 109, 110, 111, 112, 114, 116, 117, 118
// , 119, 123, 124, 125, 126, 127, 128, 129, 131, 133, 135, 136, 137, 138, 139, 140
// , 141, 142, 143, 144, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 159
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 131) {
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
            result = LLVMFuzzerTestOneInput_12(alignedData, remainSize);
            break;
        case 12:
            result = LLVMFuzzerTestOneInput_13(alignedData, remainSize);
            break;
        case 13:
            result = LLVMFuzzerTestOneInput_14(alignedData, remainSize);
            break;
        case 14:
            result = LLVMFuzzerTestOneInput_15(alignedData, remainSize);
            break;
        case 15:
            result = LLVMFuzzerTestOneInput_16(alignedData, remainSize);
            break;
        case 16:
            result = LLVMFuzzerTestOneInput_17(alignedData, remainSize);
            break;
        case 17:
            result = LLVMFuzzerTestOneInput_18(alignedData, remainSize);
            break;
        case 18:
            result = LLVMFuzzerTestOneInput_19(alignedData, remainSize);
            break;
        case 19:
            result = LLVMFuzzerTestOneInput_20(alignedData, remainSize);
            break;
        case 20:
            result = LLVMFuzzerTestOneInput_21(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_22(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_23(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_24(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_25(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_26(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_27(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_28(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_29(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_30(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_32(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_33(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_34(alignedData, remainSize);
            break;
        case 33:
            result = LLVMFuzzerTestOneInput_35(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_36(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_37(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_38(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_39(alignedData, remainSize);
            break;
        case 38:
            result = LLVMFuzzerTestOneInput_40(alignedData, remainSize);
            break;
        case 39:
            result = LLVMFuzzerTestOneInput_41(alignedData, remainSize);
            break;
        case 40:
            result = LLVMFuzzerTestOneInput_42(alignedData, remainSize);
            break;
        case 41:
            result = LLVMFuzzerTestOneInput_43(alignedData, remainSize);
            break;
        case 42:
            result = LLVMFuzzerTestOneInput_44(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_45(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_46(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_47(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_49(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_50(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_51(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_53(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_54(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_55(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_56(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_57(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_58(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_59(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_60(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_61(alignedData, remainSize);
            break;
        case 58:
            result = LLVMFuzzerTestOneInput_63(alignedData, remainSize);
            break;
        case 59:
            result = LLVMFuzzerTestOneInput_65(alignedData, remainSize);
            break;
        case 60:
            result = LLVMFuzzerTestOneInput_67(alignedData, remainSize);
            break;
        case 61:
            result = LLVMFuzzerTestOneInput_68(alignedData, remainSize);
            break;
        case 62:
            result = LLVMFuzzerTestOneInput_70(alignedData, remainSize);
            break;
        case 63:
            result = LLVMFuzzerTestOneInput_71(alignedData, remainSize);
            break;
        case 64:
            result = LLVMFuzzerTestOneInput_74(alignedData, remainSize);
            break;
        case 65:
            result = LLVMFuzzerTestOneInput_75(alignedData, remainSize);
            break;
        case 66:
            result = LLVMFuzzerTestOneInput_76(alignedData, remainSize);
            break;
        case 67:
            result = LLVMFuzzerTestOneInput_77(alignedData, remainSize);
            break;
        case 68:
            result = LLVMFuzzerTestOneInput_79(alignedData, remainSize);
            break;
        case 69:
            result = LLVMFuzzerTestOneInput_81(alignedData, remainSize);
            break;
        case 70:
            result = LLVMFuzzerTestOneInput_82(alignedData, remainSize);
            break;
        case 71:
            result = LLVMFuzzerTestOneInput_83(alignedData, remainSize);
            break;
        case 72:
            result = LLVMFuzzerTestOneInput_84(alignedData, remainSize);
            break;
        case 73:
            result = LLVMFuzzerTestOneInput_85(alignedData, remainSize);
            break;
        case 74:
            result = LLVMFuzzerTestOneInput_86(alignedData, remainSize);
            break;
        case 75:
            result = LLVMFuzzerTestOneInput_87(alignedData, remainSize);
            break;
        case 76:
            result = LLVMFuzzerTestOneInput_88(alignedData, remainSize);
            break;
        case 77:
            result = LLVMFuzzerTestOneInput_89(alignedData, remainSize);
            break;
        case 78:
            result = LLVMFuzzerTestOneInput_90(alignedData, remainSize);
            break;
        case 79:
            result = LLVMFuzzerTestOneInput_91(alignedData, remainSize);
            break;
        case 80:
            result = LLVMFuzzerTestOneInput_92(alignedData, remainSize);
            break;
        case 81:
            result = LLVMFuzzerTestOneInput_93(alignedData, remainSize);
            break;
        case 82:
            result = LLVMFuzzerTestOneInput_95(alignedData, remainSize);
            break;
        case 83:
            result = LLVMFuzzerTestOneInput_98(alignedData, remainSize);
            break;
        case 84:
            result = LLVMFuzzerTestOneInput_100(alignedData, remainSize);
            break;
        case 85:
            result = LLVMFuzzerTestOneInput_102(alignedData, remainSize);
            break;
        case 86:
            result = LLVMFuzzerTestOneInput_103(alignedData, remainSize);
            break;
        case 87:
            result = LLVMFuzzerTestOneInput_104(alignedData, remainSize);
            break;
        case 88:
            result = LLVMFuzzerTestOneInput_105(alignedData, remainSize);
            break;
        case 89:
            result = LLVMFuzzerTestOneInput_106(alignedData, remainSize);
            break;
        case 90:
            result = LLVMFuzzerTestOneInput_108(alignedData, remainSize);
            break;
        case 91:
            result = LLVMFuzzerTestOneInput_109(alignedData, remainSize);
            break;
        case 92:
            result = LLVMFuzzerTestOneInput_110(alignedData, remainSize);
            break;
        case 93:
            result = LLVMFuzzerTestOneInput_111(alignedData, remainSize);
            break;
        case 94:
            result = LLVMFuzzerTestOneInput_112(alignedData, remainSize);
            break;
        case 95:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_123(alignedData, remainSize);
            break;
        case 101:
            result = LLVMFuzzerTestOneInput_124(alignedData, remainSize);
            break;
        case 102:
            result = LLVMFuzzerTestOneInput_125(alignedData, remainSize);
            break;
        case 103:
            result = LLVMFuzzerTestOneInput_126(alignedData, remainSize);
            break;
        case 104:
            result = LLVMFuzzerTestOneInput_127(alignedData, remainSize);
            break;
        case 105:
            result = LLVMFuzzerTestOneInput_128(alignedData, remainSize);
            break;
        case 106:
            result = LLVMFuzzerTestOneInput_129(alignedData, remainSize);
            break;
        case 107:
            result = LLVMFuzzerTestOneInput_131(alignedData, remainSize);
            break;
        case 108:
            result = LLVMFuzzerTestOneInput_133(alignedData, remainSize);
            break;
        case 109:
            result = LLVMFuzzerTestOneInput_135(alignedData, remainSize);
            break;
        case 110:
            result = LLVMFuzzerTestOneInput_136(alignedData, remainSize);
            break;
        case 111:
            result = LLVMFuzzerTestOneInput_137(alignedData, remainSize);
            break;
        case 112:
            result = LLVMFuzzerTestOneInput_138(alignedData, remainSize);
            break;
        case 113:
            result = LLVMFuzzerTestOneInput_139(alignedData, remainSize);
            break;
        case 114:
            result = LLVMFuzzerTestOneInput_140(alignedData, remainSize);
            break;
        case 115:
            result = LLVMFuzzerTestOneInput_141(alignedData, remainSize);
            break;
        case 116:
            result = LLVMFuzzerTestOneInput_142(alignedData, remainSize);
            break;
        case 117:
            result = LLVMFuzzerTestOneInput_143(alignedData, remainSize);
            break;
        case 118:
            result = LLVMFuzzerTestOneInput_144(alignedData, remainSize);
            break;
        case 119:
            result = LLVMFuzzerTestOneInput_146(alignedData, remainSize);
            break;
        case 120:
            result = LLVMFuzzerTestOneInput_147(alignedData, remainSize);
            break;
        case 121:
            result = LLVMFuzzerTestOneInput_148(alignedData, remainSize);
            break;
        case 122:
            result = LLVMFuzzerTestOneInput_149(alignedData, remainSize);
            break;
        case 123:
            result = LLVMFuzzerTestOneInput_150(alignedData, remainSize);
            break;
        case 124:
            result = LLVMFuzzerTestOneInput_151(alignedData, remainSize);
            break;
        case 125:
            result = LLVMFuzzerTestOneInput_152(alignedData, remainSize);
            break;
        case 126:
            result = LLVMFuzzerTestOneInput_153(alignedData, remainSize);
            break;
        case 127:
            result = LLVMFuzzerTestOneInput_154(alignedData, remainSize);
            break;
        case 128:
            result = LLVMFuzzerTestOneInput_155(alignedData, remainSize);
            break;
        case 129:
            result = LLVMFuzzerTestOneInput_156(alignedData, remainSize);
            break;
        case 130:
            result = LLVMFuzzerTestOneInput_159(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
