// This is the entry of 58 fuzz drivers:
// 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 2
// 3, 24, 25, 26, 27, 28, 29, 30, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 4
// 4, 45, 46, 48, 50, 51, 53, 54, 55, 56, 57, 58, 59, 60, 61, 66
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
int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size);
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
int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 58) {
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
            result = LLVMFuzzerTestOneInput_48(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_50(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_51(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_53(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_54(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_55(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_56(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_57(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_58(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_59(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_60(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_61(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_66(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
