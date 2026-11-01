// This is the entry of 81 fuzz drivers:
// 1, 2, 3, 5, 6, 7, 8, 12, 14, 16, 17, 20, 21, 22, 24, 25, 27, 28, 29, 32, 33, 34,
//  35, 36, 37, 40, 46, 48, 50, 51, 52, 53, 61, 62, 63, 65, 68, 69, 70, 71, 72, 73,
//  77, 78, 79, 81, 82, 83, 85, 88, 96, 102, 103, 104, 105, 109, 112, 113, 114, 115
// , 117, 119, 120, 122, 125, 127, 128, 131, 132, 133, 134, 135, 139, 143, 144, 145
// , 146, 148, 149, 155, 156
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size);

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
            result = LLVMFuzzerTestOneInput_2(alignedData, remainSize);
            break;
        case 2:
            result = LLVMFuzzerTestOneInput_3(alignedData, remainSize);
            break;
        case 3:
            result = LLVMFuzzerTestOneInput_5(alignedData, remainSize);
            break;
        case 4:
            result = LLVMFuzzerTestOneInput_6(alignedData, remainSize);
            break;
        case 5:
            result = LLVMFuzzerTestOneInput_7(alignedData, remainSize);
            break;
        case 6:
            result = LLVMFuzzerTestOneInput_8(alignedData, remainSize);
            break;
        case 7:
            result = LLVMFuzzerTestOneInput_12(alignedData, remainSize);
            break;
        case 8:
            result = LLVMFuzzerTestOneInput_14(alignedData, remainSize);
            break;
        case 9:
            result = LLVMFuzzerTestOneInput_16(alignedData, remainSize);
            break;
        case 10:
            result = LLVMFuzzerTestOneInput_17(alignedData, remainSize);
            break;
        case 11:
            result = LLVMFuzzerTestOneInput_20(alignedData, remainSize);
            break;
        case 12:
            result = LLVMFuzzerTestOneInput_21(alignedData, remainSize);
            break;
        case 13:
            result = LLVMFuzzerTestOneInput_22(alignedData, remainSize);
            break;
        case 14:
            result = LLVMFuzzerTestOneInput_24(alignedData, remainSize);
            break;
        case 15:
            result = LLVMFuzzerTestOneInput_25(alignedData, remainSize);
            break;
        case 16:
            result = LLVMFuzzerTestOneInput_27(alignedData, remainSize);
            break;
        case 17:
            result = LLVMFuzzerTestOneInput_28(alignedData, remainSize);
            break;
        case 18:
            result = LLVMFuzzerTestOneInput_29(alignedData, remainSize);
            break;
        case 19:
            result = LLVMFuzzerTestOneInput_32(alignedData, remainSize);
            break;
        case 20:
            result = LLVMFuzzerTestOneInput_33(alignedData, remainSize);
            break;
        case 21:
            result = LLVMFuzzerTestOneInput_34(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_35(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_36(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_37(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_40(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_46(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_48(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_50(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_51(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_52(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_53(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_61(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_68(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_69(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_77(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_78(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_79(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_81(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_82(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_83(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_85(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_88(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_96(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_102(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_103(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_104(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_105(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_109(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_112(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_113(alignedData, remainSize);
            break;
        case 58:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
            break;
        case 59:
            result = LLVMFuzzerTestOneInput_115(alignedData, remainSize);
            break;
        case 60:
            result = LLVMFuzzerTestOneInput_117(alignedData, remainSize);
            break;
        case 61:
            result = LLVMFuzzerTestOneInput_119(alignedData, remainSize);
            break;
        case 62:
            result = LLVMFuzzerTestOneInput_120(alignedData, remainSize);
            break;
        case 63:
            result = LLVMFuzzerTestOneInput_122(alignedData, remainSize);
            break;
        case 64:
            result = LLVMFuzzerTestOneInput_125(alignedData, remainSize);
            break;
        case 65:
            result = LLVMFuzzerTestOneInput_127(alignedData, remainSize);
            break;
        case 66:
            result = LLVMFuzzerTestOneInput_128(alignedData, remainSize);
            break;
        case 67:
            result = LLVMFuzzerTestOneInput_131(alignedData, remainSize);
            break;
        case 68:
            result = LLVMFuzzerTestOneInput_132(alignedData, remainSize);
            break;
        case 69:
            result = LLVMFuzzerTestOneInput_133(alignedData, remainSize);
            break;
        case 70:
            result = LLVMFuzzerTestOneInput_134(alignedData, remainSize);
            break;
        case 71:
            result = LLVMFuzzerTestOneInput_135(alignedData, remainSize);
            break;
        case 72:
            result = LLVMFuzzerTestOneInput_139(alignedData, remainSize);
            break;
        case 73:
            result = LLVMFuzzerTestOneInput_143(alignedData, remainSize);
            break;
        case 74:
            result = LLVMFuzzerTestOneInput_144(alignedData, remainSize);
            break;
        case 75:
            result = LLVMFuzzerTestOneInput_145(alignedData, remainSize);
            break;
        case 76:
            result = LLVMFuzzerTestOneInput_146(alignedData, remainSize);
            break;
        case 77:
            result = LLVMFuzzerTestOneInput_148(alignedData, remainSize);
            break;
        case 78:
            result = LLVMFuzzerTestOneInput_149(alignedData, remainSize);
            break;
        case 79:
            result = LLVMFuzzerTestOneInput_155(alignedData, remainSize);
            break;
        case 80:
            result = LLVMFuzzerTestOneInput_156(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
