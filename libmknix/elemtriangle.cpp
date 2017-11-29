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

#include "elemtriangle.h"
#include "material.h"
#include "node.h"
#include "gausspoint2D.h"
#include <vector>

namespace mknix {

ElemTriangle::ElemTriangle()
    : CellTriang()
{
}


ElemTriangle::ElemTriangle( Material& material_in,
                            double alpha_in,
                            int nGPoints_in,
                            Node* n1_in,
                            Node* n2_in,
                            Node* n3_in
                          )
    : CellTriang( material_in,
                  std::string(),
                  alpha_in,
                  nGPoints_in,
                  n1_in,
                  n2_in,
                  n3_in
                )
{}


ElemTriangle::~ElemTriangle()
{
   for( int i = 0 ; i < gPoints_MC.size(); i++)
     delete gPoints_MC[i];
   gPoints_MC.clear();
}


void ElemTriangle::initialize( std::vector<Node*> & nodes_in )
{
//   cout << "CellTriang points " << this->points << endl;

  gPoints_MC.clear();
  this->createGaussPoints_MC();
    // This function can be joined with assembleGaussPoints so the Gpoints are iterated only once...
    int i;
    for ( std::vector<GaussPoint*>::iterator it = gPoints.begin();
            it != gPoints.end();
            ++it)
    {
        for( i=0; i<3; ++i )
            (*it)->addSupportNode( this->nodes[i] );
    }
    for ( std::vector<GaussPoint*>::iterator it = gPoints_MC.begin();
            it != gPoints_MC.end();
            ++it)
    {
        for( i=0; i<3; ++i )
            (*it)->addSupportNode( this->nodes[i] );
    }
}

void ElemTriangle::computeShapeFunctions(  )
{
    for ( std::vector<GaussPoint*>::iterator it = gPoints.begin();
            it != gPoints.end();
            ++it)
    {
        (*it)->fillFEmatrices(  );
    }
    for ( std::vector<GaussPoint*>::iterator it = gPoints_MC.begin();
            it != gPoints_MC.end();
            ++it)
    {
        (*it)->fillFEmatrices(  );
    }

}

void ElemTriangle::createGaussPoints_MC()
{
  int nGPoints_MC = 3;
  if(nGPoints == 1) nGPoints_MC = 3;
  else if(nGPoints == 3) nGPoints_MC = 7;
  else if(nGPoints == 4) nGPoints_MC = 7;
  
    lmx::DenseMatrix<double> gCoef( size_type(nGPoints_MC), 4);

    // reference: http://www.cs.rpi.edu/~flaherje/pdf/fea6.pdf
    if (nGPoints_MC == 1) {
        gCoef(0,0) = 1./3.;
        gCoef(0,1) = 1./3.;
        gCoef(0,2) = 1./3.;
        gCoef(0,3) = 1.;
    }
    else if(nGPoints_MC == 3) {
        gCoef(0,0) = 0.5;
        gCoef(0,1) = 0.5;
        gCoef(0,2) = 0. ;
        gCoef(0,3) = 1./3.;
        gCoef(1,0) = 0. ;
        gCoef(1,1) = 0.5;
        gCoef(1,2) = 0.5;
        gCoef(1,3) = 1./3.;
        gCoef(2,0) = 0.5;
        gCoef(2,1) = 0. ;
        gCoef(2,2) = 0.5;
        gCoef(2,3) = 1./3.;
    }
    else if(nGPoints_MC == 4) { //must be checked
        gCoef(0,0) = 1./3.;
        gCoef(0,1) = 1./3.;
        gCoef(0,2) = 1./3.;
        gCoef(0,3) = -27./48.;
        gCoef(1,0) = 0.6;
        gCoef(1,1) = 0.2;
        gCoef(1,2) = 0.2;
        gCoef(1,3) = 25./48.;
        gCoef(2,0) = 0.2;
        gCoef(2,1) = 0.6;
        gCoef(2,2) = 0.2;
        gCoef(2,3) = 25./48.;
        gCoef(3,0) = 0.2;
        gCoef(3,1) = 0.2;
        gCoef(3,2) = 0.6;
        gCoef(3,3) = 25./48.;
    }
    else if(nGPoints_MC == 6) { //must be checked
        double alpha1 = 0.8168475730;
        double alpha2 = 0.0915762135;
        double beta1 = 0.1081030182;
        double beta2 = 0.4459484909;
        gCoef(3,0) = beta1;
        gCoef(3,1) = beta1;
        gCoef(3,2) = alpha1;
        gCoef(3,3) = 0.0549758718;
        gCoef(1,0) = alpha1;
        gCoef(1,1) = beta1;
        gCoef(1,2) = beta1;
        gCoef(1,3) = 0.0549758718;
        gCoef(2,0) = beta1;
        gCoef(2,1) = alpha1;
        gCoef(2,2) = beta1;
        gCoef(2,3) = 0.0549758718;
        gCoef(3,0) = beta2;
        gCoef(3,1) = beta2;
        gCoef(3,2) = alpha2;
        gCoef(3,3) = 0.1116907998;
        gCoef(4,0) = alpha2;
        gCoef(4,1) = beta2;
        gCoef(4,2) = beta2;
        gCoef(4,3) = 0.1116907998;
        gCoef(5,0) = beta2;
        gCoef(5,1) = alpha2;
        gCoef(5,2) = beta2;
        gCoef(5,3) = 0.1116907998;
    }
    else if(nGPoints_MC == 7) {
        double alpha1 = 0.0597158717;
        double alpha2 = 0.7974269853;
        double beta1 = 0.4701420641;
        double beta2 = 0.1012865073;
        gCoef(0,0) = 1./3.;
        gCoef(0,1) = 1./3.;
        gCoef(0,2) = 1./3.;
        gCoef(0,3) = 0.225;
        gCoef(1,0) = alpha1;
        gCoef(1,1) = beta1;
        gCoef(1,2) = beta1;
        gCoef(1,3) = 0.1323941527;
        gCoef(2,0) = beta1;
        gCoef(2,1) = alpha1;
        gCoef(2,2) = beta1;
        gCoef(2,3) = 0.1323941527;
        gCoef(3,0) = beta1;
        gCoef(3,1) = beta1;
        gCoef(3,2) = alpha1;
        gCoef(3,3) = 0.1323941527;
        gCoef(4,0) = alpha2;
        gCoef(4,1) = beta2;
        gCoef(4,2) = beta2;
        gCoef(4,3) = 0.1259391805;
        gCoef(5,0) = beta2;
        gCoef(5,1) = alpha2;
        gCoef(5,2) = beta2;
        gCoef(5,3) = 0.1259391805;
        gCoef(6,0) = beta2;
        gCoef(6,1) = beta2;
        gCoef(6,2) = alpha2;
        gCoef(6,3) = 0.1259391805;
    }
    // more on ... http://electromagnetics.biz/2D%20Gauss.txt
//   else if(nGPoints == 12){
//   }
//   else if(nGPoints == 20){
//   }

    for (int i=0; i<nGPoints_MC; ++i) {
        gPoints_MC.push_back( new GaussPoint2D( this->alpha,
                                             gCoef(i,3),
                                             jacobian,
                                             mat,
                                             i,
                                             points(0,0) * gCoef(i,0) + points(1,0) * gCoef(i,1) + points(2,0) * gCoef(i,2),
                                             points(0,1) * gCoef(i,0) + points(1,1) * gCoef(i,1) + points(2,1) * gCoef(i,2),
                                             dc
                                           )
                         );
    }
}


}
