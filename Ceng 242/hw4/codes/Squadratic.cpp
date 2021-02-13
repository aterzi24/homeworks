#include "Squadratic.h"

Squadratic::Squadratic(int id, float l) : MicroOrganism(id, vector<Wall*>()), edge_length(l)
{}

Squadratic::Squadratic(const Squadratic& rhs) : MicroOrganism(rhs.id, rhs.edge), edge_length(rhs.edge_length)
{}

Squadratic::~Squadratic()
{
    for( int i = 0 ; i < edge.size() ; i++ )
        delete edge[i];
}

void Squadratic::ConnectToCell(Cell* const cell)
{
    myCell = cell;
    for( int i = 0 ; i < edge.size() ; i++ )
        delete edge[i];
    edge.clear();
    for( int i = 0 ; i < cell->GetCellWall().size() ; i++ )
        edge.push_back(cell->GetCellWall()[i]->Clone());
}

bool Squadratic::DoesFitInto(const Cell& cell) const
{
    Cell same(cell);
    same.StrengthenCellWall();

    vector<Wall*> cw = same.GetCellWall();
    if(cw.size() != 4)
        return false;

    float ang;
    for( int i = 0 ; i < 4 ; i++ )
    {
        CurvyWall *p = dynamic_cast<CurvyWall*>(cw[i]);
        if(p)
            return false;
        if(cw[i]->GetLength() != edge_length)
            return false;
        if(i == 3)
            ang = cw[i]->FindAngleBetween(*cw[0]);
        else
            ang = cw[i]->FindAngleBetween(*cw[i+1]);
        if( abs(ang - 3.14159265/2) >= EPSILON )
            return false;
    }
    return true;
}

void Squadratic::React()
{
    if(myCell == NULL)
        return ;
    vector<Particle> vc;
    int i;
    for( i = 0 ; i < 4 ; i++ )
    {
        float x1 = edge[i]->GetInitialParticle().X();
        float y1 = edge[i]->GetInitialParticle().Y();
        float x2 = edge[i]->GetFinalParticle().X();
        float y2 = edge[i]->GetFinalParticle().Y();
        vc.push_back( Particle((x1+x2)/2, (y1+y2)/2) );
    }
    float x = (vc[0].X()+vc[2].X())/2;
    float y = (vc[0].Y()+vc[2].Y())/2;
    Particle p(x,y);

    for( i = 0 ; i < 4 ; i++ )
    {
        Squadratic *temp = new Squadratic(id+1+i, edge_length/2);
        vector<Wall*> walls;
        walls.push_back(new Wall(p, vc[i]));
        walls.push_back(new Wall(vc[i], edge[i]->GetFinalParticle()));
        if(i != 3)
        {
            walls.push_back(new Wall(edge[i]->GetFinalParticle(), vc[i+1]));
            walls.push_back(new Wall(vc[i+1], p));
        }
        else
        {
            walls.push_back(new Wall(edge[i]->GetFinalParticle(), vc[0]));
            walls.push_back(new Wall(vc[0], p));
        }
        temp->edge = walls;
        children.push_back(temp);
    }
}

Squadratic& Squadratic::GetChild(float xmin, float xmax, float ymin, float ymax) const
{
    if(children.size() == 0)
        throw NotBornChildException();
    for( int i = 0 ; i < 4 ; i++ )
    {
        float x1, x2, y1, y2;
        Particle p = children[i]->edge[0]->GetInitialParticle();
        x1 = x2 = p.X();
        y1 = y2 = p.Y();
        for( int j = 1 ; j < 4 ; j++ )
        {
            p = children[i]->edge[j]->GetInitialParticle();
            if(p.X() < x1)
                x1 = p.X();
            else if(p.X() > x2)
                x2 = p.X();
            if(p.Y() < y1)
                y1 = p.Y();
            else if(p.Y() > y2)
                y2 = p.Y();
        }
        if(xmin == x1 && xmax == x2 && ymin == y1 && ymax == y2)
            return *children[i];

        if(x1 <= xmin && x2 >= xmax && y1 <= ymin && y2 >= ymax)
            return children[i]->GetChild(xmin, xmax, ymin, ymax);
    }
    throw NotBornChildException();
}
