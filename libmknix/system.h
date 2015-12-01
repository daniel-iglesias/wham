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

#ifndef MKNIXSYSTEM_H
#define MKNIXSYSTEM_H

#include <map>
#include <string>
#include "LMX/lmx.h"
#include "common.h"

namespace mknix {

class Body;
class FlexBody;
class Constraint;
class Load;
class LoadThermal;
class Node;
class Motion;

/**
  @author AUTHORS <MAILS>
*/
class System {

    friend class Reader;
    friend class ReaderFlex;
    friend class ReaderRigid;
    friend class ReaderConstraints;
    friend class Contact;

public:
    System();

    System( const char* );

    ~System();

    bool outputMaxInterfaceTemp;

    std::string getTitle( )
    {
        return this->title;
    }

    virtual Node* getNode( int index)
    {
        return groundNodes[index];
    }
    
    void getThermalNodes( std::vector<double>& );

    void getOutputSignalThermal(double* );

    void updateThermalLoads( double* );

    virtual void update( double );

    void writeRigidBodies( std::ofstream* );

    void writeFlexBodies( std::ofstream* );

    void writeJoints( std::ofstream* );
  
    void calcCapacityMatrix( );

    void calcConductivityMatrix( );

    void calcExternalHeat( );

    void assembleCapacityMatrix( lmx::Matrix<data_type>& );

    void assembleConductivityMatrix( lmx::Matrix<data_type>& );

    void assembleExternalHeat( lmx::Vector<data_type>& );

    void calcMassMatrix( );

    void calcInternalForces( );

    void calcExternalForces( );

    void calcTangentMatrix( );

    void assembleMassMatrix( lmx::Matrix<data_type>& );

    void assembleInternalForces( lmx::Vector<data_type>& );

    void assembleExternalForces( lmx::Vector<data_type>& );

    void assembleTangentMatrix( lmx::Matrix<data_type>& );

    void assembleConstraintForces( lmx::Vector<data_type>& );

    void setMechanical( );

    void outputStep( const lmx::Vector<data_type>&, const lmx::Vector<data_type>& );

    void outputStep( const lmx::Vector<data_type>& );

    void outputToFile( std::ofstream* );

    bool checkAugmented();

    void clearAugmented();

    void writeBoundaryNodes( std::vector<Node*>& );

    void writeBoundaryConnectivity( std::vector< std::vector<Node*> >& );

protected:
    std::string title;
    std::vector<Node*> groundNodes;

    std::map< std::string, System* > subSystems;
    std::map< std::string, FlexBody* > flexBodies;
    std::map< std::string, Body* > thermalBodies;
    std::map< std::string, Constraint* > constraints;
    std::vector< Load* > loads;
    std::vector< LoadThermal* > loadsThermal;
    std::vector< Node* > outputSignalThermal;
    std::vector< Motion* > motions;

    std::map< std::string, System* >::iterator itSubSystems;
    std::map< std::string, FlexBody* >::iterator itFlexBodies;
    std::map< std::string, Body* >::iterator itThermalBodies;
    std::map< std::string, Constraint* >::iterator itConstraints;
    std::vector< Load* >::iterator itLoads;
    std::vector< LoadThermal* >::iterator itLoadsThermal;
    std::vector< Node* >::iterator itOutputSignalThermal;
    std::vector< Motion* >::iterator itMotions;

};

}

#endif
