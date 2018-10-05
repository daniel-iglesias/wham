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

#ifndef MKNIXANALYSISTHERMALDYNAMIC_H
#define MKNIXANALYSISTHERMALDYNAMIC_H

#include <analysis.h>

namespace mknix {

/**
	@author AUTHORS <MAILS>
*/
class AnalysisThermalDynamic : public Analysis
{
public:
    AnalysisThermalDynamic();

    AnalysisThermalDynamic( Simulation*, double, double, double, char*, int );

    ~AnalysisThermalDynamic();

    std::string type() {
        return std::string("THERMAL");
    }

    void init(lmx::Vector< data_type > *);
    void nextStep();
    void resetAndAdvanceStep(lmx::Vector< data_type > *);
    void solve( lmx::Vector<data_type> *, lmx::Vector< data_type >*, lmx::Vector<data_type> * );

private:
    char* integratorType;
    double to, tf, At;
    int max_iter;
    lmx::DiffProblemFirst< Simulation, data_type > theProblem;

};

}

#endif
