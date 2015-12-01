/***************************************************************************
 *   Copyright (C) 2007-2015 by Daniel Iglesias                            *
 *   https://github.com/daniel-iglesias/mknix                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef MKNIXBODY_H
#define MKNIXBODY_H

#include "common.h"
#include "LMX/lmx.h"

namespace mknix {

class Node;
class Cell;
class LoadThermalBody;

/**
	@author AUTHORS <MAILS>
*/
class Body {

public:
    Body();

    Body( std::string );

    virtual ~Body();

    virtual std::string getType() = 0;

    virtual void initialize( );

    virtual void calcCapacityMatrix( );

    virtual void calcConductivityMatrix( );

    virtual void calcExternalHeat( );

    virtual void assembleCapacityMatrix( lmx::Matrix<data_type> & );

    virtual void assembleConductivityMatrix( lmx::Matrix<data_type> & );

    virtual void assembleExternalHeat( lmx::Vector<data_type> & );

    virtual void calcMassMatrix() = 0;

    virtual void calcExternalForces() = 0;

    virtual void assembleMassMatrix( lmx::Matrix<data_type> & ) = 0;

    virtual void assembleExternalForces( lmx::Vector<data_type> & ) = 0;

    virtual void setMechanical()
    {
        this->isThermal = 0;
    }

    virtual void setOutput( std::string ) = 0;

    virtual void outputStep
    ( const lmx::Vector<data_type>&, const lmx::Vector<data_type>& ) = 0;

    virtual void outputStep( const lmx::Vector<data_type>& ) = 0;

    virtual void outputStep( );

    virtual void outputToFile( std::ofstream* );

    virtual void addNode( Node* node_in )
    {
        this->nodes.push_back( node_in );
    }

    virtual int getNodesSize( )
    {
        return this->nodes.size();
    }

    virtual Node* getNode( int node_number )
    {
        return this->nodes[node_number];
    }

    virtual Node* getLastNode( )
    {
        return this->nodes.back();
    }

    virtual void addBoundaryLine( Node* node1, Node* node2 )
    {
        this->boundary[node1] = node2;
//       cout << "boundary: " << node1->getNumber()
//      << " " << node2->getNumber() << endl;
    }

    virtual void addBoundaryConnectivity( std::vector<int> connectivity_in );

    virtual int getBoundarySize( )
    {
        return this->boundary.size();
    }

    virtual Node* getBoundaryFirstNode( )
    {
        return this->boundary.begin()->first;
    }

    virtual Node* getBoundaryNextNode( Node* node_in )
    {
        return this->boundary[node_in];
    }

    virtual void addCell( int num, Cell* cell_in )
    {
        this->cells[num] = cell_in;
    }

    virtual void writeBodyInfo( std::ofstream* ) = 0;

    virtual void writeBoundaryNodes( std::vector<Node*>& ) = 0;

    virtual void writeBoundaryConnectivity(std::vector< std::vector<Node*> >&) = 0;

    virtual void translate( double, double, double );

    // Temporary, should be a pointer to a load class
    virtual void setLoadThermal( LoadThermalBody* theLoad )
    {
        loadThermalBody = theLoad;
    }


protected:
    std::string title;
    std::vector<Node*> nodes;
    std::vector< std::vector<int> >boundaryConnectivity;
    std::map<Node*,Node*> boundary; /**< Map of linear boundary. */
    std::map<int,Cell*> cells; /**< Map of integration cells. */
    bool isThermal;
    bool computeEnergy;
    std::vector< lmx::Vector<data_type>* > temperature;
    LoadThermalBody* loadThermalBody;

};

}

#endif
