//-- Licencia --
#include "gausspoint.h"
#include "node.h"
#include "shapefunctiontriangle.h"
#include "simulation.h"
#include "system.h"

namespace mknix {

GaussPoint::GaussPoint()
{
}


GaussPoint::GaussPoint( int dim_in,
                        double alpha_in,
                        double weight_in,
                        double jacobian_in,
                        Material* mat_in,
                        int num_in,
                        double coor_x,
                        double coor_y,
                        double dc_in
                      )
    : Point(dim_in, coor_x, coor_y, 0., jacobian_in, alpha_in, dc_in)
    , num(num_in)
    , weight(weight_in)
    , mat(mat_in)
{
}

GaussPoint::GaussPoint( int dim_in,
                        double alpha_in,
                        double weight_in,
                        double jacobian_in,
                        Material* mat_in,
                        int num_in,
                        double coor_x,
                        double coor_y,
                        double coor_z,
                        double dc_in
                      )
    : Point(dim_in, coor_x, coor_y, coor_z, jacobian_in, alpha_in, dc_in)
    , num(num_in)
    , weight(weight_in)
    , mat(mat_in)
{
}

GaussPoint::~GaussPoint()
{
}


void GaussPoint::shapeFunSolve( std::string type_in, double q_in )
{

}



void GaussPoint::computeCij( )
{
    //////////////// Calculation of Capacity matrix:
    //////////////// M = rho * Cp * wg * N^T * N * |Jc|
    //////////////// Mij = rho * Cp * wg * Ni * Nj * |Jc| = M(i ,j)
//   cout << mat->getDensity() << " " << mat->getCapacity() << " = density, capacity" << endl;
    int i, j;
//     C.fillIdentity(0.);
    for (i=0; i<supportNodesSize; ++i) {
//         for (j=0; j<supportNodesSize; ++j) {
//             C.writeElement( mat->getDensity() * mat->getCapacity() * weight * shapeFun->getPhi(0,i)
//                             * shapeFun->getPhi(0,j) * std::abs(jacobian), i, j );	
// Lumped matrix:
//          C.addElement( mat->getDensity() * mat->getCapacity() * weight * shapeFun->getPhi(0,i)
//                             * shapeFun->getPhi(0,j) * std::abs(jacobian), i, i );	
// 	  cout << i << "," << j << " = "
// 	       << mat->getDensity() << "*" 
// 	       << mat->getCapacity() << "*" 
// 	       << weight  << "*" 
// 	       << shapeFun->getPhi(0,i) << "*" 
// 	       << shapeFun->getPhi(0,j)  << "*" 
// 	       << std::abs(jacobian)  << " = "
// 	       << C.readElement(i,j) << endl;
//         }
// Faster lumped matrix:
         C.writeElement( mat->getDensity() * mat->getCapacity(supportNodes[i]->getTemp()) * weight * shapeFun->getPhi(0,i)
                            * std::abs(jacobian), i, i );	
    }
//     HRZ lumped matrix [Hinton et al. (1976)] (if implemented, should be in Cell class)

}

void GaussPoint::computeHij( )
{
    int i, j, m, n;
    double avgTemp=0;
    int max_deriv_index = dim+1;
    H.fillIdentity(0.);
    for ( i=0; i<supportNodesSize; ++i ) {
      avgTemp += supportNodes[i]->getTemp() * shapeFun->getPhi(0,i);
    }
    // Hij = wg * grad(N_j) * kappa * grad(N_I) * |Jc|
    for ( i=0; i<supportNodesSize; ++i ) {
        for ( j=0; j<supportNodesSize; ++j ) {
// 	  if(supportNodes[i]->getThermalNumber() == 39){
// 	    cout << "KAPPA in " << i << "," << j << " = " ;
// 	    cout << 0.5*( mat->getKappa(supportNodes[i]->getTemp()) + mat->getKappa(supportNodes[j]->getTemp()) );
// 	    cout << " with nodes temperatures:  ";
// 	    cout << supportNodes[i]->getTemp() << " and ";
// 	    cout << supportNodes[j]->getTemp() << endl;
// 	  }
            for (  m=1; m<max_deriv_index; ++m ) {
//         for ( n=1; n<max_deriv_index; ++n ){
                //           K(2*i + m, 2*j + n) = Kij(m,n);
                H.addElement(weight * (  shapeFun->getPhi(m,i) 
// 					 * mat->getKappa(supportNodes[i]->getTemp()) 
// 					 + .5*mat->getKappa(supportNodes[j]->getTemp())/*.readElement(m,n)*/
					 * mat->getKappa(avgTemp) /*.readElement(m,n)*/
// 					 * 0.5*( mat->getKappa(supportNodes[i]->getTemp()) + mat->getKappa(supportNodes[j]->getTemp()) )/*.readElement(m,n)*/
                                         * shapeFun->getPhi(m,j) ) * std::abs(jacobian),
                             i,
                             j);
// 	  cout << i << "," << j << " = "
// 	       << mat->getDensity() << "*" 
// 	       << mat->getKappa() << "*" 
// 	       << weight  << "*" 
// 	       << shapeFun->getPhi(m,i) << "*" 
// 	       << shapeFun->getPhi(m,j)  << "*" 
// 	       << jacobian  << " = "
// 	       << H.readElement(i,j) << endl;
//         }
            }
//         H.writeElement( weight * mat->getKappa() * jacobian *
//                         (  shapeFun->getPhi(1,i) * shapeFun->getPhi(1,j) +
//                          + shapeFun->getPhi(2,i) * shapeFun->getPhi(2,j)
// 			),
//                                   i,
//                                   j);
        }
    }
//     cout << "GP (" << this->getX() << ", " << this->getY() << ")" << endl;
//     cout << "H = " << H << endl;
}

void GaussPoint::computeQext( LoadThermalBody* loadThermalBody_in )
{
}


void GaussPoint::assembleCij( lmx::Matrix<data_type> & globalCapacity )
{
    int i, j;
    for (i=0; i<supportNodesSize; ++i) {
        for (j=0; j<supportNodesSize; ++j) {
            globalCapacity.addElement( C.readElement(i, j),
                                       supportNodes[i]->getThermalNumber(),
                                       supportNodes[j]->getThermalNumber()
                                     );
        }
    }
}

void GaussPoint::assembleHij( lmx::Matrix<data_type> & globalConductivity )
{
    int i, j;
    for (i=0; i<supportNodesSize; ++i) {
        for (j=0; j<supportNodesSize; ++j) {
            globalConductivity.addElement( H.readElement(i, j),
                                           supportNodes[i]->getThermalNumber(),
                                           supportNodes[j]->getThermalNumber()
                                         );
        }
    }
}

void GaussPoint::assembleQext( lmx::Vector<data_type> & globalHeat)
{
    int i;
    for (i=0; i<supportNodesSize; ++i) {
        globalHeat.addElement( Qext.readElement(i),
                               supportNodes[i]->getThermalNumber()
                             );
    }
}


void GaussPoint::gnuplotOutStress( std::ofstream & gptension )
{
    gptension << X << " " << Y << " " << tension(0) << endl;
}


} //Namespace mknix
