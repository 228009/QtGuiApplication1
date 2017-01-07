/***************************************************************************
  qgsaction.h - QgsAction

 ---------------------
 begin                : 18.4.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSACTION_H
#define QGSACTION_H

#include "qgis_core.h"
#include <QSet>
#include <QString>
#include <QIcon>
#include <QAction>
#include <QUuid>

#include "qgsexpressioncontext.h"

class QgsExpressionContextScope;

/** \ingroup core
 * Utility class that encapsulates an action based on vector attributes.
 */
class CORE_EXPORT QgsAction
{
  public:
    enum ActionType
    {
      Generic,
      GenericPython,
      Mac,
      Windows,
      Unix,
      OpenUrl,
    };

    /**
     * Default constructor
     */
    QgsAction()
        : mType( Generic )
    {}

    /**
     * Create a new QgsAction
     *
     * @param type          The type of this action
     * @param description   A human readable description string
     * @param command       The action text. Its interpretation depends on the type
     * @param capture       If this is set to true, the output will be captured when an action is run
     */
    QgsAction( ActionType type, const QString& description, const QString& command, bool capture = false )
        : mType( type )
        , mDescription( description )
        , mCommand( command )
        , mCaptureOutput( capture )
        , mId( QUuid::createUuid() )
    {}

    /**
     * Create a new QgsAction
     *
     * @param type                 The type of this action
     * @param description          A human readable description string
     * @param action               The action text. Its interpretation depends on the type
     * @param icon                 Path to an icon for this action
     * @param capture              If this is set to true, the output will be captured when an action is run
     * @param shortTitle           A short string used to label user interface elements like buttons
     * @param actionScopes         A set of scopes in which this action will be available
     */
    QgsAction( ActionType type, const QString& description, const QString& action, const QString& icon, bool capture, const QString& shortTitle = QString(), const QSet<QString>& actionScopes = QSet<QString>() )
        : mType( type )
        , mDescription( description )
        , mShortTitle( shortTitle )
        , mIcon( icon )
        , mCommand( action )
        , mCaptureOutput( capture )
        , mActionScopes( actionScopes )
        , mId( QUuid::createUuid() )
    {}

    //! The name of the action. This may be a longer description.
    QString name() const { return mDescription; }

    //! The short title is used to label user interface elements like buttons
    QString shortTitle() const { return mShortTitle; }

    /**
     * Returns a unique id for this action.
     *
     * @note Added in QGIS 3.0
     */
    QUuid id() const { return mId; }

    /**
     * Returns true if this action was a default constructed one.
     *
     * @note Added in QGIS 3.0
     */
    bool isValid() const { return !mId.isNull(); }

    //! The path to the icon
    QString iconPath() const { return mIcon; }

    //! The icon
    QIcon icon() const { return QIcon( mIcon ); }

    /**
     * Returns the command that is executed by this action.
     * How the content is interpreted depends on the type() and
     * the actionScope().
     *
     * @note Added in QGIS 3.0
     */
    QString command() const { return mCommand; }

    //! The action type
    ActionType type() const { return mType; }

    //! Whether to capture output for display when this action is run
    bool capture() const { return mCaptureOutput; }

    //! Checks if the action is runable on the current platform
    bool runable() const;

    /**
     * Run this action.
     *
     * @note Added in QGIS 3.0
     */
    void run( QgsVectorLayer* layer, const QgsFeature& feature, const QgsExpressionContext& expressionContext ) const;

    /**
     * Run this action.
     *
     * @note Added in QGIS 3.0
     */
    void run( const QgsExpressionContext& expressionContext ) const;

    /**
     * The action scopes define where an action will be available.
     * Action scopes may offer additional variables like the clicked
     * coordinate.
     *
     * @see QgsActionScope
     * @note Added in QGIS 3.0
     */
    QSet<QString> actionScopes() const;

    /**
     * The action scopes define where an action will be available.
     * Action scopes may offer additional variables like the clicked
     * coordinate.
     *
     * @note Added in QGIS 3.0
     */
    void setActionScopes( const QSet<QString>& actionScopes );

    /**
     * Reads an XML definition from actionNode
     * into this object.
     *
     * @note Added in QGIS 3.0
     */
    void readXml( const QDomNode& actionNode );

    /**
     * Appends an XML definition for this action as a new
     * child node to actionsNode.
     *
     * @note Added in QGIS 3.0
     */
    void writeXml( QDomNode& actionsNode ) const;

  private:
    ActionType mType;
    QString mDescription;
    QString mShortTitle;
    QString mIcon;
    QString mCommand;
    bool mCaptureOutput;
    QSet<QString> mActionScopes;
    mutable QSharedPointer<QAction> mAction;
    QUuid mId;
};

Q_DECLARE_METATYPE( QgsAction )

#endif // QGSACTION_H
