# CodeStyleChecker
## Build
在构建之前，首先你需要安装 clang/llvm，我使用的是 clang13。 
```
mkdir build && cd build
cmake -DCT_Clang_INSTALL_DIR={your_clang/llvm_dir} {your CodeStyleChecker dir}
make
```
## Run
构建完成之后，`build` 目录下应当有两个二进制文件，其中一个是动态链接库 `libCodeStyleChecker.so`，它是 Clang Plugin；另一个是 `ct_CodeStyleChecker`，它是独立的 clang 工具。
1. 对于 Clang Plugin
   ```
   clang -cc1 -load ./libCodeStyleChecker.so -plugin CSC  ../../test/CodeStyleCheckerFunction.cpp
   ```
2. 对于 Clang Tool
   ```
   ./ct_CodeStyleChecker ../../test/CodeStyleCheckerFunction.cpp --
   ```
   (注意上述命令在最后添加的 --，可以让它不去搜索 Compiler Database，从而避免一些 trivial 的报错)