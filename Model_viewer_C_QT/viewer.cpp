#include "viewer.h"
#include "ui_viewer.h"
#include "myopenglspace.h"
#include "qgifimage.h"

viewer::viewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::viewer) {
    ui->setupUi(this);

    int id = QFontDatabase::addApplicationFont(":/fonts/res/fonts/roboto-regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont roboto(family);
    roboto.setPointSize(10);
    qApp->setFont(roboto);

    connect(ui->openFile, &QAction::triggered, this, &viewer::open);
    connect(ui->saveAsImage, &QAction::triggered, this, &viewer::saveAsImage);
    connect(ui->saveAsGIF, &QAction::triggered, this, &viewer::saveAsGIF);
    connect(ui->openSettings, &QAction::triggered, this, &viewer::configuration);
    connect(ui->actionQuit, &QAction::triggered, this, &viewer::closeApp);
    connect(ui->myOpenGL, &QWidget::customContextMenuRequested, this, &viewer::contextMenuRequested);

    // Initializing settings
    loadSetting();
    settingWindow = new setting(nullptr, currentSetting);
    connect(settingWindow, &setting::changeSettings, this, &viewer::changeSettings);
}

viewer::~viewer() {
    delete ui;
}

void viewer::changeSettings(options newSettings) {
    currentSetting = newSettings;

    // Save local config
    settings->setValue("colorBg", currentSetting.colorBg.name());
    settings->setValue("projectionType", currentSetting.projectionType);
    settings->setValue("lineType", currentSetting.lineType);
    settings->setValue("colorLine", currentSetting.colorLine.name());
    settings->setValue("widthLine", currentSetting.widthLine);
    settings->setValue("vertexType", currentSetting.vertexType);
    settings->setValue("colorVertex", currentSetting.colorVertex.name());
    settings->setValue("vertexSize", currentSetting.vertexSize);
    settings->sync();
    ui->statusBar->showMessage("Настройки сохранены");

    ui->myOpenGL->projection = currentSetting.projectionType;
    ui->myOpenGL->lineType = currentSetting.lineType;
    ui->myOpenGL->lineWidth = currentSetting.widthLine;
    ui->myOpenGL->verticeType = currentSetting.vertexType;
    ui->myOpenGL->verticeSize = currentSetting.vertexSize;

    ui->myOpenGL->backRedColor = currentSetting.colorBg.redF();
    ui->myOpenGL->backGreenColor = currentSetting.colorBg.greenF();
    ui->myOpenGL->backBlueColor = currentSetting.colorBg.blueF();

    ui->myOpenGL->lineRedColor = currentSetting.colorLine.redF();
    ui->myOpenGL->lineGreenColor = currentSetting.colorLine.greenF();
    ui->myOpenGL->lineBlueColor = currentSetting.colorLine.blueF();

    ui->myOpenGL->vertRedColor = currentSetting.colorVertex.redF();
    ui->myOpenGL->vertGreenColor = currentSetting.colorVertex.greenF();
    ui->myOpenGL->vertBlueColor = currentSetting.colorVertex.blueF();
    ui->myOpenGL->update();
}

void viewer::loadSetting() {
    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "s21", "3D Viewer");

    if (settings->status() == QSettings::NoError) {
        QColor colorBg = settings->value("colorBg").value<QColor>();
        QColor colorLine = settings->value("colorLine").value<QColor>();
        QColor colorVertex = settings->value("colorVertex").value<QColor>();
        int projectionType = settings->value("projectionType", 0).toInt();
        int lineType = settings->value("lineType", 0).toInt();
        double widthLine = settings->value("widthLine", 1.0).toDouble();
        int vertexType = settings->value("vertexType", 0).toInt();
        double vertexSize = settings->value("vertexSize", 1.0).toDouble();

        currentSetting.colorBg = colorBg.isValid() ? colorBg : QColor(0, 0, 0);
        currentSetting.projectionType = (projectionType == 0 || projectionType == 1) ? projectionType : 0;
        currentSetting.lineType = (lineType == 0 || lineType == 1) ? lineType : 0;
        currentSetting.colorLine = colorLine.isValid() ? colorLine : QColor(255, 255, 255);
        currentSetting.widthLine = (widthLine >= 1 && widthLine <= 15) ? widthLine : 1.0;
        currentSetting.vertexType = (vertexType >= 0 && vertexType <= 2) ? vertexType : 0;
        currentSetting.colorVertex = colorVertex.isValid() ? colorVertex : QColor(255, 0, 0);
        currentSetting.vertexSize = (vertexSize >= 1 && vertexSize <= 15) ? vertexSize : 1.0;
    } else {
        currentSetting = {QColor(255, 255, 255), 0, 0, QColor(0, 0, 0), 1.0, 0, QColor(255, 0, 0), 2.0};
    }
    ui->myOpenGL->projection = currentSetting.projectionType;
    ui->myOpenGL->lineType = currentSetting.lineType;
    ui->myOpenGL->lineWidth = currentSetting.widthLine;
    ui->myOpenGL->verticeType = currentSetting.vertexType;
    ui->myOpenGL->verticeSize = currentSetting.vertexSize;

    ui->myOpenGL->backRedColor = currentSetting.colorBg.redF();
    ui->myOpenGL->backGreenColor = currentSetting.colorBg.greenF();
    ui->myOpenGL->backBlueColor = currentSetting.colorBg.blueF();

    ui->myOpenGL->lineRedColor = currentSetting.colorLine.redF();
    ui->myOpenGL->lineGreenColor = currentSetting.colorLine.greenF();
    ui->myOpenGL->lineBlueColor = currentSetting.colorLine.blueF();

    ui->myOpenGL->vertRedColor = currentSetting.colorVertex.redF();
    ui->myOpenGL->vertGreenColor = currentSetting.colorVertex.greenF();
    ui->myOpenGL->vertBlueColor = currentSetting.colorVertex.blueF();
    ui->myOpenGL->update();
}

void viewer::open() {
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть 3D-модель",
                                                    documentsLocation, "Файлы 3D-модели(*.obj)");
    if (!fileName.isEmpty() && !loadObjData(fileName)) {
            QStringList fileNameArr = fileName.split("/");
            ui->FileName->setText(fileNameArr.at(fileNameArr.size()-1));
        } else {
            QMessageBox::warning(this, "Внимание","Ошибка загрузки файла!");
        }
}

int viewer::loadObjData(QString fileName) {
    int error;
    double *vert;
    int *ind;
    int vertArrCount, edgeArrCount, vertCount, edgeCount;
    error = parse_object_file(fileName.toStdString().c_str(), &vert, &ind, &vertArrCount, &edgeArrCount, &vertCount, &edgeCount);
    if (!error) {
        double min, max;
        min_max(vert, &min, &max, vertArrCount);
        if (ui->myOpenGL != NULL) {
            free(ui->myOpenGL->vertices);
        }
        if (ui->myOpenGL->indices != NULL) {
            free(ui->myOpenGL->indices);
        }
        ui->myOpenGL->min = min;
        ui->myOpenGL->max = max;
        ui->myOpenGL->vertices = vert;
        ui->myOpenGL->indices = ind;
        ui->myOpenGL->vertArrCount = vertArrCount;
        ui->myOpenGL->edgeArrCount = edgeArrCount;
        ui->myOpenGL->vertCount = vertCount;
        ui->myOpenGL->edgeCount = edgeCount;
        ui->myOpenGL->update();


        ui->Vertices_value->setText(QString::number(vertCount));
        ui->Edges_value->setText(QString::number(edgeArrCount));
    }
    return error;
}


void viewer::saveAsImage() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить как изображение",
                                                    documentsLocation,
                                                    "Изображение (*.bmp);;"
                                                    "Изображение (*.jpeg);;"
                                                    "Изображение (*.png)");
    QImage img = ui->myOpenGL->grabFramebuffer();
    if (!fileName.isEmpty() && !fileName.isNull()) {
        img.save(fileName);
        ui->statusBar->showMessage("Изображение сохранено");
    }
}

void viewer::saveAsGIF() {
    gifPath = QFileDialog::getSaveFileName(this, "Сохранить как анимацию",
                                                    documentsLocation,
                                                    "Анимация (*.gif)");
    if(!gifPath.isEmpty() && !gifPath.isNull()) {
        timer = new QTimer();
        timer->setInterval(100);
        connect(timer, &QTimer::timeout, this, &viewer::take_picture);
        timer->start(100); //  Запустим таймер
    }
}

void viewer::take_picture() {

    QImage image = ui->myOpenGL->grabFramebuffer();
    scrinshot_list.append(image);
    if (scrinshot_list.size() == 50) {
        timer->stop();

        QGifImage gif(QSize(640, 480));
        gif.setDefaultDelay(100);
        for (int i=0; i<50; ++i) {
            gif.addFrame(scrinshot_list[i], QPoint(0, 0));
        }
        gif.save(gifPath);
        scrinshot_list.clear();
        ui->statusBar->showMessage("Гифка сохранена");
    }
}

void viewer::configuration() {
    settingWindow->setWindowFlags( Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    settingWindow->show();
}

void viewer::closeApp() {
    qApp->quit();
}

void viewer::contextMenuRequested(const QPoint &pos) {
    Q_UNUSED(pos);

    QMenu *menuList = new QMenu(this);
    menuList->addAction(ui->openFile);
    menuList->addAction(ui->saveAsImage);
    menuList->addAction(ui->saveAsGIF);
    menuList->addSeparator();
    menuList->addAction(ui->openSettings);
    menuList->exec(QCursor::pos());

    delete menuList;
}

void viewer::on_pushButton_x_rotate_plus_clicked()
{
    rotateMatrix(ui->myOpenGL->vertices,  &(ui->myOpenGL->vertArrCount), ui->doubleSpinBox_rotate_x->value()/180*M_PI, 0.0, 0.0);
    ui->myOpenGL->update();
}

void viewer::on_pushButton_x_rotate_minus_clicked()
{
    rotateMatrix(ui->myOpenGL->vertices,  &(ui->myOpenGL->vertArrCount), -(ui->doubleSpinBox_rotate_x->value()/180*M_PI), 0.0, 0.0);
    ui->myOpenGL->update();
}

void viewer::on_pushButton_y_rotate_plus_clicked()
{
    rotateMatrix(ui->myOpenGL->vertices,  &(ui->myOpenGL->vertArrCount), 0.0, ui->doubleSpinBox_rotate_y->value()/180*M_PI, 0.0);
    ui->myOpenGL->update();
}

void viewer::on_pushButton_y_rotate_minus_clicked()
{
    rotateMatrix(ui->myOpenGL->vertices,  &(ui->myOpenGL->vertArrCount), 0.0, -(ui->doubleSpinBox_rotate_y->value()/180*M_PI), 0.0);
    ui->myOpenGL->update();
}

void viewer::on_pushButton_z_rotate_plus_clicked()
{
    rotateMatrix(ui->myOpenGL->vertices,  &(ui->myOpenGL->vertArrCount), 0.0, 0.0, ui->doubleSpinBox_rotate_z->value()/180*M_PI);
    ui->myOpenGL->update();
}

void viewer::on_pushButton_z_rotate_minus_clicked()
{
    rotateMatrix(ui->myOpenGL->vertices,  &(ui->myOpenGL->vertArrCount), 0.0, 0.0, -(ui->doubleSpinBox_rotate_z->value()/180*M_PI));
    ui->myOpenGL->update();
}

void viewer::on_pushButton_x_move_plus_clicked()
{
    moveMatrix(ui->myOpenGL->vertices,  &(ui->myOpenGL->vertArrCount), ui->doubleSpinBox_move_x->value(), 0.0, 0.0);
    ui->myOpenGL->update();
}

void viewer::on_pushButton_x_move_minus_clicked()
{
    moveMatrix(ui->myOpenGL->vertices,  &(ui->myOpenGL->vertArrCount), -(ui->doubleSpinBox_move_x->value()), 0.0, 0.0);
    ui->myOpenGL->update();
}

void viewer::on_pushButton_y_move_plus_clicked()
{
    moveMatrix(ui->myOpenGL->vertices,  &(ui->myOpenGL->vertArrCount), 0.0, ui->doubleSpinBox_move_y->value(), 0.0);
    ui->myOpenGL->update();
}

void viewer::on_pushButton_y_move_minus_clicked()
{
    moveMatrix(ui->myOpenGL->vertices,  &(ui->myOpenGL->vertArrCount), 0.0, -(ui->doubleSpinBox_move_y->value()), 0.0);
    ui->myOpenGL->update();
}

void viewer::on_pushButton_z_move_plus_clicked()
{
    moveMatrix(ui->myOpenGL->vertices,  &(ui->myOpenGL->vertArrCount), 0.0, 0.0, ui->doubleSpinBox_move_z->value());
    ui->myOpenGL->update();
}

void viewer::on_pushButton_z_move_minus_clicked()
{
    moveMatrix(ui->myOpenGL->vertices,  &(ui->myOpenGL->vertArrCount), 0.0, 0.0, -(ui->doubleSpinBox_move_z->value()));
    ui->myOpenGL->update();
}


void viewer::on_horizontalSlider_zoom_valueChanged(int value)
{
    double k;
    if (value > ui->myOpenGL->previousScale) {
        k = double(value - ui->myOpenGL->previousScale)*1.3;
    } else if (value < ui->myOpenGL->previousScale) {
        k = 1.0 / (double(ui->myOpenGL->previousScale-value)*1.3);
    }
    scaleMatrix(ui->myOpenGL->vertices,  &(ui->myOpenGL->vertArrCount), k, k, k);
    ui->myOpenGL->previousScale = value;
    ui->myOpenGL->update();
}
