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

#include "readerflex.h"
#include "simulation.h"
#include "bodyflexglobalgalerkin.h"
#include "celltriang.h"
#include "elemtriangle.h"
#include "node.h"
#include "system.h"
#include <sstream>
#include <string>
#include <unistd.h>


namespace mknix {

ReaderFlex::ReaderFlex()
    : theSimulation(0)
    , output(0)
    , input(0)
{
}

ReaderFlex::ReaderFlex( Simulation* simulation_in,
                        std::ofstream & output_in,
                        std::stringstream & input_in,
                        std::stringstream & mesh_in
                      )
    : theSimulation( simulation_in )
    , output(& output_in)
    , input(& input_in)
    , mesh(& mesh_in)
{
}


ReaderFlex::~ReaderFlex()
{
}


} // namespace mknix


void mknix::ReaderFlex::readFlexBodies(System * system_in)
{
    char keyword[20], bodyType[20];
    std::string boundaryType("CLOCKWISE");
    std::string stringKeyword, flexTitle;
    std::string meshfreeFormulation("RPIM");

    while(*input >> bodyType) {
        if(!strcmp(bodyType,"ENDFLEXBODIES")) return;
        else if( !strcmp(bodyType,"MESHFREE") || !strcmp(bodyType,"FEMESH") ) {
            char a;
            *input >> flexTitle;
            *output << "FLEXBODY: "
                    << system_in->getTitle()
                    << "."
                    << flexTitle << std::endl;
            system_in->flexBodies[flexTitle]
                = new FlexGlobalGalerkin( flexTitle );
            system_in->flexBodies[flexTitle]->setType( bodyType );
            system_in->thermalBodies[flexTitle] = system_in->flexBodies[flexTitle];

            while(*input >> keyword) {
                if( !strcmp(keyword,"ENDMESHFREE") || !strcmp(keyword,"ENDFEMESH") )
                    break;
                else if(!strcmp(keyword,"OUTPUT")) {
                    *input >> stringKeyword;
                    if( stringKeyword == "STRESS" ) {
                        system_in->flexBodies[flexTitle]->setOutput( stringKeyword );
                        *output << "OUTPUT: " << stringKeyword << endl;
                    }
                    if( stringKeyword == "ENERGY" ) {
                        system_in->flexBodies[flexTitle]->setOutput( stringKeyword );
                        *output << "OUTPUT: " << stringKeyword << endl;
                    }
                }
                else if(!strcmp(keyword,"FORMULATION")) {
                    *input >> stringKeyword;
                    system_in->flexBodies[flexTitle]->setFormulation( stringKeyword );
                    *output << "FORMULATION: " << stringKeyword << endl;
                    if( !strcmp(bodyType,"MESHFREE") ) {
                    }
                }
                else if(!strcmp(keyword,"METHOD")) {
                    *input >> meshfreeFormulation; // Type of formulation: EFG or RPIM
                    *output << "METHOD SET TO: "
                            << meshfreeFormulation
                            << std::endl;
                }
                else if(!strcmp(keyword,"BOUNDARY")) {
                    *input >> boundaryType;
                    *output << "BOUNDARY: " << boundaryType << endl;
                }
                else if(!strcmp(keyword,"MESH")) {
                    char a;
                    do {
                        input->get(a);
                    }
                    while(a!='\n');
                    int material, nGPs;
                    double alpha;
                    int totalNodes, totalCells;
//          int nodeNumber;
//          double x, y;
//          double z = 0.0; // Initialized for 2-D problems.
                    *output << "MESH:" << endl;
                    *input >> material >> nGPs >> alpha;
                    *output << "Material: " << material << endl;
                    *output << "Number of GPs per cell: " << nGPs << endl;
                    *output << "Influence domain factor (alpha): "  << alpha << endl;
                    do {
                        input->get(a);
                    }
                    while(a!='\n');
                    *input >> keyword;
                    if(!strcmp(keyword,"TRIANGLES")) {
                        *input >> keyword;
                        *output << "FILE: " << keyword << endl;
//                         std::ifstream meshfile( keyword ); // file to read points from
                        int nodeNumberInFile; // read number but do not use it
                        double x, y, z;
                        int old_size = theSimulation->nodes.end()->first;
                        int thermal_old_size = theSimulation->thermalNodes.end()->first;
                        std::map<int,int> nodesFile;
                        *mesh >> totalNodes >> totalCells;
                        *output << '\t' << "Nodes: " << totalNodes << endl;
                        *output << "\tNode\tx\ty\tz \n";
                        for (int i=0; i<totalNodes; ++i) {
                            *mesh >> nodeNumberInFile >> x >> y >> z;
                            nodesFile[nodeNumberInFile] = old_size + i;
                            theSimulation->nodes[old_size + i] =
                                new Node(old_size + i, x , y, z);
                            theSimulation->nodes[old_size + i]->setThermalNumber(thermal_old_size + i);
                            theSimulation->thermalNodes[thermal_old_size + i]
                                = theSimulation->nodes[old_size + i];
                            system_in->flexBodies[flexTitle]
                            ->addNode( theSimulation->nodes[old_size + i] );
                            *output
                                    << '\t' << theSimulation->nodes[old_size + i]->getNumber()
                                    << "\t" << theSimulation->nodes[old_size + i]->getx()
                                    << "\t" << theSimulation->nodes[old_size + i]->gety()
                                    << "\t" << theSimulation->nodes[old_size + i]->getz()
                                    << std::endl;
                        }
                        int elementType, node1, node2, node3, cellCount;
                        *output << '\t' << "Cells: " << totalCells << endl;
                        *output << "\tCell\tnode1\tnode2\tnode3 \n";
//             old_size = theSimulation->cells.end()->first;
                        cellCount = 0;
                        for (int i=0; i<totalCells; ++i) {
                            *mesh >> nodeNumberInFile >> elementType;
                            if(elementType == 203) {
                                *mesh >> node1 >> node2 >> node3;
                                if( !strcmp(bodyType,"MESHFREE") ) {
                                    system_in->flexBodies[flexTitle]
                                    ->addCell(/*old_size +*/ cellCount,
                                                             new CellTriang
                                                             ( theSimulation->materials[material],
                                                               meshfreeFormulation,
                                                               alpha,
                                                               nGPs,
                                                               theSimulation->nodes[nodesFile[node1]],
                                                               theSimulation->nodes[nodesFile[node2]],
                                                               theSimulation->nodes[nodesFile[node3]]
                                                             )
                                             );
                                    *output
                                            << '\t'
                                            << /*old_size +*/ cellCount
                                            << "\t"
                                            << theSimulation->nodes[nodesFile[node1]]->getNumber()
                                            << "\t"
                                            << theSimulation->nodes[nodesFile[node2]]->getNumber()
                                            << "\t"
                                            << theSimulation->nodes[nodesFile[node3]]->getNumber()
                                            << std::endl;
                                }
                                else if( !strcmp(bodyType,"FEMESH") ) {
                                    system_in->flexBodies[flexTitle]
                                    ->addCell( /*old_size +*/ cellCount,
                                                              new ElemTriangle
                                                              ( theSimulation->materials[material],
                                                                alpha,
                                                                nGPs,
                                                                theSimulation->nodes[nodesFile[node1]],
                                                                theSimulation->nodes[nodesFile[node2]],
                                                                theSimulation->nodes[nodesFile[node3]]
                                                              )
                                             );
                                    *output
                                            << '\t'
                                            << /*old_size +*/ cellCount
                                            << "\t"
                                            << theSimulation->nodes[nodesFile[node1]]->getNumber()
                                            << "\t"
                                            << theSimulation->nodes[nodesFile[node2]]->getNumber()
                                            << "\t"
                                            << theSimulation->nodes[nodesFile[node3]]->getNumber()
                                            << std::endl;
                                }

                                ++cellCount;
                            }
                            else if(elementType == 102) {
                                *mesh >> node1 >> node2;
                                if(boundaryType == "CLOCKWISE") {
                                    system_in->flexBodies[flexTitle]->addBoundaryLine
                                    ( system_in->flexBodies[flexTitle]->getNode(node1-1),
                                      system_in->flexBodies[flexTitle]->getNode(node2-1)
                                    );
                                }
                                else if(boundaryType == "COUNTERCLOCKWISE") {
                                    system_in->flexBodies[flexTitle]->addBoundaryLine
                                    ( system_in->flexBodies[flexTitle]->getNode(node2-1),
                                      system_in->flexBodies[flexTitle]->getNode(node1-1)
                                    );
                                }
                                else
                                    cerr << ":::ERROR: BOUNDARY ORIENTATION UNKNOWN:::" << endl;
                                *output << "\t Bound"
                                        << "\t" << node1
                                        << "\t" << node2
                                        << std::endl;
                            }
                            else
                                do {
                                    mesh->get(a);
                                }
                                while(a!='\n');
                        }
                        do {
                            input->get(a);
                        }
                        while(a!='\n');
                    }
                }
            }
            system_in->flexBodies[flexTitle]->initialize();
        } // MESHFREE
    }
}

