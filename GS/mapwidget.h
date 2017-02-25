#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QList>
#include <QWebEngineView>
#include <QUrl>
#include <QWebChannel>
#include <QWebSocketServer>
#include <QWebChannelAbstractTransport>
#include "actionpacket.h"
#include "websocketclientwrapper.h"
#include "websockettransport.h"
#include "flightpath.h"

/**
 * @brief The MapWidget class
 * @author Jordan Dickson
 */
class MapWidget : public QWebEngineView {
Q_OBJECT
private:
    bool loading = false;
    QList<QList<Protocol::Waypoint> >* lists;
    QWebChannel channel;
    QWebSocketServer server;
    WebSocketClientWrapper clientWrapper;

    const QUrl MapURL = QUrl("qrc:/res/html/mapsPlanning.html");


public:
    MapWidget(QWidget *parent = 0);
    ~MapWidget();

    /**
     * @brief ready
     * @return True if the MapWidget input or output Waypoints
     */
    bool ready();

    /**
     * @brief createNewPath
     * @return A new path ID for adding waypoints to and retrieving the path.
     */
    int createNewPath();

    /**
     * @brief appendGPSCoordToPath
     * @param lat
     * @param lng
     * @param pathID
     */
    void appendGPSCoordToPath(double lat, double lng, int pathID);

    /**
     * @brief getPath
     * @param pathID
     * @return An ordered list of all the waypoints in a certain path
     */
    QList<Protocol::Waypoint>* getPath(int pathID);

    /**
     * @brief MapURL
     */
    //const QUrl MapURL = QUrl("qrc:/res/html/mapsPlanningGoogle.html");

private slots:
    /**
     * @brief loadStartedSlot tied to the loadStarted signal
     */
    void loadStartedSlot();

    /**
     * @brief loadFinishedSlot tied to the loadFinished signal
     */
    void loadFinishedSlot();

public slots:
    /**
     * @brief addPointToTable
     * @param lat
     * @param lng
     */
    void addPointToTable(double lat, double lng);

    /**
     * @brief clearMap
     */
    void clearMap();

    /**
     * @brief disconnectWebSocket must be called before opening another MapWidget
     * Later we'll work a solution that allows for multiple MapWidgets to be "open"
     * at the same time.
     */
    void disconnectWebSocket();

    /**
     * @brief addFlightpath
     * @param fp
     * @param index
     */
    void addFlightPath(FlightPath* fp, int index = 0);

    /**
     * @brief clearTable
    */
    void clearTable();

signals:
    /**
     * @brief sendPointToMap
     * @param lat
     * @param lng
     * @param pathID
     */
    void sendPointToMap(double lat, double lng, int pathID);

    /**
     * @brief pointAddedToMap
     * @param lat
     * @param lng
     */
    void pointAddedToMap(double lat, double lng, int pathID);

    /**
     * @brief clearFlightPath
     * @param pathID
     */
    void clearFlightPath(int pathID);

    /**
     * @brief tableCleared
     */
    void tableCleared();

    /**
     * @brief flightPathSent
     */
    void flightPathSent();
};

#endif // MAPWIDGET_H
