/*****************************************************************************
**  $Id: vec2web.h,v 1.8 2003/02/13 20:54:33 xiru Exp $
**
**  This is part of the vec2web tool
**  Copyright (C) 2000 Andrew Mustun, Causeway Technologies
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License (version 2) as
**  published by the Free Software Foundation.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
******************************************************************************/

#ifndef _VEC2WEB_H
#define _VEC2WEB_H

#include "rs_graphic.h"
#include "rs_vector.h"
#include <stdio.h>


/**
 * Class for converting between formats or showing a file in a window.
 */
class Vec2Web {
public:
    Vec2Web();

    void setInputFile(const char* fn) {
        strncpy(inputFile, fn, FILENAME_MAX);
    }
    void setOutputFile(const char* fn) {
        strncpy(outputFile, fn, FILENAME_MAX);
    }
    void setMaxSize(int x, int y) {
        maxSize.set(x,y);
    }
    void setScaleUp(bool on) {
        scaleUp=on;
    }

    void convert();
    bool output(const char* format);
    bool outputQt(const char* format);
    bool outputMing(int compressLevel=9);
    bool outputDXML();

    double transformX(double x);
    double transformY(double y, bool swap=false);
    double transformD(double d);

private:
    RS_Graphic graphic;

    char inputFile[FILENAME_MAX+1];
    char outputFile[FILENAME_MAX+1];

    //! Max size for the output bitmap (pixel)
    RS_Vector maxSize;
    //! Exact size for the output bitmap (pixel)
    RS_Vector size;
    //! Scale graphic up if too small?
    bool scaleUp;
    //! Scale Factor for the graphic
    double factor;
    //! Offset of the graphics zero point in pixel
    RS_Vector offset;
};

#endif
