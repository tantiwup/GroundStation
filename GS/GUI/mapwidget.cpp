#include "mapwidget.h"

MapWidget::MapWidget(QWidget *parent) : QQuickWidget(parent) {
    setResizeMode(QQuickWidget::SizeRootObjectToView);
    setSource(QUrl("qrc:/res/map.qml"));
    map = this->rootObject()->findChild<QObject*>("map");
}

void MapWidget::drawPoint(QVector2D point, QColor color) {
    QMetaObject::invokeMethod(map, "drawPoint",
            Q_ARG(QVariant, point),
            Q_ARG(QVariant, color));
}

void MapWidget::drawPolyline(QVariantList points, QColor color){
    QMetaObject::invokeMethod(map, "drawPolyline",
            Q_ARG(QVariant, QVariant::fromValue(points)),
            Q_ARG(QVariant, color));
}

void MapWidget::drawPolygon(QVariantList points, QColor color) {
    QMetaObject::invokeMethod(map, "drawPolygon",
           Q_ARG(QVariant, QVariant::fromValue(points)),
           Q_ARG(QVariant, color));
}

// Delete
//void MapWidget::updateUAVvPosition(mavlink_gps_raw_int_t gps) {
//    qDebug() << "mapWidget updateUAVvPosition";
//    if (gps.lat != currentUAVlat && gps.lon != currentUAVlon) {
//        currentUAVlat = gps.lat; currentUAVlon = gps.lon;
//        QString coords = QGeoCoordinate((float)gps.lat/10000000, (float)gps.lon/10000000).toString(QGeoCoordinate::DegreesWithHemisphere);

//        this->rootObject()->setProperty("coords", QVariant(coords));
//        QMetaObject::invokeMethod(this->rootObject()->findChild<QObject*>("uavPosition"), "updateUAVPosition",
//               Q_ARG(QVariant, coords));
//    }
//}

void MapWidget::clearMap() {
    QMetaObject::invokeMethod(map, "clearMap");
}

void MapWidget::drawMission(Mission * mission) {
    clearMap();
    drawPoint(mission->home_pos, QColor(255, 0, 0));
    drawPoint(mission->air_drop_pos, QColor(0, 230, 230));
    drawPoint(mission->off_axis_odlc_pos, QColor(10,10, 200));
    drawPoint(mission->emergent_last_known_pos, QColor(0,0,0));
    drawPolyline(toQVariantList(mission->mission_waypoints.waypoints), QColor("blue"));
    drawPolygon(toQVariantList(mission->search_grid_points), QColor("yellow"));
    for (int i = 0; i < mission->fly_zones->size(); i++)
        drawPolygon(toQVariantList(mission->fly_zones->at(i).boundary_points), QColor(0, 255, 0, 50));

}

void MapWidget::drawUAV(double lat, double lon, double heading) {
    QMetaObject::invokeMethod(map, "drawUAV",
            Q_ARG(QVariant, lat),
            Q_ARG(QVariant, lon),
            Q_ARG(QVariant, heading));
}

void MapWidget::updateCenter(double lat, double lon) {
    QMetaObject::invokeMethod(map, "updateCenter",
            Q_ARG(QVariant, lat),
            Q_ARG(QVariant, lon));
}

void MapWidget::updateUAV() {
    if (blinkUAV) {
        QTimer timer;
        QEventLoop loop;

        QMetaObject::invokeMethod(map, "removeUAV");

        drawUAV(uav_latitude, uav_longitude, uav_heading);

        timer.setSingleShot(true);
        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        timer.start(50);
        loop.exec();

        QMetaObject::invokeMethod(map, "removeUAV");

        timer.setSingleShot(true);
        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        timer.start(50);
        loop.exec();

        drawUAV(uav_latitude, uav_longitude, uav_heading);
    } else drawUAV(uav_latitude, uav_longitude, uav_heading);
}

void MapWidget::updateUAVPosition(mavlink_gps_raw_int_t gps) {
    if ((double)gps.lat/10000000 != uav_latitude && (double)gps.lon/10000000 != uav_longitude) {
        uav_latitude = (double)gps.lat/10000000;
        uav_longitude = (double)gps.lon/10000000;
        QString coords = QGeoCoordinate(uav_latitude, uav_longitude).toString(QGeoCoordinate::DegreesWithHemisphere);

        this->rootObject()->setProperty("coords", QVariant(coords));
        QMetaObject::invokeMethod(this->rootObject()->findChild<QObject*>("uavPosition"), "updateUAVPosition",
               Q_ARG(QVariant, coords));
    }
    if (updateUAVConstant) {
        updateUAV();
    }
    if (updateCenterConstant) {
        updateCenter(uav_latitude, uav_longitude);
    }
}

void MapWidget::updateUAVHeading(mavlink_vfr_hud_t vfr) {
    if (vfr.heading != uav_heading) {
        uav_heading = vfr.heading;
        QString heading = headingToCompass(uav_heading);
        this->rootObject()->setProperty("heading", QVariant(heading));
        QMetaObject::invokeMethod(this->rootObject()->findChild<QObject*>("uavHeading"), "updateUAVHeading",
                Q_ARG(QVariant, heading));
    }
}

//if (gps_int.hdg/100 != uav_heading) {
//    if (gps_int.hdg == UINT16_MAX)
//        qDebug() << "Unable to received accurate heading data from UAV, UAV default heading: North";
//    else
//        uav_heading = gps_int.hdg/100;
//    QString heading = headingToCompass(uav_heading);
//    this->rootObject()->setProperty("heading", QVariant(heading));
//    QMetaObject::invokeMethod(this->rootObject()->findChild<QObject*>("uavHeading"), "updateUAVHeading",
//            Q_ARG(QVariant, heading));
//}

QString MapWidget::headingToCompass(int heading) {
    QString compass = "Compass bearing: ";
    if ((heading<360 && heading>348.75) || (heading>=0 && heading<=11.25))
        return compass.append("N");
    if (heading>11.25 && heading<=33.75)
        return compass.append("NNE");
    if (heading>33.75 && heading<=56.25)
        return compass.append("NE");
    if (heading>56.25 && heading<=78.75)
        return compass.append("ENE");
    if (heading>78.75 && heading<=101.25)
        return compass.append("E");
    if (heading>101.25 && heading<=123.75)
        return compass.append("ESE");
    if (heading>123.75 && heading<=146.25)
        return compass.append("SE");
    if (heading>146.25 && heading<=168.75)
        return compass.append("SSE");
    if (heading>168.75 && heading<=191.25)
        return compass.append("S");
    if (heading>191.25 && heading<=213.75)
        return compass.append("SSW");
    if (heading>213.75 && heading<=236.25)
        return compass.append("SW");
    if (heading>236.25 && heading<=258.75)
        return compass.append("WSW");
    if (heading>258.75 && heading<=281.25)
        return compass.append("W");
    if (heading>281.25 && heading<=303.75)
        return compass.append("WNW");
    if (heading>303.75 && heading<=326.25)
        return compass.append("NW");
    if (heading>326.25 && heading<=348.75)
        return compass.append("NNW");
    return "MapWidget::headingToCompass - Greater than 359";
}