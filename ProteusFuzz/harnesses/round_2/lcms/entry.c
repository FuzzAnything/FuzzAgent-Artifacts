// This is the entry of 123 fuzz drivers:
// 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 23, 24, 
// 25, 26, 28, 33, 36, 38, 41, 45, 46, 47, 49, 50, 51, 52, 54, 55, 56, 58, 59, 61, 
// 63, 64, 65, 67, 68, 69, 70, 71, 72, 74, 78, 82, 83, 84, 85, 89, 93, 94, 95, 97, 
// 98, 99, 102, 104, 106, 107, 110, 112, 113, 114, 115, 117, 118, 119, 120, 121, 12
// 2, 123, 124, 125, 126, 127, 128, 130, 131, 133, 134, 135, 137, 138, 139, 140, 14
// 1, 144, 145, 146, 148, 150, 151, 152, 155, 156, 157, 158, 160, 161, 162, 164, 16
// 6, 167, 168, 170, 173, 175, 179, 180, 181
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
int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_157(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_166(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_167(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_168(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_170(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_173(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_175(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_179(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_180(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_181(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_182(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_183(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_184(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_185(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 123) {
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
            result = LLVMFuzzerTestOneInput_8(alignedData, remainSize);
            break;
        case 7:
            result = LLVMFuzzerTestOneInput_9(alignedData, remainSize);
            break;
        case 8:
            result = LLVMFuzzerTestOneInput_10(alignedData, remainSize);
            break;
        case 9:
            result = LLVMFuzzerTestOneInput_11(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_17(alignedData, remainSize);
            break;
        case 16:
            result = LLVMFuzzerTestOneInput_18(alignedData, remainSize);
            break;
        case 17:
            result = LLVMFuzzerTestOneInput_19(alignedData, remainSize);
            break;
        case 18:
            result = LLVMFuzzerTestOneInput_20(alignedData, remainSize);
            break;
        case 19:
            result = LLVMFuzzerTestOneInput_22(alignedData, remainSize);
            break;
        case 20:
            result = LLVMFuzzerTestOneInput_23(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_24(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_25(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_26(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_28(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_33(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_36(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_38(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_41(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_45(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_46(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_47(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_49(alignedData, remainSize);
            break;
        case 33:
            result = LLVMFuzzerTestOneInput_50(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_51(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_52(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_54(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_55(alignedData, remainSize);
            break;
        case 38:
            result = LLVMFuzzerTestOneInput_56(alignedData, remainSize);
            break;
        case 39:
            result = LLVMFuzzerTestOneInput_58(alignedData, remainSize);
            break;
        case 40:
            result = LLVMFuzzerTestOneInput_59(alignedData, remainSize);
            break;
        case 41:
            result = LLVMFuzzerTestOneInput_61(alignedData, remainSize);
            break;
        case 42:
            result = LLVMFuzzerTestOneInput_63(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_64(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_65(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_67(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_68(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_69(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_70(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_71(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_72(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_74(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_78(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_82(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_83(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_84(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_85(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_89(alignedData, remainSize);
            break;
        case 58:
            result = LLVMFuzzerTestOneInput_93(alignedData, remainSize);
            break;
        case 59:
            result = LLVMFuzzerTestOneInput_94(alignedData, remainSize);
            break;
        case 60:
            result = LLVMFuzzerTestOneInput_95(alignedData, remainSize);
            break;
        case 61:
            result = LLVMFuzzerTestOneInput_97(alignedData, remainSize);
            break;
        case 62:
            result = LLVMFuzzerTestOneInput_98(alignedData, remainSize);
            break;
        case 63:
            result = LLVMFuzzerTestOneInput_99(alignedData, remainSize);
            break;
        case 64:
            result = LLVMFuzzerTestOneInput_102(alignedData, remainSize);
            break;
        case 65:
            result = LLVMFuzzerTestOneInput_104(alignedData, remainSize);
            break;
        case 66:
            result = LLVMFuzzerTestOneInput_106(alignedData, remainSize);
            break;
        case 67:
            result = LLVMFuzzerTestOneInput_107(alignedData, remainSize);
            break;
        case 68:
            result = LLVMFuzzerTestOneInput_110(alignedData, remainSize);
            break;
        case 69:
            result = LLVMFuzzerTestOneInput_112(alignedData, remainSize);
            break;
        case 70:
            result = LLVMFuzzerTestOneInput_113(alignedData, remainSize);
            break;
        case 71:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
            break;
        case 72:
            result = LLVMFuzzerTestOneInput_115(alignedData, remainSize);
            break;
        case 73:
            result = LLVMFuzzerTestOneInput_117(alignedData, remainSize);
            break;
        case 74:
            result = LLVMFuzzerTestOneInput_118(alignedData, remainSize);
            break;
        case 75:
            result = LLVMFuzzerTestOneInput_119(alignedData, remainSize);
            break;
        case 76:
            result = LLVMFuzzerTestOneInput_120(alignedData, remainSize);
            break;
        case 77:
            result = LLVMFuzzerTestOneInput_121(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_130(alignedData, remainSize);
            break;
        case 86:
            result = LLVMFuzzerTestOneInput_131(alignedData, remainSize);
            break;
        case 87:
            result = LLVMFuzzerTestOneInput_133(alignedData, remainSize);
            break;
        case 88:
            result = LLVMFuzzerTestOneInput_134(alignedData, remainSize);
            break;
        case 89:
            result = LLVMFuzzerTestOneInput_135(alignedData, remainSize);
            break;
        case 90:
            result = LLVMFuzzerTestOneInput_137(alignedData, remainSize);
            break;
        case 91:
            result = LLVMFuzzerTestOneInput_138(alignedData, remainSize);
            break;
        case 92:
            result = LLVMFuzzerTestOneInput_139(alignedData, remainSize);
            break;
        case 93:
            result = LLVMFuzzerTestOneInput_140(alignedData, remainSize);
            break;
        case 94:
            result = LLVMFuzzerTestOneInput_141(alignedData, remainSize);
            break;
        case 95:
            result = LLVMFuzzerTestOneInput_144(alignedData, remainSize);
            break;
        case 96:
            result = LLVMFuzzerTestOneInput_145(alignedData, remainSize);
            break;
        case 97:
            result = LLVMFuzzerTestOneInput_146(alignedData, remainSize);
            break;
        case 98:
            result = LLVMFuzzerTestOneInput_148(alignedData, remainSize);
            break;
        case 99:
            result = LLVMFuzzerTestOneInput_150(alignedData, remainSize);
            break;
        case 100:
            result = LLVMFuzzerTestOneInput_151(alignedData, remainSize);
            break;
        case 101:
            result = LLVMFuzzerTestOneInput_152(alignedData, remainSize);
            break;
        case 102:
            result = LLVMFuzzerTestOneInput_155(alignedData, remainSize);
            break;
        case 103:
            result = LLVMFuzzerTestOneInput_156(alignedData, remainSize);
            break;
        case 104:
            result = LLVMFuzzerTestOneInput_157(alignedData, remainSize);
            break;
        case 105:
            result = LLVMFuzzerTestOneInput_158(alignedData, remainSize);
            break;
        case 106:
            result = LLVMFuzzerTestOneInput_160(alignedData, remainSize);
            break;
        case 107:
            result = LLVMFuzzerTestOneInput_161(alignedData, remainSize);
            break;
        case 108:
            result = LLVMFuzzerTestOneInput_162(alignedData, remainSize);
            break;
        case 109:
            result = LLVMFuzzerTestOneInput_164(alignedData, remainSize);
            break;
        case 110:
            result = LLVMFuzzerTestOneInput_166(alignedData, remainSize);
            break;
        case 111:
            result = LLVMFuzzerTestOneInput_167(alignedData, remainSize);
            break;
        case 112:
            result = LLVMFuzzerTestOneInput_168(alignedData, remainSize);
            break;
        case 113:
            result = LLVMFuzzerTestOneInput_170(alignedData, remainSize);
            break;
        case 114:
            result = LLVMFuzzerTestOneInput_173(alignedData, remainSize);
            break;
        case 115:
            result = LLVMFuzzerTestOneInput_175(alignedData, remainSize);
            break;
        case 116:
            result = LLVMFuzzerTestOneInput_179(alignedData, remainSize);
            break;
        case 117:
            result = LLVMFuzzerTestOneInput_180(alignedData, remainSize);
            break;
        case 118:
            result = LLVMFuzzerTestOneInput_181(alignedData, remainSize);
            break;
        case 119:
            result = LLVMFuzzerTestOneInput_182(alignedData, remainSize);
            break;
        case 120:
            result = LLVMFuzzerTestOneInput_183(alignedData, remainSize);
            break;
        case 121:
            result = LLVMFuzzerTestOneInput_184(alignedData, remainSize);
            break;
        case 122:
            result = LLVMFuzzerTestOneInput_185(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
