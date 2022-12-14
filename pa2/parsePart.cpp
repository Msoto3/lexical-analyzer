/* Implementation of Recursive-Descent Parser
 * parse.cpp
 * Programming Assignment 2
 * Fall 2022
*/

#include "parse.h"
#include "lex.h"
map<string, bool> defVar;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool IdentList(istream& in, int& line){
    bool status = true;
    for (auto itr = defVar.begin(); itr != defVar.end(); itr++){
        if (itr -> second == false){
            status = false;
            break;
        }  

    } 
    return status;
}



//Prog ::= PROGRAM IDENT StmtList END PROGRAM
bool Prog(istream& in, int& line){
    
	bool status;
	LexItem t = Parser::GetNextToken(in, line);
    if(t.GetToken()!=PROGRAM){
        ParseError(line, "Missing PROGRAM.");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if(t.GetToken()!=IDENT){
        ParseError(line, "Missing Program Name.");
        return false;
        
    }
    status = StmtList(in, line);
    
    if (!status){
        ParseError(line, "Incorrect Program Body.");
        return false;
        
    }
    
    
    t = Parser::GetNextToken(in, line);
    if(t!= END){
        ParseError(line, "Missing END.");
        return false;
    }
    
    t = Parser::GetNextToken(in, line);
    if(t!= PROGRAM){
        ParseError(line, "Missing PROGRAM keyword.");
        return false;
        
    }
    
    
    
    
    
    
    return status;
}

//StmtList ::= Stmt; { Stmt; }
bool StmtList(istream& in, int& line){
    bool status = Stmt(in, line);
    if(!status){
		ParseError(line, "Syntactic error in Program Body.");
		return false;
	}
    
    LexItem tok = Parser::GetNextToken(in, line);
    if(tok.GetToken()!=SEMICOL){
       ParseError(line, "Missing Semicolon.");
       return false;
    }
    
    tok = Parser::GetNextToken(in, line);
    if(tok.GetToken()==END){
        Parser::PushBackToken(tok);
        
        
    }
    else if (tok.GetToken()==ELSE)
    {
        Parser::PushBackToken(tok);
        
    }
    
    
    
    
    else{
        Parser::PushBackToken(tok);
        status = StmtList(in,line);
    }
    
    
    return status;
    
    
    
    
    
    
    
    
    
    
	
    
    
   
}

//Stmt ::= DeclStmt | ControlStmt
bool Stmt(istream& in, int& line){
    bool status = false;
    LexItem t = Parser::GetNextToken(in, line);
    
    if(t.GetToken()== INT || t.GetToken() == FLOAT || t.GetToken() == BOOL){
       Parser::PushBackToken(t);
       defVar["declare"] = true;
       defVar["control"] = false;
       status = DeclStmt(in,line);
       if(!status){
           ParseError(line, "Incorrect Decleration.");
           return false;
           
       }
    }
    else if(t.GetToken()== IDENT || t.GetToken() == IF || t.GetToken() == PRINT){
        Parser::PushBackToken(t);
        defVar["control"] = true;
        defVar["declare"] = false;
        status = ControlStmt(in,line);
        if(!status){
           ParseError(line, "Incorrect control Statement.");
           return false;
           
       }
    }
    
 
    return status;
    
    
 
    
}

//DeclStmt ::= ( INT | FLOAT | BOOL ) VarList
bool DeclStmt(istream& in, int& line){
    bool status;
    LexItem t = Parser::GetNextToken(in, line);
    
    switch( t.GetToken() ) {

	case BOOL:
		status = VarList(in,line);
		break;

	case FLOAT:
		status = VarList(in,line);
		break;

	case INT:
		status = VarList(in,line);
		break;
		
	default:
		Parser::PushBackToken(t);
		return false;
	}
    
    
    
    return status;
    
}


//VarList ::= Var { ,Var }
bool VarList(istream& in, int& line){
    bool status = Var(in, line);
    if(!status){
       ParseError(line, "Missing Var.");
       return false;
    
    
    }
    LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok.GetToken() == COMMA) {
		status = VarList(in, line);
	}
    else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
    return status;
    
    
}


//ControlStmt ::= AssigStmt | IfStmt | PrintStmt 
bool ControlStmt(istream& in, int& line) {
	bool status;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {

	case PRINT:
		status = PrintStmt(in, line);
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);
		break;
		
	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
}//End of ControlStmt


//PrintStmt:= PRINT (ExpreList) 
bool PrintStmt(istream& in, int& line) {
	LexItem t;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression list after Print");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	return ex;
}//End of PrintStmt


//IfStmt ::= IF (Expr) THEN StmtList { ELSE StmtList } END IF
bool IfStmt(istream& in, int& line){
    LexItem t = Parser::GetNextToken(in, line);
    if(t!=LPAREN){
        ParseError(line, "Missing Left Parenthesis");
        return false;
    }
    
    bool status = Expr(in, line);
    if( !status ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
    t = Parser::GetNextToken(in, line);
    if(t.GetToken()!=THEN){
        ParseError(line, "Missing THEN");
        return false;
    }
    
   status = StmtList(in,line);
   if(!status){
       t = Parser::GetNextToken(in, line);
       if(t.GetToken()==ELSE){
            status = StmtList(in,line);
            if (!status)
            {
                ParseError(line, "Missing statement for ELSE");
                return false;
            }
            
           
       }
       else{
            Parser::PushBackToken(t);
            ParseError(line, "Missing statement for IF");
            return false;
           
           
           
           
           
       }
       
       
       
   }
    t = Parser::GetNextToken(in, line);
    if(t.GetToken()==ELSE){
         StmtList(in,line);
        
    }
    else{
        Parser::PushBackToken(t);

    }
    t = Parser::GetNextToken(in, line);
    
    if(t.GetToken() == END){
        t = Parser::GetNextToken(in, line);
        
    }
    else{
        ParseError(line, "missing END FOR IF ");
        return false;
       
    }
    
    
    
       
    
    return status;
    
    
   
    
    
    
	
}

//AssignStmt ::= Var = Expr
bool AssignStmt(istream& in, int& line){
	bool status = Var( in, line);
         
            




    if(!status){
        ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
        return false;
        
    }
    LexItem t = Parser::GetNextToken(in, line);
    if(t.GetToken()!=ASSOP){
        Parser::PushBackToken(t);
        ParseError(line, "Missing ASSOP");
        return false;
    }
    status = Expr(in, line);
    if(!status){
        ParseError(line, "Missing right expression");
        return false;
    }
    return status;
}


//Var ::= IDENT
bool Var(istream& in, int& line){
    LexItem t = Parser::GetNextToken(in, line);
    if (defVar.count(t.GetLexeme())>0)
    {
        defVar.at(t.GetLexeme()) = true;
    }
    else{
        defVar[t.GetLexeme()] = false;
    }

    
    

    
    
    if(t.GetToken()!=IDENT){
        ParseError(line, "Undeclared Variable");
        return false;
    }
    if (defVar.at(t.GetLexeme())==false && defVar.at("control")==true )
    {
        ParseError(line, "Undeclared Variable");
        return false;
    }

    if (defVar.at(t.GetLexeme())==true && defVar.at("declare")==true)
    {
        ParseError(line, "Variable Redefinition");
        return false;
    }
    
    

    
    
    
 
    
    return true;
        

    
}

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status;
	status = Expr(in, line);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		status = ExprList(in, line);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList



//Expr ::= LogORExpr ::= LogANDExpr { || LogANDRxpr }
bool Expr(istream& in, int& line){
	bool status = LogANDExpr(in, line);
    if(!status){
        return false;
    }
    
    LexItem t = Parser::GetNextToken(in, line);
    if (t.GetToken() == OR) {
		status = Expr(in, line);
	}
	else if(t.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << t.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(t);
		return true;
	}
    
    return status;
    
}


//LogANDExpr ::= EqualExpr { && EqualExpr }
bool LogANDExpr(istream& in, int& line){
    
    bool status = EqualExpr(in, line);
    if(!status){
        return false;
    }
    
    LexItem t = Parser::GetNextToken(in, line);
    if (t.GetToken() == AND) {
		status = LogANDExpr(in, line);
	}
	else if(t.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << t.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(t);
		return true;
	}
    
    return status;
    
}



//EqualExpr ::= RelExpr [== RelExpr ]
bool EqualExpr(istream& in, int& line){
    bool status = RelExpr(in, line);
    
    if(!status){
        return false;
    }
    
    LexItem t = Parser::GetNextToken(in, line);
    
    if (t.GetToken()==EQUAL)
    {
        status = RelExpr(in,line);
        if (!status)
        {
            ParseError(line, "wrong relexpr after ==");
            return false;
        }
        
    }
    else{
        Parser::PushBackToken(t);
        return true;

    }

    t = Parser::GetNextToken(in, line);
    if (t.GetToken()==EQUAL)
    {
        ParseError(line, "Illegal Equality Expression.");
        return false;
    }
    else{
        Parser::PushBackToken(t);
    }
    
    
    return status;
    
}

// RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
bool RelExpr(istream& in, int& line){
     bool status = AddExpr(in, line);
    
    if(!status){
        return false;
        
    }
    
    LexItem t = Parser::GetNextToken(in, line);

    if (t.GetToken()==LTHAN || t.GetToken()==GTHAN)
    {
        status = AddExpr(in,line);
        if (!status)
        {
            ParseError(line, "wrong addexpr after operand");
            return false;
        }
        
    }
    else{
        Parser::PushBackToken(t);
        return true;

    }

    t = Parser::GetNextToken(in, line);
    if (t.GetToken()==LTHAN || t.GetToken()==GTHAN)
    {
        ParseError(line, "Illegal Relational Expression.");
        return false;
    }
    else{
        Parser::PushBackToken(t);
    }
    
    

   
    
   
    
    
    return status;
    
    
    
}


//AddExpr :: MultExpr { ( + | - ) MultExpr }
bool AddExpr(istream& in, int& line){
    bool status = MultExpr(in, line);
    if(!status){
        return false;
    }
    
    LexItem t = Parser::GetNextToken(in, line);
    if (t.GetToken() == PLUS || t.GetToken() == MINUS) {
		status = AddExpr(in, line);
	}
	else if(t.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << t.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(t);
		return true;
	}
    
    return status;
    
}

//MultExpr ::= UnaryExpr { ( * | / ) UnaryExpr }
bool MultExpr(istream& in, int& line){
    bool status = UnaryExpr(in, line);
    if(!status){
        return false;
    }
    
    LexItem t = Parser::GetNextToken(in, line);
    if (t.GetToken() == MULT || t.GetToken() == DIV) {
		status =  MultExpr(in, line);
	}
	else if(t.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << t.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(t);
		return true;
	}
    
    return status;
    
}


// UnaryExpr ::= ( - | + | ! ) PrimaryExpr | PrimaryExpr
bool UnaryExpr(istream& in, int& line){
    LexItem t = Parser::GetNextToken(in, line);
    bool status;
    
    switch(t.GetToken()){
        case MINUS:
            status = PrimaryExpr( in,line, t.GetToken());
            break;
        case PLUS:
            status = PrimaryExpr( in,line, t.GetToken());
            break;
        case NOT:
            status = PrimaryExpr( in,line, t.GetToken());
            break;
        default:
            Parser::PushBackToken(t);
            status = PrimaryExpr( in,line, t.GetToken());
            break;
          
            
    }
    if(!status){
        return false;
    }
    return status;
    
    
}


//PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | ( Expr )
bool PrimaryExpr(istream& in, int& line, int sign){
    LexItem t = Parser::GetNextToken(in, line);
    bool status;
 
    
    switch(t.GetToken()){
        case IDENT:
            return true;
        case ICONST:
            return true;
        case RCONST:
            return true;
        case SCONST:
            return true;
        case BCONST:
            return true;
        case LPAREN:
            status = Expr(in, line);
            break;
        default:
            Parser::PushBackToken(t);
            return false;       
    }
	
	if( !status ) {
		ParseError(line, "Missing expression after Left Parenthesis");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t.GetToken() != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	return status;
}

















