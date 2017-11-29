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

#include "bodyflex.h"
#include "node.h"
#include "simulation.h"

namespace mknix {

FlexBody::FlexBody()
    : Body()
    , formulation( "NONLINEAR" )
    , computeStress(0)
    , computeEnergy(0)
{
}


FlexBody::FlexBody( std::string title_in )
    : Body( title_in )
    , formulation( "NONLINEAR" )
    , computeStress(0)
    , computeEnergy(0)
{
}


FlexBody::~FlexBody()
{
   for( int i = 0 ; i < points.size(); i++)
     delete points[i];
   points.clear();
 
   for( int i = 0 ; i < stress.size(); i++)
     delete stress[i];
   stress.clear();

   for( int i = 0 ; i < energy.size(); i++)
     delete energy[i];
   energy.clear();
}

void FlexBody::addPoint( const Node* node_in ) // problems with munmap_chunk(). Use the later function
{   // Creates or redefines a point, not part of the calculation domain:
//       cout << "addPoint, with points(size) = " << this->points.size() << endl;
    this->points.push_back( new Node(node_in) );
    points.back()->findSupportNodes( this->nodes );
    cout << "POINT ADDED in FLEXBODY:" << points.back()->getNumber()
         << ", Support nodes: " << points.back()->getSupportSize(0) << endl;
    points.back()->setShapeFunType( "MLS" );
    points.back()->shapeFunSolve( "MLS", 1.03 );
}


void FlexBody::addPoint( int nodeNumber, double x, double y, double z, double alpha, double dc )
{   // Creates or redefines a point, not part of the calculation domain:
//       cout << "addPoint, with points(size) = " << this->points.size() << endl;
    this->points.push_back( new Node( nodeNumber, x, y, z ) );
    points.back()->setAlphai( alpha );
    points.back()->setDc( dc );
    
    points.back()->findSupportNodes( this->nodes );
    cout << "POINT ADDED in FLEXBODY:" << points.back()->getNumber()
         << ", Support nodes: " << points.back()->getSupportSize(0) << endl;
    points.back()->setShapeFunType( "MLS" );
    points.back()->shapeFunSolve( "MLS", 1.03 );
}


/**
 * @brief Activates a flag for output data at the end of the analysis.
 *
 * @see outputToFile()
 * @see outputStep()
 * @param outputType_in Keyword of the flag. Options are: [STRESS, ENERGY]
 * @return void
 **/
void FlexBody::setOutput(std::string outputType_in )
{
    if(outputType_in == "STRESS")
        computeStress = 1;
    else if(outputType_in == "ENERGY")
        computeEnergy = 1;
}


/**
 * @brief Streams the data stored during the analysis to a file.
 *
 * @param outFile Output files
 * @return void
 **/
void FlexBody::outputToFile(std::ofstream * outFile)
{
    Body::outputToFile(outFile);

    if( computeStress ) {
        std::vector< lmx::Vector<data_type>* >::iterator itStress;
        int i, vectorSize;

        *outFile << "STRESS " << title << endl;
        for( itStress = stress.begin();
                itStress!= stress.end();
                ++itStress
           )
        {
            vectorSize = (*itStress)->size();
            for( i=0; i<vectorSize; ++i) {
                *outFile << (*itStress)->readElement(i) << " ";
            }
            *outFile << endl;
        }
    }

    if( computeEnergy ) {
        std::vector< lmx::Vector<data_type>* >::iterator itEnergy;
        int i, vectorSize;

        *outFile << "ENERGY " << title << endl;
        for( itEnergy = energy.begin();
                itEnergy!= energy.end();
                ++itEnergy
           )
        {
            vectorSize = (*itEnergy)->size();
            for( i=0; i<vectorSize; ++i) {
                *outFile << (*itEnergy)->readElement(i) << " ";
            }
            *outFile << endl;
        }
    }
}

void FlexBody::writeBodyInfo(std::ofstream* outFile)
{
// TODO: set type to GALERKIN, export the connectivity and read it in mknixPost
//    if(itFlexBodies->second->getType() == "MESHFREE"){
//    *outFile << "\t" << itFlexBodies->second->getType() << " "
    *outFile << "\t" << "MESHFREE" << " "
             << this->title << endl;
    *outFile << "\t" << "\t" << "NODES "
             << this->getNode(0)->getNumber() << " "
             << this->getLastNode()->getNumber() << endl;

    //TODO: implement for 3D
   *outFile << "\t" << "\t" << "BOUNDARY "
       << getBoundarySize() << endl;
   int firstNode = getBoundaryFirstNode()->getNumber();
   int actualNode = firstNode;
   Node* p_actualNode = getBoundaryFirstNode();
   *outFile << "\t" << "\t" << firstNode << " " ;
//    std::map<Node*,Node*>::iterator it_boundary;
//    for( int i=1; i<boundary.size(); ++i )
//    {
//      actualNode = p_actualNode->getNumber();
//      *outFile << actualNode << " " ;
//      cout <<actualNode << endl;
//      p_actualNode = getBoundaryNextNode( p_actualNode );
//    }
// Following code produced SegFault with some meshes. Haven't found why...
   do{
     p_actualNode = getBoundaryNextNode( p_actualNode );
     actualNode = p_actualNode->getNumber();
     *outFile << actualNode << " " ;
     cout <<actualNode << endl;
   } while( actualNode != firstNode );
   *outFile << endl;// << "\t" << "\t" << "ENDBOUNDARY" << endl;

//  }

//    *outFile << "\t" << "END" << itFlexBodies->second->getType() << endl;
    *outFile << "\n\t" << "END" << "MESHFREE" << endl;
}


void FlexBody::writeBoundaryNodes( std::vector<Node*>& boundary_nodes )
{
    std::map<Node*,Node*>::iterator it_boundary;
    for( it_boundary = boundary.begin();
            it_boundary!= boundary.end();
            ++it_boundary )
    {
        boundary_nodes.push_back( it_boundary->first );
    }
}

void FlexBody::writeBoundaryConnectivity( std::vector< std::vector<Node*> >& connectivity_nodes )
{
    std::map<Node*,Node*>::reverse_iterator it_boundary;
    connectivity_nodes.push_back( std::vector< Node* >() );
    Node* first_node = boundary.begin()->first;
    connectivity_nodes.back().push_back( first_node );
    Node* next_node = boundary[first_node];
    while (next_node != first_node)
    {
        connectivity_nodes.back().push_back( next_node );
        next_node = boundary[next_node];
    }
}

}
