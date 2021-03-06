/****************************************************************************
** $Id: rs_painterqt.cpp,v 1.18 2004/12/09 23:09:53 andrew Exp $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use 
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/


#include <stdio.h>
#include <qpaintdevicemetrics.h>

#include "rs_painterqt.h"

#include "rs_math.h"
#include "rs_application.h"
#include "rs_color.h"
//#include "rs_settings.h"

/**
 * Constructor.
 */
RS_PainterQt::RS_PainterQt(const QPaintDevice* pd)
        : QPainter(pd), RS_Painter() {}


/**
 * Destructor
 */
RS_PainterQt::~RS_PainterQt() {}

	
	
void RS_PainterQt::moveTo(int x, int y) {
	QPainter::moveTo(x, y);
}


void RS_PainterQt::lineTo(int x, int y) {
	QPainter::lineTo(x, y);
}

/**
 * Draws a grid point at (x1, y1).
 */
void RS_PainterQt::drawGridPoint(const RS_Vector& p) {
    QPainter::drawPoint(toScreenX(p.x), toScreenY(p.y));
}



/**
 * Draws a point at (x1, y1).
 */
void RS_PainterQt::drawPoint(const RS_Vector& p) {
    QPainter::drawLine(toScreenX(p.x-1), toScreenY(p.y),
                       toScreenX(p.x+1), toScreenY(p.y));
    QPainter::drawLine(toScreenX(p.x), toScreenY(p.y-1),
                       toScreenX(p.x), toScreenY(p.y+1));
}



/**
 * Draws a line from (x1, y1) to (x2, y2).
 */
void RS_PainterQt::drawLine(const RS_Vector& p1, const RS_Vector& p2) {
#ifdef __APPLE__
	int w2 = getPen().getScreenWidth()/2;
    QPainter::drawLine(toScreenX(p1.x-w2), toScreenY(p1.y-w2),
                       toScreenX(p2.x-w2), toScreenY(p2.y-w2));
#else
    QPainter::drawLine(toScreenX(p1.x), toScreenY(p1.y),
                       toScreenX(p2.x), toScreenY(p2.y));
#endif
}






/**
 * Draws a rectangle with corners p1, p2.
 */
/*void RS_PainterQt::drawRect(const RS_Vector& p1, const RS_Vector& p2) {
	/ *QPainter::drawRect(toScreenX(p1.x), toScreenY(p1.y),
					   abs(toScreenX(p2.x) - toScreenX(p1.x)), 
					   abs(toScreenY(p2.y) - toScreenY(p1.y)));* /
	QPainter::drawLine(toScreenX(p1.x), toScreenY(p1.y),
                       toScreenX(p2.x), toScreenY(p1.y));
    QPainter::drawLine(toScreenX(p2.x), toScreenY(p1.y),
                       toScreenX(p2.x), toScreenY(p2.y));
    QPainter::drawLine(toScreenX(p2.x), toScreenY(p2.y),
                       toScreenX(p1.x), toScreenY(p2.y));
    QPainter::drawLine(toScreenX(p1.x), toScreenY(p2.y),
                       toScreenX(p1.x), toScreenY(p1.y));
}*/


/**
 * Draws an arc which starts / ends exactly at the given coordinates.
 *
 * @param cx center in x
 * @param cy center in y
 * @param radius Radius
 * @param a1 Angle 1 in rad
 * @param a2 Angle 2 in rad
 * @param x1 startpoint x
 * @param y1 startpoint y
 * @param x2 endpoint x
 * @param y2 endpoint y
 * @param reversed true: clockwise, false: counterclockwise
 */
void RS_PainterQt::drawArc(const RS_Vector& cp, double radius,
                           double a1, double a2,
                           const RS_Vector& p1, const RS_Vector& p2,
                           bool reversed) {
    /*
    QPainter::drawArc(cx-radius, cy-radius,
                      2*radius, 2*radius,
                      a1*16, (a2-a1)*16);
    */

    if(radius<=0.5) {
        drawGridPoint(cp);
    } else {
        int   cix;            // Next point on circle
        int   ciy;            //
        double aStep;         // Angle Step (rad)
        double a;             // Current Angle (rad)
        double linStep;       // linear step (pixels)

        if (drawingMode==RS2::ModePreview) {
            linStep = 20.0;
        } else {
            linStep = 6.0;
        }

        if (fabs(linStep/radius)<=1.0) {
            aStep=asin(linStep/radius);
        } else {
            aStep=1.0;
        }

        if (aStep<0.05) {
            aStep = 0.05;
        }

        if(!reversed) {
            // Arc Counterclockwise:
            if(a1>a2-1.0e-10) {
                a2+=2*M_PI;
            }
            //moveTo(toScreenX(p1.x), toScreenY(p1.y));
            QPointArray pa;
            int i=0;
            pa.resize(i+1);
            pa.setPoint(i++, toScreenX(p1.x), toScreenY(p1.y));
            for(a=a1+aStep; a<=a2; a+=aStep) {
                cix = toScreenX(cp.x+cos(a)*radius);
                ciy = toScreenY(cp.y-sin(a)*radius);
                //lineTo(cix, ciy);
                pa.resize(i+1);
                pa.setPoint(i++, cix, ciy);
            }
            //lineTo(toScreenX(p2.x), toScreenY(p2.y));
            pa.resize(i+1);
            pa.setPoint(i++, toScreenX(p2.x), toScreenY(p2.y));
            drawPolyline(pa);
        } else {
            // Arc Clockwise:
            if(a1<a2+1.0e-10) {
                a2-=2*M_PI;
            }
            QPointArray pa;
            int i=0;
            pa.resize(i+1);
            pa.setPoint(i++, toScreenX(p1.x), toScreenY(p1.y));
            //moveTo(toScreenX(p1.x), toScreenY(p1.y));
            for(a=a1-aStep; a>=a2; a-=aStep) {
                cix = toScreenX(cp.x+cos(a)*radius);
                ciy = toScreenY(cp.y-sin(a)*radius);
                //lineTo(cix, ciy);
                pa.resize(i+1);
                pa.setPoint(i++, cix, ciy);
            }
            //lineTo(toScreenX(p2.x), toScreenY(p2.y));
            pa.resize(i+1);
            pa.setPoint(i++, toScreenX(p2.x), toScreenY(p2.y));
            drawPolyline(pa);
        }
    }
}



/**
 * Draws an arc.
 *
 * @param cx center in x
 * @param cy center in y
 * @param radius Radius
 * @param a1 Angle 1 in rad
 * @param a2 Angle 2 in rad
 * @param reversed true: clockwise, false: counterclockwise
 */
void RS_PainterQt::drawArc(const RS_Vector& cp, double radius,
                           double a1, double a2,
                           bool reversed) {
    /*
    QPainter::drawArc(cp.x-radius, cp.y-radius,
                      2*radius, 2*radius,
                      a1*16, (a2-a1)*16);
    */


    if(radius<=0.5) {
        drawGridPoint(cp);
    } else {
        QPointArray pa;
        createArc(pa, cp, radius, a1, a2, reversed);

        /*
              int   cix;            // Next point on circle
              int   ciy;            //
              double aStep;         // Angle Step (rad)
              double a;             // Current Angle (rad)

              if(2.0/radius<=1.0) {
                  aStep=asin(2.0/radius);
              } else {
                  aStep=1.0;
              }

              if (aStep<0.05) {
                  aStep = 0.05;
              }

              QPointArray pa;
              int i=0;
              pa.resize(i+1);
              pa.setPoint(i++, toScreenX(cp.x+cos(a1)*radius),
                          toScreenY(cp.y-sin(a1)*radius));
              //moveTo(toScreenX(cp.x+cos(a1)*radius),
              //       toScreenY(cp.y-sin(a1)*radius));
              if(!reversed) {
                  // Arc Counterclockwise:
                  if(a1>a2-1.0e-10) {
                      a2+=2*M_PI;
                  }
                  for(a=a1+aStep; a<=a2; a+=aStep) {
                      cix = toScreenX(cp.x+cos(a)*radius);
                      ciy = toScreenY(cp.y-sin(a)*radius);
                      //lineTo(cix, ciy);
                      pa.resize(i+1);
                      pa.setPoint(i++, cix, ciy);
                  }
              } else {
                  // Arc Clockwise:
                  if(a1<a2+1.0e-10) {
                      a2-=2*M_PI;
                  }
                  for(a=a1-aStep; a>=a2; a-=aStep) {
                      cix = toScreenX(cp.x+cos(a)*radius);
                      ciy = toScreenY(cp.y-sin(a)*radius);
                      //lineTo(cix, ciy);
                      pa.resize(i+1);
                      pa.setPoint(i++, cix, ciy);
                  }
              }
              //lineTo(toScreenX(cp.x+cos(a2)*radius),
              //       toScreenY(cp.y-sin(a2)*radius));
              pa.resize(i+1);
              pa.setPoint(i++,
                          toScreenX(cp.x+cos(a2)*radius),
                          toScreenY(cp.y-sin(a2)*radius));
        */
        drawPolyline(pa);
    }
}



/**
 * Draws a circle.
 * @param cx center in x
 * @param cy center in y
 * @param radius Radius
 */
void RS_PainterQt::drawCircle(const RS_Vector& cp, double radius) {

    if (drawingMode==RS2::ModeXOR && radius<500) {
        // This is _very_ slow for large arcs:
        QPainter::drawEllipse(toScreenX(cp.x-radius),
                              toScreenY(cp.y-radius),
                              RS_Math::round(2.0*radius),
                              RS_Math::round(2.0*radius));
    } else {
        drawArc(cp,
                radius,
                0.0, 2*M_PI,
                cp + RS_Vector(radius, 0.0),
                cp + RS_Vector(radius, 0.0),
                false);
    }
}



/**
 * Draws a rotated ellipse arc.
 */
void RS_PainterQt::drawEllipse(const RS_Vector& cp,
                               double radius1, double radius2,
                               double angle,
                               double a1, double a2,
                               bool reversed) {

    //if(radius1<=0.5 && radius2<=0.5) {
    //    drawGridPoint((int)cp.x, (int)cp.y);
    //} else {
    //int   cix;            // Next point on arc
    //int   ciy;            //
    double aStep;         // Angle Step (rad)
    double a;             // Current Angle (rad)

    // Angle step in rad
    /*
       if (radius1<radius2) {
           if (radius1<RS_TOLERANCE) {
               aStep=1.0;
           } else {
    		if (fabs(2.0/radius1)<=1.0) {
               	aStep=asin(2.0/radius1);
    		}
    		else {
    			aStep = 1.0;
    		}
           }
       } else {
           if (radius2<RS_TOLERANCE) {
               aStep=1.0;
           } else {
    		if (fabs(2.0/radius2)<=1.0) {
               	aStep=asin(2.0/radius2);
    		}
    		else {
    			aStep = 1.0;
    		}
           }
       }

       if(aStep<0.05) {
           aStep = 0.05;
       }
    */

    aStep=0.01;

    RS_Vector vp;
    RS_Vector vc(cp.x, cp.y);
    vp.set(cp.x+cos(a1)*radius1,
           cp.y-sin(a1)*radius2);
    vp.rotate(vc, -angle);
    moveTo(toScreenX(vp.x),
           toScreenY(vp.y));
    if(!reversed) {
        // Arc Counterclockwise:
        if(a1>a2-RS_TOLERANCE) {
            a2+=2*M_PI;
        }
        for(a=a1+aStep; a<=a2; a+=aStep) {
            vp.set(cp.x+cos(a)*radius1,
                   cp.y-sin(a)*radius2);
            vp.rotate(vc, -angle);
            lineTo(toScreenX(vp.x),
                   toScreenY(vp.y));
        }
    } else {
        // Arc Clockwise:
        if(a1<a2+RS_TOLERANCE) {
            a2-=2*M_PI;
        }
        for(a=a1-aStep; a>=a2; a-=aStep) {
            vp.set(cp.x+cos(a)*radius1,
                   cp.y-sin(a)*radius2);
            vp.rotate(vc, -angle);
            lineTo(toScreenX(vp.x),
                   toScreenY(vp.y));
        }
    }
    vp.set(cp.x+cos(a2)*radius1,
           cp.y-sin(a2)*radius2);
    vp.rotate(vc, -angle);
    lineTo(toScreenX(vp.x),
           toScreenY(vp.y));
    //}
}



/**
 * Draws image.
 */
void RS_PainterQt::drawImg(RS_Img& img, const RS_Vector& pos,
                           double angle, const RS_Vector& factor,
                           int sx, int sy, int sw, int sh) {
#if QT_VERSION>=0x030000
    save();

    QWMatrix wm;
    wm.translate(pos.x, pos.y);
    wm.scale(factor.x, factor.y);
    wm.rotate(RS_Math::rad2deg(-angle));
    setWorldMatrix(wm);
#else

    RS_DEBUG->print(RS_Debug::D_WARNING,
    	"RS_PainterQt::drawImg: rotated images not supported");
#endif


    if (fabs(angle)<1.0e-4) {
        drawImage(0+sx,-img.height()+sy, img, sx, sy, sw, sh);
    }
    else {
        drawImage(0,-img.height(), img);
    }

    restore();
}



void RS_PainterQt::drawTextH(int x1, int y1,
                             int x2, int y2,
                             const QString& text) {
    drawText(x1, y1, x2, y2,
             Qt::AlignRight|Qt::AlignVCenter,
             text);
}



void RS_PainterQt::drawTextV(int x1, int y1,
                             int x2, int y2,
                             const QString& text) {
#if QT_VERSION>=0x030000
    save();
    QWMatrix wm = worldMatrix();
    wm.rotate(-90.0);
    setWorldMatrix(wm);
    //rotate(-90.0);

    drawText(x1, y1, x2, y2,
             Qt::AlignRight|Qt::AlignVCenter,
             text);

    restore();
#else

    RS_DEBUG->print(RS_Debug::D_WARNING,
    	"RS_PainterQt::drawTextV: not supported");
#endif
}

void RS_PainterQt::fillRect(int x1, int y1, int w, int h,
                            const RS_Color& col) {
    QPainter::fillRect(x1, y1, w, h, col);
}


void RS_PainterQt::fillTriangle(const RS_Vector& p1,
                                const RS_Vector& p2,
                                const RS_Vector& p3) {

    QPointArray arr(3);
    arr.putPoints(0, 3,
                  toScreenX(p1.x),toScreenY(p1.y),
                  toScreenX(p2.x),toScreenY(p2.y),
                  toScreenX(p3.x),toScreenY(p3.y));
    setBrush(pen().color());
    drawPolygon(arr);
}


void RS_PainterQt::erase() {
    QPainter::eraseRect(0,0,getWidth(),getHeight());
}


int RS_PainterQt::getWidth() {
    QPaintDeviceMetrics m(device());
    return m.width();
}


int RS_PainterQt::getHeight() {
    QPaintDeviceMetrics m(device());
    return m.height();
}


void RS_PainterQt::setPreviewPen() {
    setPen(QColor(0,255,255));
}

RS_Pen RS_PainterQt::getPen() {
    return lpen;
    //RS_Pen p(pen().color(),
    //         RS2::qw(pen().width()),
    //         RS2::qw(pen().style()));
    //return QPainter::pen();
    //return p;
}

void RS_PainterQt::setPen(const RS_Pen& pen) {
    lpen = pen;
    if (drawingMode==RS2::ModeBW) {
        lpen.setColor(RS_Color(0,0,0));
    }
    QPen p(lpen.getColor(), RS_Math::round(lpen.getScreenWidth()),
           RS2::rsToQtLineType(lpen.getLineType()));
    p.setJoinStyle(Qt::RoundJoin);
    p.setCapStyle(Qt::RoundCap);
    QPainter::setPen(p);
}

void RS_PainterQt::setPen(const RS_Color& color) {
    if (drawingMode==RS2::ModeBW) {
        lpen.setColor(RS_Color(0,0,0));
        QPainter::setPen(RS_Color(0,0,0));
    } else {
        lpen.setColor(color);
        QPainter::setPen(color);
    }
}

void RS_PainterQt::setPen(int r, int g, int b) {
    if (drawingMode==RS2::ModeBW) {
        setPen(QColor(0, 0, 0));
    } else {
        setPen(QColor(r, g, b));
    }
}

void RS_PainterQt::disablePen() {
    lpen = RS_Pen(RS2::FlagInvalid);
    QPainter::setPen(Qt::NoPen);
}

void RS_PainterQt::setBrush(const RS_Color& color) {
    if (drawingMode==RS2::ModeBW) {
        QPainter::setBrush(QColor(0, 0, 0));
    } else {
        QPainter::setBrush(color);
    }
}


void RS_PainterQt::drawPolygon(const RS_PointArray& a) {
    QPainter::drawPolygon(a);
}

//void RS_PainterQt::setColor(const QColor& color) {
//	setPen(color);
//}

void RS_PainterQt::setXORMode() {
    setRasterOp(XorROP);
}

void RS_PainterQt::setNormalMode() {
    setRasterOp(CopyROP);
}

void RS_PainterQt::setClipRect(int x, int y, int w, int h) {
    QPainter::setClipRect(x, y, w, h);
    setClipping(true);
}

void RS_PainterQt::resetClipping() {
    setClipping(false);
}


