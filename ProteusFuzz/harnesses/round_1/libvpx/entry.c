// This is the entry of 81 fuzz drivers:
// 1, 3, 6, 8, 13, 14, 15, 16, 17, 18, 19, 20, 21, 24, 26, 27, 28, 30, 37, 39, 42, 
// 43, 45, 47, 48, 49, 50, 53, 57, 60, 63, 70, 72, 73, 75, 86, 87, 94, 95, 96, 98, 
// 99, 100, 101, 104, 105, 106, 107, 108, 112, 114, 117, 118, 120, 121, 122, 125, 1
// 27, 129, 131, 133, 134, 135, 137, 138, 139, 145, 146, 148, 149, 150, 153, 156, 1
// 58, 159, 160, 161, 163, 164
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_165(const uint8_t *Data, size_t Size);

int LLVMFuzzerTestOneInput_166(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 81) {
        case 0:
            result = LLVMFuzzerTestOneInput_1(alignedData, remainSize);
            break;
        case 1:
            result = LLVMFuzzerTestOneInput_3(alignedData, remainSize);
            break;
        case 2:
            result = LLVMFuzzerTestOneInput_6(alignedData, remainSize);
            break;
        case 3:
            result = LLVMFuzzerTestOneInput_8(alignedData, remainSize);
            break;
        case 4:
            result = LLVMFuzzerTestOneInput_13(alignedData, remainSize);
            break;
        case 5:
            result = LLVMFuzzerTestOneInput_14(alignedData, remainSize);
            break;
        case 6:
            result = LLVMFuzzerTestOneInput_15(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_19(alignedData, remainSize);
            break;
        case 11:
            result = LLVMFuzzerTestOneInput_20(alignedData, remainSize);
            break;
        case 12:
            result = LLVMFuzzerTestOneInput_21(alignedData, remainSize);
            break;
        case 13:
            result = LLVMFuzzerTestOneInput_24(alignedData, remainSize);
            break;
        case 14:
            result = LLVMFuzzerTestOneInput_26(alignedData, remainSize);
            break;
        case 15:
            result = LLVMFuzzerTestOneInput_27(alignedData, remainSize);
            break;
        case 16:
            result = LLVMFuzzerTestOneInput_28(alignedData, remainSize);
            break;
        case 17:
            result = LLVMFuzzerTestOneInput_30(alignedData, remainSize);
            break;
        case 18:
            result = LLVMFuzzerTestOneInput_37(alignedData, remainSize);
            break;
        case 19:
            result = LLVMFuzzerTestOneInput_39(alignedData, remainSize);
            break;
        case 20:
            result = LLVMFuzzerTestOneInput_42(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_43(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_45(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_47(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_48(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_49(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_50(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_53(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_57(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_60(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_63(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_70(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_72(alignedData, remainSize);
            break;
        case 33:
            result = LLVMFuzzerTestOneInput_73(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_75(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_86(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_87(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_94(alignedData, remainSize);
            break;
        case 38:
            result = LLVMFuzzerTestOneInput_95(alignedData, remainSize);
            break;
        case 39:
            result = LLVMFuzzerTestOneInput_96(alignedData, remainSize);
            break;
        case 40:
            result = LLVMFuzzerTestOneInput_98(alignedData, remainSize);
            break;
        case 41:
            result = LLVMFuzzerTestOneInput_99(alignedData, remainSize);
            break;
        case 42:
            result = LLVMFuzzerTestOneInput_100(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_101(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_104(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_105(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_106(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_107(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_108(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_112(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_117(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_118(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_120(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_121(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_122(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_125(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_127(alignedData, remainSize);
            break;
        case 58:
            result = LLVMFuzzerTestOneInput_129(alignedData, remainSize);
            break;
        case 59:
            result = LLVMFuzzerTestOneInput_131(alignedData, remainSize);
            break;
        case 60:
            result = LLVMFuzzerTestOneInput_133(alignedData, remainSize);
            break;
        case 61:
            result = LLVMFuzzerTestOneInput_134(alignedData, remainSize);
            break;
        case 62:
            result = LLVMFuzzerTestOneInput_135(alignedData, remainSize);
            break;
        case 63:
            result = LLVMFuzzerTestOneInput_137(alignedData, remainSize);
            break;
        case 64:
            result = LLVMFuzzerTestOneInput_138(alignedData, remainSize);
            break;
        case 65:
            result = LLVMFuzzerTestOneInput_139(alignedData, remainSize);
            break;
        case 66:
            result = LLVMFuzzerTestOneInput_145(alignedData, remainSize);
            break;
        case 67:
            result = LLVMFuzzerTestOneInput_146(alignedData, remainSize);
            break;
        case 68:
            result = LLVMFuzzerTestOneInput_148(alignedData, remainSize);
            break;
        case 69:
            result = LLVMFuzzerTestOneInput_149(alignedData, remainSize);
            break;
        case 70:
            result = LLVMFuzzerTestOneInput_150(alignedData, remainSize);
            break;
        case 71:
            result = LLVMFuzzerTestOneInput_153(alignedData, remainSize);
            break;
        case 72:
            result = LLVMFuzzerTestOneInput_156(alignedData, remainSize);
            break;
        case 73:
            result = LLVMFuzzerTestOneInput_158(alignedData, remainSize);
            break;
        case 74:
            result = LLVMFuzzerTestOneInput_159(alignedData, remainSize);
            break;
        case 75:
            result = LLVMFuzzerTestOneInput_160(alignedData, remainSize);
            break;
        case 76:
            result = LLVMFuzzerTestOneInput_161(alignedData, remainSize);
            break;
        case 77:
            result = LLVMFuzzerTestOneInput_163(alignedData, remainSize);
            break;
        case 78:
            result = LLVMFuzzerTestOneInput_164(alignedData, remainSize);
            break;
        case 79:
            result = LLVMFuzzerTestOneInput_165(alignedData, remainSize);
            break;
        case 80:
            result = LLVMFuzzerTestOneInput_166(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
