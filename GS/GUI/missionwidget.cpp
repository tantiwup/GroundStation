#include "missionwidget.h"
#include "ui_missionwidget.h"
//#include "qtmaterialraisedbutton.h"
#include <QDebug>
#include "point.hpp"
#include "plan_mission.hpp"
MissionWidget::MissionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MissionWidget)
{
    ui->setupUi(this);
    missions = new QVector<Mission*>();
    ui->missionList->setEditable(true);
    ui->missionList->lineEdit()->setReadOnly(true);
    ui->missionList->lineEdit()->setAlignment(Qt::AlignCenter);
    connect(ui->loadButton, &QPushButton::clicked, this, &MissionWidget::loadMission);
    connect(ui->writeButton, &QPushButton::clicked, this, &MissionWidget::writeButtonClicked);
    connect(ui->readButton, &QPushButton::clicked, this, &MissionWidget::readButtonClicked);
    connect(ui->clearButton, &QPushButton::clicked, this, &MissionWidget::clearButtonClicked);
}

bool MissionWidget::hasMission() {
    return missions->size() > 0;
}

void MissionWidget::loadMission() {
    if (hasMission()) {
        PlanMission pm;
        Mission * selectedMission = missions->at(ui->missionList->currentIndex());
        for (int i = 1; i < selectedMission->mission_waypoints.waypoints->size(); i++) {
            QVector3D point = selectedMission->mission_waypoints.waypoints->at(i);
            qDebug() << "X: " << point.x() << " Y: " << point.y() << " Z: " << point.z();
            pm.add_goal_point(selectedMission->toECEF(point.x(), point.y(), point.z()));
        }
        QVector3D start_point = selectedMission->mission_waypoints.waypoints->at(0);
        qDebug() << "Start X: " << start_point.x() << " Y: " << start_point.y() << " Z: " << start_point.z();
        QString sb = "{"
                     "    \"moving_obstacles\": ["
                     "        {"
                     "            \"altitude_msl\": 189.56748784643966,"
                     "            \"latitude\": 34.141826869853645,"
                     "            \"longitude\": -76.43199876559223,"
                     "            \"sphere_radius\": 150.0"
                     "        },"
                     "        {"
                     "            \"altitude_msl\": 250.0,"
                     "            \"latitude\": 34.14923628783763,"
                     "            \"longitude\": -76.43238529543882,"
                     "            \"sphere_radius\": 150.0"
                     "        }"
                     "    ],"
                     "    \"stationary_obstacles\": ["
                     "        {"
                     "            \"cylinder_height\": 750.0,"
                     "            \"cylinder_radius\": 300.0,"
                     "            \"latitude\": 34.141826869853645,"
                     "            \"longitude\": -76.43199876559223"
                     "        },"
                     "        {"
                     "            \"cylinder_height\": 400.0,"
                     "            \"cylinder_radius\": 100.0,"
                     "            \"latitude\": 34.149156,"
                     "            \"longitude\": -76.430622"
                     "        }"
                     "    ]"
                     "}";
        pm.set_obstacles(QJsonDocument::fromJson(sb.toUtf8()));
        qDebug() << pm.get_path(selectedMission->toECEF(start_point.x(), start_point.y(), start_point.z()));
        emit (drawMission(selectedMission));
        model = createMissionModel(selectedMission);
        ui->missionTable->setModel(model);
        ui->missionTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        for (int c = 1; c < ui->missionTable->horizontalHeader()->count(); ++c) {
            ui->missionTable->horizontalHeader()->setSectionResizeMode(
                c, QHeaderView::ResizeToContents);
        }
    }
}

void MissionWidget::writeButtonClicked() {
    // Writes only the first mission in the mission vector
//    if (missions->empty()) qDebug() << "MissionWidget::writeButtonClicked -> empty mission vector";
//    else
//        emit(writeMissionsSignal(constructWaypoints(*missions->at(0)),
//                              missions->at(0)->mission_waypoints.waypoints->length()));

    // Writing CUSTOM TEST missions TODO Revert
    emit(writeMissionsSignal(testWaypoints(), 3));
}
void MissionWidget::readButtonClicked() {
    emit(readMissionsSignal());
}
void MissionWidget::clearButtonClicked() {
    emit(clearMissions());
}

void MissionWidget::readMissions(Waypoint::WP * waypoints, uint16_t size) {
    qDebug() << "!-----------------------!";
    qDebug() << "MissionWidget::readMissions test";
    qDebug() << "Mission waypoints length:" << size;
    for (uint16_t i = 0; i < size; i++) {
        qDebug() << "Waypoint" << waypoints[i].id << "->" << waypoints[i].x << waypoints[i].y << waypoints[i].z;
    }
    qDebug() << "!-----------------------!";
}

void MissionWidget::writeMissionsStatus(bool success) {
    if (success) qDebug() << "Write Missions Successful";
    else qDebug() << "Write Missions failed";
}

QStandardItemModel * MissionWidget::createMissionModel(Mission * mission) {
    QList<QVector3D> * waypoints = mission->mission_waypoints.waypoints;
    QStandardItemModel *model = new QStandardItemModel;
    model->setHorizontalHeaderLabels(QList<QString>({"LAT,LON", " ALT ", " CMD "}));
    for (int i = 0; i < waypoints->size(); i++) {
        QString coords = QGeoCoordinate(waypoints->at(i).x(), waypoints->at(i).y()).toString(QGeoCoordinate::DegreesWithHemisphere);
        QStandardItem * latlng = new QStandardItem(coords);
        QStandardItem * alt = new QStandardItem(QString("%0 m.").arg(waypoints->at(i).z()));
        QStandardItem * action = new QStandardItem(QString("%0").arg(mission->mission_waypoints.actions->at(i)));
        QList<QStandardItem*> row({latlng, alt, action});
        for (int j = 0; j < row.size(); j++) {
            row.at(j)->setTextAlignment(Qt::AlignCenter);
        }
        model->appendRow(row);
    }
    return model;
}

void MissionWidget::getMissions(Interop * i) {
    QJsonArray interopMissions = i->getMissions().array();
    for (int j = 0; j < interopMissions.size(); j++) {
        missions->append(new Mission(interopMissions.at(j).toObject()));
        ui->missionList->addItem("Mission " + QString::number(j+1));
        ui->missionList->setItemData(j, Qt::AlignCenter, Qt::TextAlignmentRole);
    }
}

//void MissionWidget::getObstacles(Interop * i) {
//    Obstacles obstacles = interop->getObstacles();
//    obstacles.loadStationaryObjects(mapWidget);

Waypoint::WP* MissionWidget::constructWaypoints(const Mission& mission) {
    Waypoint::WP* waypoints = new Waypoint::WP[missions->length()];
    for (uint16_t i = 0; i < mission.mission_waypoints.waypoints->length(); i++) {
        Waypoint::WP wp;
        qDebug() << "MissionWidget::constructWaypoints -> Temporary placeholder values. MUST change for actual flight";
//        TEMPORARY VALUES. NEEDS TO BE MODIFIED FOR ACTUAL FLIGHT
        wp.id = i;
        wp.command = 16;
        wp.autocontinue = 1;
        wp.current = 0;
        wp.param1 = 0;
        wp.param2 = 10;
        wp.param3 = 0;
        wp.x = mission.mission_waypoints.waypoints->at(i).x();
        wp.y = mission.mission_waypoints.waypoints->at(i).y();
        wp.z = mission.mission_waypoints.waypoints->at(i).z();
        waypoints[i] = wp;
    }
    return waypoints;
}

MissionWidget::~MissionWidget()
{
    delete ui;
}


// TEST WAYPOINTS TODO: delete
Waypoint::WP* MissionWidget::testWaypoints() {
    // TEST WAYPOINTS TODO: delete
        int num_waypoints = 6;
        Waypoint::WP* waypoints = new Waypoint::WP[num_waypoints];
        // Home
        waypoints[0].autocontinue = 1;
        waypoints[0].command = 16;
        waypoints[0].current = 0;
        waypoints[0].param1 = 0;
        waypoints[0].param2 = 0;
        waypoints[0].param3 = 0;
        waypoints[0].param4 = 0;
        waypoints[0].x = -35.36326;
        waypoints[0].y = 149.16524;
        waypoints[0].z = 584.1;
        waypoints[0].id = 0;
        waypoints[0].frame = 0;

        // Takeoff
        waypoints[1].autocontinue = 1;
        waypoints[1].command = 22;
        waypoints[1].current = 0;
        waypoints[1].frame = 3;
        waypoints[1].id = 1;
        waypoints[1].param1 = 15;
        waypoints[1].param2 = 0;
        waypoints[1].param3 = 0;
        waypoints[1].param4 = 0;
        waypoints[1].x = 33.6405;
        waypoints[1].y = -117.8443;
        waypoints[1].z = 50;

        for (int i = 2; i < 5; i++) {
            waypoints[i].autocontinue = 1;
            waypoints[i].command = 16;
            waypoints[i].current = 0;
            waypoints[i].frame = 3;
            waypoints[i].id = i;
            waypoints[i].param1 = 0;
            waypoints[i].param2 = 0;
            waypoints[i].param3 = 0;
            waypoints[i].param4 = 0;
            waypoints[i].x = 33.6405;
            waypoints[i].y = -117.8443;
            waypoints[i].z = 50;
        }
        // Loiter
//        waypoints[2].autocontinue = 1;
//        waypoints[2].command = 19;
//        waypoints[2].current = 0;
//        waypoints[2].frame = 3;
//        waypoints[2].id = 2;
//        waypoints[2].param1 = 20;
//        waypoints[2].param2 = 0;
//        waypoints[2].param3 = 1;
//        waypoints[2].param4 = 1;
//        waypoints[2].x = -35.36326;
//        waypoints[2].y = 149.16524;
//        waypoints[2].z = 50;

        // Land
        waypoints[2].autocontinue = 1;
        waypoints[2].command = 21;
        waypoints[2].current = 0;
        waypoints[2].frame = 3;
        waypoints[2].id = 2;
        waypoints[2].param1 = 25;
        waypoints[2].param2 = 2;
        waypoints[2].param3 = 0;
        waypoints[2].param4 = 0;
        waypoints[2].x = 33.6405;
        waypoints[2].y = -117.8443;
        waypoints[2].z = 0;

    return waypoints;
}