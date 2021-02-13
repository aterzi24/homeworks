#include "NucleoBase.h"

NucleoBase::NucleoBase(string str, NucleoBase *nb) : nucleo(str[0])
{
    if(str.size() == 1)
    {
        next = nb;
        return ;
    }
    next = new NucleoBase(str.erase(0,1), nb);
}

NucleoBase::NucleoBase(const NucleoBase& rhs) : nucleo(rhs.nucleo)
{
    if(rhs.next == NULL)
    {
        next = NULL;
        return ;
    }
    next = new NucleoBase(*rhs.next);
}

NucleoBase::~NucleoBase()
{
    if(next)
        delete next;
}

NucleoBase* NucleoBase::GetLast()
{
    NucleoBase *temp = this;
    while(1)
    {
        if(temp->next == NULL)
            return temp;
        temp = temp->next;
    }
}

ostream& operator<<(ostream& os, const NucleoBase& nb)
{
    NucleoBase *temp = (NucleoBase *)&nb;
    while(temp)
    {
        os << temp->nucleo;
        temp = temp->next;
    }
    return os;
}
