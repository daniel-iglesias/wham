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

#ifndef MKNIXSIMULATION_H
#define MKNIXSIMULATION_H

#include "LMX/lmx.h"
#include "common.h"
#include "material.h"

namespace mknix {

class Reader;
class Contact;
class System;
class Analysis;
class Node;


/**
  @author AUTHORS <MAILS>
*/
class Simulation {

    friend class Reader;
    friend class ReaderConstraints;
    friend class ReaderFlex;
    friend class ReaderRigid;
    friend class Contact;
    friend class SystemChain;

public:
    Simulation();

    ~Simulation();

    void inputFromFile(char*);
//     void geometryFile(char*);
//     void outputFile(char*);
    std::vector<double> getInterfaceNodesCoords();
    void setOutputFilesDetail(int level_in) // 0 none, 1 only times, 2 all
    { outputFilesDetail = level_in;}
    
    void init(int);
    void setInitialTemperatures(double);
    void initThermalSimulation(Analysis*);
    void solveStep(double *, double * o_output=0);
    void resetStep(double);
    void endSimulation();
    
    void run();

    void runThermalAnalysis(Analysis*);

    void runMechanicalAnalysis(Analysis*);

    void writeSystem();

    void staticThermalResidue  ( lmx::Vector<data_type>& residue,
                                 lmx::Vector<data_type>& q
                               );

    void staticThermalTangent  ( lmx::Matrix<data_type>& tangent_in,
                                 lmx::Vector<data_type>& q
                               );

    bool staticThermalConvergence( lmx::Vector<data_type>& res,
                                   lmx::Vector<data_type>& q
                                 );

    void explicitThermalEvaluation
    ( const lmx::Vector<data_type>& qt
      , lmx::Vector<data_type>& qtdot
      , double time
    );

    void dynamicThermalEvaluation( const lmx::Vector<data_type>& q,
                                   lmx::Vector<data_type>& qdot,
                                   double time
                                 );

    void dynamicThermalResidue  ( lmx::Vector<data_type>& residue,
                                  const lmx::Vector<data_type>& q,
                                  const lmx::Vector<data_type>& qdot,
                                  double time
                                );

    void dynamicThermalTangent  ( lmx::Matrix<data_type>& tangent_in,
                                  const lmx::Vector<data_type>& q,
                                  double partial_qdot,
                                  double time
                                );

    bool dynamicThermalConvergence( const lmx::Vector<data_type>& q,
                                    const lmx::Vector<data_type>& qdot,
                                    double time
                                  );

    bool dynamicThermalConvergenceInThermomechanical
    ( const lmx::Vector<data_type>& q,
      const lmx::Vector<data_type>& qdot,
      double time
    );

    void explicitAcceleration( const lmx::Vector<data_type>& q,
                               const lmx::Vector<data_type>& qdot,
                               lmx::Vector<data_type>& qddot,
                               double time
                             );

    void dynamicAcceleration( const lmx::Vector<data_type>& q,
                              const lmx::Vector<data_type>& qdot,
                              lmx::Vector<data_type>& qddot,
                              double time
                            );

    void dynamicResidue  ( lmx::Vector<data_type>& residue,
                           const lmx::Vector<data_type>& q,
                           const lmx::Vector<data_type>& qdot,
                           const lmx::Vector<data_type>& qddot,
                           double time
                         );

    void dynamicTangent  ( lmx::Matrix<data_type>& tangent_in,
                           const lmx::Vector<data_type>& q,
                           const lmx::Vector<data_type>& qdot,
                           double partial_qdot,
                           double partial_qddot,
                           double time
                         );

    bool dynamicConvergence( const lmx::Vector<data_type>& q,
                             const lmx::Vector<data_type>& qdot,
                             const lmx::Vector<data_type>& qddot,
                             double time
                           );

    void staticResidue  ( lmx::Vector<data_type>& residue,
                          lmx::Vector<data_type>& q
                        );

    void staticTangent  ( lmx::Matrix<data_type>& tangent_in,
                          lmx::Vector<data_type>& q
                        );

    bool staticConvergence( lmx::Vector<data_type>& res,
                            lmx::Vector<data_type>& q
                          );

    void stepTriggered( );

    void writeConfStep( );

    lmx::DenseMatrix<data_type>& getSparsePattern()
    {
        return this->globalSparsePattern;
    }

    static double getGravity(int component)
    {
        return gravity.readElement(component);
    }

    static double getAlpha() {
        return alpha;
    }

    static double getTime() {
        return stepTime;
    }

    static int getDim() {
        return dimension;
    }

    static std::string getConstraintMethod() {
        return constraintMethod;
    }

    static std::string getSmoothingType() {
        return smoothingType;
    }

private:
  void systemOuputStep( const lmx::Vector<data_type>&, const lmx::Vector<data_type>& );
  void systemOuputStep( const lmx::Vector<data_type>& );
    char title[40];
    Reader* theReader;
    System* baseSystem;
    Contact* theContact;
    Analysis* theAnalysis;
    std::vector<Analysis*> analysis;
    std::map<int,Node*> nodes;
    std::map<int,Node*> thermalNodes;
    std::map<int,Material> materials; /**< Map of materials. */

    lmx::ExactStopwatch* globalTimer;
    std::ofstream* timerFile;
    std::ofstream* configurationFile;
    static double stepTime, oldClockTime;
    std::vector<double> stepTimes, accumulatedTimes, stepIterations;
    int iterationsNLSolver;
    int outputFilesDetail;

    double initialTemperature;
    lmx::Matrix<data_type> globalCapacity;
    lmx::Matrix<data_type> globalConductivity;
    lmx::Vector<data_type> globalRHSHeat;
    lmx::Vector<data_type> globalExternalHeat;
    lmx::Matrix<data_type> globalMass;
    lmx::Vector<data_type> globalRHSForces;
    lmx::Vector<data_type> globalInternalForces;
    lmx::Vector<data_type> globalExternalForces;
    lmx::DenseMatrix<data_type> globalSparsePattern;

    std::ofstream* outFile;

    static lmx::Vector<double> gravity;
    static double alpha;
    static int dimension;
    static std::string contact;
    static bool visualization;
    static bool outputMatrices;
    static std::string constraintMethod;
    static double epsilon;
    static std::string smoothingType;
};

}

#endif
