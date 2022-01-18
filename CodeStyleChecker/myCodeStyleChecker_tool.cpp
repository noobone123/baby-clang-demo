#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"

#include "myCodeStyleChecker.h"

using namespace clang;
using namespace clang::tooling;
using namespace llvm;

// ----------------------------------------------------------------
// Clang Tooling 的 FrontAction, 和 Clang Plugin 的使用方法类似
// ----------------------------------------------------------------
class CodeStyleCheckerAction : public ASTFrontendAction {
public:
    virtual std::unique_ptr<ASTConsumer> 
    CreateASTConsumer(CompilerInstance &compiler, llvm::StringRef InFile) {
        return std::make_unique<CodeStyleCheckerConsumer>(
            compiler.getASTContext(), compiler.getSourceManager());
    }
};

// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("CodeStyleChecker-tool options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n"); 


int main(int Argc, const char **Argv) {
    // CommonOptionsParser constructor will parse arguments and create a
    // CompilationDatabase.  In case of error it will terminate the program.
    auto ExpectedParser = CommonOptionsParser::create(Argc, Argv, MyToolCategory);
    if (!ExpectedParser) {
        // Fail gracefully for unsupported options.
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }
    CommonOptionsParser& OptionsParser = ExpectedParser.get();
    // Use OptionsParser.getCompilations() and OptionsParser.getSourcePathList()
    // to retrieve CompilationDatabase and the list of input file paths.

    // A clang tool can run over a number of sources in the same process...
    // std::vector<std::string> Sources;
    // Sources.push_back("a.cc");
    // Sources.push_back("b.cc");
    // We hand the CompilationDatabase we created and the sources to run over into
    // the tool constructor.
    // ClangTool Tool(OptionsParser.getCompilations(), Sources);
    ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());
    return Tool.run(newFrontendActionFactory<CodeStyleCheckerAction>().get());
}