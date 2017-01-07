/***************************************************************************
    qgspluginlayer.h
    ---------------------
    begin                : January 2010
    copyright            : (C) 2010 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSPLUGINLAYER_H
#define QGSPLUGINLAYER_H

#include "qgis_core.h"
#include "qgsmaplayer.h"

typedef QList< QPair<QString, QPixmap> > QgsLegendSymbologyList;

/** \ingroup core
  Base class for plugin layers. These can be implemented by plugins
  and registered in QgsPluginLayerRegistry.

  In order to be readable from project files, they should set these attributes in layer DOM node:
   "type" = "plugin"
   "name" = "your_layer_type"
 */
class CORE_EXPORT QgsPluginLayer : public QgsMapLayer
{
    Q_OBJECT

  public:
    QgsPluginLayer( const QString& layerType, const QString& layerName = QString() );

    //! Return plugin layer type (the same as used in QgsPluginLayerRegistry)
    QString pluginLayerType();

    //! Set extent of the layer
    void setExtent( const QgsRectangle &extent ) override;

    /** Set source string. This is used for example in layer tree to show tooltip.
     * @note added in 2.16
     */
    void setSource( const QString& source );

    //! return a list of symbology items for the legend
    //! (defult implementation returns nothing)
    //! @note Added in v2.1
    virtual QgsLegendSymbologyList legendSymbologyItems( QSize iconSize );

  protected:
    QString mPluginLayerType;
};

#endif // QGSPLUGINLAYER_H
