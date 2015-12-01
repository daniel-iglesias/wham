//-- Licencia --
#include "shapefunction.h"
#include "node.h"
#include "point.h"
#include "simulation.h"

namespace mknix {

ShapeFunction::ShapeFunction()
    : dim(Simulation::getDim())
{
}

ShapeFunction::ShapeFunction( const ShapeFunction* sf_in )
    : dim(sf_in->dim)
    , phi(sf_in->phi)
    , gp(sf_in->gp)
{
}


ShapeFunction::ShapeFunction( Point* gp_in )
    : dim( Simulation::getDim() )
    , gp( gp_in )
{
}


ShapeFunction::~ShapeFunction()
{
}

void ShapeFunction::outputValues()
{
    // output values:
    double tempx = (*gp->supportNodes.begin() )->getx();
    int counter = 0;


    cout << endl << gp->X << " " << gp->Y << endl;

    for ( std::vector<Node*>::iterator it = gp->supportNodes.begin();
            it != gp->supportNodes.end();
            ++it)
    {
        if(tempx != (*it)->getx() ) {
            tempx = (*it)->getx();
        }
        cout  << (*it)->getx() << " "
              << (*it)->gety() << " "
              << phi(0,counter) << " "
              << phi(1,counter) << " "
              << phi(2,counter) << endl;

        counter++;
    }
}


void ShapeFunction::gnuplotOut()
{   // for some reason, can't use a std::vector...
    std::ofstream data("shapefunction.dat");
    std::ofstream data_x("shapefunction_x.dat");
    std::ofstream data_y("shapefunction_y.dat");
    double tempx = (*gp->supportNodes.begin() )->getx();
    int counter = 0;

    for ( std::vector<Node*>::iterator it = gp->supportNodes.begin();
            it != gp->supportNodes.end();
            ++it)
    {
        if(tempx != (*it)->getx() ) {
            data << endl;
            data_x << endl;
            data_y << endl;
            tempx = (*it)->getx();
        }

        data  << (*it)->getx() << " "
              << (*it)->gety() << " "
              << phi(0,counter) << endl;
        data_x  << (*it)->getx() << " "
                << (*it)->gety() << " "
                << phi(1,counter) << endl;
        data_y  << (*it)->getx() << " "
                << (*it)->gety() << " "
                << phi(2,counter) << endl;

        counter++;
    }
}

} //Namespace mknix
