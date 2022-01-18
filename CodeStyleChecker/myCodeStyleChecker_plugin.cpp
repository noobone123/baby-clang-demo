#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

#include "myCodeStyleChecker.h"

using namespace clang;

//-----------------------------------------------------------------------------
// FrontendAction of Clang Plugin
//-----------------------------------------------------------------------------
class CodeStyleCheckerAction : public PluginASTAction {
public:
    std::unique_ptr<ASTConsumer> 
    CreateASTConsumer(CompilerInstance &compiler, llvm::StringRef InFile) override {
        return std::make_unique<CodeStyleCheckerConsumer>(
            compiler.getASTContext(), compiler.getSourceManager());
    }

    bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &Args) override {
        for (StringRef Arg : Args) {
            if (Arg.startswith("-help"))
                PrintHelp(llvm::errs());
        }

        return true;
    }

    void PrintHelp(llvm::raw_ostream &OS) {
        OS << "Help for CodeStyleChecker plugin goes here.\n";
    }
};

//-----------------------------------------------------------------------------
// Registration
//-----------------------------------------------------------------------------
static clang::FrontendPluginRegistry::Add<CodeStyleCheckerAction>
    X(/*Name=*/"CSC",
      /*Description=*/"Checks whether class, variable and function names "
                      "adhere to LLVM's guidelines");