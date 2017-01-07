/***************************************************************************
                    qgspluginlayerregistry.cpp - class for
                    registering plugin layer creators
                             -------------------
    begin                : Mon Nov 30 2009
    copyright            : (C) 2009 by Mathias Walker, Sourcepole
    email                : mwa at sourcepole.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSPLUGINLAYERREGSITRY_H
#define QGSPLUGINLAYERREGSITRY_H

#include <QMap>
#include <QDomNode>

#include "qgis_core.h"

class QgsPluginLayer;

/** \ingroup core
    class for creating plugin specific layers
*/
class CORE_EXPORT QgsPluginLayerType
{
  public:

    QgsPluginLayerType( const QString& name );
    virtual ~QgsPluginLayerType() = default;

    QString name();

    //! Return new layer of this type. Return NULL on error
    virtual QgsPluginLayer* createLayer();

    /** Return new layer of this type, using layer URI (specific to this plugin layer type). Return NULL on error.
     * @note added in 2.10
     */
    virtual QgsPluginLayer* createLayer( const QString& uri );

    //! Show plugin layer properties dialog. Return false if the dialog cannot be shown.
    virtual bool showLayerProperties( QgsPluginLayer* layer );

  protected:
    QString mName;
};

//=============================================================================

/** \ingroup core
 * A registry of plugin layers types.
 *
 * QgsPluginLayerRegistry is not usually directly created, but rather accessed through
 * QgsApplication::pluginLayerRegistry().
*/
class CORE_EXPORT QgsPluginLayerRegistry
{
  public:

    QgsPluginLayerRegistry();
    ~QgsPluginLayerRegistry();

    //! QgsPluginLayerRegistry cannot be copied.
    QgsPluginLayerRegistry( const QgsPluginLayerRegistry& rh ) = delete;
    //! QgsPluginLayerRegistry cannot be copied.
    QgsPluginLayerRegistry& operator=( const QgsPluginLayerRegistry& rh ) = delete;

    /** List all known layer types
     *  \note added in v2.1 */
    QStringList pluginLayerTypes();

    //! Add plugin layer type (take ownership) and return true on success
    bool addPluginLayerType( QgsPluginLayerType* pluginLayerType );

    //! Remove plugin layer type and return true on success
    bool removePluginLayerType( const QString& typeName );

    //! Return plugin layer type metadata or NULL if doesn't exist
    QgsPluginLayerType* pluginLayerType( const QString& typeName );

    /** Return new layer if corresponding plugin has been found, else return NULL.
     * @note optional param uri added in 2.10
     */
    QgsPluginLayer* createLayer( const QString& typeName, const QString& uri = QString() );

  private:

    typedef QMap<QString, QgsPluginLayerType*> PluginLayerTypes;

    PluginLayerTypes mPluginLayerTypes;
};

#endif // QGSPLUGINLAYERREGSITRY_H