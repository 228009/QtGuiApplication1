/***************************************************************************
                         qgsmultibandcolorrenderer.h
                         ---------------------------
    begin                : December 2011
    copyright            : (C) 2011 by Marco Hugentobler
    email                : marco at sourcepole dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSMULTIBANDCOLORRENDERER_H
#define QGSMULTIBANDCOLORRENDERER_H

#include "qgis_core.h"
#include "qgsrasterrenderer.h"

class QgsContrastEnhancement;
class QDomElement;

/** \ingroup core
  * Renderer for multiband images with the color components
*/
class CORE_EXPORT QgsMultiBandColorRenderer: public QgsRasterRenderer
{
  public:
    QgsMultiBandColorRenderer( QgsRasterInterface* input, int redBand, int greenBand, int blueBand,
                               QgsContrastEnhancement* redEnhancement = nullptr, QgsContrastEnhancement* greenEnhancement = nullptr,
                               QgsContrastEnhancement* blueEnhancement = nullptr );
    ~QgsMultiBandColorRenderer();

    //! QgsMultiBandColorRenderer cannot be copied. Use clone() instead.
    QgsMultiBandColorRenderer( const QgsMultiBandColorRenderer& ) = delete;
    //! QgsMultiBandColorRenderer cannot be copied. Use clone() instead.
    const QgsMultiBandColorRenderer& operator=( const QgsMultiBandColorRenderer& ) = delete;

    QgsMultiBandColorRenderer * clone() const override;

    static QgsRasterRenderer* create( const QDomElement& elem, QgsRasterInterface* input );

    QgsRasterBlock* block( int bandNo, const QgsRectangle & extent, int width, int height, QgsRasterBlockFeedback* feedback = nullptr ) override;

    int redBand() const { return mRedBand; }
    void setRedBand( int band ) { mRedBand = band; }
    int greenBand() const { return mGreenBand; }
    void setGreenBand( int band ) { mGreenBand = band; }
    int blueBand() const { return mBlueBand; }
    void setBlueBand( int band ) { mBlueBand = band; }

    const QgsContrastEnhancement* redContrastEnhancement() const { return mRedContrastEnhancement; }
    //! Takes ownership
    void setRedContrastEnhancement( QgsContrastEnhancement* ce );

    const QgsContrastEnhancement* greenContrastEnhancement() const { return mGreenContrastEnhancement; }
    //! Takes ownership
    void setGreenContrastEnhancement( QgsContrastEnhancement* ce );

    const QgsContrastEnhancement* blueContrastEnhancement() const { return mBlueContrastEnhancement; }
    //! Takes ownership
    void setBlueContrastEnhancement( QgsContrastEnhancement* ce );

    void writeXml( QDomDocument& doc, QDomElement& parentElem ) const override;

    QList<int> usesBands() const override;

  private:
    int mRedBand;
    int mGreenBand;
    int mBlueBand;

    QgsContrastEnhancement* mRedContrastEnhancement;
    QgsContrastEnhancement* mGreenContrastEnhancement;
    QgsContrastEnhancement* mBlueContrastEnhancement;

};

#endif // QGSMULTIBANDCOLORRENDERER_H
