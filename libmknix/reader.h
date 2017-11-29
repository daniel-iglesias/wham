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

#ifndef MKNIXREADER_H
#define MKNIXREADER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <unistd.h>

namespace mknix {

class ReaderRigid;
class ReaderFlex;
class ReaderConstraints;
class Simulation;
class System;

/**
	@author AUTHORS <MAILS>
*/
class Reader
{
public:
    Reader();

    Reader( Simulation* );

    ~Reader();

    void generateTile(char*, char*, char*, char*);
    void generateTile6_104();
    void generateTile6_109();
    void readInput();

private:
    void readSystem( System* );
    void readBodyPoints( System* );
    void readLoads( System* );
    void readEnvironment( System* );
    void readMotion( System* );
    void readAnalysis();


private:
    Simulation * theSimulation;
    std::ofstream output;
    std::stringstream input; //
    std::stringstream mesh; // 
    std::stringstream thermalcapacity; // 
    std::stringstream thermalconductivity; // 
    ReaderRigid * theReaderRigid; //used to read RIGIDBODIES
    ReaderFlex * theReaderFlex; //used to read FLEXBODIES
    ReaderConstraints * theReaderConstraints; //used to read CONSTRAINTS
};

}

#endif
