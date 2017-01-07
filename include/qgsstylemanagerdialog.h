/***************************************************************************
    qgsstylemanagerdialog.h
    ---------------------
    begin                : November 2009
    copyright            : (C) 2009 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSSTYLEV2MANAGERDIALOG_H
#define QGSSTYLEV2MANAGERDIALOG_H

#include <QDialog>
#include <QStandardItem>
#include <QAction>
#include <QMenu>

#include "ui_qgsstylemanagerdialogbase.h"
#include "qgscontexthelp.h"
#include "qgis_gui.h"

class QgsStyle;

/** \ingroup gui
 * \class QgsStyleManagerDialog
 */
class GUI_EXPORT QgsStyleManagerDialog : public QDialog, private Ui::QgsStyleManagerDialogBase
{
    Q_OBJECT

  public:
    QgsStyleManagerDialog( QgsStyle* style, QWidget* parent = nullptr );

    //! open add color ramp dialog, return color ramp's name if the ramp has been added
    static QString addColorRampStatic( QWidget* parent, QgsStyle* style,
                                       QString RampType = QString() );

  public slots:
    void addItem();
    void editItem();
    void removeItem();
    void exportItemsSVG();
    void exportItemsPNG();
    void exportSelectedItemsImages( const QString& dir, const QString& format, QSize size );
    void exportItems();
    void importItems();

    void on_tabItemType_currentChanged( int );
    //! adds symbols of some type to list
    void populateList();

    //! called when the dialog is going to be closed
    void onFinished();

    void on_buttonBox_helpRequested() { QgsContextHelp::run( metaObject()->className() ); }

    void itemChanged( QStandardItem* item );

    void groupChanged( const QModelIndex& );
    void groupRenamed( QStandardItem * );
    //! add a tag
    int addTag();
    //! add a smartgroup
    int addSmartgroup();
    //! remove a tag or smartgroup
    void removeGroup();

    //! carry out symbol tagging using check boxes
    void tagSymbolsAction();

    //! edit the selected smart group
    void editSmartgroupAction();

    //! symbol changed from one group
    void regrouped( QStandardItem* );

    //! filter the symbols based on input search term
    void filterSymbols( const QString& );

    //! Perform symbol specific tasks when selected
    void symbolSelected( const QModelIndex& );

    //! Perform tasks when the selected symbols change
    void selectedSymbolsChanged( const QItemSelection& selected, const QItemSelection& deselected );

    //! Context menu for the groupTree
    void grouptreeContextMenu( QPoint );

    //! Context menu for the listItems ( symbols list )
    void listitemsContextMenu( QPoint );

  protected slots:
    bool addColorRamp( QAction* action );
    //! Add selected symbols to favorites
    void addFavoriteSelectedSymbols();
    //! Remove selected symbols from favorites
    void removeFavoriteSelectedSymbols();
    //! Tag selected symbols using menu item selection
    void tagSelectedSymbols( bool newTag = false );
    //! Remove all tags from selected symbols
    void detagSelectedSymbols();

  protected:

    //! populate combo box with known style items (symbols, color ramps)
    void populateTypes();

    //! populate the groups
    void populateGroups();
    //! to set symbols checked when in editing mode
    void setSymbolsChecked( const QStringList& );

    //! populate list view with symbols of the current type with the given names
    void populateSymbols( const QStringList& symbolNames, bool checkable = false );

    //! populate list view with color ramps
    void populateColorRamps( const QStringList& colorRamps, bool checkable = false );

    int currentItemType();
    QString currentItemName();

    //! add a new symbol to style
    bool addSymbol();
    //! add a new color ramp to style
    bool addColorRamp();

    bool editSymbol();
    bool editColorRamp();

    bool removeSymbol();
    bool removeColorRamp();

    //! Enables or disbables the symbol specific inputs
    void enableSymbolInputs( bool );
    //! Enables or disables the groupTree specific inputs
    void enableGroupInputs( bool );
    //! Enables or diables the groupTree items for grouping mode
    void enableItemsForGroupingMode( bool );

    //! sets the text of the item with bold font
    void setBold( QStandardItem* );

    QgsStyle* mStyle;

    QString mStyleFilename;

    bool mModified;

    //! Mode to display the symbol list
    bool mGrouppingMode;

    //! space to store symbol tags
    QStringList mTagList;

    //! Context menu for the symbols/colorramps
    QMenu *mGroupMenu;

    //! Sub-menu of @c mGroupMenu, dynamically filled to show one entry for every group
    QMenu *mGroupListMenu;

    //! Context menu for the group tree
    QMenu* mGroupTreeContextMenu;

    //! Menu for the "Add item" toolbutton when in colorramp mode
    QMenu* mMenuBtnAddItemColorRamp;
};

#endif
