#include "MicroOrganism.h"

MicroOrganism::MicroOrganism(int id, const vector<Wall*>& edges) : myCell(NULL), id(id)
{
    for( unsigned i = 0 ; i < edges.size() ; i++ )
        edge.push_back(edges[i]->Clone());
}

ostream& operator<<(ostream& os, const MicroOrganism& rhs)
{
    if(rhs.myCell)
        os << "The microorganism " << rhs.id << " was successfully placed into the cell " << rhs.myCell->GetCellID() << '.';
    else
        os << "The microorganism " << rhs.id << " could not be placed into any cell!";
    return os;
}

MicroOrganism::~MicroOrganism()
{}
