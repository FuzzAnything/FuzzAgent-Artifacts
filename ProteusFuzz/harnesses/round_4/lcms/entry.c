// This is the entry of 114 fuzz drivers:
// 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 18, 19, 20, 22, 23, 24, 25, 2
// 6, 29, 30, 31, 32, 34, 35, 36, 38, 40, 41, 43, 44, 45, 46, 47, 49, 50, 52, 53, 5
// 4, 55, 56, 57, 60, 61, 62, 66, 67, 70, 72, 74, 75, 76, 77, 79, 80, 81, 83, 90, 9
// 1, 92, 93, 96, 99, 103, 104, 106, 109, 110, 113, 114, 115, 116, 118, 120, 122, 1
// 23, 124, 125, 126, 127, 128, 129, 131, 132, 133, 135, 136, 137, 138, 139, 140, 1
// 41, 142, 144, 145, 146, 147, 148, 149, 152, 153, 155, 156, 158
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
int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 114) {
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
            result = LLVMFuzzerTestOneInput_12(alignedData, remainSize);
            break;
        case 11:
            result = LLVMFuzzerTestOneInput_13(alignedData, remainSize);
            break;
        case 12:
            result = LLVMFuzzerTestOneInput_14(alignedData, remainSize);
            break;
        case 13:
            result = LLVMFuzzerTestOneInput_15(alignedData, remainSize);
            break;
        case 14:
            result = LLVMFuzzerTestOneInput_16(alignedData, remainSize);
            break;
        case 15:
            result = LLVMFuzzerTestOneInput_18(alignedData, remainSize);
            break;
        case 16:
            result = LLVMFuzzerTestOneInput_19(alignedData, remainSize);
            break;
        case 17:
            result = LLVMFuzzerTestOneInput_20(alignedData, remainSize);
            break;
        case 18:
            result = LLVMFuzzerTestOneInput_22(alignedData, remainSize);
            break;
        case 19:
            result = LLVMFuzzerTestOneInput_23(alignedData, remainSize);
            break;
        case 20:
            result = LLVMFuzzerTestOneInput_24(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_25(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_26(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_29(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_30(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_31(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_32(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_38(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_40(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_41(alignedData, remainSize);
            break;
        case 33:
            result = LLVMFuzzerTestOneInput_43(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_44(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_45(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_46(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_47(alignedData, remainSize);
            break;
        case 38:
            result = LLVMFuzzerTestOneInput_49(alignedData, remainSize);
            break;
        case 39:
            result = LLVMFuzzerTestOneInput_50(alignedData, remainSize);
            break;
        case 40:
            result = LLVMFuzzerTestOneInput_52(alignedData, remainSize);
            break;
        case 41:
            result = LLVMFuzzerTestOneInput_53(alignedData, remainSize);
            break;
        case 42:
            result = LLVMFuzzerTestOneInput_54(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_55(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_56(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_57(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_60(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_61(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_62(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_66(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_67(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_70(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_72(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_74(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_75(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_76(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_77(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_79(alignedData, remainSize);
            break;
        case 58:
            result = LLVMFuzzerTestOneInput_80(alignedData, remainSize);
            break;
        case 59:
            result = LLVMFuzzerTestOneInput_81(alignedData, remainSize);
            break;
        case 60:
            result = LLVMFuzzerTestOneInput_83(alignedData, remainSize);
            break;
        case 61:
            result = LLVMFuzzerTestOneInput_90(alignedData, remainSize);
            break;
        case 62:
            result = LLVMFuzzerTestOneInput_91(alignedData, remainSize);
            break;
        case 63:
            result = LLVMFuzzerTestOneInput_92(alignedData, remainSize);
            break;
        case 64:
            result = LLVMFuzzerTestOneInput_93(alignedData, remainSize);
            break;
        case 65:
            result = LLVMFuzzerTestOneInput_96(alignedData, remainSize);
            break;
        case 66:
            result = LLVMFuzzerTestOneInput_99(alignedData, remainSize);
            break;
        case 67:
            result = LLVMFuzzerTestOneInput_103(alignedData, remainSize);
            break;
        case 68:
            result = LLVMFuzzerTestOneInput_104(alignedData, remainSize);
            break;
        case 69:
            result = LLVMFuzzerTestOneInput_106(alignedData, remainSize);
            break;
        case 70:
            result = LLVMFuzzerTestOneInput_109(alignedData, remainSize);
            break;
        case 71:
            result = LLVMFuzzerTestOneInput_110(alignedData, remainSize);
            break;
        case 72:
            result = LLVMFuzzerTestOneInput_113(alignedData, remainSize);
            break;
        case 73:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
            break;
        case 74:
            result = LLVMFuzzerTestOneInput_115(alignedData, remainSize);
            break;
        case 75:
            result = LLVMFuzzerTestOneInput_116(alignedData, remainSize);
            break;
        case 76:
            result = LLVMFuzzerTestOneInput_118(alignedData, remainSize);
            break;
        case 77:
            result = LLVMFuzzerTestOneInput_120(alignedData, remainSize);
            break;
        case 78:
            result = LLVMFuzzerTestOneInput_122(alignedData, remainSize);
            break;
        case 79:
            result = LLVMFuzzerTestOneInput_123(alignedData, remainSize);
            break;
        case 80:
            result = LLVMFuzzerTestOneInput_124(alignedData, remainSize);
            break;
        case 81:
            result = LLVMFuzzerTestOneInput_125(alignedData, remainSize);
            break;
        case 82:
            result = LLVMFuzzerTestOneInput_126(alignedData, remainSize);
            break;
        case 83:
            result = LLVMFuzzerTestOneInput_127(alignedData, remainSize);
            break;
        case 84:
            result = LLVMFuzzerTestOneInput_128(alignedData, remainSize);
            break;
        case 85:
            result = LLVMFuzzerTestOneInput_129(alignedData, remainSize);
            break;
        case 86:
            result = LLVMFuzzerTestOneInput_131(alignedData, remainSize);
            break;
        case 87:
            result = LLVMFuzzerTestOneInput_132(alignedData, remainSize);
            break;
        case 88:
            result = LLVMFuzzerTestOneInput_133(alignedData, remainSize);
            break;
        case 89:
            result = LLVMFuzzerTestOneInput_135(alignedData, remainSize);
            break;
        case 90:
            result = LLVMFuzzerTestOneInput_136(alignedData, remainSize);
            break;
        case 91:
            result = LLVMFuzzerTestOneInput_137(alignedData, remainSize);
            break;
        case 92:
            result = LLVMFuzzerTestOneInput_138(alignedData, remainSize);
            break;
        case 93:
            result = LLVMFuzzerTestOneInput_139(alignedData, remainSize);
            break;
        case 94:
            result = LLVMFuzzerTestOneInput_140(alignedData, remainSize);
            break;
        case 95:
            result = LLVMFuzzerTestOneInput_141(alignedData, remainSize);
            break;
        case 96:
            result = LLVMFuzzerTestOneInput_142(alignedData, remainSize);
            break;
        case 97:
            result = LLVMFuzzerTestOneInput_144(alignedData, remainSize);
            break;
        case 98:
            result = LLVMFuzzerTestOneInput_145(alignedData, remainSize);
            break;
        case 99:
            result = LLVMFuzzerTestOneInput_146(alignedData, remainSize);
            break;
        case 100:
            result = LLVMFuzzerTestOneInput_147(alignedData, remainSize);
            break;
        case 101:
            result = LLVMFuzzerTestOneInput_148(alignedData, remainSize);
            break;
        case 102:
            result = LLVMFuzzerTestOneInput_149(alignedData, remainSize);
            break;
        case 103:
            result = LLVMFuzzerTestOneInput_152(alignedData, remainSize);
            break;
        case 104:
            result = LLVMFuzzerTestOneInput_153(alignedData, remainSize);
            break;
        case 105:
            result = LLVMFuzzerTestOneInput_155(alignedData, remainSize);
            break;
        case 106:
            result = LLVMFuzzerTestOneInput_156(alignedData, remainSize);
            break;
        case 107:
            result = LLVMFuzzerTestOneInput_158(alignedData, remainSize);
            break;
        case 108:
            result = LLVMFuzzerTestOneInput_159(alignedData, remainSize);
            break;
        case 109:
            result = LLVMFuzzerTestOneInput_160(alignedData, remainSize);
            break;
        case 110:
            result = LLVMFuzzerTestOneInput_161(alignedData, remainSize);
            break;
        case 111:
            result = LLVMFuzzerTestOneInput_162(alignedData, remainSize);
            break;
        case 112:
            result = LLVMFuzzerTestOneInput_163(alignedData, remainSize);
            break;
        case 113:
            result = LLVMFuzzerTestOneInput_164(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
