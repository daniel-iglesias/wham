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

#ifndef MKNIXFLEXBODY_H
#define MKNIXFLEXBODY_H

#include "body.h"

namespace mknix {

class Node;

/**
	@author AUTHORS <MAILS>
*/
class FlexBody : public Body {

public:
    FlexBody();

    FlexBody( std::string );

    virtual ~FlexBody();

    virtual Node* getNode( int node_number )
    {   if(node_number<0) return this->points[-1-node_number];
        else return this->nodes[node_number];
    }

    virtual void setType( std::string type_in ) = 0;

    virtual void setFormulation( std::string formulation_in )
    {
        formulation = formulation_in;
        cout << formulation << endl;
    }

//     virtual void initialize( ) = 0;

    virtual void calcInternalForces( ) = 0;

    virtual void calcTangentMatrix( ) = 0;

    virtual void assembleInternalForces( lmx::Vector<data_type> & ) = 0;

    virtual void assembleTangentMatrix( lmx::Matrix<data_type> & ) = 0;

    void addPoint( const Node* );
    
    void addPoint( int, double, double, double, double, double );
    
    virtual int getNumberOfPoints( )
    { return points.size();  }

//     Node* getPoint( int node_number )
//     {
// //       cout << "getPoint( " << node_number << "), with points(size) = " << this->points.size() << endl;
//       return this->points[node_number];
//     }

    void setOutput( std::string );

    void outputToFile( std::ofstream* );

    void writeBodyInfo( std::ofstream* );

    void writeBoundaryNodes( std::vector<Node*>& );

    void writeBoundaryConnectivity(std::vector< std::vector<Node*> >&);

protected:
    std::string formulation;
    std::vector<Node*> points;
    bool computeStress;
    bool computeEnergy;
//     std::vector<Node*> points; /**< Additional points to define loads or constraints */
    lmx::Matrix< data_type > smoothingMassMatrix;
    std::vector< lmx::Vector<data_type>* > stress;
    std::vector< lmx::Vector<data_type>* > energy;
};

}

#endif
