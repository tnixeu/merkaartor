
#include "MapView.h"
#include "MainWindow.h"
#include "Document.h"

#include <QPainter>
#include <QDockWidget>
#include <QMouseEvent>
#include <QShortcut>
#include <QDrag>

class ImageView;

class GeoImageDock : public MDockAncestor
{
    Q_OBJECT

public:
    GeoImageDock(MainWindow *aMain);
    ~GeoImageDock(void);

    void clear();
    void loadImage(QString fileName, Coord pos);
    void loadImages(QStringList fileNames);
    void setImage(Node *Pt);
    void setImage(int ImageId);

    static void addGeoDataToImage(Coord pos, const QString & file);
    static Coord getGeoDataFromImage(const QString & file);

private slots:
    void removeImages(void);
    void toClipboard(void);
    void selectNext(void);
    void selectPrevious(void);
    void centerMap(void);
    void saveImage(void);

private:

    MainWindow *Main;
    QAction *centerAction;
    QAction *remImagesAction;
    QAction *toClipboardAction;
    QAction *nextImageAction;
    QAction *previousImageAction;
    QAction *saveImageAction;


    QStringList Images;
    int curImage, lastImage;
    bool updateByMe;
    QPointer<TrackLayer> photoLayer;

    ImageView *Image;

    struct NodeData {
        NodeData(Node* mNode, const QString & mFilename, const QDateTime & mTimestamp, bool mInserted)
         : node(mNode), filename(mFilename), inserted(mInserted), timestamp(mTimestamp) { }
        bool operator<(const NodeData & other) const { return timestamp < other.timestamp; }
        Node* node;
        QString filename;
        bool inserted;
        QDateTime timestamp;
    };
    QList<NodeData> usedTrackPoints;

    void addUsedTrackpoint(NodeData data);
    bool getWalkingPapersDetails(const QUrl& reqUrl, double &lat, double &lon, bool& positionValid) const;
    bool askAndgetWalkingPapersDetails(double &lat, double &lon, bool& positionValid) const;

public:
    void changeEvent(QEvent*);
    void retranslateUi();
};

class ImageView : public QWidget
{
public:
    ImageView(QWidget *parent);
    ~ImageView();

    void setImage(QString filename, bool movable=true);
    void setMovable(bool movable=true);

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *e);

private:
    bool Movable;
    QImage image;
    QString name;
    QPoint mousePos;
    QRect rect;
    QRectF area;

    double zoomLevel; // zoom in percent

    void zoom(double levelStep); // zoom levelStep steps

};
