// This is the entry of 288 fuzz drivers:
// 1, 3, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 
// 28, 29, 30, 31, 33, 34, 35, 36, 37, 38, 39, 40, 42, 43, 44, 45, 48, 49, 50, 52, 
// 54, 55, 56, 57, 61, 62, 63, 64, 65, 66, 67, 68, 69, 71, 72, 73, 75, 76, 77, 78, 
// 79, 80, 81, 83, 84, 85, 86, 87, 88, 89, 91, 93, 94, 95, 96, 99, 100, 102, 103, 1
// 04, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 1
// 20, 121, 122, 123, 124, 126, 127, 130, 132, 133, 134, 135, 137, 138, 139, 140, 1
// 41, 143, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 158, 159, 160, 161, 1
// 62, 164, 165, 166, 167, 168, 171, 172, 174, 175, 176, 177, 178, 180, 181, 184, 1
// 85, 186, 187, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 2
// 02, 203, 204, 205, 206, 207, 209, 210, 211, 213, 214, 215, 216, 218, 220, 221, 2
// 22, 223, 224, 225, 226, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 2
// 40, 241, 243, 244, 245, 246, 247, 248, 250, 251, 252, 253, 254, 256, 257, 258, 2
// 59, 260, 261, 262, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 276, 2
// 77, 278, 280, 281, 283, 284, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 2
// 97, 298, 299, 300, 301, 302, 303, 304, 305, 307, 308, 309, 310, 311, 312, 313, 3
// 14, 315, 316, 317, 318, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 3
// 31, 333, 334, 336, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>

// Declarations of all fuzz drivers `LLVMFuzzerTestOneInput` functions
int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size);
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
int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size);
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
int LLVMFuzzerTestOneInput_127(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_145(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_152(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_158(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_160(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_165(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_166(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_167(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_168(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_171(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_172(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_174(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_175(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_176(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_177(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_178(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_180(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_181(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_184(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_185(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_186(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_187(const uint8_t *Data, size_t Size);
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
int LLVMFuzzerTestOneInput_209(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_210(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_211(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_213(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_214(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_215(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_216(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_218(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_220(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_221(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_222(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_223(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_224(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_225(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_226(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_228(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_229(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_230(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_231(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_232(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_233(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_234(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_235(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_236(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_237(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_238(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_240(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_241(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_243(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_244(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_245(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_246(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_247(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_248(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_250(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_251(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_252(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_253(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_254(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_256(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_257(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_258(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_259(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_260(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_261(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_262(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_264(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_265(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_266(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_267(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_268(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_269(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_270(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_271(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_272(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_273(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_274(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_276(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_277(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_278(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_280(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_281(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_283(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_284(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_286(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_287(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_288(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_289(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_290(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_291(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_292(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_293(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_294(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_295(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_297(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_298(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_299(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_300(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_301(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_302(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_303(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_304(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_305(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_307(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_308(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_309(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_310(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_311(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_312(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_313(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_314(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_315(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_316(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_317(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_318(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_320(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_321(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_322(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_323(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_324(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_325(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_326(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_327(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_328(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_329(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_330(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_331(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_333(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_334(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_336(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_338(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_339(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_340(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_341(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_342(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_343(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_344(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_345(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_346(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_347(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_348(const uint8_t *Data, size_t Size);
int LLVMFuzzerTestOneInput_349(const uint8_t *Data, size_t Size);

// Entry function
int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // Driver selector
    if (Size < 2) {
        return 0;
    }
    const uint8_t *selector = Data;
    unsigned int driverIndex = 0;
    memcpy(&driverIndex, selector, 2);

    // Remain data and size
    const uint8_t *remainData = Data + 2;
    size_t remainSize = Size - 2;
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
    switch (driverIndex % 288) {
        case 0:
            result = LLVMFuzzerTestOneInput_1(alignedData, remainSize);
            break;
        case 1:
            result = LLVMFuzzerTestOneInput_3(alignedData, remainSize);
            break;
        case 2:
            result = LLVMFuzzerTestOneInput_8(alignedData, remainSize);
            break;
        case 3:
            result = LLVMFuzzerTestOneInput_9(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_42(alignedData, remainSize);
            break;
        case 34:
            result = LLVMFuzzerTestOneInput_43(alignedData, remainSize);
            break;
        case 35:
            result = LLVMFuzzerTestOneInput_44(alignedData, remainSize);
            break;
        case 36:
            result = LLVMFuzzerTestOneInput_45(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_52(alignedData, remainSize);
            break;
        case 41:
            result = LLVMFuzzerTestOneInput_54(alignedData, remainSize);
            break;
        case 42:
            result = LLVMFuzzerTestOneInput_55(alignedData, remainSize);
            break;
        case 43:
            result = LLVMFuzzerTestOneInput_56(alignedData, remainSize);
            break;
        case 44:
            result = LLVMFuzzerTestOneInput_57(alignedData, remainSize);
            break;
        case 45:
            result = LLVMFuzzerTestOneInput_61(alignedData, remainSize);
            break;
        case 46:
            result = LLVMFuzzerTestOneInput_62(alignedData, remainSize);
            break;
        case 47:
            result = LLVMFuzzerTestOneInput_63(alignedData, remainSize);
            break;
        case 48:
            result = LLVMFuzzerTestOneInput_64(alignedData, remainSize);
            break;
        case 49:
            result = LLVMFuzzerTestOneInput_65(alignedData, remainSize);
            break;
        case 50:
            result = LLVMFuzzerTestOneInput_66(alignedData, remainSize);
            break;
        case 51:
            result = LLVMFuzzerTestOneInput_67(alignedData, remainSize);
            break;
        case 52:
            result = LLVMFuzzerTestOneInput_68(alignedData, remainSize);
            break;
        case 53:
            result = LLVMFuzzerTestOneInput_69(alignedData, remainSize);
            break;
        case 54:
            result = LLVMFuzzerTestOneInput_71(alignedData, remainSize);
            break;
        case 55:
            result = LLVMFuzzerTestOneInput_72(alignedData, remainSize);
            break;
        case 56:
            result = LLVMFuzzerTestOneInput_73(alignedData, remainSize);
            break;
        case 57:
            result = LLVMFuzzerTestOneInput_75(alignedData, remainSize);
            break;
        case 58:
            result = LLVMFuzzerTestOneInput_76(alignedData, remainSize);
            break;
        case 59:
            result = LLVMFuzzerTestOneInput_77(alignedData, remainSize);
            break;
        case 60:
            result = LLVMFuzzerTestOneInput_78(alignedData, remainSize);
            break;
        case 61:
            result = LLVMFuzzerTestOneInput_79(alignedData, remainSize);
            break;
        case 62:
            result = LLVMFuzzerTestOneInput_80(alignedData, remainSize);
            break;
        case 63:
            result = LLVMFuzzerTestOneInput_81(alignedData, remainSize);
            break;
        case 64:
            result = LLVMFuzzerTestOneInput_83(alignedData, remainSize);
            break;
        case 65:
            result = LLVMFuzzerTestOneInput_84(alignedData, remainSize);
            break;
        case 66:
            result = LLVMFuzzerTestOneInput_85(alignedData, remainSize);
            break;
        case 67:
            result = LLVMFuzzerTestOneInput_86(alignedData, remainSize);
            break;
        case 68:
            result = LLVMFuzzerTestOneInput_87(alignedData, remainSize);
            break;
        case 69:
            result = LLVMFuzzerTestOneInput_88(alignedData, remainSize);
            break;
        case 70:
            result = LLVMFuzzerTestOneInput_89(alignedData, remainSize);
            break;
        case 71:
            result = LLVMFuzzerTestOneInput_91(alignedData, remainSize);
            break;
        case 72:
            result = LLVMFuzzerTestOneInput_93(alignedData, remainSize);
            break;
        case 73:
            result = LLVMFuzzerTestOneInput_94(alignedData, remainSize);
            break;
        case 74:
            result = LLVMFuzzerTestOneInput_95(alignedData, remainSize);
            break;
        case 75:
            result = LLVMFuzzerTestOneInput_96(alignedData, remainSize);
            break;
        case 76:
            result = LLVMFuzzerTestOneInput_99(alignedData, remainSize);
            break;
        case 77:
            result = LLVMFuzzerTestOneInput_100(alignedData, remainSize);
            break;
        case 78:
            result = LLVMFuzzerTestOneInput_102(alignedData, remainSize);
            break;
        case 79:
            result = LLVMFuzzerTestOneInput_103(alignedData, remainSize);
            break;
        case 80:
            result = LLVMFuzzerTestOneInput_104(alignedData, remainSize);
            break;
        case 81:
            result = LLVMFuzzerTestOneInput_105(alignedData, remainSize);
            break;
        case 82:
            result = LLVMFuzzerTestOneInput_106(alignedData, remainSize);
            break;
        case 83:
            result = LLVMFuzzerTestOneInput_107(alignedData, remainSize);
            break;
        case 84:
            result = LLVMFuzzerTestOneInput_108(alignedData, remainSize);
            break;
        case 85:
            result = LLVMFuzzerTestOneInput_109(alignedData, remainSize);
            break;
        case 86:
            result = LLVMFuzzerTestOneInput_110(alignedData, remainSize);
            break;
        case 87:
            result = LLVMFuzzerTestOneInput_111(alignedData, remainSize);
            break;
        case 88:
            result = LLVMFuzzerTestOneInput_112(alignedData, remainSize);
            break;
        case 89:
            result = LLVMFuzzerTestOneInput_113(alignedData, remainSize);
            break;
        case 90:
            result = LLVMFuzzerTestOneInput_114(alignedData, remainSize);
            break;
        case 91:
            result = LLVMFuzzerTestOneInput_115(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_127(alignedData, remainSize);
            break;
        case 103:
            result = LLVMFuzzerTestOneInput_130(alignedData, remainSize);
            break;
        case 104:
            result = LLVMFuzzerTestOneInput_132(alignedData, remainSize);
            break;
        case 105:
            result = LLVMFuzzerTestOneInput_133(alignedData, remainSize);
            break;
        case 106:
            result = LLVMFuzzerTestOneInput_134(alignedData, remainSize);
            break;
        case 107:
            result = LLVMFuzzerTestOneInput_135(alignedData, remainSize);
            break;
        case 108:
            result = LLVMFuzzerTestOneInput_137(alignedData, remainSize);
            break;
        case 109:
            result = LLVMFuzzerTestOneInput_138(alignedData, remainSize);
            break;
        case 110:
            result = LLVMFuzzerTestOneInput_139(alignedData, remainSize);
            break;
        case 111:
            result = LLVMFuzzerTestOneInput_140(alignedData, remainSize);
            break;
        case 112:
            result = LLVMFuzzerTestOneInput_141(alignedData, remainSize);
            break;
        case 113:
            result = LLVMFuzzerTestOneInput_143(alignedData, remainSize);
            break;
        case 114:
            result = LLVMFuzzerTestOneInput_145(alignedData, remainSize);
            break;
        case 115:
            result = LLVMFuzzerTestOneInput_146(alignedData, remainSize);
            break;
        case 116:
            result = LLVMFuzzerTestOneInput_147(alignedData, remainSize);
            break;
        case 117:
            result = LLVMFuzzerTestOneInput_148(alignedData, remainSize);
            break;
        case 118:
            result = LLVMFuzzerTestOneInput_149(alignedData, remainSize);
            break;
        case 119:
            result = LLVMFuzzerTestOneInput_150(alignedData, remainSize);
            break;
        case 120:
            result = LLVMFuzzerTestOneInput_151(alignedData, remainSize);
            break;
        case 121:
            result = LLVMFuzzerTestOneInput_152(alignedData, remainSize);
            break;
        case 122:
            result = LLVMFuzzerTestOneInput_153(alignedData, remainSize);
            break;
        case 123:
            result = LLVMFuzzerTestOneInput_154(alignedData, remainSize);
            break;
        case 124:
            result = LLVMFuzzerTestOneInput_158(alignedData, remainSize);
            break;
        case 125:
            result = LLVMFuzzerTestOneInput_159(alignedData, remainSize);
            break;
        case 126:
            result = LLVMFuzzerTestOneInput_160(alignedData, remainSize);
            break;
        case 127:
            result = LLVMFuzzerTestOneInput_161(alignedData, remainSize);
            break;
        case 128:
            result = LLVMFuzzerTestOneInput_162(alignedData, remainSize);
            break;
        case 129:
            result = LLVMFuzzerTestOneInput_164(alignedData, remainSize);
            break;
        case 130:
            result = LLVMFuzzerTestOneInput_165(alignedData, remainSize);
            break;
        case 131:
            result = LLVMFuzzerTestOneInput_166(alignedData, remainSize);
            break;
        case 132:
            result = LLVMFuzzerTestOneInput_167(alignedData, remainSize);
            break;
        case 133:
            result = LLVMFuzzerTestOneInput_168(alignedData, remainSize);
            break;
        case 134:
            result = LLVMFuzzerTestOneInput_171(alignedData, remainSize);
            break;
        case 135:
            result = LLVMFuzzerTestOneInput_172(alignedData, remainSize);
            break;
        case 136:
            result = LLVMFuzzerTestOneInput_174(alignedData, remainSize);
            break;
        case 137:
            result = LLVMFuzzerTestOneInput_175(alignedData, remainSize);
            break;
        case 138:
            result = LLVMFuzzerTestOneInput_176(alignedData, remainSize);
            break;
        case 139:
            result = LLVMFuzzerTestOneInput_177(alignedData, remainSize);
            break;
        case 140:
            result = LLVMFuzzerTestOneInput_178(alignedData, remainSize);
            break;
        case 141:
            result = LLVMFuzzerTestOneInput_180(alignedData, remainSize);
            break;
        case 142:
            result = LLVMFuzzerTestOneInput_181(alignedData, remainSize);
            break;
        case 143:
            result = LLVMFuzzerTestOneInput_184(alignedData, remainSize);
            break;
        case 144:
            result = LLVMFuzzerTestOneInput_185(alignedData, remainSize);
            break;
        case 145:
            result = LLVMFuzzerTestOneInput_186(alignedData, remainSize);
            break;
        case 146:
            result = LLVMFuzzerTestOneInput_187(alignedData, remainSize);
            break;
        case 147:
            result = LLVMFuzzerTestOneInput_189(alignedData, remainSize);
            break;
        case 148:
            result = LLVMFuzzerTestOneInput_190(alignedData, remainSize);
            break;
        case 149:
            result = LLVMFuzzerTestOneInput_191(alignedData, remainSize);
            break;
        case 150:
            result = LLVMFuzzerTestOneInput_192(alignedData, remainSize);
            break;
        case 151:
            result = LLVMFuzzerTestOneInput_193(alignedData, remainSize);
            break;
        case 152:
            result = LLVMFuzzerTestOneInput_194(alignedData, remainSize);
            break;
        case 153:
            result = LLVMFuzzerTestOneInput_195(alignedData, remainSize);
            break;
        case 154:
            result = LLVMFuzzerTestOneInput_196(alignedData, remainSize);
            break;
        case 155:
            result = LLVMFuzzerTestOneInput_197(alignedData, remainSize);
            break;
        case 156:
            result = LLVMFuzzerTestOneInput_198(alignedData, remainSize);
            break;
        case 157:
            result = LLVMFuzzerTestOneInput_199(alignedData, remainSize);
            break;
        case 158:
            result = LLVMFuzzerTestOneInput_200(alignedData, remainSize);
            break;
        case 159:
            result = LLVMFuzzerTestOneInput_201(alignedData, remainSize);
            break;
        case 160:
            result = LLVMFuzzerTestOneInput_202(alignedData, remainSize);
            break;
        case 161:
            result = LLVMFuzzerTestOneInput_203(alignedData, remainSize);
            break;
        case 162:
            result = LLVMFuzzerTestOneInput_204(alignedData, remainSize);
            break;
        case 163:
            result = LLVMFuzzerTestOneInput_205(alignedData, remainSize);
            break;
        case 164:
            result = LLVMFuzzerTestOneInput_206(alignedData, remainSize);
            break;
        case 165:
            result = LLVMFuzzerTestOneInput_207(alignedData, remainSize);
            break;
        case 166:
            result = LLVMFuzzerTestOneInput_209(alignedData, remainSize);
            break;
        case 167:
            result = LLVMFuzzerTestOneInput_210(alignedData, remainSize);
            break;
        case 168:
            result = LLVMFuzzerTestOneInput_211(alignedData, remainSize);
            break;
        case 169:
            result = LLVMFuzzerTestOneInput_213(alignedData, remainSize);
            break;
        case 170:
            result = LLVMFuzzerTestOneInput_214(alignedData, remainSize);
            break;
        case 171:
            result = LLVMFuzzerTestOneInput_215(alignedData, remainSize);
            break;
        case 172:
            result = LLVMFuzzerTestOneInput_216(alignedData, remainSize);
            break;
        case 173:
            result = LLVMFuzzerTestOneInput_218(alignedData, remainSize);
            break;
        case 174:
            result = LLVMFuzzerTestOneInput_220(alignedData, remainSize);
            break;
        case 175:
            result = LLVMFuzzerTestOneInput_221(alignedData, remainSize);
            break;
        case 176:
            result = LLVMFuzzerTestOneInput_222(alignedData, remainSize);
            break;
        case 177:
            result = LLVMFuzzerTestOneInput_223(alignedData, remainSize);
            break;
        case 178:
            result = LLVMFuzzerTestOneInput_224(alignedData, remainSize);
            break;
        case 179:
            result = LLVMFuzzerTestOneInput_225(alignedData, remainSize);
            break;
        case 180:
            result = LLVMFuzzerTestOneInput_226(alignedData, remainSize);
            break;
        case 181:
            result = LLVMFuzzerTestOneInput_228(alignedData, remainSize);
            break;
        case 182:
            result = LLVMFuzzerTestOneInput_229(alignedData, remainSize);
            break;
        case 183:
            result = LLVMFuzzerTestOneInput_230(alignedData, remainSize);
            break;
        case 184:
            result = LLVMFuzzerTestOneInput_231(alignedData, remainSize);
            break;
        case 185:
            result = LLVMFuzzerTestOneInput_232(alignedData, remainSize);
            break;
        case 186:
            result = LLVMFuzzerTestOneInput_233(alignedData, remainSize);
            break;
        case 187:
            result = LLVMFuzzerTestOneInput_234(alignedData, remainSize);
            break;
        case 188:
            result = LLVMFuzzerTestOneInput_235(alignedData, remainSize);
            break;
        case 189:
            result = LLVMFuzzerTestOneInput_236(alignedData, remainSize);
            break;
        case 190:
            result = LLVMFuzzerTestOneInput_237(alignedData, remainSize);
            break;
        case 191:
            result = LLVMFuzzerTestOneInput_238(alignedData, remainSize);
            break;
        case 192:
            result = LLVMFuzzerTestOneInput_240(alignedData, remainSize);
            break;
        case 193:
            result = LLVMFuzzerTestOneInput_241(alignedData, remainSize);
            break;
        case 194:
            result = LLVMFuzzerTestOneInput_243(alignedData, remainSize);
            break;
        case 195:
            result = LLVMFuzzerTestOneInput_244(alignedData, remainSize);
            break;
        case 196:
            result = LLVMFuzzerTestOneInput_245(alignedData, remainSize);
            break;
        case 197:
            result = LLVMFuzzerTestOneInput_246(alignedData, remainSize);
            break;
        case 198:
            result = LLVMFuzzerTestOneInput_247(alignedData, remainSize);
            break;
        case 199:
            result = LLVMFuzzerTestOneInput_248(alignedData, remainSize);
            break;
        case 200:
            result = LLVMFuzzerTestOneInput_250(alignedData, remainSize);
            break;
        case 201:
            result = LLVMFuzzerTestOneInput_251(alignedData, remainSize);
            break;
        case 202:
            result = LLVMFuzzerTestOneInput_252(alignedData, remainSize);
            break;
        case 203:
            result = LLVMFuzzerTestOneInput_253(alignedData, remainSize);
            break;
        case 204:
            result = LLVMFuzzerTestOneInput_254(alignedData, remainSize);
            break;
        case 205:
            result = LLVMFuzzerTestOneInput_256(alignedData, remainSize);
            break;
        case 206:
            result = LLVMFuzzerTestOneInput_257(alignedData, remainSize);
            break;
        case 207:
            result = LLVMFuzzerTestOneInput_258(alignedData, remainSize);
            break;
        case 208:
            result = LLVMFuzzerTestOneInput_259(alignedData, remainSize);
            break;
        case 209:
            result = LLVMFuzzerTestOneInput_260(alignedData, remainSize);
            break;
        case 210:
            result = LLVMFuzzerTestOneInput_261(alignedData, remainSize);
            break;
        case 211:
            result = LLVMFuzzerTestOneInput_262(alignedData, remainSize);
            break;
        case 212:
            result = LLVMFuzzerTestOneInput_264(alignedData, remainSize);
            break;
        case 213:
            result = LLVMFuzzerTestOneInput_265(alignedData, remainSize);
            break;
        case 214:
            result = LLVMFuzzerTestOneInput_266(alignedData, remainSize);
            break;
        case 215:
            result = LLVMFuzzerTestOneInput_267(alignedData, remainSize);
            break;
        case 216:
            result = LLVMFuzzerTestOneInput_268(alignedData, remainSize);
            break;
        case 217:
            result = LLVMFuzzerTestOneInput_269(alignedData, remainSize);
            break;
        case 218:
            result = LLVMFuzzerTestOneInput_270(alignedData, remainSize);
            break;
        case 219:
            result = LLVMFuzzerTestOneInput_271(alignedData, remainSize);
            break;
        case 220:
            result = LLVMFuzzerTestOneInput_272(alignedData, remainSize);
            break;
        case 221:
            result = LLVMFuzzerTestOneInput_273(alignedData, remainSize);
            break;
        case 222:
            result = LLVMFuzzerTestOneInput_274(alignedData, remainSize);
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
            result = LLVMFuzzerTestOneInput_284(alignedData, remainSize);
            break;
        case 230:
            result = LLVMFuzzerTestOneInput_286(alignedData, remainSize);
            break;
        case 231:
            result = LLVMFuzzerTestOneInput_287(alignedData, remainSize);
            break;
        case 232:
            result = LLVMFuzzerTestOneInput_288(alignedData, remainSize);
            break;
        case 233:
            result = LLVMFuzzerTestOneInput_289(alignedData, remainSize);
            break;
        case 234:
            result = LLVMFuzzerTestOneInput_290(alignedData, remainSize);
            break;
        case 235:
            result = LLVMFuzzerTestOneInput_291(alignedData, remainSize);
            break;
        case 236:
            result = LLVMFuzzerTestOneInput_292(alignedData, remainSize);
            break;
        case 237:
            result = LLVMFuzzerTestOneInput_293(alignedData, remainSize);
            break;
        case 238:
            result = LLVMFuzzerTestOneInput_294(alignedData, remainSize);
            break;
        case 239:
            result = LLVMFuzzerTestOneInput_295(alignedData, remainSize);
            break;
        case 240:
            result = LLVMFuzzerTestOneInput_297(alignedData, remainSize);
            break;
        case 241:
            result = LLVMFuzzerTestOneInput_298(alignedData, remainSize);
            break;
        case 242:
            result = LLVMFuzzerTestOneInput_299(alignedData, remainSize);
            break;
        case 243:
            result = LLVMFuzzerTestOneInput_300(alignedData, remainSize);
            break;
        case 244:
            result = LLVMFuzzerTestOneInput_301(alignedData, remainSize);
            break;
        case 245:
            result = LLVMFuzzerTestOneInput_302(alignedData, remainSize);
            break;
        case 246:
            result = LLVMFuzzerTestOneInput_303(alignedData, remainSize);
            break;
        case 247:
            result = LLVMFuzzerTestOneInput_304(alignedData, remainSize);
            break;
        case 248:
            result = LLVMFuzzerTestOneInput_305(alignedData, remainSize);
            break;
        case 249:
            result = LLVMFuzzerTestOneInput_307(alignedData, remainSize);
            break;
        case 250:
            result = LLVMFuzzerTestOneInput_308(alignedData, remainSize);
            break;
        case 251:
            result = LLVMFuzzerTestOneInput_309(alignedData, remainSize);
            break;
        case 252:
            result = LLVMFuzzerTestOneInput_310(alignedData, remainSize);
            break;
        case 253:
            result = LLVMFuzzerTestOneInput_311(alignedData, remainSize);
            break;
        case 254:
            result = LLVMFuzzerTestOneInput_312(alignedData, remainSize);
            break;
        case 255:
            result = LLVMFuzzerTestOneInput_313(alignedData, remainSize);
            break;
        case 256:
            result = LLVMFuzzerTestOneInput_314(alignedData, remainSize);
            break;
        case 257:
            result = LLVMFuzzerTestOneInput_315(alignedData, remainSize);
            break;
        case 258:
            result = LLVMFuzzerTestOneInput_316(alignedData, remainSize);
            break;
        case 259:
            result = LLVMFuzzerTestOneInput_317(alignedData, remainSize);
            break;
        case 260:
            result = LLVMFuzzerTestOneInput_318(alignedData, remainSize);
            break;
        case 261:
            result = LLVMFuzzerTestOneInput_320(alignedData, remainSize);
            break;
        case 262:
            result = LLVMFuzzerTestOneInput_321(alignedData, remainSize);
            break;
        case 263:
            result = LLVMFuzzerTestOneInput_322(alignedData, remainSize);
            break;
        case 264:
            result = LLVMFuzzerTestOneInput_323(alignedData, remainSize);
            break;
        case 265:
            result = LLVMFuzzerTestOneInput_324(alignedData, remainSize);
            break;
        case 266:
            result = LLVMFuzzerTestOneInput_325(alignedData, remainSize);
            break;
        case 267:
            result = LLVMFuzzerTestOneInput_326(alignedData, remainSize);
            break;
        case 268:
            result = LLVMFuzzerTestOneInput_327(alignedData, remainSize);
            break;
        case 269:
            result = LLVMFuzzerTestOneInput_328(alignedData, remainSize);
            break;
        case 270:
            result = LLVMFuzzerTestOneInput_329(alignedData, remainSize);
            break;
        case 271:
            result = LLVMFuzzerTestOneInput_330(alignedData, remainSize);
            break;
        case 272:
            result = LLVMFuzzerTestOneInput_331(alignedData, remainSize);
            break;
        case 273:
            result = LLVMFuzzerTestOneInput_333(alignedData, remainSize);
            break;
        case 274:
            result = LLVMFuzzerTestOneInput_334(alignedData, remainSize);
            break;
        case 275:
            result = LLVMFuzzerTestOneInput_336(alignedData, remainSize);
            break;
        case 276:
            result = LLVMFuzzerTestOneInput_338(alignedData, remainSize);
            break;
        case 277:
            result = LLVMFuzzerTestOneInput_339(alignedData, remainSize);
            break;
        case 278:
            result = LLVMFuzzerTestOneInput_340(alignedData, remainSize);
            break;
        case 279:
            result = LLVMFuzzerTestOneInput_341(alignedData, remainSize);
            break;
        case 280:
            result = LLVMFuzzerTestOneInput_342(alignedData, remainSize);
            break;
        case 281:
            result = LLVMFuzzerTestOneInput_343(alignedData, remainSize);
            break;
        case 282:
            result = LLVMFuzzerTestOneInput_344(alignedData, remainSize);
            break;
        case 283:
            result = LLVMFuzzerTestOneInput_345(alignedData, remainSize);
            break;
        case 284:
            result = LLVMFuzzerTestOneInput_346(alignedData, remainSize);
            break;
        case 285:
            result = LLVMFuzzerTestOneInput_347(alignedData, remainSize);
            break;
        case 286:
            result = LLVMFuzzerTestOneInput_348(alignedData, remainSize);
            break;
        case 287:
            result = LLVMFuzzerTestOneInput_349(alignedData, remainSize);
            break;
        default:
            break;
    }
    free(alignedData);
    return result;
    return 0;
}
