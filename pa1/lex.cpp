#include <iostream>
#include <regex>
#include <string>
#include "lex.h"
#include <map>

using namespace std;


static map<Token,string> tokenCout{
    {PROGRAM,"PROGRAM"},{PRINT,"PRINT"},
    {INT,"INT"},{END,"END"},{IF,"IF"},
    {FLOAT,"FLOAT"},{BOOL,"BOOL"},{ELSE,"ELSE"},
    {THEN,"THEN"},{TRUE,"TRUE"},{FALSE,"FALSE"},
    {IDENT,"IDENT"},{ICONST,"ICONST"},{RCONST,"RCONST"},
    {SCONST,"SCONST"},{BCONST,"BCONST"},{PLUS,"PLUS"},
    {MINUS,"MINUS"},{MULT,"MULT"},{DIV,"DIV"},{ASSOP,"ASSOP"},
    {LPAREN,"LPAREN"},{RPAREN,"RPAREN"},{COMMA,"COMMA"},
    {EQUAL,"EQUAL"},{GTHAN,"GTHAN"},{LTHAN,"LTHAN"},
    {SEMICOL,"SEMICOL"},{AND,"AND"},{OR,"OR"},{NOT,"NOT"},
    {ERR,"ERR"},{DONE,"DONE"},
};


ostream& operator<<(ostream& out, const LexItem& tok) {
    string *token = &tokenCout[tok.GetToken()];
    out << *token;
   
    if((tok.GetToken() == SCONST) || (tok.GetToken() == RCONST) ||(tok.GetToken() == ICONST) || (tok.GetToken() == IDENT)  ||(tok.GetToken() == BCONST)){
        cout << "(" << tok.GetLexeme() << ")";
        
    }
    
   
        
    return out;

}
LexItem id_or_kw(const string& lexeme, int linenum){
    LexItem res;
   
    if (lexeme=="program"||lexeme=="PROGRAM")
    {
        
        res = LexItem(PROGRAM,lexeme,linenum);
    }
    else if (lexeme=="true"||lexeme=="false" || lexeme=="TRUE"||lexeme=="FALSE")
    {
        res = LexItem(BCONST,lexeme,linenum);
    }
    else if (lexeme=="end"||lexeme=="END")
    {

        
        res = LexItem(END,lexeme,linenum);
    }
    else if (lexeme=="print"||lexeme=="PRINT")
    {
        res = LexItem(PRINT,lexeme,linenum);
    }
    else if (lexeme=="if"||lexeme=="IF")
    {
        res = LexItem(IF,lexeme,linenum);
    }
    else if (lexeme=="int"||lexeme=="INT")
    {
        res = LexItem(INT,lexeme,linenum);
    }
    else if (lexeme=="float"||lexeme=="FLOAT")
    {
        res = LexItem(FLOAT,lexeme,linenum);
    }
    else if (lexeme=="bool"||lexeme=="BOOL")
    {
        res = LexItem(BOOL,lexeme,linenum);
    }
    else if (lexeme=="then"||lexeme=="THEN")
    {
        res = LexItem(THEN,lexeme,linenum);
    }
    else if (lexeme=="else"||lexeme=="ELSE")
    {
        res = LexItem(ELSE,lexeme,linenum);
    }
    else{
        
        res = LexItem(IDENT,lexeme,linenum);
    }
    
    
    return res;    
}
LexItem getNextToken (istream& in, int& linenumber){
    enum TokenState { START, INCOMMENT, INID, INSTRING, ININT, INREAL, SYMBOLS} lexstate = START;
    string lexeme;
    char ch;
    while (in.get(ch)){
        
        switch (lexstate)
        {
        case START:
            if (ch=='\n')
            {
                linenumber++;
            }
            if (isspace(ch))
            {
                continue;
            }
            if (isalpha(ch))
            {
                ch = toupper(ch);
            }
            
            lexeme = ch;
            if (isalpha(ch)||ch=='_')
            {
                if (isalpha(ch))
                {
                    ch = toupper(ch);
                }
                
                lexstate = INID;
                continue;
                
            }
            if (ch=='\"')
            {
                lexstate = INSTRING;
                continue;
            }
            if (isdigit(ch))
            {
                lexstate = ININT;
                continue;
            }
            if (ch=='/'&&char(in.peek())=='*')
            {
                lexstate = INCOMMENT;
                continue;
            }
            if (ch=='.')
            {
                lexstate = INREAL;
                continue;
            }
            if (ch=='+'||ch=='-'||ch=='*'||ch=='/'||ch=='='||ch=='('||ch==')'||ch=='>'||ch=='<'||ch=='&'||ch=='|'||ch=='!'||ch==','||ch==';')
            {
                lexstate = SYMBOLS;
                in.putback(ch);
                continue;
            }
            return LexItem(ERR, lexeme, linenumber);

            
            
            
            
            

            
            
        case INCOMMENT:
            if (ch=='\n')
            {
                
                linenumber++;
                
                

            }
            else if (ch=='*'&&char(in.peek())=='/')
            {
                in.get(ch);
                lexstate = START;
            }
            if (in.peek()==EOF)
            {
                return LexItem(ERR,"*/",linenumber);
            }
            
           
            
            continue;
        case INID:
            if (isalpha(ch)||isdigit(ch)||ch=='_'||ch=='@')
            {
                if (isalpha(ch))
                {
                    ch = toupper(ch);
                }
                
                lexeme+=ch;
                

            }
            else{
                lexstate = START;
                in.putback(ch);
                return id_or_kw(lexeme,linenumber);
            }
            
            break;
        case INSTRING:
            if (regex_match(lexeme+ch,regex("\"[ -~]*\"")))
            {
                lexeme+=ch;
                lexstate = START;
                return LexItem(SCONST,lexeme.substr(1,lexeme.length()-2),linenumber);
                
            }
            else if (regex_match(lexeme+ch,regex("\"[ -~]*")))
            {
                lexeme+=ch;
                
                continue;
                
            }
            else if (ch=='\n')
            {
                return LexItem(ERR,lexeme,linenumber);
            }
            
          
            break;
        case ININT:
            if (isalpha(ch)&&!isalnum(char(in.peek()))&&char(in.peek())!='@'&&char(in.peek())!='_')
            {
                return LexItem(ERR,lexeme+ch,linenumber);
            }
            if (isdigit(ch))
            {
                lexeme+=ch;
            }
            else if (ch=='.')
            {
                lexstate=INREAL;
                in.putback(ch);
                continue;
            }
            else{
                lexstate = START;
                in.putback(ch);
                return LexItem(ICONST,lexeme,linenumber);
            }
            break;
        case INREAL:
            if (isalpha(ch))
            {
                return LexItem(ERR,lexeme+ch,linenumber);
            }
            if (regex_match(lexeme+ch,regex("([0-9]+)\\.([0-9]+)")))

            {
                lexeme+=ch;
            }
            else if (ch=='.')
            {
                lexeme+=ch;
            }
            
            else{
                if (lexeme[lexeme.length()-1]=='.')
                {
                    return LexItem(ERR,lexeme,linenumber);
                }
                lexstate = START;
                in.putback(ch);
                return LexItem(RCONST,lexeme,linenumber);
                
            }
            break;
        case SYMBOLS:
            if (ch=='+')
            {
                return LexItem(PLUS,lexeme,linenumber);
            }
            else if (ch=='-')
            {
                return LexItem(MINUS,lexeme,linenumber);
            }
            else if (ch=='(')
            {
                return LexItem(LPAREN,lexeme,linenumber);
            }
            else if (ch==')')
            {
                return LexItem(RPAREN,lexeme,linenumber);
            }
            else if (ch=='*')
            {
                return LexItem(MULT,lexeme,linenumber);
            }
            else if (ch=='/')
            {
                return LexItem(DIV,lexeme,linenumber);
            }
            else if (ch=='='&&in.peek()!='=')
            {
                return LexItem(ASSOP,lexeme,linenumber);
            }
            else if (ch=='>')
            {
                return LexItem(GTHAN,lexeme,linenumber);
            }
            else if (ch=='<')
            {
                return LexItem(LTHAN,lexeme,linenumber);
            }
            else if (ch=='!')
            {
                return LexItem(NOT,lexeme,linenumber);
            }
            else if (ch==',')
            {
                return LexItem(COMMA,lexeme,linenumber);
            }
            else if (ch==';')
            {
                return LexItem(SEMICOL,lexeme,linenumber);
            }
           else if (ch == '&' && char(in.peek()) == '&')
            {
                in.get(ch);
                return LexItem(AND, lexeme + char(in.peek()), linenumber);
            }
            else if (ch == '|' && char(in.peek()) == '|')
            {
                in.get(ch);
                return LexItem(OR, lexeme + char(in.peek()), linenumber);
            }
            else if (ch == '=' && char(in.peek()) == '=')
            {
                in.get(ch);
                return LexItem(EQUAL, lexeme + char(in.peek()), linenumber);
            }
            
            else{
                lexstate = START;
                return LexItem(ERR,lexeme,linenumber);
            }
            break;
            
            
            
            
            
            
            
            
            
            
            
            
            
            

    

            
            
            
            
            
            
        
        
            

             


            
            

            
            
            
            

            

            
            
            
            
            
            




            

            
        }
       
    }
    
       
    


    return LexItem(DONE, "", linenumber);
}
        
        






