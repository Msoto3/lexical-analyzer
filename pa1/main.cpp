#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <queue> 
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include "lex.h"
 
using namespace std;



int main(int argc, char** argv){
   
    
    
    vector<string> flags;
    
    
    
    string inFile = "";
    string flag="";
    int lineNumber = 0;
    int tokenCount = 0;
    LexItem tok;
    queue<LexItem> tokens;
   
    set<string> Str;
    set<int> Ints;
    set<string> Reals;
    set<string> Idents;
    set<string> bools;
    set<string>::iterator itr;
    set<int>::iterator itr2;
    vector<string> invIdent{"(",")",",","+","-",";","=","END","PROGRAM","BOOL","ELSE","END","FALSE","FLOAT","IF","INT","PRINT","THEN"};
    
    
    
    if (argc > 1)
    {
        inFile = argv[1];
    }

    else
    {
        cout << "NO SPECIFIED INPUT FILE NAME."<<endl;
        return 1;
        
    }

    for(int i=2;i<argc;i++)
    {
       flag = argv[i];
      
       flags.push_back(flag);
        if(flag[0]!='-'){
            cout << "ONLY ONE FILE NAME ALLOWED."<<endl;
            return 1;
        }
        
    }
    
  
    
    

  
    
    if(flag!="-v"&&flag!="-iconst"&&flag!="-rconst"&&flag!="-sconst"&&flag!="-bconst"&&flag!="-ident"&&flag.length()>0)
    {
        cout << "UNRECOGNIZED FLAG "<<flag<<endl;
        return 1;
    }
    
    ifstream file(inFile);
    if(!file)
    {
        cout<<"CANNOT OPEN THE FILE "<<inFile<<endl;
        return 1;
    }
    if (file.peek()==ifstream::traits_type::eof())
    {
        cout << "Lines: 0" << endl;
        return 1;
    }
      while((tok = getNextToken(file, lineNumber)) != DONE && tok != ERR ){
            tokenCount++;
            if (tok==SCONST)
            {
                Str.insert(tok.GetLexeme());
            }
            else if (tok==ICONST)
            {
                Ints.insert(stoi(tok.GetLexeme()));
            }
            else if (tok==RCONST)
            {
                Reals.insert(tok.GetLexeme());
            }
            else if (tok==BCONST)
            {
                bools.insert(tok.GetLexeme());
            }
           
            else{
                Idents.insert(tok.GetLexeme());
            }
            

            
            
            tokens.push(tok);
            

    }
    if (tok==ERR)
    {
        if (tok.GetLexeme()=="*/")
        {
            while (!tokens.empty())
            {
                LexItem item = tokens.front();
                
                
                cout << item << endl;
               
                
                
                tokens.pop();
            }
            
            
             cout << "\nMissing a comment end delimiters '*/' at line "<<lineNumber<<endl;
             cout << "Lines: "<<lineNumber << endl << "Tokens: "<< tokenCount << endl;
             
            return 1;
        }
        
        if (flag=="-rconst"||flag.length()==0)
        {
            while (!tokens.empty())
            {
                LexItem item = tokens.front();
                if (item==RCONST)
                {
                    cout << item << endl;
                }
                
                
                tokens.pop();
            }
            
        }
        
        lineNumber++;
        cout << "Error in line "<<lineNumber<<" "<<"("+tok.GetLexeme()+")"<<endl;
        return 1;
    }
     while (!tokens.empty())
            {
                 if (tokenCount<=3)
                {
                    break;
                }
                LexItem item = tokens.front();
                cout << item << endl;
                
                
                
                tokens.pop();
            }
    
    cout << "Lines: "<<lineNumber << endl << "Tokens: "<< tokenCount << endl;
    if (find(flags.begin(),flags.end(),"-sconst")!=flags.end())
    {
        cout << "STRINGS:"<<endl;
        for (itr = Str.begin();itr!=Str.end();itr++)
        {
            cout <<"\""<< *itr<<"\"" << endl;
        }
    }
    if (find(flags.begin(),flags.end(),"-iconst")!=flags.end())
    {
        cout << "INTEGERS:"<<endl;

        for (itr2 = Ints.begin();itr2!=Ints.end();itr2++)
        {
            cout << *itr2 << endl;
        }
    }
    
    
    if (find(flags.begin(),flags.end(),"-rconst")!=flags.end())
    {
        cout << "REALS:"<<endl;
        for (itr = Reals.begin();itr!=Reals.end();itr++)
        {
            cout << *itr << endl;
        }
    }
    if (find(flags.begin(),flags.end(),"-bconst")!=flags.end())
    {
        cout << "Booleans:"<<endl;
        for (itr = bools.begin();itr!=bools.end();itr++)
        {
            cout << *itr << endl;
        }
        
    }
    
    
    if (find(flags.begin(),flags.end(),"-ident")!=flags.end())
    {
        
        queue<string> container;
        int count = 1;
        cout << "IDENTIFIERS:"<<endl;
        for (itr = Idents.begin();itr!=Idents.end();itr++)
        {
            if (find(invIdent.begin(),invIdent.end(),*itr)==invIdent.end())
            {
                container.push(*itr);
                
            }
            
        
        }
        int size = container.size();
        while (!container.empty())
        {
            string word = container.front();
            if (count==size)
            {
                cout << word << endl; 
            }
            else{
                cout << word << ", ";
                count++;
            }
            container.pop();
            
        }
        
    }
  
   
   
   
}