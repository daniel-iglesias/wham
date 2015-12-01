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

#ifndef MKNIXNODE_H
#define MKNIXNODE_H

#include "point.h"
#include "LMX/lmx.h"

namespace mknix {

/**
  @author AUTHORS <MAILS>
*/
class Node : public Point {
public:
    Node();

    Node( const Node& );

    Node( const Node* );

    Node(int i_in, double x_in, double y_in, double z_in);

    ~Node();

    void addWeight( double w_in ) {
        this->weight += w_in;
    }

    inline const double& getWeight() {
        return this->weight;
    }

    inline const int& getNumber() const {
        return this->num;
    }

    inline const int& getThermalNumber() const {
        return this->thermalNum;
    }

    inline const void setNumber( int num_in ) {
        num = num_in;
    }

    inline const void setThermalNumber( int num_in ) {
        thermalNum = num_in;
    }

    inline const double& getx() const {
        return this->x;
    }

    inline const double& gety() const {
        return this->y;
    }

    inline const double& getz() const {
        return this->z;
    }

    inline const double& getqt() const {
        return this->qt;
    }

    inline double getx(int i)
    {
        if(i==0) return x;
        if(i==1) return y;
        if(i==2) return z;
        else return 1E10;
    }

    inline double getUx() {
        return x-X;
    }

    inline double getUy() {
        return y-Y;
    }

    inline double getUz() {
        return z-Z;
    }

    inline double getU(int i)
    {
        if(i==0) return x-X;
        if(i==1) return y-Y;
        if(i==2) return z-Z;
        else return 1E10;
    }

    double getConf(int dof);

    double getTemp();

    size_t getSupportSize( int deriv );

    int getSupportNodeNumber( int deriv, int s_node );

    double getShapeFunValue( int deriv, int s_node );

//     void setShapeFunValue( double the_value, int deriv, int s_node );

    inline void setUx(double ux_in) {
        x = X + ux_in;
    }

    inline void setUy(double uy_in) {
        y = Y + uy_in;
    }

    inline void setUz(double uz_in) {
        z = Z + uz_in;
    }

    void setx( const lmx::Vector<data_type>& globalConf );

    void setqt( const lmx::Vector<data_type>& globalConf );

    inline void setx(double x_in) {
        x = x_in;
    }

    inline void sety(double y_in) {
        y = y_in;
    }

    inline void setz(double z_in) {
        z = z_in;
    }

    inline void setX(double X_in) {
        X = X_in;
        x = X_in;
    }

    inline void setY(double Y_in) {
        Y = Y_in;
        y = Y_in;
    }

    inline void setZ(double Z_in) {
        Z = Z_in;
        z = Z_in;
    }

private:
    int num, thermalNum;
    double x, y, z, qt;
//    double X, Y, Z;
    double weight;

////// Members possibly needed for additional
//     std::string shapeFunTypeThermal;
//     std::vector<Node*> supportNodesThermal;
//     ShapeFunction* shapeFunThermal;
//     int supportNodesSizeThermal;
};

}

#endif
