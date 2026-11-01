# ProteusFuzz Library Build Scripts

Per-target library build scripts used by ProteusFuzz, one directory per library. Unlike the
baselines, the library build is **not** per-run: every run of a given library (`round_1` ~
`round_5`) is built from the same pinned source commit and the same build scripts, so the
scripts live directly under `build/<lib>/`.

Each `build/<lib>/` contains:

- `build_cov.sh` — source-based coverage instrumentation
  (`-fprofile-instr-generate -fcoverage-mapping`), producing the archive(s) linked into the
  coverage dispatcher binary.
- `build_aflpp.sh` — AFL++ instrumentation (`afl-clang-fast` + PCGUARD) used for the fuzzing
  binaries.
- `build_asan.sh` — ASan + UBSan instrumentation used for crash reproduction.
- `build-manifest.json` — the pinned `source_commit` of the library snapshot, the produced
  target archives (afl / coverage / asan) with sha256, the include dirs, and the compiler
  toolchain (LLVM 18.1.8) used throughout.

The scripts are plain `bash` driven by the standard `CC`/`CXX`/`CFLAGS`/`CXXFLAGS`/`WORK`/`SRC`
environment variables (cmake-based where the library uses cmake). They reference the original
experiment's absolute paths — adjust `WORK`/`SRC` to your own source and output locations when
reusing them.

Pinned source commits per library (identical across all 5 rounds; also recorded in each
`build-manifest.json`):

| Library | source_commit |
|---|---|
| c-ares | 2870f6b14723b3bb77a365f4745abcbd74c2ffad |
| cjson | b2890c8d76bbb64e710585ebc0a917196b9c67e7 |
| curl | e8415ad3c7ab69a7056daa4b39e7a0044c43f5ba |
| lcms | 360391a0916f7c42eb31b3359514bffa7d651d69 |
| libaom | 16a97b8b40a81ba9e5b10d5ae6ac8eee1887a2f0 |
| libjpeg-turbo | 466c3448781cac35236b5e6770477a66d627b521 |
| liblouis | e90a9befd1cd00b483dcdd25efc8d614ce635f7b |
| libmagic | 772f20eee4b88f0a459cd4011bbb9773fda1e01a |
| libpcap | a516cf3c9614d096cf351bfd5239c7f17ab41388 |
| libpng | 7c67f70da1db5f1d7a75cf518d8e5bd704ba89ce |
| libtiff | 5fe20d0e9aba49a6a350ed533459d1505203838f |
| libvpx | cb5a578a5767a95f29990861a9572ccb27f25793 |
| pugixml | 710052066cc0a7210d7f554196ae43bd1cd9da3e |
| re2 | 972a15cedd008d846f1a39b2e88ce48d7f166cbd |
| sqlite3 | db4d8946ef5074c34086983b5a3446bd43cc9c93 |
| tinygltf | bdc37385f198c787ba143e18f01b06164f8c7d15 |
| zlib | 09a1572aa624e5ddb6c075dc013880de70b1b9b9 |
