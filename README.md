# baby-clang-demo
Some simple but interesting clang plugins/tools for learning.

## Introduction
这学期通过一门课程学习到了 clang/llvm，并基于 clang/llvm 开发了些许解释执行和指针分析的工具。虽然写了不少代码，但都着重在 API 的使用和算法的实现，感觉对于 Clang 和 LLVM 工具的整体结构依然缺乏更深入的了解，以此为契机，我最近参考了仓库 [clang-tutor](https://github.com/banach-space/clang-tutor)，并在其中一部分代码的基础上做了些许修改和注释，以帮助自身更好理解 Clang 相关工具的结构。

## CodeStyleChecker
[传送门](https://github.com/noobone123/baby-clang-demo/tree/main/CodeStyleChecker)
对于该工具，这里直接引用原始仓库中作者的介绍，如下：
> This plugin demonstrates how to use Clang's DiagnosticEngine to generate custom compiler warnings. Essentially, CodeStyleChecker checks whether names of classes, functions and variables in the input translation unit adhere to LLVM's style guide. If not, a warning is printed. For every warning, CodeStyleChecker generates a suggestion that would fix the corresponding issue. This is done with the FixItHint API. SourceLocation API is used to generate valid source location. CodeStyleChecker is robust enough to cope with complex examples like vector.h from STL, yet the actual implementation is fairly compact. For example, it can correctly analyze names expanded from macros and knows that it should ignore user-defined conversion operators.

## Obfuscator
Updating...

(PS: 配合 [Blog](https://noobone123.github.io/2021/%E4%BB%8E%E9%9B%B6%E5%BC%80%E5%A7%8B%E7%9A%84Clang-LLVM/) 使用更佳
