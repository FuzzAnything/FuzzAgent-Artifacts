# ProteusFuzz Artifacts

Evaluation artifacts of **ProteusFuzz** — adaptive agent-guided library fuzzing that generates static
fuzzing harnesses and extends them at runtime when coverage stalls. ProteusFuzz was evaluated
end-to-end on **17 real-world C/C++ libraries** across **5 independent runs** (`round_1` ~ `round_5`),
each run being a full **24-hour end-to-end campaign** per library (harness generation → build →
fuzz → adaptive harness extension → coverage/crash analysis).

All data is organized by run (`round_1` ~ `round_5`) and by target library. The library set is
identical to the PromeFuzz baseline section: c-ares, cjson, curl, lcms, libaom, libjpeg-turbo,
liblouis, libmagic, libpcap, libpng, libtiff, libvpx, pugixml, re2, sqlite3, tinygltf, zlib.

## Data Overview

```
ProteusFuzz/
├── harnesses/          # Final fuzzing harness source files (dispatcher) for each run and library
├── build/              # Per-target library build scripts (aflpp / coverage / asan instrumentations)
└── 25_times_coverage/  # Per-run LLVM source-based coverage (llvm_cov.profdata + index.txt)
```

## How ProteusFuzz differs from the baselines

- **Dispatcher harness layout.** Instead of one harness per file executed independently, the
  harnesses generated in one run are compiled into a *single* dispatcher binary. Each numbered
  harness file `N.c` (or `N.cpp`) defines `LLVMFuzzerTestOneInput_N`, and `entry.c`
  (`entry.cpp`) dispatches on the first input byte: `switch (driverIndex % N)` routes the
  remaining bytes to harness `driverIndex % N`. The number of harnesses can therefore grow
  *during* a run (adaptive extension), and the file set in `harnesses/round_N/<lib>/` is the
  **final** dispatcher state at the end of the 24h campaign.
- **One merged coverage profile per run.** The `llvm_cov.profdata` in
  `25_times_coverage/round_N/<lib>/output_1/` is produced by replaying the **whole campaign
  corpus** (all fuzzing-queue seeds plus all adaptively injected seeds) through the final
  coverage-instrumented dispatcher binary. There are **5 coverage trials per library**
  (`round_1` ~ `round_5`), not 25 — one per end-to-end run.

See the README in each subdirectory for details:

- [harnesses/README.md](harnesses/README.md) — harness structure, dispatcher protocol, and the
  exact compile command used to build the coverage driver for each run.
- [build/README.md](build/README.md) — per-target library build scripts and pinned source
  commits.
- [25_times_coverage/README.md](25_times_coverage/README.md) — how the coverage profiles were
  produced and how to re-export `index.txt`.
