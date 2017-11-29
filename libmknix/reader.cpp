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

#include "analysisthermaldynamic.h"
#include "loadthermal.h"
#include "bodyflex.h"
#include "node.h"
#include "reader.h"
#include "readerflex.h"
#include "simulation.h"
#include "system.h"

namespace mknix {

Reader::Reader()
    : theSimulation(0)
    , theReaderRigid(0)
{
}

Reader::Reader(Simulation* simulation_in)
    : theSimulation( simulation_in )
    , theReaderRigid(0)
{
}


Reader::~Reader()
{
   delete theReaderFlex; theReaderFlex=0;
}

void Reader::generateTile(char *input_in, char *mesh_in, char *capacity_in, char *conductivity_in)
{
  input << input_in;
  mesh << mesh_in;
  thermalcapacity << capacity_in;
  thermalconductivity << conductivity_in;
}

void Reader::generateTile6_104()
{
  input << "TITLE tile6\n";
  input << "\n";
  input << "\n";
  input << "DIMENSION 2\n";
  input << "\n";
  input << "MATERIALS // Total number of materials:\n";
  input << "  THERMAL 1 1348 70 0 1800 // material #, Capacity, Conductivity, Thermal expansion and density\n";
  input << "  FILES\n";
  input << "    CAPACITY 1 varTHERMAL\n";
  thermalcapacity << "25\t704\n";
  thermalcapacity << "200\t1174\n";
  thermalcapacity << "300\t1362\n";
  thermalcapacity << "400\t1508\n";
  thermalcapacity << "500\t1620\n";
  thermalcapacity << "600\t1706\n";
  thermalcapacity << "700\t1777\n";
  thermalcapacity << "800\t1836\n";
  thermalcapacity << "900\t1883\n";
  thermalcapacity << "1000\t1924\n";
  thermalcapacity << "1200\t2050\n";
  thermalcapacity << "2000\t2200\n";
  
  input << "    CONDUCTIVITY 1 varTHERMAL\n";
  thermalconductivity << "20\t318\n";
  thermalconductivity << "100\t286\n";
  thermalconductivity << "200\t253\n";
  thermalconductivity << "300\t225\n";
  thermalconductivity << "400\t202\n";
  thermalconductivity << "500\t183\n";
  thermalconductivity << "600\t166\n";
  thermalconductivity << "700\t153\n";
  thermalconductivity << "800\t141\n";
  thermalconductivity << "900\t130\n";
  thermalconductivity << "1000\t122\n";
  thermalconductivity << "1100\t114\n";
  thermalconductivity << "1200\t107\n";
  thermalconductivity << "1300\t101\n";
  thermalconductivity << "1400\t96\n";
  thermalconductivity << "1500\t91\n";
  thermalconductivity << "1600\t87\n";
  thermalconductivity << "1700\t83\n";
  thermalconductivity << "1800\t79\n";
  thermalconductivity << "1900\t76\n";
  thermalconductivity << "2000\t73\n";
  thermalconductivity << "3000\t70\n";
  
  input << "  ENDFILES\n";
  input << "ENDMATERIALS\n";
  input << "\n";
  input << "\n";
  input << "SYSTEM tiles\n";
  input << "  FLEXBODIES\n";
  input << "    FEMESH tile6\n";
  input << "      FORMULATION THERMAL\n";
  input << "      METHOD RPIM\n";
  input << "      BOUNDARY CLOCKWISE\n";
  input << "      MESH\n";
  input << "      1 1 4.5\n";
  input << "      TRIANGLES noFile \n";
  
  mesh << "104 206\n";
  mesh << "1 5.049890e-02 4.166574e-03 1.143152e-06\n";
  mesh << "2 1.274317e-01 3.321200e-02 2.775558e-17\n";
  mesh << "3 1.835680e-01 -5.256000e-03 0.000000e+00\n";
  mesh << "4 -4.144763e-04 -2.425600e-02 -4.553649e-17\n";
  mesh << "5 9.101796e-02 -2.425600e-02 0.000000e+00\n";
  mesh << "6 8.187472e-02 -2.425600e-02 0.000000e+00\n";
  mesh << "7 7.273147e-02 -2.425600e-02 0.000000e+00\n";
  mesh << "8 6.358823e-02 -2.425600e-02 0.000000e+00\n";
  mesh << "9 5.444499e-02 -2.425600e-02 0.000000e+00\n";
  mesh << "10 4.530174e-02 -2.425600e-02 0.000000e+00\n";
  mesh << "11 3.615850e-02 -2.425600e-02 0.000000e+00\n";
  mesh << "12 2.701525e-02 -2.425600e-02 0.000000e+00\n";
  mesh << "13 1.787201e-02 -2.425600e-02 0.000000e+00\n";
  mesh << "14 8.728767e-03 -2.425600e-02 0.000000e+00\n";
  mesh << "15 9.101796e-02 -1.504062e-02 1.462953e-16\n";
  mesh << "16 9.297018e-02 -6.827062e-03 2.690463e-16\n";
  mesh << "17 1.006295e-01 -5.256000e-03 -4.707184e-18\n";
  mesh << "18 1.098449e-01 -5.256000e-03 -4.184163e-18\n";
  mesh << "19 1.190603e-01 -5.256000e-03 -3.661143e-18\n";
  mesh << "20 1.282757e-01 -5.256000e-03 -3.138122e-18\n";
  mesh << "21 1.374911e-01 -5.256000e-03 -2.615102e-18\n";
  mesh << "22 1.467064e-01 -5.256000e-03 -2.092082e-18\n";
  mesh << "23 1.559218e-01 -5.256000e-03 -1.569061e-18\n";
  mesh << "24 1.651372e-01 -5.256000e-03 -1.046041e-18\n";
  mesh << "25 1.743526e-01 -5.256000e-03 -5.230204e-19\n";
  mesh << "26 1.855680e-01 1.735578e-03 -2.951632e-17\n";
  mesh << "27 1.855680e-01 1.060203e-02 -2.107154e-17\n";
  mesh << "28 1.855680e-01 1.946848e-02 -1.849815e-17\n";
  mesh << "29 1.855680e-01 2.849784e-02 -3.186332e-17\n";
  mesh << "30 1.806304e-01 3.319346e-02 0.000000e+00\n";
  mesh << "31 1.717639e-01 3.319655e-02 0.000000e+00\n";
  mesh << "32 1.628975e-01 3.319964e-02 0.000000e+00\n";
  mesh << "33 1.540310e-01 3.320273e-02 0.000000e+00\n";
  mesh << "34 1.451646e-01 3.320582e-02 0.000000e+00\n";
  mesh << "35 1.362981e-01 3.320891e-02 0.000000e+00\n";
  mesh << "36 1.182935e-01 3.237314e-02 0.000000e+00\n";
  mesh << "37 1.094626e-01 2.987840e-02 0.000000e+00\n";
  mesh << "38 1.010226e-01 2.624486e-02 0.000000e+00\n";
  mesh << "39 9.260198e-02 2.256514e-02 0.000000e+00\n";
  mesh << "40 8.418136e-02 1.888543e-02 0.000000e+00\n";
  mesh << "41 7.576075e-02 1.520571e-02 0.000000e+00\n";
  mesh << "42 6.734013e-02 1.152599e-02 0.000000e+00\n";
  mesh << "43 5.891951e-02 7.846272e-03 0.000000e+00\n";
  mesh << "44 4.176677e-02 1.274189e-03 8.780310e-07\n";
  mesh << "45 3.265252e-02 3.057255e-05 2.442915e-08\n";
  mesh << "46 2.344148e-02 -1.275265e-05 -9.491975e-08\n";
  mesh << "47 1.422975e-02 -7.596961e-06 -9.654254e-08\n";
  mesh << "48 5.018024e-03 -2.441257e-06 -9.755034e-08\n";
  mesh << "49 -1.433286e-03 -1.703404e-03 0.000000e+00\n";
  mesh << "50 7.856015e-02 -1.550700e-02 2.219363e-07\n";
  mesh << "51 5.821379e-02 -1.395425e-02 2.524954e-07\n";
  mesh << "52 4.127902e-02 -1.596329e-02 1.918940e-07\n";
  mesh << "53 2.291128e-02 -1.606353e-02 1.803387e-07\n";
  mesh << "54 8.392021e-02 -7.773121e-03 4.260936e-07\n";
  mesh << "55 9.533177e-02 3.539857e-03 7.264697e-07\n";
  mesh << "56 1.054222e-01 3.179447e-03 7.219902e-07\n";
  mesh << "57 1.147165e-01 3.422011e-03 7.328367e-07\n";
  mesh << "58 1.238292e-01 3.731745e-03 7.453452e-07\n";
  mesh << "59 1.329329e-01 3.956082e-03 7.556239e-07\n";
  mesh << "60 1.420808e-01 4.049683e-03 7.625172e-07\n";
  mesh << "61 1.512201e-01 3.883648e-03 7.626399e-07\n";
  mesh << "62 1.599293e-01 2.684785e-03 7.356375e-07\n";
  mesh << "63 1.689231e-01 2.194599e-03 7.272420e-07\n";
  mesh << "64 1.780031e-01 4.042710e-03 7.798253e-07\n";
  mesh << "65 1.765707e-01 1.602499e-02 1.091390e-06\n";
  mesh << "66 1.779626e-01 2.554204e-02 1.340085e-06\n";
  mesh << "67 1.690608e-01 2.475532e-02 1.315249e-06\n";
  mesh << "68 1.597567e-01 2.401211e-02 1.291351e-06\n";
  mesh << "69 1.506102e-01 2.337211e-02 1.270219e-06\n";
  mesh << "70 1.414576e-01 2.326112e-02 1.262870e-06\n";
  mesh << "71 1.324294e-01 2.309855e-02 1.254238e-06\n";
  mesh << "72 1.235208e-01 2.261788e-02 1.237374e-06\n";
  mesh << "73 1.148041e-01 2.147736e-02 1.203408e-06\n";
  mesh << "74 1.064489e-01 1.981769e-02 1.156088e-06\n";
  mesh << "75 9.247760e-02 1.348132e-02 9.841580e-07\n";
  mesh << "76 8.436030e-02 8.322194e-03 8.457572e-07\n";
  mesh << "77 7.444220e-02 5.401866e-03 7.648236e-07\n";
  mesh << "78 6.616601e-02 1.650362e-03 6.630286e-07\n";
  mesh << "79 5.743605e-02 -2.494165e-03 5.507704e-07\n";
  mesh << "80 4.809132e-02 -7.191544e-03 4.238053e-07\n";
  mesh << "81 3.743390e-02 -7.657792e-03 4.064659e-07\n";
  mesh << "82 2.787305e-02 -7.971159e-03 3.936444e-07\n";
  mesh << "83 1.858160e-02 -7.934184e-03 3.900842e-07\n";
  mesh << "84 9.154113e-03 -7.518024e-03 3.963394e-07\n";
  mesh << "85 6.868370e-02 -1.532771e-02 2.218002e-07\n";
  mesh << "86 4.963174e-02 -1.690652e-02 1.713797e-07\n";
  mesh << "87 3.211595e-02 -1.601516e-02 1.860808e-07\n";
  mesh << "88 1.392964e-02 -1.588789e-02 1.805430e-07\n";
  mesh << "89 5.632228e-03 -1.531159e-02 1.915216e-07\n";
  mesh << "90 9.883575e-02 1.858885e-02 1.120358e-06\n";
  mesh << "91 8.779631e-02 -3.946186e-04 6.202671e-07\n";
  mesh << "92 6.486155e-02 -7.232159e-03 4.309120e-07\n";
  mesh << "93 1.015975e-01 1.134023e-02 9.328007e-07\n";
  mesh << "94 7.380521e-02 -5.699843e-03 4.751992e-07\n";
  mesh << "95 1.193740e-01 1.267411e-02 9.762172e-07\n";
  mesh << "96 1.282995e-01 1.324008e-02 9.953120e-07\n";
  mesh << "97 1.373702e-01 1.351661e-02 1.006935e-06\n";
  mesh << "98 1.466731e-01 1.357954e-02 1.013104e-06\n";
  mesh << "99 1.565713e-01 1.336968e-02 1.012454e-06\n";
  mesh << "100 1.642271e-01 8.963386e-03 9.013525e-07\n";
  mesh << "101 1.712191e-01 9.748794e-03 9.252247e-07\n";
  mesh << "102 1.661388e-01 1.663710e-02 1.102263e-06\n";
  mesh << "103 1.105146e-01 1.189204e-02 9.515227e-07\n";
  mesh << "104 8.053084e-02 -1.585237e-05 6.266004e-07\n";
  mesh << "1 102 5 6 \n";
  mesh << "2 102 6 7 \n";
  mesh << "3 102 7 8 \n";
  mesh << "4 102 8 9 \n";
  mesh << "5 102 9 10 \n";
  mesh << "6 102 10 11 \n";
  mesh << "7 102 11 12 \n";
  mesh << "8 102 12 13 \n";
  mesh << "9 102 13 14 \n";
  mesh << "10 102 14 4 \n";
  mesh << "11 102 15 5 \n";
  mesh << "12 102 16 15 \n";
  mesh << "13 102 17 16 \n";
  mesh << "14 102 18 17 \n";
  mesh << "15 102 19 18 \n";
  mesh << "16 102 20 19 \n";
  mesh << "17 102 21 20 \n";
  mesh << "18 102 22 21 \n";
  mesh << "19 102 23 22 \n";
  mesh << "20 102 24 23 \n";
  mesh << "21 102 25 24 \n";
  mesh << "22 102 3 25 \n";
  mesh << "23 102 26 3 \n";
  mesh << "24 102 27 26 \n";
  mesh << "25 102 28 27 \n";
  mesh << "26 102 29 28 \n";
  mesh << "27 102 30 29 \n";
  mesh << "28 102 31 30 \n";
  mesh << "29 102 32 31 \n";
  mesh << "30 102 33 32 \n";
  mesh << "31 102 34 33 \n";
  mesh << "32 102 35 34 \n";
  mesh << "33 102 2 35 \n";
  mesh << "34 102 36 2 \n";
  mesh << "35 102 37 36 \n";
  mesh << "36 102 38 37 \n";
  mesh << "37 102 39 38 \n";
  mesh << "38 102 40 39 \n";
  mesh << "39 102 41 40 \n";
  mesh << "40 102 42 41 \n";
  mesh << "41 102 43 42 \n";
  mesh << "42 102 1 43 \n";
  mesh << "43 102 44 1 \n";
  mesh << "44 102 45 44 \n";
  mesh << "45 102 46 45 \n";
  mesh << "46 102 47 46 \n";
  mesh << "47 102 48 47 \n";
  mesh << "48 102 49 48 \n";
  mesh << "49 102 4 49 \n";
  mesh << "50 203 5 6 15 \n";
  mesh << "51 203 6 7 50 \n";
  mesh << "52 203 8 9 51 \n";
  mesh << "53 203 10 11 52 \n";
  mesh << "54 203 12 13 53 \n";
  mesh << "55 203 16 15 54 \n";
  mesh << "56 203 17 16 55 \n";
  mesh << "57 203 18 17 56 \n";
  mesh << "58 203 19 18 57 \n";
  mesh << "59 203 18 56 57 \n";
  mesh << "60 203 20 19 58 \n";
  mesh << "61 203 19 57 58 \n";
  mesh << "62 203 21 20 59 \n";
  mesh << "63 203 20 58 59 \n";
  mesh << "64 203 22 21 60 \n";
  mesh << "65 203 21 59 60 \n";
  mesh << "66 203 23 22 61 \n";
  mesh << "67 203 22 60 61 \n";
  mesh << "68 203 24 23 62 \n";
  mesh << "69 203 23 61 62 \n";
  mesh << "70 203 25 24 63 \n";
  mesh << "71 203 24 62 63 \n";
  mesh << "72 203 3 25 64 \n";
  mesh << "73 203 26 3 64 \n";
  mesh << "74 203 25 63 64 \n";
  mesh << "75 203 27 26 64 \n";
  mesh << "76 203 28 27 65 \n";
  mesh << "77 203 29 28 66 \n";
  mesh << "78 203 28 65 66 \n";
  mesh << "79 203 30 29 66 \n";
  mesh << "80 203 31 30 66 \n";
  mesh << "81 203 32 31 67 \n";
  mesh << "82 203 31 66 67 \n";
  mesh << "83 203 33 32 68 \n";
  mesh << "84 203 32 67 68 \n";
  mesh << "85 203 34 33 69 \n";
  mesh << "86 203 33 68 69 \n";
  mesh << "87 203 35 34 70 \n";
  mesh << "88 203 34 69 70 \n";
  mesh << "89 203 2 35 71 \n";
  mesh << "90 203 35 70 71 \n";
  mesh << "91 203 36 2 72 \n";
  mesh << "92 203 2 71 72 \n";
  mesh << "93 203 37 36 73 \n";
  mesh << "94 203 36 72 73 \n";
  mesh << "95 203 38 37 74 \n";
  mesh << "96 203 37 73 74 \n";
  mesh << "97 203 40 39 75 \n";
  mesh << "98 203 41 40 76 \n";
  mesh << "99 203 40 75 76 \n";
  mesh << "100 203 42 41 77 \n";
  mesh << "101 203 41 76 77 \n";
  mesh << "102 203 43 42 78 \n";
  mesh << "103 203 42 77 78 \n";
  mesh << "104 203 1 43 79 \n";
  mesh << "105 203 43 78 79 \n";
  mesh << "106 203 44 1 80 \n";
  mesh << "107 203 45 44 81 \n";
  mesh << "108 203 46 45 82 \n";
  mesh << "109 203 47 46 83 \n";
  mesh << "110 203 46 82 83 \n";
  mesh << "111 203 48 47 84 \n";
  mesh << "112 203 47 83 84 \n";
  mesh << "113 203 15 6 50 \n";
  mesh << "114 203 50 7 85 \n";
  mesh << "115 203 7 8 85 \n";
  mesh << "116 203 8 51 85 \n";
  mesh << "117 203 9 10 86 \n";
  mesh << "118 203 11 12 87 \n";
  mesh << "119 203 13 14 88 \n";
  mesh << "120 203 14 4 89 \n";
  mesh << "121 203 17 55 56 \n";
  mesh << "122 203 1 79 80 \n";
  mesh << "123 203 27 64 65 \n";
  mesh << "124 203 82 45 81 \n";
  mesh << "125 203 81 44 80 \n";
  mesh << "126 203 39 38 90 \n";
  mesh << "127 203 39 90 75 \n";
  mesh << "128 203 15 50 54 \n";
  mesh << "129 203 88 14 89 \n";
  mesh << "130 203 49 48 84 \n";
  mesh << "131 203 9 86 51 \n";
  mesh << "132 203 52 11 87 \n";
  mesh << "133 203 53 13 88 \n";
  mesh << "134 203 55 16 91 \n";
  mesh << "135 203 16 54 91 \n";
  mesh << "136 203 87 12 53 \n";
  mesh << "137 203 86 10 52 \n";
  mesh << "138 203 90 38 74 \n";
  mesh << "139 203 104 76 91 \n";
  mesh << "140 203 85 51 92 \n";
  mesh << "141 203 90 74 93 \n";
  mesh << "142 203 53 88 83 \n";
  mesh << "143 203 86 52 80 \n";
  mesh << "144 203 87 53 82 \n";
  mesh << "145 203 56 55 93 \n";
  mesh << "146 203 50 85 94 \n";
  mesh << "147 203 85 92 94 \n";
  mesh << "148 203 58 57 95 \n";
  mesh << "149 203 59 58 96 \n";
  mesh << "150 203 58 95 96 \n";
  mesh << "151 203 60 59 97 \n";
  mesh << "152 203 59 96 97 \n";
  mesh << "153 203 61 60 98 \n";
  mesh << "154 203 60 97 98 \n";
  mesh << "155 203 62 61 99 \n";
  mesh << "156 203 61 98 99 \n";
  mesh << "157 203 63 62 100 \n";
  mesh << "158 203 62 99 100 \n";
  mesh << "159 203 64 63 101 \n";
  mesh << "160 203 65 64 101 \n";
  mesh << "161 203 63 100 101 \n";
  mesh << "162 203 66 65 67 \n";
  mesh << "163 203 82 81 87 \n";
  mesh << "164 203 54 104 91 \n";
  mesh << "165 203 87 81 52 \n";
  mesh << "166 203 68 67 102 \n";
  mesh << "167 203 69 68 99 \n";
  mesh << "168 203 70 69 98 \n";
  mesh << "169 203 71 70 97 \n";
  mesh << "170 203 72 71 96 \n";
  mesh << "171 203 73 72 95 \n";
  mesh << "172 203 74 73 103 \n";
  mesh << "173 203 93 74 103 \n";
  mesh << "174 203 73 95 103 \n";
  mesh << "175 203 54 50 94 \n";
  mesh << "176 203 75 90 93 \n";
  mesh << "177 203 77 76 104 \n";
  mesh << "178 203 79 78 92 \n";
  mesh << "179 203 81 80 52 \n";
  mesh << "180 203 84 83 88 \n";
  mesh << "181 203 51 86 80 \n";
  mesh << "182 203 88 89 84 \n";
  mesh << "183 203 82 53 83 \n";
  mesh << "184 203 57 56 103 \n";
  mesh << "185 203 51 80 79 \n";
  mesh << "186 203 103 56 93 \n";
  mesh << "187 203 75 93 55 \n";
  mesh << "188 203 95 57 103 \n";
  mesh << "189 203 67 65 102 \n";
  mesh << "190 203 97 70 98 \n";
  mesh << "191 203 95 72 96 \n";
  mesh << "192 203 76 75 55 \n";
  mesh << "193 203 69 99 98 \n";
  mesh << "194 203 71 97 96 \n";
  mesh << "195 203 92 78 94 \n";
  mesh << "196 203 78 77 94 \n";
  mesh << "197 203 77 104 94 \n";
  mesh << "198 203 99 68 102 \n";
  mesh << "199 203 54 94 104 \n";
  mesh << "200 203 55 91 76 \n";
  mesh << "201 203 4 49 89 \n";
  mesh << "202 203 99 102 100 \n";
  mesh << "203 203 51 79 92 \n";
  mesh << "204 203 101 100 102 \n";
  mesh << "205 203 49 84 89 \n";
  mesh << "206 203 102 65 101 \n";
  
  input << "    ENDFEMESH\n";
  input << "  ENDFLEXBODIES\n";
  input << "\n";
  input << "MIRROR x\n";
  input << "SHIFT 2.98711 0 0\n";
  input << "\n";
  input << "  JOINTS\n";
  input << "  ENDJOINTS\n";
  input << "\n";
  input << "  LOADS\n";
  
  input <<  "    THERMALFLUENCE tile6.29 0\n";
  input <<  "    THERMALFLUENCE tile6.30 0\n";
  input <<  "    THERMALFLUENCE tile6.31 0\n";
  input <<  "    THERMALFLUENCE tile6.32 0\n";
  input <<  "    THERMALFLUENCE tile6.33 0\n";
  input <<  "    THERMALFLUENCE tile6.34 0\n";
  input <<  "    THERMALFLUENCE tile6.1 0\n";
  input <<  "    THERMALFLUENCE tile6.35 1.31E+05\n";
  input <<  "    THERMALFLUENCE tile6.36 1.22E+05\n";
  input <<  "    THERMALFLUENCE tile6.37 8.59E+04\n";
  input <<  "    THERMALFLUENCE tile6.38 5.02E+04\n";
  input <<  "    THERMALFLUENCE tile6.39 1.46E+04\n";
  input <<  "    THERMALFLUENCE tile6.40 0\n";
  input <<  "    THERMALFLUENCE tile6.41 0\n";
  input <<  "    THERMALFLUENCE tile6.42 0\n";
  input <<  "    THERMALFLUENCE tile6.0 0\n";
  input <<  "    THERMALFLUENCE tile6.43 0\n";
  input <<  "    THERMALFLUENCE tile6.44 0\n";
  input <<  "    THERMALFLUENCE tile6.45 0\n";
  input <<  "    THERMALFLUENCE tile6.46 0\n";
  input <<  "    THERMALFLUENCE tile6.47 0\n";
  input <<  "    \n";
  input <<  "    THERMALOUTPUT tile6.82 0\n";
  input <<  "    THERMALOUTPUT tile6.78 0\n";
  input <<  "    THERMALOUTPUT tile6.72 0\n";
  input <<  "    THERMALOUTPUT MAX_INTERFACE_TEMP\n";
  
  input << "\n";
  input << "  ENDLOADS\n";
  input << "\n";
  input << "ENDSYSTEM\n";
  input << "\n";
  input << "ANALYSIS\n";
  input << "  THERMALDYNAMIC\n";
  input << "    EPSILON 1E-8\n";
  input << "    INTEGRATOR BDF-1\n";
  input << "    TIME 0.0 10 10E-3\n";
  input << "  ENDTHERMALDYNAMIC\n";
  input << "ENDANALYSIS\n";
}

void Reader::generateTile6_109()
{
  input << "TITLE tile6\n";
  input << "\n";
  input << "\n";
  input << "DIMENSION 2\n";
  input << "\n";
  input << "MATERIALS // Total number of materials:\n";
  input << "  THERMAL 1 1348 70 0 1800 // material #, Capacity, Conductivity, Thermal expansion and density\n";
  input << "  FILES\n";
  input << "    CAPACITY 1 varTHERMAL\n";
  thermalcapacity << "25\t704\n";
  thermalcapacity << "200\t1174\n";
  thermalcapacity << "300\t1362\n";
  thermalcapacity << "400\t1508\n";
  thermalcapacity << "500\t1620\n";
  thermalcapacity << "600\t1706\n";
  thermalcapacity << "700\t1777\n";
  thermalcapacity << "800\t1836\n";
  thermalcapacity << "900\t1883\n";
  thermalcapacity << "1000\t1924\n";
  thermalcapacity << "1200\t2050\n";
  thermalcapacity << "2000\t2200\n";
  
  input << "    CONDUCTIVITY 1 varTHERMAL\n";
  thermalconductivity << "20\t318\n";
  thermalconductivity << "100\t286\n";
  thermalconductivity << "200\t253\n";
  thermalconductivity << "300\t225\n";
  thermalconductivity << "400\t202\n";
  thermalconductivity << "500\t183\n";
  thermalconductivity << "600\t166\n";
  thermalconductivity << "700\t153\n";
  thermalconductivity << "800\t141\n";
  thermalconductivity << "900\t130\n";
  thermalconductivity << "1000\t122\n";
  thermalconductivity << "1100\t114\n";
  thermalconductivity << "1200\t107\n";
  thermalconductivity << "1300\t101\n";
  thermalconductivity << "1400\t96\n";
  thermalconductivity << "1500\t91\n";
  thermalconductivity << "1600\t87\n";
  thermalconductivity << "1700\t83\n";
  thermalconductivity << "1800\t79\n";
  thermalconductivity << "1900\t76\n";
  thermalconductivity << "2000\t73\n";
  thermalconductivity << "3000\t70\n";
  
  input << "  ENDFILES\n";
  input << "ENDMATERIALS\n";
  input << "\n";
  input << "\n";
  input << "SYSTEM tiles\n";
  input << "  FLEXBODIES\n";
  input << "    FEMESH tile6\n";
  input << "      FORMULATION THERMAL\n";
  input << "      METHOD RPIM\n";
  input << "      BOUNDARY CLOCKWISE\n";
  input << "      MESH\n";
  input << "      1 1 4.5\n";
  input << "      TRIANGLES noFile \n";
  
  mesh << "109 216\n";
  mesh << "1 -7.101343e-04 -2.947011e-04 2.794743e-06\n";
  mesh << "2 -4.144763e-04 -2.425600e-02 -4.553649e-17\n";
  mesh << "3 9.101796e-02 -2.425600e-02 0.000000e+00\n";
  mesh << "4 1.835680e-01 -5.256000e-03 0.000000e+00\n";
  mesh << "5 1.817049e-01 3.319309e-02 -6.505213e-19\n";
  mesh << "6 6.815985e-02 -2.425600e-02 0.000000e+00\n";
  mesh << "7 4.530174e-02 -2.425600e-02 0.000000e+00\n";
  mesh << "8 2.244363e-02 -2.425600e-02 0.000000e+00\n";
  mesh << "9 1.855680e-01 1.445410e-02 -4.498950e-18\n";
  mesh << "10 1.617730e-01 -5.256000e-03 0.000000e+00\n";
  mesh << "11 1.399780e-01 -5.256000e-03 0.000000e+00\n";
  mesh << "12 1.181831e-01 -5.256000e-03 0.000000e+00\n";
  mesh << "13 9.638929e-02 -5.289026e-03 3.347710e-17\n";
  mesh << "14 9.830963e-03 -6.006798e-03 1.122403e-06\n";
  mesh << "15 2.187470e-02 -5.710756e-03 1.145264e-06\n";
  mesh << "16 3.498610e-02 -4.979808e-03 1.091806e-06\n";
  mesh << "17 4.141842e-02 -5.055218e-03 1.148784e-06\n";
  mesh << "18 4.825984e-02 -3.477657e-03 8.915719e-07\n";
  mesh << "19 5.509378e-03 -2.716292e-06 -9.921439e-08\n";
  mesh << "20 1.180407e-02 -6.239349e-06 -9.763172e-08\n";
  mesh << "21 1.809876e-02 -9.762405e-06 -9.604966e-08\n";
  mesh << "22 2.439346e-02 -1.328547e-05 -9.480210e-08\n";
  mesh << "23 3.068815e-02 -1.624814e-05 -8.306460e-08\n";
  mesh << "24 3.696452e-02 4.057304e-04 3.541966e-07\n";
  mesh << "25 4.313821e-02 1.612517e-03 1.059092e-06\n";
  mesh << "26 4.911150e-02 3.585007e-03 2.020587e-06\n";
  mesh << "27 5.488836e-02 6.084701e-03 0.000000e+00\n";
  mesh << "28 6.065637e-02 8.605260e-03 0.000000e+00\n";
  mesh << "29 6.642438e-02 1.112582e-02 0.000000e+00\n";
  mesh << "30 7.219239e-02 1.364638e-02 0.000000e+00\n";
  mesh << "31 7.796041e-02 1.616694e-02 0.000000e+00\n";
  mesh << "32 8.372842e-02 1.868749e-02 0.000000e+00\n";
  mesh << "33 8.949643e-02 2.120805e-02 0.000000e+00\n";
  mesh << "34 9.526444e-02 2.372861e-02 0.000000e+00\n";
  mesh << "35 1.010325e-01 2.624917e-02 0.000000e+00\n";
  mesh << "36 1.068005e-01 2.876973e-02 0.000000e+00\n";
  mesh << "37 1.126851e-01 3.099342e-02 0.000000e+00\n";
  mesh << "38 1.188012e-01 3.246462e-02 0.000000e+00\n";
  mesh << "39 1.250536e-01 3.315624e-02 0.000000e+00\n";
  mesh << "40 1.313474e-01 3.321063e-02 0.000000e+00\n";
  mesh << "41 1.376421e-01 3.320844e-02 0.000000e+00\n";
  mesh << "42 1.439368e-01 3.320625e-02 0.000000e+00\n";
  mesh << "43 1.502315e-01 3.320405e-02 0.000000e+00\n";
  mesh << "44 1.565262e-01 3.320186e-02 0.000000e+00\n";
  mesh << "45 1.628209e-01 3.319967e-02 0.000000e+00\n";
  mesh << "46 1.691156e-01 3.319748e-02 0.000000e+00\n";
  mesh << "47 1.754103e-01 3.319528e-02 0.000000e+00\n";
  mesh << "48 5.970730e-02 2.608291e-03 -1.995049e-07\n";
  mesh << "49 7.159245e-02 6.283023e-03 -8.254287e-07\n";
  mesh << "50 8.228939e-02 1.293032e-02 -2.029084e-06\n";
  mesh << "51 9.560329e-02 1.543347e-02 -2.420957e-06\n";
  mesh << "52 1.065634e-01 2.132571e-02 -3.478119e-06\n";
  mesh << "53 1.168169e-01 2.736029e-02 -4.567240e-06\n";
  mesh << "54 1.285525e-01 2.750285e-02 -4.516992e-06\n";
  mesh << "55 1.407816e-01 2.718377e-02 -4.374980e-06\n";
  mesh << "56 1.537928e-01 2.690758e-02 -4.236019e-06\n";
  mesh << "57 1.657972e-01 2.818428e-02 -4.401423e-06\n";
  mesh << "58 1.768238e-01 2.215766e-02 -3.173153e-06\n";
  mesh << "59 8.405363e-02 -1.368791e-02 3.085620e-06\n";
  mesh << "60 1.573947e-02 -6.639107e-03 1.282683e-06\n";
  mesh << "61 2.855023e-02 -4.893474e-03 1.032710e-06\n";
  mesh << "62 5.451169e-02 -5.406343e-04 3.698367e-07\n";
  mesh << "63 6.544069e-02 4.166347e-03 -4.603025e-07\n";
  mesh << "64 7.728384e-02 9.739958e-03 -1.450542e-06\n";
  mesh << "65 8.920572e-02 1.186501e-02 -1.779129e-06\n";
  mesh << "66 1.005465e-01 1.968474e-02 -3.203300e-06\n";
  mesh << "67 1.118928e-01 2.420396e-02 -3.994684e-06\n";
  mesh << "68 1.221554e-01 2.744151e-02 -4.547520e-06\n";
  mesh << "69 1.345940e-01 2.746237e-02 -4.469295e-06\n";
  mesh << "70 1.474221e-01 2.668047e-02 -4.234594e-06\n";
  mesh << "71 1.602002e-01 2.715531e-02 -4.241156e-06\n";
  mesh << "72 1.715645e-01 2.710397e-02 -4.156188e-06\n";
  mesh << "73 8.715526e-02 -1.135818e-03 6.997329e-07\n";
  mesh << "74 1.725214e-01 8.857554e-03 -6.518015e-07\n";
  mesh << "75 1.501593e-01 2.577487e-03 4.043387e-07\n";
  mesh << "76 1.288689e-01 3.787644e-03 3.159537e-08\n";
  mesh << "77 1.063846e-01 2.347077e-03 1.591354e-07\n";
  mesh << "78 7.348450e-02 -9.529404e-03 2.218515e-06\n";
  mesh << "79 5.951045e-02 -1.534247e-02 3.240575e-06\n";
  mesh << "80 9.670448e-02 5.764022e-03 -5.599253e-07\n";
  mesh << "81 1.586469e-01 1.035391e-02 -1.030390e-06\n";
  mesh << "82 4.800230e-02 -1.247576e-02 2.614915e-06\n";
  mesh << "83 4.682091e-03 -9.558543e-03 1.769279e-06\n";
  mesh << "84 7.034191e-02 -1.127346e-03 5.869632e-07\n";
  mesh << "85 5.478078e-02 -7.870352e-03 1.776812e-06\n";
  mesh << "86 8.307329e-02 6.227682e-03 -7.389245e-07\n";
  mesh << "87 3.919177e-02 -1.302722e-02 2.662395e-06\n";
  mesh << "88 7.766292e-02 1.008666e-03 2.258593e-07\n";
  mesh << "89 1.049245e-01 1.198329e-02 -1.697897e-06\n";
  mesh << "90 1.311254e-01 1.995417e-02 -3.052810e-06\n";
  mesh << "91 1.437654e-01 1.919855e-02 -2.824389e-06\n";
  mesh << "92 1.583409e-01 2.002874e-02 -2.887196e-06\n";
  mesh << "93 3.050546e-02 -1.767759e-02 3.496507e-06\n";
  mesh << "94 1.401957e-01 4.633218e-03 -5.563518e-08\n";
  mesh << "95 1.160990e-01 6.450060e-03 -5.632387e-07\n";
  mesh << "96 2.048033e-02 -1.436170e-02 2.794538e-06\n";
  mesh << "97 1.171350e-02 -1.533610e-02 2.923390e-06\n";
  mesh << "98 1.468050e-01 1.118381e-02 -1.267772e-06\n";
  mesh << "99 6.286831e-02 -4.834809e-03 1.248325e-06\n";
  mesh << "100 1.241421e-01 1.343784e-02 -1.849713e-06\n";
  mesh << "101 1.360920e-01 1.279620e-02 -1.647707e-06\n";
  mesh << "102 1.133263e-01 1.585580e-02 -2.384763e-06\n";
  mesh << "103 1.186065e-01 2.100399e-02 -3.336829e-06\n";
  mesh << "104 1.249754e-01 2.148896e-02 -3.387702e-06\n";
  mesh << "105 1.374152e-01 2.116429e-02 -3.243225e-06\n";
  mesh << "106 1.512828e-01 1.862239e-02 -2.664239e-06\n";
  mesh << "107 1.667289e-01 1.961789e-02 -2.752982e-06\n";
  mesh << "108 2.627839e-02 -1.097144e-02 2.182912e-06\n";
  mesh << "109 3.226443e-02 -1.048256e-02 2.128758e-06\n";
  mesh << "1 102 2 1 \n";
  mesh << "2 102 3 6 \n";
  mesh << "3 102 6 7 \n";
  mesh << "4 102 7 8 \n";
  mesh << "5 102 8 2 \n";
  mesh << "6 102 4 10 \n";
  mesh << "7 102 10 11 \n";
  mesh << "8 102 11 12 \n";
  mesh << "9 102 12 13 \n";
  mesh << "10 102 13 3 \n";
  mesh << "11 102 5 9 \n";
  mesh << "12 102 9 4 \n";
  mesh << "13 102 1 19 \n";
  mesh << "14 102 19 20 \n";
  mesh << "15 102 20 21 \n";
  mesh << "16 102 21 22 \n";
  mesh << "17 102 22 23 \n";
  mesh << "18 102 23 24 \n";
  mesh << "19 102 24 25 \n";
  mesh << "20 102 25 26 \n";
  mesh << "21 102 26 27 \n";
  mesh << "22 102 27 28 \n";
  mesh << "23 102 28 29 \n";
  mesh << "24 102 29 30 \n";
  mesh << "25 102 30 31 \n";
  mesh << "26 102 31 32 \n";
  mesh << "27 102 32 33 \n";
  mesh << "28 102 33 34 \n";
  mesh << "29 102 34 35 \n";
  mesh << "30 102 35 36 \n";
  mesh << "31 102 36 37 \n";
  mesh << "32 102 37 38 \n";
  mesh << "33 102 38 39 \n";
  mesh << "34 102 39 40 \n";
  mesh << "35 102 40 41 \n";
  mesh << "36 102 41 42 \n";
  mesh << "37 102 42 43 \n";
  mesh << "38 102 43 44 \n";
  mesh << "39 102 44 45 \n";
  mesh << "40 102 45 46 \n";
  mesh << "41 102 46 47 \n";
  mesh << "42 102 47 5 \n";
  mesh << "43 203 19 20 14 \n";
  mesh << "44 203 21 22 15 \n";
  mesh << "45 203 23 24 16 \n";
  mesh << "46 203 24 25 17 \n";
  mesh << "47 203 25 26 18 \n";
  mesh << "48 203 27 28 48 \n";
  mesh << "49 203 29 30 49 \n";
  mesh << "50 203 31 32 50 \n";
  mesh << "51 203 33 34 51 \n";
  mesh << "52 203 35 36 52 \n";
  mesh << "53 203 37 38 53 \n";
  mesh << "54 203 39 40 54 \n";
  mesh << "55 203 41 42 55 \n";
  mesh << "56 203 43 44 56 \n";
  mesh << "57 203 45 46 57 \n";
  mesh << "58 203 5 9 58 \n";
  mesh << "59 203 13 3 59 \n";
  mesh << "60 203 3 6 59 \n";
  mesh << "61 203 20 21 60 \n";
  mesh << "62 203 22 23 61 \n";
  mesh << "63 203 24 17 16 \n";
  mesh << "64 203 25 18 17 \n";
  mesh << "65 203 26 27 62 \n";
  mesh << "66 203 28 29 63 \n";
  mesh << "67 203 30 31 64 \n";
  mesh << "68 203 32 33 65 \n";
  mesh << "69 203 34 35 66 \n";
  mesh << "70 203 36 37 67 \n";
  mesh << "71 203 38 39 68 \n";
  mesh << "72 203 40 41 69 \n";
  mesh << "73 203 42 43 70 \n";
  mesh << "74 203 44 45 71 \n";
  mesh << "75 203 46 47 72 \n";
  mesh << "76 203 47 5 58 \n";
  mesh << "77 203 13 59 73 \n";
  mesh << "78 203 9 4 74 \n";
  mesh << "79 203 4 10 74 \n";
  mesh << "80 203 10 11 75 \n";
  mesh << "81 203 11 12 76 \n";
  mesh << "82 203 12 13 77 \n";
  mesh << "83 203 59 6 78 \n";
  mesh << "84 203 60 21 15 \n";
  mesh << "85 203 6 7 79 \n";
  mesh << "86 203 14 20 60 \n";
  mesh << "87 203 15 22 61 \n";
  mesh << "88 203 26 62 18 \n";
  mesh << "89 203 48 28 63 \n";
  mesh << "90 203 49 30 64 \n";
  mesh << "91 203 50 32 65 \n";
  mesh << "92 203 51 34 66 \n";
  mesh << "93 203 52 36 67 \n";
  mesh << "94 203 53 38 68 \n";
  mesh << "95 203 54 40 69 \n";
  mesh << "96 203 55 42 70 \n";
  mesh << "97 203 56 44 71 \n";
  mesh << "98 203 57 46 72 \n";
  mesh << "99 203 58 9 74 \n";
  mesh << "100 203 61 23 16 \n";
  mesh << "101 203 64 31 50 \n";
  mesh << "102 203 65 33 51 \n";
  mesh << "103 203 66 35 52 \n";
  mesh << "104 203 67 37 53 \n";
  mesh << "105 203 68 39 54 \n";
  mesh << "106 203 69 41 55 \n";
  mesh << "107 203 70 43 56 \n";
  mesh << "108 203 71 45 57 \n";
  mesh << "109 203 77 13 80 \n";
  mesh << "110 203 78 6 79 \n";
  mesh << "111 203 63 29 49 \n";
  mesh << "112 203 62 27 48 \n";
  mesh << "113 203 13 73 80 \n";
  mesh << "114 203 74 10 81 \n";
  mesh << "115 203 81 107 74 \n";
  mesh << "116 203 10 75 81 \n";
  mesh << "117 203 79 7 82 \n";
  mesh << "118 203 1 19 83 \n";
  mesh << "119 203 63 49 84 \n";
  mesh << "120 203 18 62 85 \n";
  mesh << "121 203 64 50 86 \n";
  mesh << "122 203 16 17 87 \n";
  mesh << "123 203 49 64 88 \n";
  mesh << "124 203 64 86 88 \n";
  mesh << "125 203 65 51 80 \n";
  mesh << "126 203 66 52 89 \n";
  mesh << "127 203 67 53 103 \n";
  mesh << "128 203 54 69 90 \n";
  mesh << "129 203 55 70 91 \n";
  mesh << "130 203 56 71 92 \n";
  mesh << "131 203 77 80 89 \n";
  mesh << "132 203 93 87 7 \n";
  mesh << "133 203 73 59 78 \n";
  mesh << "134 203 83 19 14 \n";
  mesh << "135 203 75 11 94 \n";
  mesh << "136 203 11 76 94 \n";
  mesh << "137 203 12 77 95 \n";
  mesh << "138 203 60 15 96 \n";
  mesh << "139 203 14 60 97 \n";
  mesh << "140 203 60 96 97 \n";
  mesh << "141 203 8 2 97 \n";
  mesh << "142 203 75 94 98 \n";
  mesh << "143 203 17 18 82 \n";
  mesh << "144 203 48 63 99 \n";
  mesh << "145 203 63 84 99 \n";
  mesh << "146 203 76 12 95 \n";
  mesh << "147 203 78 79 99 \n";
  mesh << "148 203 76 95 100 \n";
  mesh << "149 203 94 76 101 \n";
  mesh << "150 203 98 94 101 \n";
  mesh << "151 203 76 100 101 \n";
  mesh << "152 203 95 77 89 \n";
  mesh << "153 203 14 97 83 \n";
  mesh << "154 203 65 80 73 \n";
  mesh << "155 203 49 88 84 \n";
  mesh << "156 203 80 51 89 \n";
  mesh << "157 203 52 67 102 \n";
  mesh << "158 203 53 68 103 \n";
  mesh << "159 203 68 54 104 \n";
  mesh << "160 203 103 68 104 \n";
  mesh << "161 203 54 90 104 \n";
  mesh << "162 203 69 55 105 \n";
  mesh << "163 203 70 56 106 \n";
  mesh << "164 203 71 57 107 \n";
  mesh << "165 203 82 7 87 \n";
  mesh << "166 203 73 78 88 \n";
  mesh << "167 203 81 75 98 \n";
  mesh << "168 203 96 15 108 \n";
  mesh << "169 203 15 61 108 \n";
  mesh << "170 203 61 16 109 \n";
  mesh << "171 203 108 61 109 \n";
  mesh << "172 203 16 87 109 \n";
  mesh << "173 203 87 93 109 \n";
  mesh << "174 203 97 96 8 \n";
  mesh << "175 203 102 67 103 \n";
  mesh << "176 203 93 8 96 \n";
  mesh << "177 203 69 105 90 \n";
  mesh << "178 203 106 56 92 \n";
  mesh << "179 203 70 106 91 \n";
  mesh << "180 203 71 107 92 \n";
  mesh << "181 203 82 87 17 \n";
  mesh << "182 203 7 8 93 \n";
  mesh << "183 203 82 18 85 \n";
  mesh << "184 203 52 102 89 \n";
  mesh << "185 203 79 82 85 \n";
  mesh << "186 203 105 55 91 \n";
  mesh << "187 203 85 62 99 \n";
  mesh << "188 203 62 48 99 \n";
  mesh << "189 203 50 65 86 \n";
  mesh << "190 203 51 66 89 \n";
  mesh << "191 203 57 72 107 \n";
  mesh << "192 203 73 88 86 \n";
  mesh << "193 203 47 58 72 \n";
  mesh << "194 203 78 99 84 \n";
  mesh << "195 203 72 58 107 \n";
  mesh << "196 203 97 2 83 \n";
  mesh << "197 203 100 95 102 \n";
  mesh << "198 203 58 74 107 \n";
  mesh << "199 203 92 107 81 \n";
  mesh << "200 203 90 105 101 \n";
  mesh << "201 203 79 85 99 \n";
  mesh << "202 203 109 93 108 \n";
  mesh << "203 203 103 104 100 \n";
  mesh << "204 203 91 106 98 \n";
  mesh << "205 203 81 98 106 \n";
  mesh << "206 203 65 73 86 \n";
  mesh << "207 203 102 95 89 \n";
  mesh << "208 203 2 1 83 \n";
  mesh << "209 203 88 78 84 \n";
  mesh << "210 203 105 91 101 \n";
  mesh << "211 203 91 98 101 \n";
  mesh << "212 203 104 90 100 \n";
  mesh << "213 203 100 90 101 \n";
  mesh << "214 203 108 93 96 \n";
  mesh << "215 203 81 106 92 \n";
  mesh << "216 203 100 102 103 \n";
  
  input << "    ENDFEMESH\n";
  input << "  ENDFLEXBODIES\n";
  input << "\n";
  input << "MIRROR x\n";
  input << "SHIFT 2.98711 0 0\n";
  input << "\n";
  input << "  JOINTS\n";
  input << "  ENDJOINTS\n";
  input << "\n";
  input << "  LOADS\n";
  
  input << "    THERMALFLUENCE tile6.4 0\n";
  input << "    THERMALFLUENCE tile6.46 0\n";
  input << "    THERMALFLUENCE tile6.45 0\n";
  input << "    THERMALFLUENCE tile6.44 0\n";
  input << "    THERMALFLUENCE tile6.43 0\n";
  input << "    THERMALFLUENCE tile6.42 0\n";
  input << "    THERMALFLUENCE tile6.41 0\n";
  input << "    THERMALFLUENCE tile6.40 0\n";
  input << "    THERMALFLUENCE tile6.39 0\n";
  input << "    THERMALFLUENCE tile6.38 0\n";
  input << "    THERMALFLUENCE tile6.37 0\n";
  input << "    THERMALFLUENCE tile6.36 0\n";
  input << "    THERMALFLUENCE tile6.35 0\n";
  input << "    THERMALFLUENCE tile6.34 0\n";
  input << "    THERMALFLUENCE tile6.33 0\n";
  input << "    THERMALFLUENCE tile6.32 0\n";
  input << "    THERMALFLUENCE tile6.31 0\n";
  input << "    THERMALFLUENCE tile6.30 0\n";
  input << "    THERMALFLUENCE tile6.29 0\n";
  input << "    THERMALFLUENCE tile6.28 0\n";
  input << "    THERMALFLUENCE tile6.27 0\n";
  input << "    THERMALFLUENCE tile6.26 0\n";
  input << "    THERMALFLUENCE tile6.25 0\n";
  input << "    THERMALFLUENCE tile6.24 0\n";
  input << "    THERMALFLUENCE tile6.23 0\n";
  input << "    THERMALFLUENCE tile6.22 0\n";
  input << "    THERMALFLUENCE tile6.21 0\n";
  input << "    THERMALFLUENCE tile6.20 0\n";
  input << "    THERMALFLUENCE tile6.19 0\n";
  input << "    THERMALFLUENCE tile6.18 0\n";
  input << "    THERMALFLUENCE tile6.0 0\n";
  input << "    \n";
  input << "    THERMALOUTPUT tile6.102 0\n";
  input << "    THERMALOUTPUT tile6.98 0\n";
  input << "    THERMALOUTPUT tile6.14 0\n";
  input << "    THERMALOUTPUT MAX_INTERFACE_TEMP\n";
  
  input << "\n";
  input << "  ENDLOADS\n";
  input << "\n";
  input << "ENDSYSTEM\n";
  input << "\n";
  input << "ANALYSIS\n";
  input << "  THERMALDYNAMIC\n";
  input << "    EPSILON 1E-8\n";
  input << "    INTEGRATOR BDF-1\n";
  input << "    TIME 0.0 10 10E-3\n";
  input << "  ENDTHERMALDYNAMIC\n";
  input << "ENDANALYSIS\n";
}



} // namespace mknix

void mknix::Reader::readInput(int outputFileDetail)
{
  if(outputFileDetail > 0){
    output.open("output.reader");
  }

    char keyword[100];
    char a; // For reading one-by-one the file.

    while(input >> keyword) {

        if(!strcmp(keyword,"//")) {
            do {
                input.get(a);
            }
            while(a!='\n');
        }
        else if(!strcmp(keyword,"TITLE")) {
            input >> theSimulation->title;
            output << "TITLE: " << theSimulation->title << std::endl;
        }
        else if(!strcmp(keyword,"WORKINGDIR")) {
            std::string working_dir;
            input >> keyword;
            int success=chdir(keyword);
            working_dir = getcwd(keyword,100);
            output << "WORKINGDIR: " << working_dir.c_str() << std::endl;
        }
        else if(!strcmp(keyword,"GRAVITY")) {
            double value;
            input >> value;
            Simulation::gravity(0) = value;
            input >> value;
            Simulation::gravity(1) = value;
            input >> value;
            Simulation::gravity(2) = value;
            output << "GRAVITY: "
                   << Simulation::getGravity(0) << ", "
                   << Simulation::getGravity(1) << ", "
                   << Simulation::getGravity(2) << std::endl;
        }
        else if( !strcmp(keyword,"DIMENSION") )
        {
            int value;

            input  >> value;
            Simulation::dimension = value;

            output << "DIMENSION: "
                   << Simulation::dimension << std::endl;
        }
        else if(!strcmp(keyword,"MATERIALS")) {
            char a;
            do {
                input.get(a);
            }
            while(a!='\n');
            output << "MATERIALS: " << std::endl;
            while(input >> keyword) {
                if(!strcmp(keyword,"ENDMATERIALS")) break;
                // TODO Add PLSTRESS and 3D
                else if(!strcmp(keyword,"PLSTRAIN")) {
                    int num_mat;
                    double young, poisson, density;
                    input >> num_mat >> young >> poisson >> density;

                    if( theSimulation->materials.count(num_mat)==0 )
                        theSimulation->materials[num_mat];

                    theSimulation->materials.at(num_mat).setMechanicalProps( Simulation::dimension, young, poisson, density );
                    output << "MATERIAL: " << keyword
                           << ", number = " << num_mat << ",\n\t E = " << young
                           << ", mu = " << poisson << ", density = " << density << std::endl;
                    do {
                        input.get(a);
                    }
                    while(a!='\n');
                }
                else if(!strcmp(keyword,"THERMAL")) {
                    int num_mat;
                    double capacity, kappa, beta, density; // Capacity, Conductivity, Thermal expansion, Density
                    input >> num_mat >> capacity >> kappa >> beta >> density;

                    if( theSimulation->materials.count(num_mat)==0 )
                        theSimulation->materials[num_mat];

                    theSimulation->materials.at(num_mat).setThermalProps( capacity, kappa, beta, density );
                    output << "MATERIAL: " << keyword
                           << ", number = " << num_mat << ",\n\t Cp = " << capacity
                           << ", kappa = " << kappa << ", density = " << density << std::endl;
                    do {
                        input.get(a);
                    }
                    while(a!='\n');
                }
                else if(!strcmp(keyword,"FILES")) {
		  while(input >> keyword) {
		    if(!strcmp(keyword,"ENDFILES")) break;
		    else if(!strcmp(keyword,"CAPACITY")) {
		      int num_mat;
		      double temperature, capacity;
		      char a;
		      input >> num_mat >> keyword;
		      if( theSimulation->materials.count(num_mat)==0 )
			  theSimulation->materials[num_mat];
		      output << "THERMALFILE CAPACITY: " << keyword 
			     << " for mat # " << num_mat << endl;
// 		      std::ifstream thermalfile( keyword ); // file to read points from
		      while(thermalcapacity >> temperature){
			thermalcapacity >> capacity;
			theSimulation->materials.at(num_mat).addThermalCapacity( temperature, capacity );
		      output << "TEMP:" << temperature << ", \t" << capacity << endl;
		      }
		      do {
			  input.get(a);
		      }
                      while(a!='\n');
		    }
		    else if(!strcmp(keyword,"CONDUCTIVITY")) {
		      int num_mat;
		      double temperature, conductivity;
		      char a;
		      input >> num_mat >> keyword;
		      if( theSimulation->materials.count(num_mat)==0 )
			  theSimulation->materials[num_mat];
		      output << "THERMALFILE CONDUCTIVITY: " << keyword 
			     << " for mat # " << num_mat << endl;
// 		      std::ifstream thermalfile( keyword ); // file to read points from
		      while(thermalconductivity >> temperature){
			thermalconductivity >> conductivity;
			theSimulation->materials.at(num_mat).addThermalConductivity( temperature, conductivity );
		      output << "TEMP:" << temperature << ", \t" << conductivity << endl;
		      }
		      do {
			  input.get(a);
		      }
                      while(a!='\n');
		    }
		  }
		}
            }
        }
        else if( !strcmp(keyword,"CONTACT") )
        {
            std::string type;
            input  >> type; // Valid types: GLOBAL, NONE
            Simulation::contact = type;
            output << "CONTACT: "
                   << Simulation::contact << std::endl;
        }
        else if( !strcmp(keyword,"VISUALIZATION") )
        {
            std::string type;
            input  >> type;
            if(type == "ON") Simulation::visualization = 1;
            if(type == "OFF")Simulation::visualization = 0;
            output << "VISUALIZATION: "
                   << Simulation::contact << std::endl;
        }
        else if( !strcmp(keyword,"OUTPUT") )
        {
            std::string type;
            input  >> type;
            if(type == "MATRICES") {
                Simulation::outputMatrices = 1;
                output << "OUTPUT: MATRICES"
                       << Simulation::outputMatrices << std::endl;
            }
//      else if(type == "OTHER")
        }
        else if( !strcmp(keyword,"SMOOTHING") )
        {
            std::string type;
            input  >> type;
            if(type == "OFF") {
                Simulation::smoothingType = "OFF";
                output << keyword << " "
                       << Simulation::smoothingType << std::endl;
            }
            if(type == "LOCAL") {
                Simulation::smoothingType = "LOCAL";
                output << keyword << " "
                       << Simulation::smoothingType << std::endl;
            }
            if(type == "CONSTANT") {
                Simulation::smoothingType = "CONSTANT";
                output << keyword << " "
                       << Simulation::smoothingType << std::endl;
            }
            if(type == "GLOBAL") {
                Simulation::smoothingType = "GLOBAL";
                output << keyword << " "
                       << Simulation::smoothingType << std::endl;
            }
//      else if(type == "OTHER")
        }
        else if(!strcmp(keyword,"SYSTEM")) {
            this->readSystem( theSimulation->baseSystem );
        }
        else if(!strcmp(keyword,"ANALYSIS")) {
            this->readAnalysis( );
        }
    }

}

void mknix::Reader::readSystem(System* system_in)
{
    char sysTitle[30], keyword[20];
    input >> sysTitle;
    output << "SYSTEM: "
           << system_in->getTitle()
           << "."
           << sysTitle << std::endl;

    system_in->subSystems[sysTitle] = new System( sysTitle );
    while(input >> keyword) {
        if(!strcmp(keyword,"ENDSYSTEM")) return;

        else if(!strcmp(keyword,"RIGIDBODIES")) {
        }
        else if(!strcmp(keyword,"FLEXBODIES")) {
            theReaderFlex = new ReaderFlex( theSimulation, output, input, mesh );
            theReaderFlex->readFlexBodies( system_in->subSystems[sysTitle] );
            delete theReaderFlex;
            theReaderFlex = 0;
        }
        else if(!strcmp(keyword,"BODYPOINTS")) {
            this->readBodyPoints( system_in->subSystems[sysTitle] );
        }
        else if(!strcmp(keyword,"JOINTS")) {
        }
        else if(!strcmp(keyword,"LOADS")) {
            this->readLoads( system_in->subSystems[sysTitle] );
        }
        else if(!strcmp(keyword,"ENVIRONMENT")) {
            this->readEnvironment( system_in->subSystems[sysTitle] );
        }
        else if(!strcmp(keyword,"MOTION")) {
            this->readMotion( system_in->subSystems[sysTitle] );
        }
        else if(!strcmp(keyword,"SCALE")) {
          double temp, xValue, yValue, zValue;
          std::map<int,Node*>::iterator it_nodes;
          
          input >> xValue >> yValue >> zValue;
          
          for ( it_nodes = theSimulation->nodes.begin();
                it_nodes!= theSimulation->nodes.end();
                ++it_nodes){
            temp = it_nodes->second->getX();
            it_nodes->second->setX(temp*xValue);
            temp = it_nodes->second->getY();
            it_nodes->second->setY(temp*yValue);
            temp = it_nodes->second->getZ();
            it_nodes->second->setZ(temp*zValue);
          }
        }
        else if(!strcmp(keyword,"MIRROR")) {
          double temp; 
	  std::string axis;
          std::map<int,Node*>::iterator it_nodes;
          
          input >> axis;
          
	  if(axis=="x" || axis=="X"){
	    for ( it_nodes = theSimulation->nodes.begin();
		  it_nodes!= theSimulation->nodes.end();
		  ++it_nodes){
	      temp = it_nodes->second->getX();
	      it_nodes->second->setX(-temp);
	    }
	  }
          else if(axis=="y" || axis=="Y"){
	    for ( it_nodes = theSimulation->nodes.begin();
	      it_nodes!= theSimulation->nodes.end();
	      ++it_nodes){
	      temp = it_nodes->second->getY();
	      it_nodes->second->setY(-temp);
	    }
	  }
          else if(axis=="z" || axis=="Z"){
	    for ( it_nodes = theSimulation->nodes.begin();
	      it_nodes!= theSimulation->nodes.end();
	      ++it_nodes){
	      temp = it_nodes->second->getZ();
	      it_nodes->second->setZ(-temp);
	    }
	  }
        }
        else if(!strcmp(keyword,"SHIFT")) {
          double temp, xValue, yValue, zValue;
          std::map<int,Node*>::iterator it_nodes;
          
          input >> xValue >> yValue >> zValue;
          
          for ( it_nodes = theSimulation->nodes.begin();
               it_nodes!= theSimulation->nodes.end();
               ++it_nodes){
            temp = it_nodes->second->getX();
            it_nodes->second->setX(temp+xValue);
            temp = it_nodes->second->getY();
            it_nodes->second->setY(temp+yValue);
            temp = it_nodes->second->getZ();
            it_nodes->second->setZ(temp+zValue);
          }
        }
    }
}

void mknix::Reader::readBodyPoints(System * system_in)
{
    char keyword[20];
//   std::string loadTitle;

        Node * pNode=0;
        double x, y, z;

    while(input >> keyword) {
        if(!strcmp(keyword,"ENDBODYPOINTS")) return;

//       if (a == '.'){ // we read the node...
//         while(input.get(a)){
//           if (a == '\n') break;
//           else if (a==' ') break;
//           else{
//             sNode.push_back( a );
//           }
//         }
//       }
        input >> x >> y >> z;

        int nodeNumber(0);

        { //the body is a flexbody
            double alpha, dc;
            input >> alpha >> dc;
	    nodeNumber = system_in->flexBodies[keyword]->getNumberOfPoints();
            
	    system_in->flexBodies[keyword]->addPoint( nodeNumber, x, y, z, alpha, dc );
        }
    }
}


void mknix::Reader::readLoads(System * system_in)
{
    char keyword[20];
//   std::string loadTitle;

    while(input >> keyword) {
        if(!strcmp(keyword,"ENDLOADS")) return;


        else if(!strcmp(keyword,"THERMALFLUENCE")) {
            std::string sBody;
            std::string sNode;
            Node * pNode=0;
            char a;
            double fluence;

            while(input.get(a)) { // we read the body...
                if (a == '.') break;
                else if (a=='\n') break;
                else if (a==' ') ; //get blank space
                else {
                    sBody.push_back( a );
                }
            }
            if (a == '.') { // we read the node...
                while(input.get(a)) {
                    if (a == '\n') break;
                    else if (a==' ') break;
                    else {
                        sNode.push_back( a );
                    }
                }
            }
                pNode = system_in->flexBodies[sBody]->getNode( atoi( sNode.c_str() ) );

            input >> fluence;
	    output << "THERMALFLUENCE " << pNode->getNumber() << " " << fluence << endl;

            system_in->loadsThermal.push_back( new LoadThermal( pNode, fluence ) );
        }
         else if(!strcmp(keyword,"THERMALOUTPUT")) {
            std::string sBody;
            std::string sNode;
            Node * pNode=0;
            char a;

            while(input.get(a)) { // we read the body...
                if (a == '.') break;
                else if (a=='\n') break;
                else if (a==' ') ; //get blank space
                else {
                    sBody.push_back( a );
                }
            }
            if (a == '.') { // we read the node...
                while(input.get(a)) {
                    if (a == '\n') break;
                    else if (a==' ') break;
                    else {
                        sNode.push_back( a );
                    }
                }
            }
            if( sBody=="MAX_INTERFACE_TEMP" )
	    { // avoid else
	    }
            else //the body is a flexbody
                pNode = system_in->flexBodies[sBody]->getNode( atoi( sNode.c_str() ) );

	    if( sBody=="MAX_INTERFACE_TEMP" ){
	      system_in->outputMaxInterfaceTemp = true;
	    }
	    else{
	      output << "THERMALOUTPUT " << pNode->getNumber() << endl;

	      system_in->outputSignalThermal.push_back( pNode );
	    }
        }
   }
}

void mknix::Reader::readEnvironment(System * system_in)
{
    // Prepared to read radiation and convection.
    // At this moment they are implemented as loads
    char keyword[20];

    while(input >> keyword) {
        if(!strcmp(keyword,"ENDENVIRONMENT")) return;
    }
}

void mknix::Reader::readMotion(System* system_in)
{

}

void mknix::Reader::readAnalysis( )
{
    char keyword[20];

    output << "ANALYSYS: " << std::endl;

    while(input >> keyword) {
        if(!strcmp(keyword,"ENDANALYSIS")) return;
        else if(!strcmp(keyword,"THERMALDYNAMIC")) {
            char integratorType[20];
            double to, tf, At;
            output << "\t"<< keyword << ":"
                   << std::endl;
            while(input >> keyword) {
                if(!strcmp(keyword,"ENDTHERMALDYNAMIC")) break;
                else if(!strcmp(keyword,"EPSILON")) {
                    input >> to; //just to not create another variable
                    Simulation::epsilon = to;
                    output << "\t\t"
                           << "EPSILON: "
                           << Simulation::epsilon
                           << endl;
                }
                else if(!strcmp(keyword,"INTEGRATOR")) {
                    input >> integratorType;
                    output << "\t\t"
                           << "INTEGRATOR: " << integratorType
                           << std::endl;
                }
                else if(!strcmp(keyword,"TIME")) {
                    input >> to
                          >> tf
                          >> At;
                    output << "\t\t"
                           << "TIME: "
                           << to << ", "
                           << tf << ", "
                           << At << std::endl;
                }
            }
            this->theSimulation->analysis.push_back
            ( new AnalysisThermalDynamic( theSimulation, to, tf, At, integratorType ) );
        }
        else if(!strcmp(keyword,"OTRO")) {
        }
    }
}

