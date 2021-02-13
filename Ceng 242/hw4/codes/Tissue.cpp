#include "Tissue.h"

Tissue::Tissue() : xmin(), xmax(), ymin(), ymax()
{}

Tissue::Tissue(float xmin, float xmax, float ymin, float ymax) : xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax)
{}

Tissue::Tissue(const Tissue& rhs) : cells(rhs.cells), xmin(rhs.xmin), xmax(rhs.xmax), ymin(rhs.ymin), ymax(rhs.ymax)
{}

Tissue::~Tissue()
{}

float* Tissue::GetCoordinates()
{
    float *arr = new float[4];
    arr[0] = xmin;
    arr[1] = xmax;
    arr[2] = ymin;
    arr[3] = ymax;
    return arr;
}

void Tissue::AddCell(Cell& cell)
{
    cells.push_back(&cell);
}

void Tissue::RemoveCell(Cell& cell)
{
    for( int i = 0 ; i < cells.size() ; i++ )
        if(cells[i]->GetCellID() == cell.GetCellID())
        {
            cells.erase(cells.begin()+i);
            return ;
        }
}

const Cell& Tissue::GetCell(int id) const
{
    for( int i = 0 ; i < cells.size() ; i++ )
        if(cells[i]->GetCellID() == id)
            return *cells[i];
    throw DeadCellException();
}

void Tissue::Interact(const vector<MicroOrganism*>& vc)
{
    int i, j;
    for( i = 0 ; i < vc.size() ; i++ )
        for( j = 0 ; j < cells.size() ; j++ )
            if(vc[i]->DoesFitInto(*cells[i]))
            {
                vc[i]->ConnectToCell(cells[i]);
                break;
            }
}
