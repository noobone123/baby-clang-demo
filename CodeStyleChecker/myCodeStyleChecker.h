#ifndef CSC_H
#define CSC_H

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"

using namespace clang;

//-------------------------------------------------------
// RecursiveASTVisitor
//-------------------------------------------------------
class CodeStyleCheckerVisitor : public RecursiveASTVisitor<CodeStyleCheckerVisitor> {
public:
    explicit CodeStyleCheckerVisitor(ASTContext &Ctx) : Ctx(Ctx) {}
    bool VisitCXXRecordDecl(CXXRecordDecl *decl);
    bool VisitFunctionDecl(FunctionDecl *decl);
    bool VisitVarDecl(VarDecl *decl);
    bool VisitFieldDecl(FieldDecl *decl);

private:
    ASTContext &Ctx;

    // Checks whether the name in Decl contains any `_`. Issues a warnning if it does.
    void checkNoUnderscoreInName(NamedDecl *decl);
    // Checks whether the name in Decl starts with a lower case letter. Issues a warning if not.
    void checkNameStartsWithLowerCase(NamedDecl *decl);
    // Checks whether the name in Decl starts with an upper case letter. Issues a warning if not.
    void checkNameStartsWithUpperCase(NamedDecl *decl);
};


//-------------------------------------------------------
// ASTConsumer
//-------------------------------------------------------
class CodeStyleCheckerConsumer : public ASTConsumer {
public:
    // 显式声明构造函数，避免传入参数的隐式转换
    explicit CodeStyleCheckerConsumer(ASTContext &Context, SourceManager &SM)
        : visitor(Context), SM(SM) {}
    
    void HandleTranslationUnit(ASTContext &Ctx) {
        // default mode: Visit all translation units
        // visitor.TraverseDecl(Ctx.getTranslationUnitDecl());

        auto Decls = Ctx.getTranslationUnitDecl()->decls();
        for (auto &Decl : Decls) {
            // Ignore declarations out of the main translation unit.
            //
            // SourceManager::isInMainFile method takes into account locations
            // expansion like macro expansion scenario and checks expansion
            // location instead if spelling location if required.
            if (!SM.isInMainFile(Decl->getLocation()))
                continue;
            visitor.TraverseDecl(Decl);
        }
    }

private:
    CodeStyleCheckerVisitor visitor;
    SourceManager &SM;
};


//-------------------------------------------------------
// Following are the Implementation of Visitor's methods
//-------------------------------------------------------
bool CodeStyleCheckerVisitor::VisitCXXRecordDecl(CXXRecordDecl *decl) {
    if (decl->getNameAsString().size() == 0)
        return true;
    
    checkNameStartsWithUpperCase(decl);
    checkNoUnderscoreInName(decl);
    return true;
}

bool CodeStyleCheckerVisitor::VisitFunctionDecl(FunctionDecl *decl) {
    // 运算符重载
    if (isa<CXXConversionDecl>(decl))
        return true;
    checkNameStartsWithLowerCase(decl);
    checkNoUnderscoreInName(decl);
    return true;
}

bool CodeStyleCheckerVisitor::VisitVarDecl(VarDecl *decl) {
    if (isa<ParmVarDecl>(decl) && (decl->getNameAsString().size() == 0))
        return true;
    
    checkNameStartsWithUpperCase(decl);
    checkNoUnderscoreInName(decl);
    return true;
}

bool CodeStyleCheckerVisitor::VisitFieldDecl(FieldDecl *decl) {
    if (decl->getNameAsString().size() == 0)
        return true;
    
    checkNameStartsWithUpperCase(decl);
    checkNoUnderscoreInName(decl);
    return true;
}

void CodeStyleCheckerVisitor::checkNameStartsWithLowerCase(NamedDecl *decl) {
    auto Name = decl->getNameAsString();
    char FirstChar = Name[0];

    if (isLowercase(FirstChar))
        return;
    
    // 基于 FixItHint 和 DiagnosticsEngine 构造 hint
    // 构造 Hint
    std::string Hint = Name;
    Hint[0] = toLowercase(FirstChar);
    FixItHint fixithint = FixItHint::CreateReplacement(
        SourceRange(decl->getLocation(), decl->getLocation().getLocWithOffset(Name.size())),
        Hint
    );

    // 构造 diagnostic
    DiagnosticsEngine &DiagEngine = Ctx.getDiagnostics();
    unsigned DiagID = DiagEngine.getCustomDiagID(
        DiagnosticsEngine::Warning,
        "Function names should start with lower-case letter"
    );
    DiagEngine.Report(decl->getLocation(), DiagID) << fixithint;
}

void CodeStyleCheckerVisitor::checkNameStartsWithUpperCase(NamedDecl *Decl) {
    auto Name = Decl->getNameAsString();
    char FirstChar = Name[0];

    // The actual check
    if (isUppercase(FirstChar))
    return;

    // Construct the hint
    std::string Hint = Name;
    Hint[0] = toUppercase(FirstChar);
    FixItHint FixItHint = FixItHint::CreateReplacement(
        SourceRange(Decl->getLocation(),
                    Decl->getLocation().getLocWithOffset(Name.size())),
        Hint);

    // Construct the diagnostic
    DiagnosticsEngine &DiagEngine = Ctx.getDiagnostics();
    unsigned DiagID = DiagEngine.getCustomDiagID(
        DiagnosticsEngine::Warning,
        "Type and variable names should start with upper-case letter");
    DiagEngine.Report(Decl->getLocation(), DiagID) << FixItHint;
}

void CodeStyleCheckerVisitor::checkNoUnderscoreInName(NamedDecl *Decl) {
    auto Name = Decl->getNameAsString();
    size_t underscorePos = Name.find('_');

    if (underscorePos == StringRef::npos)
    return;

    std::string Hint = Name;
    auto end_pos = std::remove(Hint.begin(), Hint.end(), '_');
    Hint.erase(end_pos, Hint.end());

    FixItHint FixItHint = FixItHint::CreateReplacement(
        SourceRange(Decl->getLocation(),
                    Decl->getLocation().getLocWithOffset(Name.size())),
        Hint);

    DiagnosticsEngine &DiagEngine = Ctx.getDiagnostics();
    unsigned DiagID = DiagEngine.getCustomDiagID(DiagnosticsEngine::Warning,
                                                "`_` in names is not allowed");
    SourceLocation UnderscoreLoc =
        Decl->getLocation().getLocWithOffset(underscorePos);
    DiagEngine.Report(UnderscoreLoc, DiagID).AddFixItHint(FixItHint);
}

#endif