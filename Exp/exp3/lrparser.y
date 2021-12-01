%{
        #include "stdio.h"
        #include "math.h"
        #include "string.h"
        #include "ast.h"
        extern int yylineno;	
        extern char *yytext;
        FILE *yyin;
        past tmp;
        past newExpr(int oper, past left, past right, past next);
        past newIdent(char *strVal, past next);
        past newType(int type);
        past newNum(int value, past next);
        past newAstNode_param(char *strVal, char *nodeType, past left, past right, past next);
        past newAstNode();
        void showAst(past node, int nest);
        void freeAst(past node);
        void yyerror(char* s);
%} 

%union {
	int type_int;
        char* type_char;
	struct _ast *past;
};

%type <past> program CompUnits CompUnit Decl ConstDecl ConstDef ConstDefList BType ConstExpList ConstInitVal
%type <past> VarDecl VarDef InitVal FuncDef FuncType FuncFParams FuncFParam FuncRParams
%type <past> Block BlockItem Stmt Exp Cond LVal PrimaryExp Number UnaryExp UnaryOp MulExp AddExp 
%type <past> RelExp EqExp LAndExp LOrExp ConstExp MConstExp FuncRParam 
%type <past> ConstInitValList VarDefList InitValList ArrIndex ArrIndexList ElseStmt MulOp AddOp EqOp RelOp ArrayDecl BlockItems

%token <type_char> Ident
%token <type_int> IntConst

%token INT RETURN VOID CONST IF ELSE CONTINUE WHILE BREAK
%token ADD SUB MUL DIV MOD LT GT LE GE ASSIGN NE EQ OR AND NOT LP RP SLP SRP MLP MRP COMMA SEMI COMMENT COMMENTS

%left  ASSIGN
%left OR
%left AND 
%left LT GT LE GE EQ NE
%left ADD SUB 
%left MUL DIV MOD
%right NOT
%left SLP SRP MLP MRP


%%
program: CompUnits {$$=newAstNode_param("","CompUnit",$1,NULL,NULL);if($$){showAst($$,0);freeAst($$);printf("Program done successfully!\n");}else printf("Error!\n");}
        ;
CompUnits: {$$=NULL;}
        | CompUnit {$$ = $1;}
        | CompUnits CompUnit {if($1->flag == 0){$1->flag++;$1->next = $2;$$ = $1;$1->tail = $1->next;}else {$1->tail->next = $2;$$ = $1;$1->tail = $1->tail->next;}}
        ;
CompUnit: Decl {$$ = $1;}
        | FuncDef {$$ = $1;}
        | COMMENT CompUnit {$$ = $2;}
        | COMMENTS CompUnit {$$= $2;}
        ;
Decl:   ConstDecl {$$=$1;}
        | VarDecl {$$=$1;}
        ;
ConstDecl: CONST BType ConstDefList SEMI {$$=newAstNode_param("", "ConstDecl", $2, $3, NULL);}
        ;
ConstDefList: ConstDef {$$ = $1;}
        | ConstDefList COMMA ConstDef {if($1->flag == 0){$1->flag++;$1->next = $3;$$ = $1;$1->tail = $1->next;}else {$1->tail->next = $3;$$ = $1;$1->tail = $1->tail->next;}}
        ;
BType: INT {$$=newType(INT);}
        ;
ConstDef: Ident ASSIGN ConstInitVal {tmp = newIdent($1,NULL);$$ = newAstNode_param("", "ConstDef",tmp, $3, NULL);}
        | Ident ConstExpList ASSIGN ConstInitVal {tmp = newIdent($1,NULL);tmp ->nodeType = "ArrayID";tmp ->left = $2;$$ = newAstNode_param("", "ConstDef", tmp, $4, NULL);}
        ;
ConstExpList: MConstExp {$$ = $1;}
        | ConstExpList MConstExp {if($1->flag == 0){$1->flag++;$1->next = $2;$$ = $1;$1->tail = $1->next;}else {$1->tail->next = $2;$$ = $1;$1->tail = $1->tail->next;}}
        ;
MConstExp:MLP ConstExp MRP {$$ = newAstNode_param("", "ArrayIndex", $2, NULL, NULL);}
        ;
ConstInitVal: ConstExp {$$ = newAstNode_param("", "ConstInitVal", $1, NULL, NULL);}
        | LP RP {$$ = newAstNode_param("", "ConstInitVal", NULL, NULL, NULL);}
        | LP ConstInitValList RP {$$=newAstNode_param("", "ConstInitVal", $2, NULL, NULL);}
        ;
ConstInitValList: ConstInitVal {$$ = $1;}
        | ConstInitValList COMMA ConstInitVal {if($1->flag == 0){$1->flag++;$1->next = $3;$$ = $1;$1->tail = $1->next;}else {$1->tail->next = $3;$$ = $1;$1->tail = $1->tail->next;}}
        ;
VarDecl: BType VarDefList SEMI {$$=newAstNode_param("", "VarDecl", $1, $2, NULL);}
        ;
VarDefList:VarDef {$$ = $1;}
        | VarDefList COMMA VarDef { if($1->flag == 0){$1->flag++;$1->next = $3;$$ = $1;$1->tail = $1->next;}else {$1->tail->next = $3;$$ = $1;$1->tail = $1->tail->next;}}
        ;
VarDef: ArrayDecl { $$=newAstNode_param("", "VarDef", $1,NULL, NULL);}
        | ArrayDecl ASSIGN InitVal { $$=newAstNode_param("", "VarDef", $1,$3, NULL);}
        ;
ArrayDecl:Ident {$$=newIdent($1,NULL);}
        | Ident ConstExpList {tmp = newIdent($1,NULL);tmp -> nodeType = "ArrayID";tmp ->left = $2;$$ = tmp;}
        ;
InitVal: Exp { $$ = newAstNode_param("", "InitVal", $1, NULL, NULL);}
        | LP RP { $$ = newAstNode_param("", "InitVal", NULL, NULL, NULL);}
        | LP InitValList RP { $$ = newAstNode_param("", "InitVal", $2, NULL, NULL);}
        ;
InitValList: InitVal { $$ = $1;}
        | InitValList COMMA InitVal { if($1->flag == 0){$1->flag++;$1->next = $3;$$ = $1;$1->tail = $1->next;}else {$1->tail->next = $3;$$ = $1;$1->tail = $1->tail->next;}}
        ;        
FuncDef: FuncType Ident SLP SRP Block { tmp = newIdent($2,NULL);tmp->nodeType = "FuncName";$1->next = tmp;$$ = newAstNode_param("","FuncDef",$1,$5,NULL);}
        | BType Ident SLP SRP Block { tmp = newIdent($2,$5);tmp->nodeType = "FuncName";$$ = newAstNode_param("","FuncDef",$1,tmp,NULL);}
        | FuncType Ident SLP FuncFParams SRP Block { tmp = newIdent($2,NULL);tmp->next = newAstNode_param("","FuncFParams",$4,NULL,NULL);tmp->nodeType = "FuncName";$1->next = tmp;$$ = newAstNode_param("","FuncDef",$1,$6,NULL);}
        | BType Ident SLP FuncFParams SRP Block { tmp = newIdent($2,NULL);tmp->next = newAstNode_param("","FuncFParams",$4,NULL,NULL);tmp->nodeType = "FuncName";tmp->next->next = $6;$$ = newAstNode_param("","FuncDef",$1,tmp,NULL);}
        ; 
FuncType: VOID { $$ = newType(VOID);}
        ;
FuncFParams: FuncFParam { $$ = $1;}
        | FuncFParams COMMA FuncFParam { if($1->flag == 0){$1->flag++;$1->next = $3;$$ = $1;$1->tail = $1->next;}else {$1->tail->next = $3;$$ = $1;$1->tail = $1->tail->next;}}
        ;
FuncFParam: BType Ident { $$ = newAstNode_param("","FuncFParam",$1,newIdent($2,NULL),NULL);}
        | BType Ident ArrIndex { tmp = newIdent($2,NULL);tmp->left = $3;tmp->nodeType = "ArrayID";$$ = newAstNode_param("","FuncFParam",$1,tmp,NULL);}
        ; 
ArrIndex: MLP MRP { $$ = newAstNode_param("","ArrayIndex",NULL,NULL,NULL);}
        | ArrIndex ArrIndexList{ if($1->flag == 0){$1->flag++;$1->next = $2;$$ = $1;$1->tail = $1->next;}else {$1->tail->next = $2;$$ = $1;$1->tail = $1->tail->next;} }
        ;
ArrIndexList: MLP Exp MRP { $$ = newAstNode_param("","ArrayIndex",$2,NULL,NULL);}
        | ArrIndexList MLP Exp MRP { if($1->flag == 0){$1->flag++;$1->next = $3;$$ = $1;$1->tail = $1->next;}else {$1->tail->next = $3;$$ = $1;$1->tail = $1->tail->next;}}
        ;
Block: LP RP { $$ = newAstNode_param("","Block",NULL,NULL,NULL);}
        | LP BlockItems RP { $$ = newAstNode_param("","Block",$2,NULL,NULL);}
        ;
BlockItems: BlockItem { $$ = $1;}
        | BlockItems BlockItem { if($1->flag == 0){$1->flag++;$1->next = $2;$$ = $1;$1->tail = $1->next;}else {$1->tail->next = $2;$$ = $1;$1->tail = $1->tail->next;}}
        ;
BlockItem: Decl { $$ = $1;}
        | Stmt { $$ = $1;}
        | COMMENT BlockItem {$$ = $2;}
        | COMMENTS BlockItem {$$=$2;}
        ;
Stmt: LVal ASSIGN Exp SEMI { $$ = newAstNode_param("","AssignStmt",$1,$3,NULL);}
        | Exp SEMI { $$ = $1;}
        | SEMI { $$ = newAstNode_param("","EmptyStmt",NULL,NULL,NULL);}
        | Block { $$ = $1;}
        | IF SLP Cond SRP Stmt { $$ = newAstNode_param("","IfStmt",$3,$5,NULL);}
        | IF SLP Cond SRP Stmt ElseStmt { $3->next = $5;$$ = newAstNode_param("","IfStmt",$3,$6,NULL);}
        | WHILE SLP Cond SRP Stmt { $$=newAstNode_param("","WhileStmt",$3,$5,NULL);}
        | BREAK SEMI { $$=newAstNode_param("","BreakStmt",NULL,NULL,NULL);}
        | CONTINUE SEMI { $$ = newAstNode_param("","ContinueStmt",NULL,NULL,NULL);}
        | RETURN SEMI { $$ = newAstNode_param("","ReturnStmt",NULL,NULL,NULL);}
        | RETURN Exp SEMI { $$ = newAstNode_param("","ReturnStmt",$2,NULL,NULL);}
        ;
ElseStmt: ELSE Stmt { $$ = newAstNode_param("","ElseStmt",$2,NULL,NULL);}
        ;
Exp: AddExp { $$ = $1;}
        ;
Cond: LOrExp {$$ = $1;}
        ;
LVal: Ident { $$ = newIdent($1,NULL);}
        | Ident ArrIndexList { tmp = newIdent($1,NULL);tmp->left = $2;tmp->nodeType = "ArrayID";$$=tmp;}
        ;
PrimaryExp: SLP Exp SRP { $$ = $2;}
        | LVal { $$ = $1;}
        | Number { $$ =$1;}
        ;
Number: IntConst { $$ = newNum($1,NULL);}
        ;
UnaryExp: PrimaryExp { $$ = $1;}
        | Ident SLP SRP { tmp = newIdent($1,NULL);tmp->nodeType = "FuncName";$$ = newAstNode_param("","FuncRefer",tmp,NULL,NULL);}
        | Ident SLP FuncRParams SRP { tmp = newIdent($1,NULL);tmp->nodeType = "FuncName";$$ = newAstNode_param("","FuncRefer",tmp,newAstNode_param("","FuncRParams",$3,NULL,NULL),NULL);}
        | UnaryOp UnaryExp { $1->right = $2;$1->nodeType = "PreExpr";$$ = $1;}
        ;
UnaryOp: ADD { $$ = newExpr(ADD, NULL, NULL, NULL);}
        | SUB { $$ = newExpr(SUB, NULL, NULL, NULL);}
        | NOT { $$ = newExpr(NOT, NULL, NULL, NULL);}
        ;
FuncRParams: FuncRParam { $$ = $1;}
        | FuncRParams COMMA FuncRParam { if($1->flag == 0){$1->flag++;$1->next = $3;$$ = $1;$1->tail = $1->next;}else {$1->tail->next = $3;$$ = $1;$1->tail = $1->tail->next;}}
        ;
FuncRParam:Exp {$$ = newAstNode_param("","FuncRParam",$1,NULL,NULL);}
        ;
MulExp: UnaryExp { $$ = $1;}
        | MulExp MulOp UnaryExp { $2->left = $1;$2->right = $3;$$ =$2;}
        ;
MulOp:MUL { $$ = newExpr(MUL, NULL, NULL, NULL);}
        | DIV { $$ = newExpr(DIV, NULL, NULL, NULL);}
        | MOD { $$ = newExpr(MOD, NULL, NULL, NULL);}
        ;
AddExp: MulExp { $$ = $1;}
        | AddExp AddOp MulExp { $2->left = $1;$2->right = $3;$$ =$2;}
        ;
AddOp:ADD { $$ = newExpr(ADD, NULL, NULL, NULL);}
        | SUB { $$ = newExpr(SUB, NULL, NULL, NULL);}
        ;
RelExp: AddExp { $$ = $1;}
        | RelExp RelOp AddExp { $2->left = $1;$2->right = $3;$$ =$2;}
        ;
RelOp:LT { $$ = newExpr(LT, NULL, NULL, NULL);}
        | GT { $$ = newExpr(GT, NULL, NULL, NULL);}
        | LE { $$ = newExpr(LE, NULL, NULL, NULL);}
        | GE { $$ = newExpr(GE, NULL, NULL, NULL);}
        ;
EqExp: RelExp { $$ = $1;}
        | EqExp EqOp RelExp { $2->left = $1;$2->right = $3;$$ =$2;}
        ;
EqOp:EQ { $$ = newExpr(EQ, NULL, NULL, NULL);}
        | NE { $$ = newExpr(NE, NULL, NULL, NULL);}
        ;

LAndExp: EqExp { $$ = $1;}
        | LAndExp AND EqExp { $$ = newExpr(AND, $1, $3, NULL);}
        ;
LOrExp: LAndExp { $$ = $1;}
        | LOrExp OR LAndExp { $$ = newExpr(OR, $1, $3, NULL);}
        ;
ConstExp: AddExp { $$ = $1;}
        ;
%%

int main(int argc, char *argv[]){
        yyin=fopen(argv[1],"r");
        if (!yyin) 
                return 0;
	yyparse();
	return 0;
}

void yyerror(char *s)
{
        char * s0=strdup(yytext); 
	int len=strlen(s0);
	int i;
	char buf[512]={0};
	fprintf(stderr, "ERROR: %s at symbol '%s' on line %d\n", s, s0, yylineno);
}
