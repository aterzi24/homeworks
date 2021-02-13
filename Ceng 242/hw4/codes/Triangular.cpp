#include "Triangular.h"

Triangular::Triangular(int id, const vector<Wall*>& edges, const NucleoBase& nb) : MicroOrganism(id, edges), rna(nb)
{}

Triangular::~Triangular()
{
    for( int i = 0 ; i < edge.size() ; i++ )
        delete edge[i];
}

void Triangular::ConnectToCell(Cell* const cell)
{
    myCell = cell;
    for( int i = 0 ; i < edge.size() ; i++ )
        delete edge[i];
    edge.clear();
    for( int i = 0 ; i < cell->GetCellWall().size() ; i++ )
        edge.push_back(cell->GetCellWall()[i]->Clone());
}

bool Triangular::DoesFitInto(const Cell& cell) const
{
    Cell same(cell);
    same.StrengthenCellWall();
    int i, j;

    if(same.GetCellWall().size() != 3)
        return false;

    vector<float> lengs, ls2;
    for( i = 0 ; i < 3 ; i++ )
    {
        CurvyWall *p = dynamic_cast<CurvyWall*>(same.GetCellWall()[i]);
        if(p)
            return false;
        lengs.push_back(same.GetCellWall()[i]->GetLength());
        ls2.push_back(edge[i]->GetLength());
    }

    bool b[3] = {};
    for( i = 0 ; i < 3 ; i++ )
    {
        for( j = 0 ; j < 3 ; j++ )
        {
            if( abs(lengs[i]-ls2[i]) < EPSILON && b[j] == false )
            {
                b[j] = true;
                break;
            }
        }
        if(j == 3)
            return false;
    }
    return true;
}

void Triangular::React()
{
    if(myCell == NULL)
        return ;
    myCell->GetTissue()->RemoveCell(*myCell);
}

const NucleoBase& Triangular::GetRNA() const
{
    return rna;
}

string Triangular::makeStr()
{
    string str;
    str.push_back(rna.nucleo);
    NucleoBase *temp=rna.next;
    while(temp)
    {
        str.push_back(temp->nucleo);
        temp = temp->next;
    }
    return str;
}

void Triangular::reverseStr(string& str)
{
    int n = str.length();

    for (int i = 0; i < n / 2; i++)
        swap(str[i], str[n - i - 1]);
}

bool Triangular::changable(char x, char y)
{
    if(x == 'G' && y == 'S')
        return true;

    if(x == 'S' && y == 'G')
        return true;

    if(x == 'A' && y == 'U')
        return true;

    if(x == 'U' && y == 'A')
        return true;

    return false;
}

void Triangular::Mutate(Triangular& rhs)
{
    string str1, str2;
    bool changed = false;
    str1 = makeStr();
    str2 = rhs.makeStr();

    if(str1[0] != str2[0])
    {
        reverseStr(str2);
        changed = true;
    }

    int start = 0;
    while(str1[start] == str2[start])
    {
        start++;
    }
    int end = str1.size()-1;
    while(str1[end] == str2[end])
    {
        end--;
    }
    string sub1, sub2;
    int i;
    sub1 = str1.substr(1,start-1);
    sub2 = str2.substr(1,start-1);

    for( i = start ; i <= end ; i++ )
        if( changable(str1[i], str2[i]) )
        {
            sub1.push_back(str2[i]);
            sub2.push_back(str1[i]);
        }
    sub1.append(str1.substr(end+1));
    sub2.append(str2.substr(end+1));

    if(changed)
        reverseStr(sub2);

    delete rna.next;
    delete rhs.rna.next;

    NucleoBase *tmp = new NucleoBase(sub1, NULL);
    rna.next = tmp;

    tmp = new NucleoBase(sub2, NULL);
    rhs.rna.next = tmp;

}
