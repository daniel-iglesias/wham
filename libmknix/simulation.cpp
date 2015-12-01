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

#include "simulation.h"
#include "analysisthermaldynamic.h"
#include "system.h"
#include "node.h"
#include "reader.h"
#include "../LMX-1.0/src/LMX/lmx_mat_vector.h"

namespace mknix {


// Static variables:
double Simulation::stepTime = 0;
double Simulation::oldClockTime = 0;
lmx::Vector<double> Simulation::gravity = lmx::Vector<double>(3);
double Simulation::alpha = 1E4;
int Simulation::dimension = 2;
std::string Simulation::contact = "NONE";
bool Simulation::visualization = 0;
bool Simulation::outputMatrices = 0;
std::string Simulation::constraintMethod = "PENALTY";
double Simulation::epsilon = 1E-5;
std::string Simulation::smoothingType = "GLOBAL";

Simulation::Simulation()
    : theReader(0)
    , baseSystem(0)
//  , stepTime(0.)
    , iterationsNLSolver(0)
    , outputFilesDetail(2)
    , timerFile(0)
    , outFile(0)
    , configurationFile(0)
    , initialTemperature(0)
{
    globalTimer = new lmx::ExactStopwatch;
    globalTimer->setQuiet();
    if(outputFilesDetail>0){
      timerFile = new std::ofstream("simulation_times.dat");
      if(outputFilesDetail>1)
	configurationFile = new std::ofstream("dis.dat");
    }
}


Simulation::~Simulation()
{
  if (globalTimer)  delete globalTimer;
  if (timerFile)  delete timerFile;
  if (configurationFile)  delete configurationFile;
  if (outFile)  delete outFile;
}


void Simulation::inputFromFile(char *FileIn)
{
    if(theReader==0)  this->theReader = new Reader(this);
    if(baseSystem==0)  this->baseSystem = new System( "baseSystem" );
    theReader->generateTile6_104();
//     theReader->generateTile6_109();
    theReader->readInput();
}
std::vector<double> Simulation::getInterfaceNodesCoords()
{
  // Loads have access to the nodes, and are part of the system.
  std::vector<double> temp_x_coordinates;
  baseSystem->getThermalNodes(temp_x_coordinates);

  return temp_x_coordinates;
}


void Simulation::setInitialTemperatures(double temp_in)
{
  initialTemperature=temp_in;
}

// Part copy of run(), limited to preparation and thermal dynamic analysis
void Simulation::init(int totalSteps=0)
{
    if(outputFilesDetail>1){
      writeSystem();
      *outFile << "ANALYSIS " << endl;
    }
    if(outputFilesDetail>0){
      stepTimes.reserve(totalSteps);
      accumulatedTimes.reserve(totalSteps);
      stepIterations.reserve(totalSteps);
//       *timerFile << stepTime << "\t" << globalTimer->getTime() << std::endl;
      stepTimes.push_back(stepTime);
      accumulatedTimes.push_back(globalTimer->getTime());
      stepIterations.push_back(0.);
    }

    std::vector<Analysis*>::iterator itAnalysis;
    for ( itAnalysis = analysis.begin();
            itAnalysis!= analysis.end();
            ++itAnalysis
        )
    {   
      if( (*itAnalysis)->type() == "THERMAL" || (*itAnalysis)->type() == "THERMALSTATIC") {
        this->initThermalSimulation(*itAnalysis);
      }
      else {
	cout << "ANALYSIS NOT AVAILABLE" << endl;
//             if( (*itAnalysis)->type() == "STATIC" || (*itAnalysis)->type() == "DYNAMIC")
//                 this->baseSystem->setMechanical();
//             this->runMechanicalAnalysis(*itAnalysis);
        }
    }
}

void Simulation::initThermalSimulation(Analysis* theAnalysis_in)
{
  theAnalysis = theAnalysis_in;
  int gdlSize = nodes.size();
  lmx::Vector<data_type> q( gdlSize );
  q.fillIdentity(initialTemperature);

  globalCapacity.resize( gdlSize, gdlSize );
  globalConductivity.resize( gdlSize, gdlSize );
  globalRHSHeat.resize( gdlSize );
  globalExternalHeat.resize( gdlSize );

  baseSystem->calcCapacityMatrix();
  baseSystem->assembleCapacityMatrix( globalCapacity );
  baseSystem->calcConductivityMatrix();
  baseSystem->assembleConductivityMatrix( globalConductivity );
//   cout << globalCapacity << endl;
//   cout << globalConductivity;

    writeConfStep( );
  if(outputFilesDetail>1){
    systemOuputStep( q );
  }
  theAnalysis->init( &q );

}


void Simulation::solveStep(double* signal, double* outputSignal)
{
  baseSystem->updateThermalLoads(signal);
  theAnalysis->nextStep(  );
  if(outputSignal){
    baseSystem->getOutputSignalThermal(outputSignal);
  }
}

void Simulation::resetStep(double stepTemperature)
{
  int gdlSize = nodes.size();
  lmx::Vector<data_type> q( gdlSize );
  q.fillIdentity(stepTemperature);
  theAnalysis->resetAndAdvanceStep( &q );
}


void Simulation::endSimulation()
{
    if(outputFilesDetail>0){
//       double theTime = globalTimer->getTime();
      *timerFile << stepTimes[0] << "\t" 
		<< accumulatedTimes[0] << "\t" 
		<< accumulatedTimes[0] << "\t" 
		<< stepIterations[0] << std::endl;
      for(int i=1; i<stepTimes.size(); ++i){
      *timerFile << stepTimes[i] << "\t" 
		<< accumulatedTimes[i]-accumulatedTimes[i-1] << "\t" 
		<< accumulatedTimes[i] << "\t" 
		<< stepIterations[i] << std::endl;
      }
      
    }
    if(outputFilesDetail>1){
      std::ifstream disp("dis.dat");
      char a;

      if(outFile->is_open() ) {
	  while (disp.get(a)) {
	      outFile->put(a);
	      outFile->flush();
	  }

	  // output extra flexible bodies data...
	  baseSystem->outputToFile( outFile );
      }
    }

//       cout << "q(" << q.size()/2 << ") = " << q(q.size()/2) << endl;
//       cout << "q(" << q.size()-1 << ") = " << q(q.size()-1) << endl;

    // output f_int of constraints...
//     lmx::Vector<double> constr_forces(nodes.size()*dimension);
//     baseSystem->assembleConstraintForces( constr_forces );
//     for(size_type i=0; i< constr_forces.size(); ++i ) {
//         if(constr_forces(i) != 0. )
//             cout << "R(" << i << ") = " << constr_forces(i) << endl;
//     }
}


void Simulation::run()
{
#ifdef HAVE_VTK  
    if(Simulation::contact == "GLOBAL" || Simulation::visualization == 1) {
        this->theContact = new Contact(this, 10.);
        this->theContact->createPoints();
        this->theContact->createPolys();
        this->theContact->createDrawingObjects();
        if(Simulation::contact == "GLOBAL") {
            this->theContact->createDelaunay();
            this->theContact->createDrawingContactObjects();
        }
        this->theContact->drawObjects();
    }
#endif
    writeSystem();
    *outFile << "ANALYSIS " << endl;
//   << "FILE " << "dis.dat" << endl;
    *timerFile << stepTime << "\t" << globalTimer->getTime() << std::endl;

    std::vector<Analysis*>::iterator itAnalysis;
    for ( itAnalysis = analysis.begin();
            itAnalysis!= analysis.end();
            ++itAnalysis
        )
    {   if( (*itAnalysis)->type() == "THERMAL" || (*itAnalysis)->type() == "THERMALSTATIC") {
            this->runThermalAnalysis(*itAnalysis);
        }
        else {
            if( (*itAnalysis)->type() == "STATIC" || (*itAnalysis)->type() == "DYNAMIC")
                this->baseSystem->setMechanical();
            this->runMechanicalAnalysis(*itAnalysis);
        }
    }
}

void Simulation::runThermalAnalysis(Analysis* theAnalysis_in)
{
  theAnalysis = theAnalysis_in;
    int gdlSize = nodes.size();
    lmx::Vector<data_type> q( gdlSize );
    {
        lmx::ExactStopwatch sw;

        globalCapacity.resize( gdlSize, gdlSize );
        globalConductivity.resize( gdlSize, gdlSize );
        globalRHSHeat.resize( gdlSize );
        globalExternalHeat.resize( gdlSize );

        baseSystem->calcCapacityMatrix();
        baseSystem->assembleCapacityMatrix( globalCapacity );
        baseSystem->calcConductivityMatrix();
        baseSystem->assembleConductivityMatrix( globalConductivity );
	cout << globalCapacity << endl;
        cout << globalConductivity;

//         cout << globalCapacity;
//         cout << globalConductivity;
        // maybe is better to make an specific function call for the sparse
        // pattern, but this should work...
//        if( lmx::getMatrixType() == 1 ){
//           globalSparsePattern.resize( gdlSize, gdlSize );
//           baseSystem->calcTangentMatrix(  );
//           baseSystem->assembleTangentMatrix( globalSparsePattern );
//        }
//        // Output matrices in initial configuration:
//         if( outputMatrices ){
//           lmx::Matrix< data_type > K_temp(gdlSize,gdlSize);
//           baseSystem->calcTangentMatrix(  );
//           baseSystem->assembleTangentMatrix( K_temp );
//           K_temp.harwellBoeingSave( (char *) "K.mat");
//           globalMass.harwellBoeingSave( (char *) "M.mat");
//           // save raw matrices:
//           std::ofstream Kfile("K");
//           Kfile << K_temp;
//           Kfile.close();
//         }
        // Output to file the initial configuration:
        writeConfStep( );

        //       cout << "q0 = " << q <<endl;
        //       cout << "qdot0 = " << qdot <<endl;
        if( theAnalysis->type() == "THERMAL" ) {
            systemOuputStep( q );
            theAnalysis->solve( &q );
        }
        else if( theAnalysis->type() == "THERMALSTATIC" ) {
            // write initial configuration...
            *outFile << "0 "; //time=0
//           systemOuputStep( q ); // produces output of temperatures, incompatible with mknixpost-static
            std::map<int,Node*>::iterator it_nodes;
            for ( it_nodes = nodes.begin();
                    it_nodes!= nodes.end();
                    ++it_nodes
                )
            {
                *outFile << it_nodes->second->getx() << " ";
                *outFile << it_nodes->second->gety() << " ";
                if (Simulation::getDim() == 3)
                    *outFile << it_nodes->second->getz() << " ";
            }
            *outFile << endl;
            theAnalysis->solve( &q );
        }
    } // Output ExactStopwatch time
    if(outputFilesDetail>1){
      std::ifstream disp("dis.dat");
      char a;

      if(outFile->is_open() ) {
	  while (disp.get(a)) {
	      outFile->put(a);
	      outFile->flush();
	  }

	  // output extra flexible bodies data...
	  baseSystem->outputToFile( outFile );
      }
    }

//       cout << "q(" << q.size()/2 << ") = " << q(q.size()/2) << endl;
//       cout << "q(" << q.size()-1 << ") = " << q(q.size()-1) << endl;

    // output f_int of constraints...
    lmx::Vector<double> constr_forces(nodes.size()*dimension);
    baseSystem->assembleConstraintForces( constr_forces );
    for(size_type i=0; i< constr_forces.size(); ++i ) {
        if(constr_forces(i) != 0. )
            cout << "R(" << i << ") = " << constr_forces(i) << endl;
    }
}

void Simulation::runMechanicalAnalysis(Analysis* theAnalysis_in)
{
  theAnalysis = theAnalysis_in;
    int gdlSize = nodes.size() * Simulation::dimension;
    lmx::Vector<data_type> q( gdlSize );
    {
        lmx::ExactStopwatch sw;

        globalMass.resize( gdlSize, gdlSize );
        globalRHSForces.resize( gdlSize );
        globalInternalForces.resize( gdlSize );
        globalExternalForces.resize( gdlSize );

        int i=0;
        std::map<int,Node*>::iterator it_nodes;
        for ( it_nodes = nodes.begin();
                it_nodes!= nodes.end();
                ++it_nodes
            )
        {
            q(Simulation::dimension*i) = it_nodes->second->getx();
            q(Simulation::dimension*i+1) = it_nodes->second->gety();
            if (Simulation::getDim() == 3)
                q(Simulation::dimension*i+2) = it_nodes->second->getz();
            ++i;
        }

        baseSystem->calcMassMatrix();
        baseSystem->assembleMassMatrix( globalMass );
        // maybe is better to make an specific function call for the sparse
        // pattern, but this should work...
        if( lmx::getMatrixType() == 1 ) {
            globalSparsePattern.resize( gdlSize, gdlSize );
            baseSystem->calcTangentMatrix(  );
            baseSystem->assembleTangentMatrix( globalSparsePattern );
        }
        // Output matrices in initial configuration:
        if( outputMatrices ) {
            lmx::Matrix< data_type > K_temp(gdlSize,gdlSize);
            baseSystem->calcTangentMatrix(  );
            baseSystem->assembleTangentMatrix( K_temp );
            K_temp.harwellBoeingSave( (char *) "K.mat");
            globalMass.harwellBoeingSave( (char *) "M.mat");
            // save raw matrices:
            std::ofstream Kfile("K");
            Kfile << K_temp;
            Kfile.close();
        }
        // Output to file the initial configuration:
        writeConfStep( );

        //       cout << "q0 = " << q <<endl;
        //       cout << "qdot0 = " << qdot <<endl;
        if( theAnalysis->type() == "DYNAMIC" ) {
            lmx::Vector<data_type> qdot( gdlSize );
            // output first step data
            systemOuputStep( q );
            theAnalysis->setEpsilon( epsilon );
            theAnalysis->solve( &q, &qdot );
        }
        else if( theAnalysis->type() == "STATIC" ) {
            // write initial configuration...
            *outFile << "0 "; //time=0
            for(size_type i=0; i<q.size(); ++i) {
                *outFile << q(i) << " ";
            }
            *outFile << endl;
            theAnalysis->solve( &q );
        }
        else if( theAnalysis->type() == "THERMOMECHANICALDYNAMIC" ) {
            // Init thermal conf vector and a zero velocity vector
            int thermalSize = thermalNodes.size();
            lmx::Vector<data_type> qdot( gdlSize );
            lmx::Vector<data_type> qt( thermalSize );
            int i=0;
            std::map<int,Node*>::iterator it_nodes;
            for ( it_nodes = thermalNodes.begin();
                    it_nodes!= thermalNodes.end();
                    ++it_nodes
                )
            {
                qt(i) = it_nodes->second->getqt();
                ++i;
            }
            globalCapacity.resize( thermalSize, thermalSize );
            globalConductivity.resize( thermalSize, thermalSize );
            globalRHSHeat.resize( thermalSize );
            globalExternalHeat.resize( thermalSize );

            baseSystem->calcCapacityMatrix();
            baseSystem->assembleCapacityMatrix( globalCapacity );
            baseSystem->calcConductivityMatrix();
            baseSystem->assembleConductivityMatrix( globalConductivity );

            // output first step data
            systemOuputStep( q );
            theAnalysis->setEpsilon( epsilon );
            theAnalysis->solve( &qt, &q, &qdot );
        }

    } // Output ExactStopwatch time

    if(outputFilesDetail>1){
      std::ifstream disp("dis.dat");
      char a;

      if(outFile->is_open() ) {
	  while (disp.get(a)) {
	      outFile->put(a);
	      outFile->flush();
	  }

	  // output extra flexible bodies data...
	  baseSystem->outputToFile( outFile );
      }
    }

//       cout << "q(" << q.size()/2 << ") = " << q(q.size()/2) << endl;
//       cout << "q(" << q.size()-1 << ") = " << q(q.size()-1) << endl;

    // output f_int of constraints...
//       lmx::Vector<double> constr_forces(nodes.size()*dimension);
//       baseSystem->assembleConstraintForces( constr_forces );
//       for(size_type i=0; i< constr_forces.size(); ++i ){
//         if(constr_forces(i) != 0. )
//           cout << "R(" << i << ") = " << constr_forces(i) << endl;
//       }
}

void Simulation::writeSystem()
{
    std::stringstream ss;
    ss << title << ".mec";
    outFile = new std::ofstream( ss.str().c_str() );
    *outFile << "DIMENSION " << Simulation::dimension << endl;

    *outFile << "SYSTEM" << endl;

    *outFile << "NODES" << endl;
    std::map<int,Node*>::iterator it_nodes;
    for ( it_nodes = nodes.begin(); it_nodes != nodes.end(); ++it_nodes ) {
        *outFile
                << "\t" << it_nodes->first
                << "\t" << it_nodes->second->getx()
                << "\t" << it_nodes->second->gety()
                << "\t" << it_nodes->second->getz()
                << endl;
    }
    *outFile << "ENDNODES" << endl;

    *outFile << "RIGIDBODIES" << endl;
    baseSystem->writeRigidBodies( outFile );
    *outFile << "ENDRIGIDBODIES" << endl;

    *outFile << "FLEXBODIES" << endl;
    baseSystem->writeFlexBodies( outFile );
    *outFile << "ENDFLEXBODIES" << endl;

    *outFile << "JOINTS" << endl;
    baseSystem->writeJoints( outFile );
    *outFile << "ENDJOINTS" << endl;
  
    *outFile << "ENDSYSTEM" << endl;

    // write a standard file for nodal info:
    std::ofstream nodeFile("nodes.dat");
    for ( it_nodes = nodes.begin(); it_nodes != nodes.end(); ++it_nodes ) {
        nodeFile
                << "\t" << it_nodes->first
                << "\t" << it_nodes->second->getx()
                << "\t" << it_nodes->second->gety()
                << "\t" << it_nodes->second->getz()
                << endl;
    }

}

void Simulation::staticThermalResidue ( lmx::Vector< data_type > & residue,
                                        lmx::Vector< data_type > & q
                                      )
{
    std::map<int,Node*>::iterator itNodes;
    for( itNodes = thermalNodes.begin();
            itNodes!= thermalNodes.end();
            ++itNodes
       )
    {
        itNodes->second->setqt( q );
    }

    globalConductivity.fillIdentity(0.);
    globalExternalHeat.fillIdentity(0.);

    baseSystem->calcConductivityMatrix();
    baseSystem->calcExternalHeat();
    baseSystem->assembleConductivityMatrix( globalConductivity );
    baseSystem->assembleExternalHeat( globalExternalHeat );

    residue = globalConductivity*q;
    residue -= globalExternalHeat;

//  cout << endl << "RESIDUE PARTS: " << (globalCapacity*qdot).norm2() << " "
//  << (globalConductivity*q).norm2() << " " << globalExternalHeat.norm2() << endl;

}

void Simulation::staticThermalTangent ( lmx::Matrix< data_type > & tangent_in,
                                        lmx::Vector< data_type > & q
                                      )
{
    tangent_in.fillIdentity(0.);
//   baseSystem->calcTangentMatrix(  );
//   baseSystem->assembleTangentMatrix( tangent_in );
    tangent_in = globalConductivity;
}

bool Simulation::staticThermalConvergence ( lmx::Vector<data_type>& res,
        lmx::Vector<data_type>& q
                                          )
{
//   lmx::Vector<data_type> res( qddot.size() );
//   res =  globalInternalForces - globalExternalForces;
    if( res.norm2() <= epsilon ) {
        if( baseSystem->checkAugmented( ) ) { // if convergence...
            stepTime = 1.;
            systemOuputStep( q );
            baseSystem->clearAugmented();
            stepTriggered();
            return 1;
        }
        else return 0;
    }
    else return 0;

}


void Simulation::explicitThermalEvaluation
( const lmx::Vector<data_type>& qt
  , lmx::Vector<data_type>& qtdot
  , double time
)
{
    std::map<int,Node*>::iterator itNodes;
    for( itNodes = thermalNodes.begin();
            itNodes!= thermalNodes.end();
            ++itNodes
       )
    {
        itNodes->second->setqt( qt );
    }

    globalExternalHeat.fillIdentity(0.);

    baseSystem->calcExternalHeat();
    baseSystem->assembleExternalHeat( globalExternalHeat );
    globalRHSHeat = globalConductivity * qt;
    globalRHSHeat -= globalExternalHeat;

//   cout << globalMass << endl;
    lmx::LinearSystem<data_type> theLSolver( globalCapacity, qtdot, globalRHSHeat );
    theLSolver.solveYourself();
//  cout << "initial_flux :" << qtdot << endl;
    if( theAnalysis->type() != "THERMOMECHANICALDYNAMIC" ) { // for regular THERMAL dynamic problems
      stepTime = time;
      systemOuputStep( qt );
    }


}

void Simulation::dynamicThermalEvaluation( const lmx::Vector<data_type>& qt,
        lmx::Vector<data_type>& qtdot,
        double time
                                         )
{
    globalCapacity.fillIdentity(0.);
    globalConductivity.fillIdentity(0.);
    globalExternalHeat.fillIdentity(0.);

    baseSystem->calcCapacityMatrix();
    baseSystem->calcConductivityMatrix();
    baseSystem->calcExternalHeat();
    baseSystem->assembleCapacityMatrix( globalCapacity );
    baseSystem->assembleConductivityMatrix( globalConductivity );
    baseSystem->assembleExternalHeat( globalExternalHeat );
    globalRHSHeat = globalConductivity * qt;
    globalRHSHeat -= globalExternalHeat;

//     cout << "H = " << globalConductivity << endl;
//     cout << "C = " << globalCapacity << endl;
//     cout << globalRHSHeat << endl;
    lmx::LinearSystem<data_type> theLSolver( globalCapacity, qtdot, globalRHSHeat );
//     theLSolver.solveYourself();
//    cout << "initial_flux :" << qtdot << endl;

    stepTime = time;
}

void Simulation::dynamicThermalResidue  ( lmx::Vector<data_type>& residue,
        const lmx::Vector<data_type>& q,
        const lmx::Vector<data_type>& qdot,
        double time
                                        )
{
    std::map<int,Node*>::iterator itNodes;
    for( itNodes = thermalNodes.begin();
            itNodes!= thermalNodes.end();
            ++itNodes
       )
    {
        itNodes->second->setqt( q );
    }

    globalCapacity.fillIdentity(0.);
    globalConductivity.fillIdentity(0.);
    globalExternalHeat.fillIdentity(0.);

    baseSystem->calcCapacityMatrix();
    baseSystem->calcConductivityMatrix();
    baseSystem->calcExternalHeat();
    
    baseSystem->assembleCapacityMatrix( globalCapacity );
    baseSystem->assembleConductivityMatrix( globalConductivity );
    baseSystem->assembleExternalHeat( globalExternalHeat );

    residue = globalCapacity*qdot;
    residue += globalConductivity*q;
    residue -= globalExternalHeat;

//   cout << endl << "RESIDUE PARTS: " << (globalCapacity*qdot).norm2() << " "
//   << (globalConductivity*q).norm2() << " " << globalExternalHeat.norm2() << endl;
//     cout << "H = " << globalConductivity << endl;
//     cout << "C = " << globalCapacity << endl;
//     cout << globalRHSHeat << endl;
//     cout << "q = " << q << endl;
//     cout << "qdot = " << qdot << endl;
//     cout << "residue = " << residue << endl;

}

void Simulation::dynamicThermalTangent  ( lmx::Matrix<data_type>& tangent_in,
        const lmx::Vector<data_type>& q,
        double partial_qdot,
        double time
                                        )
{
    tangent_in.fillIdentity(0.);
//   baseSystem->calcTangentMatrix(  );
//   baseSystem->assembleTangentMatrix( tangent_in );
    tangent_in += (data_type)partial_qdot*globalCapacity;
    tangent_in += globalConductivity;
}

bool Simulation::dynamicThermalConvergence( const lmx::Vector<data_type>& q,
        const lmx::Vector<data_type>& qdot,
        double time
                                          )
{
    ++iterationsNLSolver;
    lmx::Vector<data_type> res( qdot.size() );
//  double energy_max, energy_sum;
//  cout << "\n"
//       << (globalMass*qddot).norm2() << "\t"
//       << globalInternalForces.norm2() << "\t"
//       << globalExternalForces.norm2() << "\n";
    res = globalCapacity*qdot + globalConductivity*q - globalExternalHeat;
//  energy_max = std::max( std::fabs(globalMass*qddot*q)
//                       , std::fabs(globalInternalForces*q) );
//  energy_max = std::max( energy_max, std::fabs(globalExternalForces*q) );
//  energy_sum = std::fabs(globalMass*qddot*q)
//             + std::fabs(globalInternalForces*q)
//             + std::fabs(globalExternalForces*q);
//      cout << "            : MAX_ENERGY = " << energy_max << endl
//           << "              SUM_ENERGY = " << energy_sum << endl;
    if( res.norm2() <= epsilon ) {
//  if( (energy_max / energy_sum) <= epsilon ){
        if( baseSystem->checkAugmented() ) {
//      cout << " CONVERGENCE: MAX_ENERGY = " << energy_max << endl
//           << "              SUM_ENERGY = " << energy_sum << endl;
            stepTime = time;
            systemOuputStep( q );
            baseSystem->clearAugmented();
            return 1;
        }
        else return 0;
    }
    else return 0;
}

bool Simulation::dynamicThermalConvergenceInThermomechanical( const lmx::Vector<data_type>& q,
        const lmx::Vector<data_type>& qdot,
        double time
                                                            )
{
    lmx::Vector<data_type> res( qdot.size() );
    res = globalCapacity*qdot + globalConductivity*q - globalExternalHeat;
    if( res.norm2() <= epsilon ) {
        if( baseSystem->checkAugmented() ) {
            stepTime = time;
            baseSystem->clearAugmented();
            return 1;
        }
        else return 0;
    }
    else return 0;
}


void Simulation::explicitAcceleration( const lmx::Vector< data_type > & q,
                                       const lmx::Vector< data_type > & qdot,
                                       lmx::Vector< data_type > & qddot,
                                       double time
                                     )
{
    std::map<int,Node*>::iterator itNodes;
    for( itNodes = nodes.begin();
            itNodes!= nodes.end();
            ++itNodes
       )
    {
        itNodes->second->setx( q );
    }

//   globalMass.fillIdentity(0.);
    globalInternalForces.fillIdentity(0.);
    globalExternalForces.fillIdentity(0.);

//   baseSystem->calcMassMatrix();
    baseSystem->calcInternalForces();
    baseSystem->calcExternalForces();
//   baseSystem->assembleMassMatrix( globalMass );
    baseSystem->assembleInternalForces( globalInternalForces );
    baseSystem->assembleExternalForces( globalExternalForces );
    globalRHSForces = globalExternalForces;
    globalRHSForces -= globalInternalForces;

//   cout << globalMass << endl;
    lmx::LinearSystem<data_type> theLSolver( globalMass, qddot, globalRHSForces );
    theLSolver.solveYourself();

    stepTime = time;
    systemOuputStep( q, qdot );

}

void Simulation::dynamicAcceleration( const lmx::Vector< data_type > & q,
                                      const lmx::Vector< data_type > & qdot,
                                      lmx::Vector< data_type > & qddot,
                                      double time
                                    )
{
    globalMass.fillIdentity(0.);
    globalInternalForces.fillIdentity(0.);
    globalExternalForces.fillIdentity(0.);

    baseSystem->calcMassMatrix();
    baseSystem->calcInternalForces();
    baseSystem->calcExternalForces();
    baseSystem->assembleMassMatrix( globalMass );
    baseSystem->assembleInternalForces( globalInternalForces );
    baseSystem->assembleExternalForces( globalExternalForces );
    globalRHSForces = globalExternalForces;
    globalRHSForces -= globalInternalForces;

//    cout << globalMass << endl;
    lmx::LinearSystem<data_type> theLSolver( globalMass, qddot, globalRHSForces );
    theLSolver.solveYourself();
//  cout << "initial_acceleration :" << qddot << endl;
}

void Simulation::dynamicResidue( lmx::Vector< data_type > & residue,
                                 const lmx::Vector< data_type > & q,
                                 const lmx::Vector< data_type > & qdot,
                                 const lmx::Vector< data_type > & qddot,
                                 double time
                               )
{
    std::map<int,Node*>::iterator itNodes;
    for( itNodes = nodes.begin();
            itNodes!= nodes.end();
            ++itNodes
       )
    {
        itNodes->second->setx( q );
    }
// At this time globalMass is always the same...
// so the commented lines increment efficiency by 12% aprox.

//   globalMass.fillIdentity(0.);
    globalInternalForces.fillIdentity(0.);
    globalExternalForces.fillIdentity(0.);

    baseSystem->update( time );
//   baseSystem->calcMassMatrix();
    baseSystem->calcInternalForces();
    baseSystem->calcExternalForces();
//   baseSystem->assembleMassMatrix( globalMass );
    baseSystem->assembleInternalForces( globalInternalForces );
    baseSystem->assembleExternalForces( globalExternalForces );

    residue = globalMass*qddot;
    residue += globalInternalForces;
    residue -= globalExternalForces;

//     cout << "qddot : " << qddot;
//     cout << "q : " << q;
//     cout << "globalMass*qddot : " << globalMass*qddot;
//     cout << "globalInternalForces : " << globalInternalForces;
//     cout << "globalExternalForces : " << globalExternalForces;
}

void Simulation::dynamicTangent( lmx::Matrix< data_type > & tangent_in,
                                 const lmx::Vector< data_type > & q,
                                 const lmx::Vector< data_type > & qdot,
                                 double partial_qdot,
                                 double partial_qddot,
                                 double time
                               )
{
    tangent_in.fillIdentity(0.);
    baseSystem->calcTangentMatrix(  );
    baseSystem->assembleTangentMatrix( tangent_in );
    tangent_in += (data_type)partial_qddot*globalMass;
}

bool Simulation::dynamicConvergence( const lmx::Vector<data_type>& q,
                                     const lmx::Vector<data_type>& qdot,
                                     const lmx::Vector<data_type>& qddot,
                                     double time
                                   )
{   ++iterationsNLSolver;
    lmx::Vector<data_type> res( qddot.size() );
//  double energy_max, energy_sum;
//  cout << "\n"
//       << "(globalMass*qddot).norm2() = " << "\t"
//       << (globalMass*qddot).norm2() << "\n"
//       << "globalInternalForces.norm2() = " << "\t"
//       << globalInternalForces.norm2() << "\n"
//       << "globalExternalForces.norm2() = " << "\t"
//       << globalExternalForces.norm2() << "\n";
    res = globalMass*qddot + globalInternalForces - globalExternalForces;
//  energy_max = std::max( std::fabs(globalMass*qddot*q)
//                       , std::fabs(globalInternalForces*q) );
//  energy_max = std::max( energy_max, std::fabs(globalExternalForces*q) );
//  energy_sum = std::fabs(globalMass*qddot*q)
//             + std::fabs(globalInternalForces*q)
//             + std::fabs(globalExternalForces*q);
//      cout << "            : MAX_ENERGY = " << energy_max << endl
//           << "              SUM_ENERGY = " << energy_sum << endl;
    if( res.norm2() <= epsilon ) {
//  if( (energy_max / energy_sum) <= epsilon ){
        if( baseSystem->checkAugmented() ) {
//      cout << " CONVERGENCE: MAX_ENERGY = " << energy_max << endl
//           << "              SUM_ENERGY = " << energy_sum << endl;
            stepTime = time;
            systemOuputStep( q, qdot );
            baseSystem->clearAugmented();
            return 1;
        }
        else return 0;
    }
    else return 0;

}


void Simulation::staticResidue ( lmx::Vector< data_type > & residue,
                                 lmx::Vector< data_type > & q
                               )
{
    std::map<int,Node*>::iterator itNodes;
    for( itNodes = nodes.begin();
            itNodes!= nodes.end();
            ++itNodes
       )
    {
        itNodes->second->setx( q );
    }

    globalInternalForces.fillIdentity(0.);
    globalExternalForces.fillIdentity(0.);

    baseSystem->calcInternalForces();
    baseSystem->calcExternalForces();
    baseSystem->assembleInternalForces( globalInternalForces );
    baseSystem->assembleExternalForces( globalExternalForces );

    residue = globalInternalForces;
    residue -= globalExternalForces;

//   cout << "residue:" << residue;

//   cout << "q : " << q;
//   cout << "globalInternalForces : " << globalInternalForces;
//   cout << "globalExternalForces : " << globalExternalForces;
}

void Simulation::staticTangent ( lmx::Matrix< data_type > & tangent_in,
                                 lmx::Vector< data_type > & q
                               )
{
    tangent_in.fillIdentity(0.);
    baseSystem->calcTangentMatrix(  );
    baseSystem->assembleTangentMatrix( tangent_in );
//  cout << "TANGENT:\n" << tangent_in;
}

bool Simulation::staticConvergence ( lmx::Vector<data_type>& res,
                                     lmx::Vector<data_type>& q
                                   )
{
//   lmx::Vector<data_type> res( qddot.size() );
//   res =  globalInternalForces - globalExternalForces;
    if( res.norm2() <= epsilon ) {
        if( baseSystem->checkAugmented( ) ) { // if convergence...
            stepTime = 1.;
            systemOuputStep( q );
            baseSystem->clearAugmented();
            stepTriggered();
            return 1;
        }
        else return 0;
    }
    else return 0;

}


void Simulation::stepTriggered( ) {
#ifdef HAVE_VTK  
    if(contact=="GLOBAL" || visualization==1) {
        this->theContact->updatePoints();
        this->theContact->updateLines();
        if(contact=="GLOBAL")
            this->theContact->updateDelaunay();
        this->theContact->drawObjects();
    }
#endif
// Output configuration of time step:
  if(outputFilesDetail>1)
    writeConfStep( );

    // Output timer info:
  if(outputFilesDetail>0){
//     double theTime = globalTimer->getTime();
//     *timerFile << stepTime << "\t" 
// 	       << theTime-oldClockTime << "\t" 
// 	       << theTime<< "\t" 
//                << iterationsNLSolver << std::endl;
    stepTimes.push_back(stepTime);
    accumulatedTimes.push_back(globalTimer->getTime());
    stepIterations.push_back(iterationsNLSolver);
//     oldClockTime = theTime;
    iterationsNLSolver = 0;
  }
}

void Simulation::writeConfStep( ) {
  if(outputFilesDetail>1){
    configurationFile->setf(std::ios::scientific, std::ios::floatfield);
    configurationFile->precision(6);
    *configurationFile << stepTime << "\t";
    std::map<int,Node*>::iterator it_nodes;
    int i;
    for ( it_nodes = nodes.begin();
            it_nodes!= nodes.end();
            ++it_nodes
        )
    {
        for (i=0; i<dimension; ++i) {
            *configurationFile << it_nodes->second->getx(i) << "\t";
        }
    }
    *configurationFile << endl;
  }
}

void Simulation::systemOuputStep(const lmx::Vector< data_type >& q)
{
  if(outputFilesDetail>1){
    baseSystem->outputStep( q );
  }

}

void Simulation::systemOuputStep(const lmx::Vector< data_type >& q, const lmx::Vector< data_type >& qdot)
{
  if(outputFilesDetail>1){
    baseSystem->outputStep( q, qdot );
  }
}

}

