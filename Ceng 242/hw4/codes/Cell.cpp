#include "Cell.h"

Cell::Cell(int id, const vector<Wall*>& cw, Tissue* t) : tissue(t), id(id)
{
    for( unsigned i = 0 ; i < cw.size() ; i++ )
		cellWall.push_back(cw[i]->Clone());
}

Cell::Cell(const Cell& rhs) : tissue(rhs.tissue), id(rhs.id)
{
    for( unsigned i = 0 ; i < rhs.cellWall.size() ; i++ )
		cellWall.push_back(rhs.cellWall[i]->Clone());
}

int Cell::GetCellID() const
{
    return id;
}

Tissue* Cell::GetTissue() const
{
    return tissue;
}

const vector<Wall*>& Cell::GetCellWall() const
{
    return cellWall;
}

void Cell::RenewCellWall(vector<Wall*>& new_cw)
{
    for( unsigned i = 0 ; i < cellWall.size() ; i++ )
        delete cellWall[i];
    cellWall = new_cw;
}

void Cell::StrengthenCellWall()
{
    vector<Wall*> temp;
    const Wall *t;
    unsigned i;
    bool again;
    while(cellWall.size() > 1){
        again = false;
        for( i = 0 ; i < cellWall.size()-1 ; i++ )
        {
            try{
            	t = &(*cellWall[i] + *cellWall[i+1]);
                temp.push_back((Wall *)t);
                again = true;
                delete cellWall[i];
                delete cellWall[i+1];
                i++;
            } catch(ApplePearException e){
                temp.push_back(cellWall[i]);
            }
        }
        if(i != cellWall.size())
        {
            try{
                t = &(*cellWall[i] + *temp[0]);
                again = true;
                delete cellWall[i];
                delete temp[0];
                temp[0] = (Wall *)t;
            } catch(ApplePearException e){
                temp.push_back(cellWall[i]);
            }
        }
        cellWall = temp;
        temp.clear();
        if(again == false)
            break;
    }
}

Cell::~Cell()
{
    for( unsigned i = 0 ; i < cellWall.size() ; i++ )
        delete cellWall[i];
}

ostream& operator<<(ostream& os, const Cell& rhs)
{
    os << rhs.cellWall[0]->GetInitialParticle();

    for( unsigned i = 1 ; i < rhs.cellWall.size() ; i++ ){
        os << " - " << rhs.cellWall[i]->GetInitialParticle();
    }

	return os;
}
