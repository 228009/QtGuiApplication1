#include "QtGuiApplication1.h"
#include "QtCore/QString"
#include "QtWidgets/QGraphicsView"
#include"QgsVectorLayer.h"
#include"qgsmapcanvas.h"
//#include"QgsMapLayerRegistry.h"
#include"QgsProviderRegistry.h"
#include"QDir"
#include"qstring.h"
#include<string>
using namespace std;
#pragma comment(lib,"../include/qgis_gui.lib")
#pragma comment(lib,"../include/qgis_core.lib")
QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QString plgDir = QFileInfo(QCoreApplication::applicationFilePath()).path() ;
	string a = plgDir.toStdString();
	QgsProviderRegistry::instance(plgDir+"\\plugins");
	QgsVectorLayer *vlayer = new QgsVectorLayer(plgDir+"\\map.shp", "layer_name_you_like", "ogr");
	bool b = vlayer->isValid();
	canvas = new QgsMapCanvas();
	canvas->setCanvasColor(Qt::white);

	canvas->setExtent(vlayer->extent());
	lst.append(vlayer);
	canvas->setLayers(lst);
	//QgsMapLayerRegistry::instance()->addMapLayer(vlayer);


	setCentralWidget(canvas);

	actionZoomIn = new QAction(QString("Zoom in"), this);
	actionZoomOut = new QAction(QString("Zoom out"), this);
	actionPan = new QAction(QString("Pan"), this);

	actionZoomIn->setCheckable(true);
	actionZoomOut->setCheckable(true);
	actionPan->setCheckable(true);
	connect(actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
	connect(actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
	connect(actionPan, SIGNAL(triggered()), this, SLOT(pan()));

	toolbar = addToolBar("Canvas actions");
	toolbar->addAction(actionZoomIn);
	toolbar->addAction(actionZoomOut);
	toolbar->addAction(actionPan);

	toolPan = new QgsMapToolPan(canvas);
	toolPan->setAction(actionPan);
	toolZoomIn = new QgsMapToolZoom(canvas, false);
	toolZoomIn->setAction(actionZoomIn);
	toolZoomOut = new QgsMapToolZoom(canvas, true);
	toolZoomOut->setAction(actionZoomOut);

	pan();
}
void QtGuiApplication1::zoomIn()
{
	canvas->setMapTool(toolZoomIn);
}
void QtGuiApplication1::zoomOut()
{
	canvas->setMapTool(toolZoomOut);
}
void QtGuiApplication1::pan()
{
	canvas->setMapTool(toolPan);
}
