/***************************************************************************
      qgswmsprovider.h  -  QGIS Data provider for
                           OGC Web Map Service layers
                             -------------------
    begin                : 17 Mar, 2005
    copyright            : (C) 2005 by Brendan Morley
    email                : morb at ozemail dot com dot au
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef QGSWMSPROVIDER_H
#define QGSWMSPROVIDER_H

#include "qgsrasterdataprovider.h"
#include "qgscoordinatereferencesystem.h"
#include "qgsnetworkreplyparser.h"
#include "qgswmscapabilities.h"

#include <QString>
#include <QStringList>
#include <QDomElement>
#include <QHash>
#include <QMap>
#include <QVector>
#include <QUrl>

class QgsCoordinateTransform;
class QgsNetworkAccessManager;
class QgsWmsCapabilities;

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

/**
 * \class Handles asynchronous download of WMS legend
 *
 * \todo turn into a generic async image downloader ?
 *
 */
class QgsWmsLegendDownloadHandler : public QgsImageFetcher
{
    Q_OBJECT
  public:

    QgsWmsLegendDownloadHandler( QgsNetworkAccessManager& networkAccessManager, const QgsWmsSettings& settings, const QUrl& url );
    ~QgsWmsLegendDownloadHandler();

    // Make sure to connect to "finish" before starting
    void start() override;

  private:

    // Make sure to connect to "finish" before starting
    void startUrl( const QUrl& url );

    // Delete reply (later), emit error and finish with empty image
    void sendError( const QString& msg );
    // Delete reply (later), emit finish
    void sendSuccess( const QImage& img );

    QgsNetworkAccessManager& mNetworkAccessManager;
    const QgsWmsSettings& mSettings;
    QNetworkReply* mReply;
    QSet<QUrl> mVisitedUrls;
    QUrl mInitialUrl;

  private slots:

    void errored( QNetworkReply::NetworkError code );
    void finished();
    void progressed( qint64, qint64 );
};

class QgsCachedImageFetcher: public QgsImageFetcher
{
    Q_OBJECT
  public:
    explicit QgsCachedImageFetcher( const QImage& img );
    virtual ~QgsCachedImageFetcher();
    virtual void start() override;
  private:
    const QImage _img; // copy is intentional
  private slots:
    void send()
    {
      QgsDebugMsg( QString( "XXX Sending %1x%2 image" ).arg( _img.width() ).arg( _img.height() ) );
      emit finish( _img );
    }
};


/**

  \brief Data provider for OGC WMS layers.

  This provider implements the
  interface defined in the QgsDataProvider class to provide access to spatial
  data residing in a OGC Web Map Service.

*/
class QgsWmsProvider : public QgsRasterDataProvider
{
    Q_OBJECT

  public:

    /**
     * Constructor for the provider.
     *
     * \param   uri   HTTP URL of the Web Server.  If needed a proxy will be used
     *                otherwise we contact the host directly.
     * \param   capabilities   Optionally existing parsed capabilities for the given URI
     *
     */
    QgsWmsProvider( const QString& uri = QString(), const QgsWmsCapabilities* capabilities = 0 );


    virtual ~QgsWmsProvider();

    QgsWmsProvider * clone() const override;

    virtual QgsCoordinateReferenceSystem crs() const override;

    /**
     * Reorder the list of WMS layer names to be rendered by this server
     * (in order from bottom to top)
     * \note   layers must have been previously added.
     */
    virtual void setLayerOrder( QStringList const & layers ) override;

    virtual void setSubLayerVisibility( const QString &name, bool vis ) override;

    /**
     * Set the name of the connection for use in authentication where required
     */
    void setConnectionName( QString const & connName );

    QImage *draw( QgsRectangle const &  viewExtent, int pixelWidth, int pixelHeight ) override;

    void readBlock( int bandNo, QgsRectangle  const & viewExtent, int width, int height, void *data, QgsRasterBlockFeedback* feedback = nullptr ) override;
    //void readBlock( int bandNo, QgsRectangle  const & viewExtent, int width, int height, QgsCoordinateReferenceSystem theSrcCRS, QgsCoordinateReferenceSystem theDestCRS, void *data );

    virtual QgsRectangle extent() const override;

    bool isValid() const override;

#if 0

    /** Returns true if layer has tile set profiles
     */
    virtual bool hasTiles() const;
#endif

    virtual QString getMapUrl() const;
    virtual QString getFeatureInfoUrl() const;
    virtual QString getTileUrl() const;
    virtual QString getLegendGraphicUrl() const;

    //! get WMS version string
    QString wmsVersion();

    /**
     * Sub-layers handled by this provider, in order from bottom to top
     *
     * Sub-layers are used to abstract the way the WMS server can combine
     * layers in some way at the server, before it serves them to this
     * WMS client.
     */
    QStringList subLayers() const override;

    /**
     * Sub-layer styles for each sub-layer handled by this provider,
     * in order from bottom to top
     *
     * Sub-layer styles are used to abstract the way the WMS server can symbolise
     * layers in some way at the server, before it serves them to this
     * WMS client.
     */
    QStringList subLayerStyles() const override;

    bool supportsLegendGraphic() const override { return true; }

    QImage getLegendGraphic( double scale = 0.0, bool forceRefresh = false, const QgsRectangle * visibleExtent = nullptr ) override;
    virtual QgsImageFetcher* getLegendGraphicFetcher( const QgsMapSettings* mapSettings ) override;

    // TODO: Get the WMS connection

    // TODO: Get the table name associated with this provider instance

    int capabilities() const override;
    Qgis::DataType dataType( int bandNo ) const override;
    Qgis::DataType sourceDataType( int bandNo ) const override;
    int bandCount() const override;
    QString metadata() override;
    QgsRasterIdentifyResult identify( const QgsPoint & thePoint, QgsRaster::IdentifyFormat theFormat, const QgsRectangle &theExtent = QgsRectangle(), int theWidth = 0, int theHeight = 0, int theDpi = 96 ) override;
    QString lastErrorTitle() override;
    QString lastError() override;
    QString lastErrorFormat() override;
    QString name() const override;
    QString description() const override;
    virtual void reloadData() override;

    static QVector<QgsWmsSupportedFormat> supportedFormats();

    static void showMessageBox( const QString& title, const QString& text );

    /**
     * \brief parse the full WMS ServiceExceptionReport XML document
     *
     * \note errorTitle and errorText are updated to suit the results of this function. Format of error is plain text.
     */
    static bool parseServiceExceptionReportDom( QByteArray const &xml, QString& errorTitle, QString& errorText );

    /**
     * \brief Prepare the URI so that we can later simply append param=value
     * \param uri uri to prepare
     * \retval prepared uri
     */
    static QString prepareUri( QString uri );

    //! Helper struct for tile requests
    struct TileRequest
    {
      TileRequest( const QUrl& u, const QRectF& r, int i )
          : url( u )
          , rect( r )
          , index( i )
      {}
      QUrl url;
      QRectF rect;
      int index;
    };
    typedef QList<TileRequest> TileRequests;

    //! Tile identifier within a tile source
    typedef struct TilePosition
    {
      TilePosition( int r, int c ): row( r ), col( c ) {}
      bool operator==( TilePosition other ) const { return row == other.row && col == other.col; }
      int row;
      int col;
    } TilePosition;
    typedef QList<TilePosition> TilePositions;

  signals:

    //! \brief emit a signal to notify of a progress event
    void progressChanged( int theProgress, int theTotalSteps );

    void dataChanged();

  private slots:
    void identifyReplyFinished();
    void getLegendGraphicReplyFinished( const QImage& );
    void getLegendGraphicReplyErrored( const QString& message );
    void getLegendGraphicReplyProgress( qint64, qint64 );

  private:

    //! In case of XYZ tile layer, setup capabilities from its URI
    void setupXyzCapabilities( const QString& uri );

    QImage *draw( QgsRectangle const &  viewExtent, int pixelWidth, int pixelHeight, QgsRasterBlockFeedback* feedback );

    /**
     * Try to get best extent for the layer in given CRS. Returns true on success, false otherwise (layer not found, invalid CRS, transform failed)
     */
    bool extentForNonTiledLayer( const QString& layerName, const QString& crs, QgsRectangle& extent ) const;

    // case insensitive attribute value lookup
    static QString nodeAttribute( const QDomElement &e, const QString& name, const QString& defValue = QString::null );

    /**
     * Add the list of WMS layer names to be rendered by this server
     */
    bool addLayers();

    /**
     * Set the image projection (in WMS CRS format) used in the transfer from the WMS server
     *
     * \note an empty crs value will result in the previous CRS being retained.
     */
    bool setImageCrs( QString const & crs );

    /**
     * \brief Retrieve and parse the (cached) Capabilities document from the server
     *
     * \param forceRefresh  if true, ignores any previous response cached in memory
     *                      and always contact the server for a new copy.
     * \retval false if the capabilities document could not be retrieved or parsed -
     *         see lastError() for more info
     *
     * When this returns, "layers" will make sense.
     *
     * TODO: Make network-timeout tolerant
     */
    bool retrieveServerCapabilities( bool forceRefresh = false );

    //! parse the WMS ServiceException XML element
    static void parseServiceException( QDomElement const &e, QString& errorTitle, QString& errorText );

    void parseOperationMetadata( QDomElement const &e );


    /**
     * \brief Calculates the combined extent of the layers selected by layersDrawn
     *
     * \retval false if the capabilities document could not be retrieved or parsed -
     *         see lastError() for more info
     */
    bool calculateExtent() const;

    /* \brief Bounding box in WMS format
     *
     * \note it does not perform any escape
     */
    QString toParamValue( const QgsRectangle& rect, bool changeXY );

    /* \brief add SRS or CRS parameter */
    void setSRSQueryItem( QUrl& url );

  private:

    QUrl createRequestUrlWMS( const QgsRectangle& viewExtent, int pixelWidth, int pixelHeight );
    void createTileRequestsWMSC( const QgsWmtsTileMatrix* tm, const QgsWmsProvider::TilePositions& tiles, QgsWmsProvider::TileRequests& requests );
    void createTileRequestsWMTS( const QgsWmtsTileMatrix* tm, const QgsWmsProvider::TilePositions& tiles, QgsWmsProvider::TileRequests& requests );
    void createTileRequestsXYZ( const QgsWmtsTileMatrix* tm, const QgsWmsProvider::TilePositions& tiles, QgsWmsProvider::TileRequests& requests );

    //! Helper structure to store a cached tile image with its rectangle
    typedef struct TileImage
    {
      TileImage( const QRectF& r, const QImage& i ): rect( r ), img( i ) {}
      QRectF rect; //!< Destination rectangle for a tile (in screen coordinates)
      QImage img;  //!< Cached tile to be drawn
    } TileImage;
    //! Get tiles from a different resolution to cover the missing areas
    void fetchOtherResTiles( QgsTileMode tileMode, const QgsRectangle& viewExtent, int imageWidth, QList<QRectF>& missing, double tres, int resOffset, QList<TileImage> &otherResTiles );

    /** Return the full url to request legend graphic
     * The visibleExtent isi only used if provider supports contextual
     * legends according to the QgsWmsSettings
     * @added in 2.8
     */
    QUrl getLegendGraphicFullURL( double scale, const QgsRectangle& visibleExtent );

    //QStringList identifyAs( const QgsPoint &point, QString format );

    QString layerMetadata( QgsWmsLayerProperty &layer );

    //! remove query item and replace it with a new value
    void setQueryItem( QUrl &url, const QString& key, const QString& value );

    //! add image FORMAT parameter to url
    void setFormatQueryItem( QUrl &url );

    //! Name of the stored connection
    QString mConnectionName;

    /**
     * Flag indicating if the layer data source is a valid WMS layer
     */
    bool mValid;

    /**
     * Spatial reference id of the layer
     */
    QString mSrid;

    /**
     * Rectangle that contains the extent (bounding box) of the layer
     */
    mutable QgsRectangle mLayerExtent;

    /**
     * GetLegendGraphic of the WMS (raw)
     */
    QByteArray mHttpGetLegendGraphicResponse;

    /**
     * GetLegendGraphic WMS Pixmap result
     */
    QImage mGetLegendGraphicImage;

    /**
     * GetLegendGraphic scale for the WMS Pixmap result
     */
    double mGetLegendGraphicScale;

    QgsRectangle mGetLegendGraphicExtent;

    QScopedPointer<QgsImageFetcher> mLegendGraphicFetcher;

    /**
     * Visibility status of the given active sublayer
     */
    QMap<QString, bool> mActiveSubLayerVisibility;

    /**
     * WMS CRS type of the image CRS used from the WMS server
     */
    QString mImageCrs;

    /**
     * The reply to the capabilities request
     */
    QNetworkReply *mIdentifyReply;

    /**
     * The result of the identify reply
     */
    //QString mIdentifyResult;
    QList< QgsNetworkReplyParser::RawHeaderMap > mIdentifyResultHeaders;
    QList<QByteArray> mIdentifyResultBodies;

    // TODO: better
    QString mIdentifyResultXsd;

    /**
     * The error caption associated with the last WMS error.
     */
    QString mErrorCaption;

    /**
     * The error message associated with the last WMS error.
     */
    QString mError;


    /** The mime type of the message
     */
    QString mErrorFormat;

    //! See if calculateExtents() needs to be called before extent() returns useful data
    mutable bool mExtentDirty;

    QString mServiceMetadataURL;

    //! tile request number, cache hits and misses
    int mTileReqNo;

    //! chosen tile layer
    QgsWmtsTileLayer        *mTileLayer;
    //! chosen matrix set
    QgsWmtsTileMatrixSet    *mTileMatrixSet;

    //! supported formats for GetFeatureInfo in order of preference
    QStringList mSupportedGetFeatureFormats;

    QgsCoordinateReferenceSystem mCrs;

    //! Parsed response of server's capabilities - initially (or on error) may be invalid
    QgsWmsCapabilities mCaps;

    //! User's settings (URI, authorization, layer, style, ...)
    QgsWmsSettings mSettings;
};


//! Handler for downloading of non-tiled WMS requests, the data are written to the given image
class QgsWmsImageDownloadHandler : public QObject
{
    Q_OBJECT
  public:
    QgsWmsImageDownloadHandler( const QString& providerUri, const QUrl& url, const QgsWmsAuthorization& auth, QImage* image, QgsRasterBlockFeedback* feedback );
    ~QgsWmsImageDownloadHandler();

    void downloadBlocking();

  protected slots:
    void cacheReplyFinished();
    void cacheReplyProgress( qint64 bytesReceived, qint64 bytesTotal );
    void cancelled();

  protected:
    void finish() { QMetaObject::invokeMethod( mEventLoop, "quit", Qt::QueuedConnection ); }

    QString mProviderUri;

    QNetworkReply* mCacheReply;
    QImage* mCachedImage;

    QEventLoop* mEventLoop;

    QgsRasterBlockFeedback* mFeedback;
};


//! Handler for tiled WMS-C/WMTS requests, the data are written to the given image
class QgsWmsTiledImageDownloadHandler : public QObject
{
    Q_OBJECT
  public:

    QgsWmsTiledImageDownloadHandler( const QString& providerUri, const QgsWmsAuthorization& auth, int reqNo, const QgsWmsProvider::TileRequests& requests, QImage* image, const QgsRectangle& viewExtent, bool smoothPixmapTransform, QgsRasterBlockFeedback* feedback );
    ~QgsWmsTiledImageDownloadHandler();

    void downloadBlocking();

  protected slots:
    void tileReplyFinished();
    void cancelled();

  protected:

    /**
     * \brief Relaunch tile request cloning previous request parameters and managing max repeat
     *
     * \param oldRequest request to clone to generate new tile request
     *
     * request is not launched if max retry is reached. Message is logged.
     */
    void repeatTileRequest( QNetworkRequest const &oldRequest );

    void finish() { QMetaObject::invokeMethod( mEventLoop, "quit", Qt::QueuedConnection ); }

    QString mProviderUri;

    QgsWmsAuthorization mAuth;

    QImage* mImage;
    QgsRectangle mViewExtent;

    QEventLoop* mEventLoop;

    int mTileReqNo;
    bool mSmoothPixmapTransform;

    //! Running tile requests
    QList<QNetworkReply*> mReplies;

    QgsRasterBlockFeedback* mFeedback;
};


//! Class keeping simple statistics for WMS provider - per unique URI
class QgsWmsStatistics
{
  public:
    struct Stat
    {
      Stat()
          : errors( 0 )
          , cacheHits( 0 )
          , cacheMisses( 0 )
      {}
      int errors;
      int cacheHits;
      int cacheMisses;
    };

    //! get reference to layer's statistics - insert to map if does not exist yet
    static Stat& statForUri( const QString& uri ) { return sData[uri]; }

  protected:
    static QMap<QString, Stat> sData;
};

Q_DECLARE_TYPEINFO( QgsWmsProvider::TilePosition, Q_PRIMITIVE_TYPE );

#endif

// ENDS