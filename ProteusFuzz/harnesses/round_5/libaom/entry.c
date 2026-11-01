// This is the entry of 249 fuzz drivers:
// 1, 2, 3, 4, 5, 6, 8, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 24, 25, 26, 28, 29,
//  30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 43, 44, 47, 48, 49, 50, 51, 52,
//  53, 54, 55, 56, 57, 58, 59, 61, 62, 63, 65, 66, 67, 68, 69, 71, 72, 73, 76, 77,
//  78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 90, 91, 92, 93, 94, 96, 97, 98, 100
// , 101, 102, 103, 106, 107, 108, 109, 110, 111, 114, 116, 117, 118, 119, 120, 121
// , 122, 123, 124, 126, 128, 129, 130, 133, 134, 138, 140, 141, 142, 143, 144, 145
// , 146, 147, 148, 149, 151, 152, 153, 154, 155, 156, 159, 160, 162, 163, 164, 167
// , 168, 169, 170, 171, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 184, 185
// , 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201
// , 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 216, 217, 220, 221
// , 222, 223, 224, 225, 226, 227, 229, 230, 231, 232, 234, 235, 236, 238, 239, 240
// , 241, 243, 244, 245, 246, 247, 248, 249, 251, 253, 255, 256, 258, 259, 260, 261
// , 262, 263, 264, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278
// , 280, 281, 283, 285, 286, 288, 289, 290, 291, 292, 293, 294, 295, 296, 298, 299
// , 300, 301, 302, 305, 307, 308, 309
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
int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size);
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
int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_167(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_168(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_169(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_170(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_171(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_173(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_174(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_175(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_176(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_177(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_178(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_179(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_180(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_181(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_182(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_184(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_185(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_186(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_187(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_188(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_189(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_190(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_191(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_192(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_193(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_194(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_195(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_196(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_197(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_198(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_199(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_200(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_201(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_202(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_203(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_204(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_205(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_206(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_207(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_208(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_209(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_210(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_211(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_212(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_213(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_216(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_217(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_220(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_221(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_222(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_223(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_224(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_225(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_226(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_227(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_229(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_230(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_231(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_232(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_234(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_235(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_236(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_238(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_239(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_240(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_241(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_243(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_244(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_245(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_246(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_247(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_248(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_249(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_251(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_253(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_255(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_256(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_258(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_259(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_260(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_261(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_262(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_263(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_264(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_266(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_267(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_268(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_269(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_270(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_271(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_272(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_273(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_274(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_275(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_276(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_277(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_278(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_280(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_281(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_283(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_285(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_286(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_288(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_289(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_290(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_291(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_292(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_293(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_294(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_295(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_296(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_298(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_299(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_300(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_301(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_302(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_305(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_307(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_308(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_309(const uint8_t *Data, size_t Size);

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
    switch (driverIndex % 249) {
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
            result = LLVMFuzzerTestOneInput_12(alignedData, remainSize);
            break;
        case 8:
            result = LLVMFuzzerTestOneInput_13(alignedData, remainSize);
            break;
        case 9:
            result = LLVMFuzzerTestOneInput_14(alignedData, remainSize);
            break;
        case 10:
            result = LLVMFuzzerTestOneInput_15(alignedData, remainSize);
            break;
        case 11:
            result = LLVMFuzzerTestOneInput_16(alignedData, remainSize);
            break;
        case 12:
            result = LLVMFuzzerTestOneInput_17(alignedData, remainSize);
            break;
        case 13:
            result = LLVMFuzzerTestOneInput_18(alignedData, remainSize);
            break;
        case 14:
            result = LLVMFuzzerTestOneInput_19(alignedData, remainSize);
            break;
        case 15:
            result = LLVMFuzzerTestOneInput_21(alignedData, remainSize);
            break;
        case 16:
            result = LLVMFuzzerTestOneInput_22(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_29(alignedData, remainSize);
            break;
        case 22:
            result = LLVMFuzzerTestOneInput_30(alignedData, remainSize);
            break;
        case 23:
            result = LLVMFuzzerTestOneInput_31(alignedData, remainSize);
            break;
        case 24:
            result = LLVMFuzzerTestOneInput_32(alignedData, remainSize);
            break;
        case 25:
            result = LLVMFuzzerTestOneInput_33(alignedData, remainSize);
            break;
        case 26:
            result = LLVMFuzzerTestOneInput_34(alignedData, remainSize);
            break;
        case 27:
            result = LLVMFuzzerTestOneInput_35(alignedData, remainSize);
            break;
        case 28:
            result = LLVMFuzzerTestOneInput_36(alignedData, remainSize);
            break;
        case 29:
            result = LLVMFuzzerTestOneInput_37(alignedData, remainSize);
            break;
        case 30:
            result = LLVMFuzzerTestOneInput_38(alignedData, remainSize);
            break;
        case 31:
            result = LLVMFuzzerTestOneInput_39(alignedData, remainSize);
            break;
        case 32:
            result = LLVMFuzzerTestOneInput_40(alignedData, remainSize);
            break;
        case 33:
            result = LLVMFuzzerTestOneInput_41(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_43(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_44(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_47(alignedData, remainSize);
            break;
        case 37:
            result = LLVMFuzzerTestOneInput_48(alignedData, remainSize);
            break;
        case 38:
            result = LLVMFuzzerTestOneInput_49(alignedData, remainSize);
            break;
        case 39:
            result = LLVMFuzzerTestOneInput_50(alignedData, remainSize);
            break;
        case 40:
            result = LLVMFuzzerTestOneInput_51(alignedData, remainSize);
            break;
        case 41:
            result = LLVMFuzzerTestOneInput_52(alignedData, remainSize);
            break;
        case 42:
            result = LLVMFuzzerTestOneInput_53(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_54(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_55(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_56(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_57(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_58(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_59(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_61(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_62(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_63(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_65(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_66(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_67(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_68(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_69(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_71(alignedData, remainSize);
            break;
        case 58:
            result = LLVMFuzzerTestOneInput_72(alignedData, remainSize);
            break;
        case 59:
            result = LLVMFuzzerTestOneInput_73(alignedData, remainSize);
            break;
        case 60:
            result = LLVMFuzzerTestOneInput_76(alignedData, remainSize);
            break;
        case 61:
            result = LLVMFuzzerTestOneInput_77(alignedData, remainSize);
            break;
        case 62:
            result = LLVMFuzzerTestOneInput_78(alignedData, remainSize);
            break;
        case 63:
            result = LLVMFuzzerTestOneInput_79(alignedData, remainSize);
            break;
        case 64:
            result = LLVMFuzzerTestOneInput_80(alignedData, remainSize);
            break;
        case 65:
            result = LLVMFuzzerTestOneInput_81(alignedData, remainSize);
            break;
        case 66:
            result = LLVMFuzzerTestOneInput_82(alignedData, remainSize);
            break;
        case 67:
            result = LLVMFuzzerTestOneInput_83(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_88(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_96(alignedData, remainSize);
            break;
        case 79:
            result = LLVMFuzzerTestOneInput_97(alignedData, remainSize);
            break;
        case 80:
            result = LLVMFuzzerTestOneInput_98(alignedData, remainSize);
            break;
        case 81:
            result = LLVMFuzzerTestOneInput_100(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_106(alignedData, remainSize);
            break;
        case 86:
            result = LLVMFuzzerTestOneInput_107(alignedData, remainSize);
            break;
        case 87:
            result = LLVMFuzzerTestOneInput_108(alignedData, remainSize);
            break;
        case 88:
            result = LLVMFuzzerTestOneInput_109(alignedData, remainSize);
            break;
        case 89:
            result = LLVMFuzzerTestOneInput_110(alignedData, remainSize);
            break;
        case 90:
            result = LLVMFuzzerTestOneInput_111(alignedData, remainSize);
            break;
        case 91:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
            break;
        case 92:
            result = LLVMFuzzerTestOneInput_116(alignedData, remainSize);
            break;
        case 93:
            result = LLVMFuzzerTestOneInput_117(alignedData, remainSize);
            break;
        case 94:
            result = LLVMFuzzerTestOneInput_118(alignedData, remainSize);
            break;
        case 95:
            result = LLVMFuzzerTestOneInput_119(alignedData, remainSize);
            break;
        case 96:
            result = LLVMFuzzerTestOneInput_120(alignedData, remainSize);
            break;
        case 97:
            result = LLVMFuzzerTestOneInput_121(alignedData, remainSize);
            break;
        case 98:
            result = LLVMFuzzerTestOneInput_122(alignedData, remainSize);
            break;
        case 99:
            result = LLVMFuzzerTestOneInput_123(alignedData, remainSize);
            break;
        case 100:
            result = LLVMFuzzerTestOneInput_124(alignedData, remainSize);
            break;
        case 101:
            result = LLVMFuzzerTestOneInput_126(alignedData, remainSize);
            break;
        case 102:
            result = LLVMFuzzerTestOneInput_128(alignedData, remainSize);
            break;
        case 103:
            result = LLVMFuzzerTestOneInput_129(alignedData, remainSize);
            break;
        case 104:
            result = LLVMFuzzerTestOneInput_130(alignedData, remainSize);
            break;
        case 105:
            result = LLVMFuzzerTestOneInput_133(alignedData, remainSize);
            break;
        case 106:
            result = LLVMFuzzerTestOneInput_134(alignedData, remainSize);
            break;
        case 107:
            result = LLVMFuzzerTestOneInput_138(alignedData, remainSize);
            break;
        case 108:
            result = LLVMFuzzerTestOneInput_140(alignedData, remainSize);
            break;
        case 109:
            result = LLVMFuzzerTestOneInput_141(alignedData, remainSize);
            break;
        case 110:
            result = LLVMFuzzerTestOneInput_142(alignedData, remainSize);
            break;
        case 111:
            result = LLVMFuzzerTestOneInput_143(alignedData, remainSize);
            break;
        case 112:
            result = LLVMFuzzerTestOneInput_144(alignedData, remainSize);
            break;
        case 113:
            result = LLVMFuzzerTestOneInput_145(alignedData, remainSize);
            break;
        case 114:
            result = LLVMFuzzerTestOneInput_146(alignedData, remainSize);
            break;
        case 115:
            result = LLVMFuzzerTestOneInput_147(alignedData, remainSize);
            break;
        case 116:
            result = LLVMFuzzerTestOneInput_148(alignedData, remainSize);
            break;
        case 117:
            result = LLVMFuzzerTestOneInput_149(alignedData, remainSize);
            break;
        case 118:
            result = LLVMFuzzerTestOneInput_151(alignedData, remainSize);
            break;
        case 119:
            result = LLVMFuzzerTestOneInput_152(alignedData, remainSize);
            break;
        case 120:
            result = LLVMFuzzerTestOneInput_153(alignedData, remainSize);
            break;
        case 121:
            result = LLVMFuzzerTestOneInput_154(alignedData, remainSize);
            break;
        case 122:
            result = LLVMFuzzerTestOneInput_155(alignedData, remainSize);
            break;
        case 123:
            result = LLVMFuzzerTestOneInput_156(alignedData, remainSize);
            break;
        case 124:
            result = LLVMFuzzerTestOneInput_159(alignedData, remainSize);
            break;
        case 125:
            result = LLVMFuzzerTestOneInput_160(alignedData, remainSize);
            break;
        case 126:
            result = LLVMFuzzerTestOneInput_162(alignedData, remainSize);
            break;
        case 127:
            result = LLVMFuzzerTestOneInput_163(alignedData, remainSize);
            break;
        case 128:
            result = LLVMFuzzerTestOneInput_164(alignedData, remainSize);
            break;
        case 129:
            result = LLVMFuzzerTestOneInput_167(alignedData, remainSize);
            break;
        case 130:
            result = LLVMFuzzerTestOneInput_168(alignedData, remainSize);
            break;
        case 131:
            result = LLVMFuzzerTestOneInput_169(alignedData, remainSize);
            break;
        case 132:
            result = LLVMFuzzerTestOneInput_170(alignedData, remainSize);
            break;
        case 133:
            result = LLVMFuzzerTestOneInput_171(alignedData, remainSize);
            break;
        case 134:
            result = LLVMFuzzerTestOneInput_173(alignedData, remainSize);
            break;
        case 135:
            result = LLVMFuzzerTestOneInput_174(alignedData, remainSize);
            break;
        case 136:
            result = LLVMFuzzerTestOneInput_175(alignedData, remainSize);
            break;
        case 137:
            result = LLVMFuzzerTestOneInput_176(alignedData, remainSize);
            break;
        case 138:
            result = LLVMFuzzerTestOneInput_177(alignedData, remainSize);
            break;
        case 139:
            result = LLVMFuzzerTestOneInput_178(alignedData, remainSize);
            break;
        case 140:
            result = LLVMFuzzerTestOneInput_179(alignedData, remainSize);
            break;
        case 141:
            result = LLVMFuzzerTestOneInput_180(alignedData, remainSize);
            break;
        case 142:
            result = LLVMFuzzerTestOneInput_181(alignedData, remainSize);
            break;
        case 143:
            result = LLVMFuzzerTestOneInput_182(alignedData, remainSize);
            break;
        case 144:
            result = LLVMFuzzerTestOneInput_184(alignedData, remainSize);
            break;
        case 145:
            result = LLVMFuzzerTestOneInput_185(alignedData, remainSize);
            break;
        case 146:
            result = LLVMFuzzerTestOneInput_186(alignedData, remainSize);
            break;
        case 147:
            result = LLVMFuzzerTestOneInput_187(alignedData, remainSize);
            break;
        case 148:
            result = LLVMFuzzerTestOneInput_188(alignedData, remainSize);
            break;
        case 149:
            result = LLVMFuzzerTestOneInput_189(alignedData, remainSize);
            break;
        case 150:
            result = LLVMFuzzerTestOneInput_190(alignedData, remainSize);
            break;
        case 151:
            result = LLVMFuzzerTestOneInput_191(alignedData, remainSize);
            break;
        case 152:
            result = LLVMFuzzerTestOneInput_192(alignedData, remainSize);
            break;
        case 153:
            result = LLVMFuzzerTestOneInput_193(alignedData, remainSize);
            break;
        case 154:
            result = LLVMFuzzerTestOneInput_194(alignedData, remainSize);
            break;
        case 155:
            result = LLVMFuzzerTestOneInput_195(alignedData, remainSize);
            break;
        case 156:
            result = LLVMFuzzerTestOneInput_196(alignedData, remainSize);
            break;
        case 157:
            result = LLVMFuzzerTestOneInput_197(alignedData, remainSize);
            break;
        case 158:
            result = LLVMFuzzerTestOneInput_198(alignedData, remainSize);
            break;
        case 159:
            result = LLVMFuzzerTestOneInput_199(alignedData, remainSize);
            break;
        case 160:
            result = LLVMFuzzerTestOneInput_200(alignedData, remainSize);
            break;
        case 161:
            result = LLVMFuzzerTestOneInput_201(alignedData, remainSize);
            break;
        case 162:
            result = LLVMFuzzerTestOneInput_202(alignedData, remainSize);
            break;
        case 163:
            result = LLVMFuzzerTestOneInput_203(alignedData, remainSize);
            break;
        case 164:
            result = LLVMFuzzerTestOneInput_204(alignedData, remainSize);
            break;
        case 165:
            result = LLVMFuzzerTestOneInput_205(alignedData, remainSize);
            break;
        case 166:
            result = LLVMFuzzerTestOneInput_206(alignedData, remainSize);
            break;
        case 167:
            result = LLVMFuzzerTestOneInput_207(alignedData, remainSize);
            break;
        case 168:
            result = LLVMFuzzerTestOneInput_208(alignedData, remainSize);
            break;
        case 169:
            result = LLVMFuzzerTestOneInput_209(alignedData, remainSize);
            break;
        case 170:
            result = LLVMFuzzerTestOneInput_210(alignedData, remainSize);
            break;
        case 171:
            result = LLVMFuzzerTestOneInput_211(alignedData, remainSize);
            break;
        case 172:
            result = LLVMFuzzerTestOneInput_212(alignedData, remainSize);
            break;
        case 173:
            result = LLVMFuzzerTestOneInput_213(alignedData, remainSize);
            break;
        case 174:
            result = LLVMFuzzerTestOneInput_216(alignedData, remainSize);
            break;
        case 175:
            result = LLVMFuzzerTestOneInput_217(alignedData, remainSize);
            break;
        case 176:
            result = LLVMFuzzerTestOneInput_220(alignedData, remainSize);
            break;
        case 177:
            result = LLVMFuzzerTestOneInput_221(alignedData, remainSize);
            break;
        case 178:
            result = LLVMFuzzerTestOneInput_222(alignedData, remainSize);
            break;
        case 179:
            result = LLVMFuzzerTestOneInput_223(alignedData, remainSize);
            break;
        case 180:
            result = LLVMFuzzerTestOneInput_224(alignedData, remainSize);
            break;
        case 181:
            result = LLVMFuzzerTestOneInput_225(alignedData, remainSize);
            break;
        case 182:
            result = LLVMFuzzerTestOneInput_226(alignedData, remainSize);
            break;
        case 183:
            result = LLVMFuzzerTestOneInput_227(alignedData, remainSize);
            break;
        case 184:
            result = LLVMFuzzerTestOneInput_229(alignedData, remainSize);
            break;
        case 185:
            result = LLVMFuzzerTestOneInput_230(alignedData, remainSize);
            break;
        case 186:
            result = LLVMFuzzerTestOneInput_231(alignedData, remainSize);
            break;
        case 187:
            result = LLVMFuzzerTestOneInput_232(alignedData, remainSize);
            break;
        case 188:
            result = LLVMFuzzerTestOneInput_234(alignedData, remainSize);
            break;
        case 189:
            result = LLVMFuzzerTestOneInput_235(alignedData, remainSize);
            break;
        case 190:
            result = LLVMFuzzerTestOneInput_236(alignedData, remainSize);
            break;
        case 191:
            result = LLVMFuzzerTestOneInput_238(alignedData, remainSize);
            break;
        case 192:
            result = LLVMFuzzerTestOneInput_239(alignedData, remainSize);
            break;
        case 193:
            result = LLVMFuzzerTestOneInput_240(alignedData, remainSize);
            break;
        case 194:
            result = LLVMFuzzerTestOneInput_241(alignedData, remainSize);
            break;
        case 195:
            result = LLVMFuzzerTestOneInput_243(alignedData, remainSize);
            break;
        case 196:
            result = LLVMFuzzerTestOneInput_244(alignedData, remainSize);
            break;
        case 197:
            result = LLVMFuzzerTestOneInput_245(alignedData, remainSize);
            break;
        case 198:
            result = LLVMFuzzerTestOneInput_246(alignedData, remainSize);
            break;
        case 199:
            result = LLVMFuzzerTestOneInput_247(alignedData, remainSize);
            break;
        case 200:
            result = LLVMFuzzerTestOneInput_248(alignedData, remainSize);
            break;
        case 201:
            result = LLVMFuzzerTestOneInput_249(alignedData, remainSize);
            break;
        case 202:
            result = LLVMFuzzerTestOneInput_251(alignedData, remainSize);
            break;
        case 203:
            result = LLVMFuzzerTestOneInput_253(alignedData, remainSize);
            break;
        case 204:
            result = LLVMFuzzerTestOneInput_255(alignedData, remainSize);
            break;
        case 205:
            result = LLVMFuzzerTestOneInput_256(alignedData, remainSize);
            break;
        case 206:
            result = LLVMFuzzerTestOneInput_258(alignedData, remainSize);
            break;
        case 207:
            result = LLVMFuzzerTestOneInput_259(alignedData, remainSize);
            break;
        case 208:
            result = LLVMFuzzerTestOneInput_260(alignedData, remainSize);
            break;
        case 209:
            result = LLVMFuzzerTestOneInput_261(alignedData, remainSize);
            break;
        case 210:
            result = LLVMFuzzerTestOneInput_262(alignedData, remainSize);
            break;
        case 211:
            result = LLVMFuzzerTestOneInput_263(alignedData, remainSize);
            break;
        case 212:
            result = LLVMFuzzerTestOneInput_264(alignedData, remainSize);
            break;
        case 213:
            result = LLVMFuzzerTestOneInput_266(alignedData, remainSize);
            break;
        case 214:
            result = LLVMFuzzerTestOneInput_267(alignedData, remainSize);
            break;
        case 215:
            result = LLVMFuzzerTestOneInput_268(alignedData, remainSize);
            break;
        case 216:
            result = LLVMFuzzerTestOneInput_269(alignedData, remainSize);
            break;
        case 217:
            result = LLVMFuzzerTestOneInput_270(alignedData, remainSize);
            break;
        case 218:
            result = LLVMFuzzerTestOneInput_271(alignedData, remainSize);
            break;
        case 219:
            result = LLVMFuzzerTestOneInput_272(alignedData, remainSize);
            break;
        case 220:
            result = LLVMFuzzerTestOneInput_273(alignedData, remainSize);
            break;
        case 221:
            result = LLVMFuzzerTestOneInput_274(alignedData, remainSize);
            break;
        case 222:
            result = LLVMFuzzerTestOneInput_275(alignedData, remainSize);
            break;
        case 223:
            result = LLVMFuzzerTestOneInput_276(alignedData, remainSize);
            break;
        case 224:
            result = LLVMFuzzerTestOneInput_277(alignedData, remainSize);
            break;
        case 225:
            result = LLVMFuzzerTestOneInput_278(alignedData, remainSize);
            break;
        case 226:
            result = LLVMFuzzerTestOneInput_280(alignedData, remainSize);
            break;
        case 227:
            result = LLVMFuzzerTestOneInput_281(alignedData, remainSize);
            break;
        case 228:
            result = LLVMFuzzerTestOneInput_283(alignedData, remainSize);
            break;
        case 229:
            result = LLVMFuzzerTestOneInput_285(alignedData, remainSize);
            break;
        case 230:
            result = LLVMFuzzerTestOneInput_286(alignedData, remainSize);
            break;
        case 231:
            result = LLVMFuzzerTestOneInput_288(alignedData, remainSize);
            break;
        case 232:
            result = LLVMFuzzerTestOneInput_289(alignedData, remainSize);
            break;
        case 233:
            result = LLVMFuzzerTestOneInput_290(alignedData, remainSize);
            break;
        case 234:
            result = LLVMFuzzerTestOneInput_291(alignedData, remainSize);
            break;
        case 235:
            result = LLVMFuzzerTestOneInput_292(alignedData, remainSize);
            break;
        case 236:
            result = LLVMFuzzerTestOneInput_293(alignedData, remainSize);
            break;
        case 237:
            result = LLVMFuzzerTestOneInput_294(alignedData, remainSize);
            break;
        case 238:
            result = LLVMFuzzerTestOneInput_295(alignedData, remainSize);
            break;
        case 239:
            result = LLVMFuzzerTestOneInput_296(alignedData, remainSize);
            break;
        case 240:
            result = LLVMFuzzerTestOneInput_298(alignedData, remainSize);
            break;
        case 241:
            result = LLVMFuzzerTestOneInput_299(alignedData, remainSize);
            break;
        case 242:
            result = LLVMFuzzerTestOneInput_300(alignedData, remainSize);
            break;
        case 243:
            result = LLVMFuzzerTestOneInput_301(alignedData, remainSize);
            break;
        case 244:
            result = LLVMFuzzerTestOneInput_302(alignedData, remainSize);
            break;
        case 245:
            result = LLVMFuzzerTestOneInput_305(alignedData, remainSize);
            break;
        case 246:
            result = LLVMFuzzerTestOneInput_307(alignedData, remainSize);
            break;
        case 247:
            result = LLVMFuzzerTestOneInput_308(alignedData, remainSize);
            break;
        case 248:
            result = LLVMFuzzerTestOneInput_309(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
