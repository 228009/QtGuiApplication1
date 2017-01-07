/***************************************************************************
                             qgspainteffectregistry.h
                             ------------------------
    begin                : January 2015
    copyright            : (C) 2015 Nyall Dawson
    email                : nyall dot dawson at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSPAINTEFFECTREGISTRY_H
#define QGSPAINTEFFECTREGISTRY_H

#include "qgis_core.h"
#include "qgis.h"
#include <QDomElement>
#include <QDomDocument>

class QgsPaintEffect;
class QgsPaintEffectWidget;

/** \ingroup core
 * \class QgsPaintEffectAbstractMetadata
 * \brief Stores metadata about a paint effect class.
 *
 * \note It's necessary to implement the createPaintEffect() function.
 *  In C++ you can use the QgsPaintEffectMetadata convenience class to
 * simplify creation of the metadata.
 *
 * \note Added in version 2.9
 */
class CORE_EXPORT QgsPaintEffectAbstractMetadata
{
  public:

    /** Construct a new QgsPaintEffectAbstractMetadata
     * @param name unique string representing paint effect class
     * @param visibleName user visible name representing paint effect class
     */
    QgsPaintEffectAbstractMetadata( const QString& name, const QString& visibleName );

    virtual ~QgsPaintEffectAbstractMetadata() = default;

    /** Returns the unique string representing the paint effect class
     * @returns unique string
     * @see visibleName
     */
    QString name() const { return mName; }

    /** Returns the user visible string representing the paint effect class
     * @returns friendly user visible string
     * @see name
     */
    QString visibleName() const { return mVisibleName; }

    /** Create a paint effect of this class given an encoded map of properties.
     * @param map properties string map
     * @returns new paint effect
     */
    virtual QgsPaintEffect* createPaintEffect( const QgsStringMap& map ) = 0;

    /** Create configuration widget for paint effect of this class. Can return nullptr
     * if there's no GUI for the paint effect class.
     * @returns configuration widget
     */
    virtual QgsPaintEffectWidget* createWidget() { return nullptr; }

  protected:
    QString mName;
    QString mVisibleName;

};

typedef QgsPaintEffect*( *QgsPaintEffectCreateFunc )( const QgsStringMap& );
typedef QgsPaintEffectWidget*( *QgsPaintEffectWidgetFunc )();

/** \ingroup core
 * \class QgsPaintEffectMetadata
 * \brief Convenience metadata class that uses static functions to create an effect and its widget.
 *
 * \note Added in version 2.9
 */
class CORE_EXPORT QgsPaintEffectMetadata : public QgsPaintEffectAbstractMetadata
{

  public:

    /** Create effect metadata from static functions
     * @param name unique string representing paint effect class
     * @param visibleName user visible name representing paint effect class
     * @param pfCreate paint effect creation function
     * @param pfWidget widget creation function
     * @note not available in python bindings
     */
    QgsPaintEffectMetadata( const QString& name, const QString& visibleName,
                            QgsPaintEffectCreateFunc pfCreate,
                            QgsPaintEffectWidgetFunc pfWidget = nullptr )
        : QgsPaintEffectAbstractMetadata( name, visibleName )
        , mCreateFunc( pfCreate )
        , mWidgetFunc( pfWidget )
    {}

    /** Returns the paint effect creation function for the paint effect class
     * @returns creation function
     * @note not available in python bindings
     */
    QgsPaintEffectCreateFunc createFunction() const { return mCreateFunc; }

    /** Returns the paint effect properties widget creation function for the paint effect class
     * @returns widget creation function
     * @note not available in python bindings
     * @see setWidgetFunction
     */
    QgsPaintEffectWidgetFunc widgetFunction() const { return mWidgetFunc; }

    /** Sets the paint effect properties widget creation function for the paint effect class
     * @param f widget creation function
     * @note not available in python bindings
     * @see widgetFunction
     */
    void setWidgetFunction( QgsPaintEffectWidgetFunc f ) { mWidgetFunc = f; }

    /** Creates a new paint effect of the metadata's effect class
     * @param map string map of effect properties
     * @returns new paint effect
     * @note not available in python bindings
     * @see createWidget
     */
    virtual QgsPaintEffect* createPaintEffect( const QgsStringMap& map ) override { return mCreateFunc ? mCreateFunc( map ) : nullptr; }

    /** Creates a new paint effect properties widget for the metadata's effect class
     * @returns effect properties widget
     * @note not available in python bindings
     * @see createWidget
     */
    virtual QgsPaintEffectWidget* createWidget() override { return mWidgetFunc ? mWidgetFunc() : nullptr; }

  protected:
    QgsPaintEffectCreateFunc mCreateFunc;
    QgsPaintEffectWidgetFunc mWidgetFunc;
};


/** \ingroup core
 * \class QgsPaintEffectRegistry
 * \brief Registry of available paint effects.
 *
 * QgsPaintEffectRegistry is not usually directly created, but rather accessed through
 * QgsApplication::paintEffectRegistry().
 *
 * \note Added in version 2.9
 */
class CORE_EXPORT QgsPaintEffectRegistry
{
  public:

    QgsPaintEffectRegistry();
    ~QgsPaintEffectRegistry();

    //! QgsPaintEffectRegistry cannot be copied.
    QgsPaintEffectRegistry( const QgsPaintEffectRegistry& rh ) = delete;
    //! QgsPaintEffectRegistry cannot be copied.
    QgsPaintEffectRegistry& operator=( const QgsPaintEffectRegistry& rh ) = delete;

    /** Returns the metadata for a specific effect.
     * @param name unique string name for paint effect class
     * @returns paint effect metadata if found, otherwise nullptr
     */
    QgsPaintEffectAbstractMetadata* effectMetadata( const QString& name ) const;

    /** Registers a new effect type.
     * @param metadata effect metadata. Ownership is transferred to the registry.
     * @returns true if add was successful.
     */
    bool addEffectType( QgsPaintEffectAbstractMetadata* metadata );

    /** Creates a new paint effect given the effect name and properties map.
     * @param name unique name representing paint effect class
     * @param properties encoded string map of effect properties
     * @returns new paint effect of specified class, or nullptr if matching
     * paint effect could not be created
     */
    QgsPaintEffect* createEffect( const QString& name, const QgsStringMap& properties = QgsStringMap() ) const;

    /** Creates a new paint effect given a DOM element storing paint effect
     * properties.
     * @param element encoded DOM element of effect properties
     * @returns new paint effect, or nullptr if matching
     * paint effect could not be created
     */
    QgsPaintEffect* createEffect( const QDomElement& element ) const;

    /** Returns a list of known paint effects.
     * @returns list of paint effect names
     */
    QStringList effects() const;

    /** Returns a new effect stack consisting of a sensible selection of default
     * effects. All effects except the standard draw source effect are disabled,
     * but are included so that they can be easily drawn just by enabling the effect.
     * @returns default effects stack
     * @see isDefaultStack()
     */
    static QgsPaintEffect* defaultStack();

    /** Tests whether a paint effect matches the default effects stack.
     * @param effect paint effect to test
     * @returns true if effect is default stack
     * @note added in QGIS 2.12
     * @see defaultStack()
     */
    static bool isDefaultStack( QgsPaintEffect* effect );

  private:

    QMap<QString, QgsPaintEffectAbstractMetadata*> mMetadata;
};

#endif //QGSPAINTEFFECTREGISTRY_H
