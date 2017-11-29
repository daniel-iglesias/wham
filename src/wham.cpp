/***************************************************************************
 *   Copyright (C) 2007-2015 by Daniel Iglesias                            *
 *   https://github.com/daniel-iglesias/mknix                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "simulation.h"

using namespace std;

int main(int argc, char *argv[])
{
  lmx::setMatrixType( 2 );
  lmx::setLinSolverType( 0 );

//   int totalPulses(1000), loadSteps(10E2), cooldownSteps(100E2);
  int totalPulses(1), loadSteps(10), cooldownSteps(1000);
//   int totalPulses(3), loadSteps(10), cooldownSteps(1000);

//   double stepTime(0);
//   lmx::ExactStopwatch* globalTimer;
//     globalTimer = new lmx::ExactStopwatch;
//     globalTimer->setQuiet();
//   std::ofstream timerFile;
//   timerFile.open("simulation_times.dat");
//   std::vector<double> stepTimes, accumulatedTimes, stepIterations;
//       stepTimes.reserve(loadSteps+cooldownSteps+1);
//       accumulatedTimes.reserve(loadSteps+cooldownSteps+1);
//       stepIterations.reserve(loadSteps+cooldownSteps+1);


  mknix::Simulation mySimulation;
  mySimulation.setOutputFilesDetail(0); // none
//  mySimulation.setOutputFilesDetail(1); // only times and output.reader
//  mySimulation.setOutputFilesDetail(2); // all output files
  mySimulation.inputFromFile("Tile6-RT");

  std::vector<double> intNodes = mySimulation.getInterfaceNodesCoords();
  cout << "NODES: " << intNodes.size() << endl;
  cout << intNodes[0] << " ... " << intNodes[72];
  mySimulation.setInitialTemperatures(107);
  mySimulation.init(totalPulses*(loadSteps+cooldownSteps));
  double heatFluence[21];
//   double heatFluence[31];
  double temperatures[4];
  for(int pulses=0; pulses<totalPulses; ++pulses){
    // set initial conditions!
    for(int i=0; i<loadSteps; ++i){
	 heatFluence[0] = 0;
	 heatFluence[1] = 0;
	 heatFluence[2] = 0;
	 heatFluence[3] = 0;
	 heatFluence[4] = 0;
	 heatFluence[5] = 0;
	 heatFluence[6] = 0;
	 heatFluence[7] = 7.17E3;
	 heatFluence[8] = 6.53E3;
	 heatFluence[9] = 5.05E3;
	 heatFluence[10] = 3.56E3;
	 heatFluence[11] = 2.08E3;
	 heatFluence[12] = 6.02E2;
	 heatFluence[13] = 0;
	 heatFluence[14] = 0;
	 heatFluence[15] = 0;
	 heatFluence[16] = 0;
	 heatFluence[17] = 0;
	 heatFluence[18] = 0;
	 heatFluence[19] = 0;
	 heatFluence[20] = 0;
//       heatFluence[0] = 0;
//       heatFluence[1] = 0;
//       heatFluence[2] = 0;
//       heatFluence[3] = 0;
//       heatFluence[4] = 0;
//       heatFluence[5] = 0;
//       heatFluence[6] = 0;
//       heatFluence[7] = 0;
//       heatFluence[8] = 0;
//       heatFluence[9] = 0;
//       heatFluence[10] = 0;
//       heatFluence[11] = 0;
//       heatFluence[12] = 0;
//       heatFluence[13] = 0;
//       heatFluence[14] = 0;
//       heatFluence[15] = 0;
//       heatFluence[16] = 66066.8;
//       heatFluence[17] = 126981;
//       heatFluence[18] = 97147.6;
//       heatFluence[19] = 67313.8;
//       heatFluence[20] = 37480.1;
//       heatFluence[21] = 8532.35;
//       heatFluence[22] = 0;
//       heatFluence[23] = 0;
//       heatFluence[24] = 0;
//       heatFluence[25] = 0;
//       heatFluence[26] = 0;
//       heatFluence[27] = 0;
//       heatFluence[28] = 0;
//       heatFluence[29] = 0;
//       heatFluence[30] = 0;

      mySimulation.solveStep(heatFluence, temperatures);
//       stepTime+=0.01;
//       stepTimes.push_back(stepTime);
//       accumulatedTimes.push_back(globalTimer->getTime());
//       stepIterations.push_back(0);

//       sleep( 1E-4);

    }
    for(int i=0; i<cooldownSteps; ++i){
      heatFluence[0] = 0;
      heatFluence[1] = 0;
      heatFluence[2] = 0;
      heatFluence[3] = 0;
      heatFluence[4] = 0;
      heatFluence[5] = 0;
      heatFluence[6] = 0;
      heatFluence[7] = 0;
      heatFluence[8] = 0;
      heatFluence[9] = 0;
      heatFluence[10] = 0;
      heatFluence[11] = 0;
      heatFluence[12] = 0;
      heatFluence[13] = 0;
      heatFluence[14] = 0;
      heatFluence[15] = 0;
      heatFluence[16] = 0;
      heatFluence[17] = 0;
      heatFluence[18] = 0;
      heatFluence[19] = 0;
      heatFluence[20] = 0;
//       heatFluence[21] = 0;
//       heatFluence[22] = 0;
//       heatFluence[23] = 0;
//       heatFluence[24] = 0;
//       heatFluence[25] = 0;
//       heatFluence[26] = 0;
//       heatFluence[27] = 0;
//       heatFluence[28] = 0;
//       heatFluence[29] = 0;
//       heatFluence[30] = 0;
      mySimulation.solveStep(heatFluence, temperatures);
//       stepTime+=0.01;
//       stepTimes.push_back(stepTime);
//       accumulatedTimes.push_back(globalTimer->getTime());
//       stepIterations.push_back(0);
      sleep( 1E-4);
    }
    mySimulation.resetStep(107);
//       stepTime+=0.01;
//       stepTimes.push_back(stepTime);
//       accumulatedTimes.push_back(globalTimer->getTime());
//       stepIterations.push_back(0);
//
//       timerFile << stepTimes[0] << "\t"
// 		<< accumulatedTimes[0] << "\t"
// 		<< accumulatedTimes[0] << "\t"
// 		<< stepIterations[0] << std::endl;
//       for(int i=1; i<stepTimes.size(); ++i){
//       timerFile << stepTimes[i] << "\t"
// 		<< accumulatedTimes[i]-accumulatedTimes[i-1] << "\t"
// 		<< accumulatedTimes[i] << "\t"
// 		<< stepIterations[i] << std::endl;
//       }
//       stepTimes.clear();
//       accumulatedTimes.clear();
//       stepIterations.clear();
  }
  mySimulation.endSimulation();

  return EXIT_SUCCESS;
}
