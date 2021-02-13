#include "Circular.h"

Circular::Circular(int id, float radius) : MicroOrganism(id, vector<Wall*>()), radius(radius)
{}

Circular::~Circular()
{
    for( int i = 0 ; i < edge.size() ; i++ )
        delete edge[i];
}

void Circular::ConnectToCell(Cell* const cell)
{
    myCell = cell;
    for( int i = 0 ; i < edge.size() ; i++ )
        delete edge[i];
    edge.clear();
    for( int i = 0 ; i < cell->GetCellWall().size() ; i++ )
        edge.push_back(cell->GetCellWall()[i]->Clone());
}

bool Circular::DoesFitInto(const Cell& cell) const
{

    Cell same(cell);
    same.StrengthenCellWall();

    if(same.GetCellWall().size() != 1)
        return false;
    CurvyWall *p = dynamic_cast<CurvyWall*>(same.GetCellWall()[0]);
    cout<<(p->GetRadius())<<"aaaaaaaaaaaaaa\n";
    if(!p)
        return false;
    if( radius != p->GetRadius() )
        return false;
    return true;
}

void Circular::React()
{
    if(myCell == NULL)
        return ;
    Cell same = *myCell;
    same.StrengthenCellWall();
    const Particle point = ((CurvyWall*)same.GetCellWall()[0])->GetInitialParticle();
    const Particle center = ((CurvyWall*)same.GetCellWall()[0])->GetCenter();
    float x_diff = point.X() - center.X();
    float y_diff = point.Y() - center.Y();
    Particle newPoint(x_diff+point.X(), y_diff+point.Y());
    CurvyWall *newWall = new CurvyWall(newPoint, newPoint, center);
    vector<Wall*> vc;
    vc.push_back(newWall);
    myCell->RenewCellWall(vc);
}

bool Circular::DoesContain(const Particle& p) const
{
    if(!myCell)
        return false;
    const Particle center = ((CurvyWall *)edge[0])->GetCenter();
    float dist = center.FindDistance(p);
    if(dist <= radius)
        return true;
    return false;
}
