#ifndef IMAGECONTENTPROVIDER_P_H
#define IMAGECONTENTPROVIDER_P_H

#include "wallpaperimagecontentprovider.h"

class TrackerLiveQuery;
class GalleryModel;

class ImageContentProviderPrivate: public QObject
{
    Q_OBJECT

public:
    ImageContentProviderPrivate(GalleryModel& galleryModel);
    virtual ~ImageContentProviderPrivate();

public Q_SLOTS:
    void onInitialQueryFinished();

public:
    TrackerLiveQuery *mLiveQuery;

    bool mQueryRunning;
};

#endif

