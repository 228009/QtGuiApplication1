/***************************************************************************
    qgsmaptooladdcircularstring.h  -  map tool for adding circular strings
    ---------------------
    begin                : December 2014
    copyright            : (C) 2014 by Marco Hugentobler
    email                : marco dot hugentobler at sourcepole dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSMAPTOOLADDCIRCULARSTRING_H
#define QGSMAPTOOLADDCIRCULARSTRING_H

#include "qgsmaptoolcapture.h"

class QgsGeometryRubberBand;

class QgsMapToolAddCircularString: public QgsMapToolCapture
{
    Q_OBJECT
  public:
    QgsMapToolAddCircularString( QgsMapToolCapture* parentTool, QgsMapCanvas* canvas, CaptureMode mode = CaptureLine );
    ~QgsMapToolAddCircularString();

    void keyPressEvent( QKeyEvent* e ) override;
    void keyReleaseEvent( QKeyEvent* e ) override;

    void deactivate() override;

    void activate() override;

  private slots:
    void setParentTool( QgsMapTool* newTool, QgsMapTool* oldTool );

  protected:
    explicit QgsMapToolAddCircularString( QgsMapCanvas* canvas ); //forbidden

    /** The parent map tool, e.g. the add feature tool.
     *  Completed circular strings will be added to this tool by calling its addCurve() method.
     * */
    QgsMapToolCapture* mParentTool;
    //! Circular string points (in map coordinates)
    QgsPointSequence mPoints;
    //! The rubberband to show the already completed circular strings
    QgsGeometryRubberBand* mRubberBand;
    //! The rubberband to show the circular string currently working on
    QgsGeometryRubberBand* mTempRubberBand;

    //center point rubber band
    bool mShowCenterPointRubberBand;
    QgsGeometryRubberBand* mCenterPointRubberBand;

    void createCenterPointRubberBand();
    void updateCenterPointRubberBand( const QgsPointV2& pt );
    void removeCenterPointRubberBand();
};

#endif // QGSMAPTOOLADDCIRCULARSTRING_H
