/***************************************************************************
                         qgsattributedialog.h  -  description
                             -------------------
    begin                : October 2004
    copyright            : (C) 2004 by Marco Hugentobler
    email                : marco.hugentobler@autoform.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSATTRIBUTEDIALOG_H
#define QGSATTRIBUTEDIALOG_H

#include "qgsattributeeditorcontext.h"
#include "qgsattributeform.h"
#include "qgstrackedvectorlayertools.h"

#include <QDialog>
#include <QMenuBar>
#include <QGridLayout>
#include "qgis_gui.h"

class QgsDistanceArea;
class QgsHighlight;

/** \ingroup gui
 * \class QgsAttributeDialog
 */
class GUI_EXPORT QgsAttributeDialog : public QDialog
{
    Q_OBJECT

  public:

    /**
     * Create an attribute dialog for a given layer and feature
     *
     * @param vl                The layer for which the dialog will be generated
     * @param thepFeature       A feature for which the dialog will be generated
     * @param featureOwner      Set to true, if the dialog should take ownership of the feature
     * @param parent            A parent widget for the dialog
     * @param showDialogButtons True: Show the dialog buttons accept/cancel
     * @param context           The context in which this dialog is created
     *
     */
    QgsAttributeDialog( QgsVectorLayer *vl, QgsFeature *thepFeature, bool featureOwner, QWidget* parent = nullptr, bool showDialogButtons = true, const QgsAttributeEditorContext& context = QgsAttributeEditorContext() );

    ~QgsAttributeDialog();

    /** Saves the size and position for the next time
     *  this dialog box will be used.
     */
    void saveGeometry();

    /** Restores the size and position from the last time
     *  this dialog box was used.
     */
    void restoreGeometry();

    /**
     * @brief setHighlight
     * @param h The highlight. Ownership is taken.
     */
    void setHighlight( QgsHighlight *h );

    QgsAttributeForm *attributeForm() { return mAttributeForm; }

    const QgsFeature *feature() { return &mAttributeForm->feature(); }

    /**
     * Is this dialog editable?
     *
     * @return returns true, if this dialog was created in an editable manner.
     */
    bool editable() { return mAttributeForm->editable(); }

    /**
     * Toggles the form mode.
     * @param mode form mode. For example, if set to QgsAttributeForm::AddFeatureMode, the dialog will be editable even with an invalid feature and
     * will add a new feature when the form is accepted.
     */
    void setMode( QgsAttributeForm::Mode mode ) { mAttributeForm->setMode( mode ); }

    /**
     * Sets the edit command message (Undo) that will be used when the dialog is accepted
     *
     * @param message The message
     */
    void setEditCommandMessage( const QString& message ) { mAttributeForm->setEditCommandMessage( message ); }

    /**
     * Intercept window activate/deactive events to show/hide the highlighted feature.
     *
     * @param e The event
     *
     * @return The same as the parent QDialog
     */
    virtual bool event( QEvent *e ) override;

  public slots:
    void accept() override;
    void reject() override;

    //! Show the dialog non-blocking. Reparents this dialog to be a child of the dialog form
    void show();

  private:
    void init( QgsVectorLayer* layer, QgsFeature* feature, const QgsAttributeEditorContext& context, bool showDialogButtons );

    QString mSettingsPath;
    // Used to sync multiple widgets for the same field
    QgsHighlight *mHighlight;
    int mFormNr;
    bool mShowDialogButtons;
    QString mReturnvarname;
    QgsAttributeForm* mAttributeForm;
    QgsFeature *mOwnedFeature;

    QgsTrackedVectorLayerTools mTrackedVectorLayerTools;

    // true if this dialog is editable
    bool mEditable;

    static int sFormCounter;
    static QString sSettingsPath;

};

#endif
