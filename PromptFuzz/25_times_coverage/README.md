# PromptFuzz Code Coverage
Coverage artifacts of one of the paper's baselines, [PromptFuzz](https://github.com/pvz122/PromeFuzz).

We provided both the raw code coverage data [llvm_cov.profdata](round_1/cjson/output_1/llvm_cov.profdata) and formatted data [index.txt](round_1/cjson/output_1/index.txt) for each project each trial.

To export the code coverage from [llvm_cov.profdata](round_1/cjson/output_1/llvm_cov.profdata), you should use [llvm-cov](https://llvm.org/docs/CommandGuide/llvm-cov.html) built with llvm-21:
```
llvm-cov report --instr-profile=<path_to_profdata> <cov_lib>
```

In this process, you should provide the [source-base code coverage](https://clang.llvm.org/docs/SourceBasedCodeCoverage.html#introduction) instrumented shared binary for this library. Due to different build flags, the code coverage exported might be differ. We highly recommend use our build scripts to build the library: [build script for cJSON](../build/round_1/cjson/build/build.sh).


Note:
- In our paper's evaluation results, we removed the code coverage from third-party libraries (e.g., `absl` for `RE2`). So the code coverage provided in this directory could be higher than the results in our paper. 
