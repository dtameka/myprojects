#ifndef VIEWER_H_
#define VIEWER_H_

#include <QMainWindow>
#include <QStandardPaths>
#include <QOpenGLWidget>
#include <QFileDialog>
#include <QDebug>
#include <QXmlStreamReader>
#include <QFontDatabase>
#include <QMessageBox>
#include <QPainter>
#include <QList>
#include <QTimer>

#include "setting.h"
#include "parser.h"
#include "qgifimage.h"
#include "qgifglobal.h"
#include "qgifimage_p.h"

QT_BEGIN_NAMESPACE
namespace Ui { class viewer; }
QT_END_NAMESPACE

class viewer : public QMainWindow {
    Q_OBJECT
public:
    viewer(QWidget *parent = nullptr);
    ~viewer();

public slots:
    void changeSettings(options newSettings);

private:
    Ui::viewer *ui;
    QSettings *settings;
    setting *settingWindow;
    options currentSetting;
    QString documentsLocation = QDir::homePath() + "/" +
            QStandardPaths::displayName(QStandardPaths::DocumentsLocation);
    QTimer *timer;
    QList<QImage> scrinshot_list;
    QString gifPath;
    void loadSetting();
    int loadObjData(QString fileName);

private slots:
    void open();
    void saveAsImage();
    void saveAsGIF();
    void configuration();
    void closeApp();
    void contextMenuRequested(const QPoint &pos);
    void take_picture();


    void on_pushButton_x_rotate_plus_clicked();
    void on_pushButton_x_rotate_minus_clicked();
    void on_pushButton_y_rotate_plus_clicked();
    void on_pushButton_y_rotate_minus_clicked();
    void on_pushButton_z_rotate_plus_clicked();
    void on_pushButton_z_rotate_minus_clicked();
    void on_pushButton_x_move_plus_clicked();
    void on_pushButton_x_move_minus_clicked();
    void on_pushButton_y_move_plus_clicked();
    void on_pushButton_y_move_minus_clicked();
    void on_pushButton_z_move_plus_clicked();
    void on_pushButton_z_move_minus_clicked();
    void on_horizontalSlider_zoom_valueChanged(int value);
};
#endif  // VIEWER_H_
