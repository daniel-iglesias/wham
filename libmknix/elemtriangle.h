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

#ifndef MKNIXELEMTRIANGLE_H
#define MKNIXELEMTRIANGLE_H

#include <celltriang.h>

namespace mknix {

/**
	@author AUTHORS <MAILS>
*/
class ElemTriangle : public CellTriang
{
public:
    ElemTriangle();

//    ElemTriangle( Material&, double, int,
//                  double, double,
//                  double, double,
//                  double, double );

    ElemTriangle( Material&, double, int,
                  Node*,
                  Node*,
                  Node*
                );

    ~ElemTriangle();

    void initialize( std::vector<Node*> & );

    void computeShapeFunctions(  );

protected:
    void createGaussPoints_MC( );

};

}

#endif
