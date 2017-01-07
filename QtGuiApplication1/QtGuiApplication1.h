#pragma once

#include "qmainwindow.h"
#include "ui_QtGuiApplication1.h"
#include"QAction"
#include"QToolBar"
#include"QList"
#include "QgsMapCanvas.h"
#include "QgsMapToolPan.h"
#include "QgsMapToolZoom.h"

class QtGuiApplication1 : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication1(QWidget *parent = nullptr);

private:
	Ui::QtGuiApplication1Class ui;
	QgsMapCanvas *canvas;
	QAction *actionZoomIn, *actionZoomOut, *actionPan;
	QToolBar *toolbar;
	QgsMapToolPan *toolPan;
	QgsMapToolZoom *toolZoomIn, *toolZoomOut;
	QList<QgsMapLayer*> lst;
private slots:
	void zoomIn();
	void zoomOut();
	void pan();

};
