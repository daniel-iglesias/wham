//-- Licencia --
#ifndef CELL_H
#define CELL_H

#include <vector>
#include <string>
#include "common.h"

//////////////////////////////////////////// Doxygen file documentation entry:
/*!
  \file cell.h

  \brief Background cells for integration.

  \author Daniel Iglesias

 */
//////////////////////////////////////////// Doxygen file documentation (end)

namespace lmx {
template <typename T> class Vector;
template <typename T> class Matrix;
}

namespace mknix {

class LoadThermalBody;
class Material;
class GaussPoint;
class Node;

/**
@author Daniel Iglesias
*/
class Cell {

public:
    Cell();

    Cell( Material&, std::string, double, int );

    virtual ~Cell();

    virtual void initialize( std::vector<Node*> & );

    virtual void computeShapeFunctions(  );

    void computeCapacityGaussPoints(  );

    void assembleCapacityGaussPoints( lmx::Matrix<data_type> & );

    void computeConductivityGaussPoints(  );

    void assembleConductivityGaussPoints( lmx::Matrix<data_type> & );

    void computeQextGaussPoints( LoadThermalBody* );

    void assembleQextGaussPoints( lmx::Vector<data_type> & );

    void computeMGaussPoints(  );

    void assembleMGaussPoints( lmx::Matrix<data_type> & );

    void computeFintGaussPoints(  );

    void computeNLFintGaussPoints(  );

    void assembleFintGaussPoints( lmx::Vector<data_type> & );

    void computeFextGaussPoints(  );

    void assembleFextGaussPoints( lmx::Vector<data_type> & );

    void computeKGaussPoints(  );

    void computeNLKGaussPoints(  );

    void assembleKGaussPoints( lmx::Matrix<data_type> & );

    void assembleRGaussPoints( lmx::Vector<data_type> &, int );

    void assembleNLRGaussPoints( lmx::Vector<data_type> &, int );

    double calcPotentialEGaussPoints( const lmx::Vector<data_type> & );

    double calcKineticEGaussPoints( const lmx::Vector<data_type> & );

    double calcElasticEGaussPoints(  );

    virtual void gnuplotOut( std::ofstream&, std::ofstream& ) = 0;

    void gnuplotOutStress( std::ofstream& );

protected:
    Material* mat;
    std::string formulation;
    double alpha;
    int nGPoints; /**< number of Gauss Points */
    std::vector<GaussPoint*> gPoints;
    std::vector<GaussPoint*> gPoints_MC;
    double jacobian;
    std::vector< Node* > nodes;
    double dc;

};

} //Namespace mknix

#endif
