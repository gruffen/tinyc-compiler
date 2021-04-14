#ifndef AST_H
#define AST_H

typedef int bool;
#define true 1
#define false 0

typedef enum
{eInt, eFloat, eVoid, eChar} Type_e;
typedef enum
{eExpr, eAssign, eRet, eWhile, eDo, eFor, eIf, eCompound, eSemi} Stmt_e;
typedef enum
{eOper, eRef, eIntnum, eFloatnum, eCharnum} Expr_e;
typedef enum
{eUn, eAddi, eMult, eRela, eEqlt, eBracket} Oper_e;
typedef enum
{eVar, eCall} Ref_e;
typedef enum
{eNegative} Unop_e;
typedef enum
{ePlus, eMinus} Addi_e;
typedef enum
{eMul, eDiv} Mult_e;
typedef enum
{eLT, eGT, eLE, eGE} Rela_e;
typedef enum
{eEQ, eNE} Eqlt_e;
typedef enum
{eNon, eAsInt, eAsFloat, eAsChar} Assign_e;

// Program := (MethodList)? MainFunc
struct Program {
	struct IncludeMacro *includeMacro;
	struct MethodDef *methodDef;
	struct MainFunc *mainFunc;
};

// Include
struct IncludeMacro {
	char *headerFile;
	struct IncludeMacro *prev;
};

// Member := (VarDeclList)? (MethodDeclList)? (MethodDefList)?
struct Member {
	struct VarDecl *varDecl;
	struct MethodDecl *methodDecl;
	struct MethodDef *methodDef;
};

// VarDeclList := (VarDecl)+
// VarDecl := Type Ident (= (intnum|floatnum))? ;
struct VarDecl {
	struct Type *type;
	struct Ident *ident;
	Assign_e assignType;
	union {
		int intnum;
		float floatnum;
		char charnum;
	} assigner;
	struct VarDecl *prev;
};

// MethodDeclList := (MethodDecl)+
// MethodDecl := Type id ( (ParamList)? ) ;
struct MethodDecl {
	char *id;
	struct Type *type;
	struct Param *param;
	struct MethodDecl *prev;
};

// MethodDefList := (MethodDef)+
// MethodDef := Type id ( (ParamList)? ) CompoundStmt
struct MethodDef {
	char *id;
	struct Type *type;
	struct Param *param;
	struct CompoundStmt *compoundStmt;
	struct MethodDef *prev;
};

// MainFunc := int main ( ) CompoundStmt
struct MainFunc {
	struct CompoundStmt *compoundStmt;
};

// ParamList := Param (, Param)*
// Param := Type Ident
struct Param {
	struct Type *type;
	struct Ident *ident;
	struct Param *prev;
};

// Ident := id
//        | id [ intnum ]
struct Ident {
	char *id;
	int len; // 0 if scalar
};

// Type := int
//       | float | char | void
//       | id
struct Type {
	char *id; 
	Type_e e;
};

// CompoundStmt := { (VarDeclList)? (StmtList)? }
struct CompoundStmt {
	struct VarDecl *varDecl;
	struct Stmt *stmt;
};

// StmtList := (Stmt)+
// Stmt := ExprStmt
//       | AssignStmt
//       | RetStmt
//       | WhileStmt
//       | DoStmt
//       | ForStmt
//       | IfStmt
//       | CompoundStmt
//       | ;
struct Stmt {
	Stmt_e e;
	union {
		struct ExprStmt *exprStmt;
		struct AssignStmt *assignStmt;
		struct RetStmt *retStmt;
		struct WhileStmt *whileStmt;
		struct DoStmt *doStmt;
		struct ForStmt *forStmt;
		struct IfStmt *ifStmt;
		struct CompoundStmt *compoundStmt;
	} type;
	struct Stmt *prev;
};

// ExprStmt := Expr ;
struct ExprStmt {
	struct Expr *expr;
};

// AssignStmt := RefVarExpr = Expr ;
struct AssignStmt {
	struct RefVarExpr *refVarExpr;
	struct Expr *expr;
};

// RetStmt := return (Expr)? ;
struct RetStmt {
	struct Expr *expr;
};

// WhileStmt := while ( Expr ) Stmt
struct WhileStmt {
	struct Expr *cond;
	struct Stmt *body;
};

// DoStmt := do Stmt while ( Expr ) ;
struct DoStmt {
	struct Expr *cond;
	struct Stmt *body;
};

// ForStmt := for ( Expr ; Expr ; Expr ) Stmt
struct ForStmt {
	struct Expr *init, *cond, *incr;
	struct Stmt *body;
};

// IfStmt := if ( Expr ) Stmt (else Stmt)?
struct IfStmt {
	struct Expr *cond;
	struct Stmt *ifBody;
	struct Stmt *elseBody;
};

// Expr := OperExpr
//       | RefExpr
//       | intnum
//       | floatnum
struct Expr {
	Expr_e e;
	union {
		struct OperExpr *operExpr;
		struct RefExpr *refExpr;
		int intnum;
		float floatnum;
		char charnum;
	} type;
};

// OperExpr := unop Expr
//           | Expr addiop Expr
//           | Expr multop Expr
//           | Expr relaop Expr
//           | Expr eqltop Expr
//           | ( Expr )
struct OperExpr {
	Oper_e e;
	union {
		struct UnOp *un;
		struct AddiOp *addi;
		struct MultOp *mult;
		struct RelaOp *rela;
		struct EqltOp *eqlt;
		struct Expr *bracket;
	} type;
};

// RefExpr := RefVarExpr
//          | RefCallExpr
struct RefExpr {
	Ref_e e;
	union {
		struct RefVarExpr *refVarExpr;
		struct RefCallExpr *refCallExpr;
	} type;
};

// RefVarExpr := (RefExpr .)? IdentExpr
struct RefVarExpr {
	struct RefExpr *refExpr;
	struct IdentExpr *identExpr;
};

// RefCallExpr := (RefExpr .)? CallExpr
struct RefCallExpr {
	struct RefExpr *refExpr;
	struct CallExpr *callExpr;
};

// IdentExpr := id [ Expr ]
//            | id
struct IdentExpr {
	char *id;
	struct Expr *expr; // NULL if scalar
};

// CallExpr := id ( (ArgList)? )
struct CallExpr {
	char *id;
	struct Arg *arg;
};

// ArgList := Expr (, Expr)*
struct Arg {
	struct Expr *expr;
	struct Arg *prev;
};

/** Following definitions are additional structs for OperExpr. **/
struct UnOp {
	Unop_e e;
	struct Expr *expr;
};

struct AddiOp {
	Addi_e e;
	struct Expr *lhs;
	struct Expr *rhs;
};

struct MultOp {
	Mult_e e;
	struct Expr *lhs;
	struct Expr *rhs;
};

struct RelaOp {
	Rela_e e;
	struct Expr *lhs;
	struct Expr *rhs;
};

struct EqltOp {
	Eqlt_e e;
	struct Expr *lhs;
	struct Expr *rhs;
};

#endif