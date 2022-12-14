
#include <iostream>

#include "val.h"
using namespace std;

using namespace std; 

Value Value::operator!() const {
    Value noMatch;
    if (this->GetType()==VBOOL)
        return this->GetBool()?false:true;
    return noMatch;
}

Value Value::operator&&(const Value& op) const {
    Value noMatch;
    if(this->GetType()==VBOOL&& op.GetType()==VBOOL){
        return this->Btemp&&op.Btemp?true:false;
    }
    return noMatch;
  
}

Value Value::operator||(const Value& op) const {
    Value noMatch;
    if(this->GetType()==VBOOL&&op.GetType()==VBOOL){
        return this->Btemp||op.Btemp?true:false;
    }
    return noMatch;

   
}

Value Value:: operator-(const Value& op) const{
    Value noMatch;
   if (this->GetType()==VINT&&op.GetType()==VINT)
   {
        return this->Itemp - op.Itemp;
   }
   else if (this->GetType()==VREAL&&op.GetType()==VINT)
   {
        return this->Rtemp - op.Itemp;
   }
   else if (this->GetType()==VINT&&op.GetType()==VREAL)
   {
        return this->Itemp - op.Rtemp;
   }
   else if (this->GetType()==VREAL&&op.GetType()==VREAL)
   {
        return this->Rtemp - op.Rtemp;
   }
   
   
   
    
   
   return noMatch;
    
}

Value Value:: operator*(const Value& op) const{
     Value noMatch;
    if (this->GetType()==VINT&&op.GetType()==VINT)
    {
            return this->Itemp * op.Itemp;
    }
    else if (this->GetType()==VREAL&&op.GetType()==VINT)
    {
            return this->Rtemp * op.Itemp;
    }
    else if (this->GetType()==VINT&&op.GetType()==VREAL)
    {
            return this->Itemp * op.Rtemp;
    }
    else if (this->GetType()==VREAL&&op.GetType()==VREAL)
    {
            return this->Rtemp * op.Rtemp;
    }
    
    
    
        
    
    return noMatch;

}

Value Value:: operator+(const Value& op) const{
     Value noMatch;
    if (this->GetType()==VINT&&op.GetType()==VINT)
    {
            return this->Itemp + op.Itemp;
    }
    else if (this->GetType()==VREAL&&op.GetType()==VINT)
    {
            return this->Rtemp + op.Itemp;
    }
    else if (this->GetType()==VINT&&op.GetType()==VREAL)
    {
            return this->Itemp + op.Rtemp;
    }
    else if (this->GetType()==VREAL&&op.GetType()==VREAL)
    {
            return this->Rtemp + op.Rtemp;
    }
    
    
    
        
    
    return noMatch;

}

Value Value:: operator/(const Value& op) const{
     Value noMatch;
    if (this->GetType()==VINT&&op.GetType()==VINT)
    {
            if (op.Itemp==0)
            {
                return noMatch;
            }
            return this->Itemp / op.Itemp;
    }
    else if (this->GetType()==VREAL&&op.GetType()==VINT)
    {
            if (op.Itemp==0)
            {
                return noMatch;
            }
            return this->Rtemp / op.Itemp;
    }
    else if (this->GetType()==VINT&&op.GetType()==VREAL)
    {
            if (op.Rtemp==0)
        {
            return noMatch;
        }
            return this->Itemp / op.Rtemp;
    }
    else if (this->GetType()==VREAL&&op.GetType()==VREAL)
    {
                if (op.Rtemp==0)
                {
                    return noMatch;
                }
            return this->Rtemp / op.Rtemp;
    }
    
    
    
        
    
    return noMatch;

}


Value Value::operator==(const Value& op)const{
    Value noMatch;
    if (this->GetType()==VBOOL&&op.GetType()==VBOOL)
    {
        return this->Btemp==op.Btemp?true:false;
        
    }
    else if (this->GetType()==VSTRING&&op.GetType()==VSTRING)
    {
        return this->Stemp==op.Stemp?true:false;
        
    }
    else if (this->GetType()==VINT&&op.GetType()==VINT)
    {
        return this->Itemp==op.Itemp?true:false;
        
    }
    else if (this->GetType()==VREAL&&op.GetType()==VREAL)
    {
        return this->Rtemp==op.Rtemp?true:false;
        
    }
    else if (this->GetType()==VREAL&&op.GetType()==VINT)
    {
        return this->Rtemp==op.Itemp?true:false;

    }
    else if (this->GetType()==VINT&&op.GetType()==VREAL)
    {
        return this->Itemp==op.Rtemp?true:false;
        
    }

    return noMatch;
    
    
    
    
}

Value Value:: operator>(const Value& op) const{
    Value noMatch;
     if (this->GetType()==VINT&&op.GetType()==VINT)
    {
            return this->Itemp > op.Itemp?true:false;
    }
    else if (this->GetType()==VREAL&&op.GetType()==VINT)
    {
            return this->Rtemp > op.Itemp?true:false;
    }
    else if (this->GetType()==VINT&&op.GetType()==VREAL)
    {
            return this->Itemp > op.Rtemp?true:false;
    }
    else if (this->GetType()==VREAL&&op.GetType()==VREAL)
    {
            return this->GetReal()>op.GetReal()?true:false;
    }
    else if (this->GetType()==VBOOL&&op.GetType()==VBOOL)
    {
        return this->Btemp>op.Btemp?true:false;
    }
    
        
    
    
    return noMatch;

}

Value Value:: operator<(const Value& op) const{
    Value noMatch;
     if (this->GetType()==VINT&&op.GetType()==VINT)
    {
            return this->Itemp < op.Itemp?true:false;
    }
    else if (this->GetType()==VREAL&&op.GetType()==VINT)
    {
            return this->Rtemp < op.Itemp?true:false;
    }
    else if (this->GetType()==VINT&&op.GetType()==VREAL)
    {
            return this->Itemp < op.Rtemp?true:false;
    }
    else if (this->GetType()==VREAL&&op.GetType()==VREAL)
    {
            return this->GetReal()<op.GetReal()?true:false;
    }
    else if (this->GetType()==VBOOL&&op.GetType()==VBOOL)
    {
        return this->Btemp<op.Btemp?true:false;
    }
    
        
    
    
    return noMatch;


}