/***************************************************************************
                         qgsnewmemorylayerdialog.h
                             -------------------
    begin                : September 2014
    copyright            : (C) 2014 by Nyall Dawson, Marco Hugentobler
    email                : nyall dot dawson at gmail dot com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSNEWMEMORYLAYERDIALOG_H
#define QGSNEWMEMORYLAYERDIALOG_H

#include "ui_qgsnewmemorylayerdialogbase.h"
#include "qgisgui.h"
#include "qgis.h"
#include "qgis_gui.h"

class QgsVectorLayer;

/** \ingroup gui
 * \class QgsNewMemoryLayerDialog
 */
class GUI_EXPORT QgsNewMemoryLayerDialog: public QDialog, private Ui::QgsNewMemoryLayerDialogBase
{
    Q_OBJECT

  public:

    /** Runs the dialoag and creates a new memory layer
     * @param parent parent widget
     * @returns new memory layer
     */
    static QgsVectorLayer* runAndCreateLayer( QWidget* parent = nullptr );

    QgsNewMemoryLayerDialog( QWidget *parent = nullptr, Qt::WindowFlags fl = QgisGui::ModalDialogFlags );
    ~QgsNewMemoryLayerDialog();

    //! Returns the selected geometry type
    QgsWkbTypes::Type selectedType() const;

    //! Returns the selected crs
    QgsCoordinateReferenceSystem crs() const;

    //! Returns the layer name
    QString layerName() const;

  private:

    QString mCrsId;
};

#endif //QGSNEWMEMORYLAYERDIALOG_H
