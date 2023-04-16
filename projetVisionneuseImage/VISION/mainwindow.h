#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGraphicsView>
#include <QFileDialog>
#include <QTextEdit>
#include <QDirIterator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void buttonRight();
    void buttonLeft();
    void zoomIn();
    void zoomOut();
    void rotationD();
    void rotationG();
    void buttonTime();
    void supp();
    void rename();
    void tailleNormal();

private:
    Ui::MainWindow *ui;

    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    QPushButton *button5;
    QPushButton *button6;
    QPushButton *button7;
    QPushButton *button8;
    QPushButton *button9;


    QLabel *label;

    QGridLayout *layout;

    QWidget *zoneImage;

    QScrollArea *scrollArea;

    QLabel *image;
    QString fichier;

    QMainWindow fenetre;

    QMenu *menuFichier;
    QMenu *menuEdition;
    QMenu *menuAffichage;

    QAction *openAct;
    QAction *openActDir;
    QAction *exitAct;
    QAction *undoAct;
    QAction *redoAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *saveAsAct;
    QAction *metaAct;
    QAction *renameAct;
    QAction *tailleNormalAct;

    QTextEdit *text;
    QStringList fileList;

    void ouvrirDialog();
    void parcourir();

    void createMenus();
    void createActions();
    void createButton();
    void createImage();

    void openDir();
    void echelleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void saveAs();
    void meta();


    int w,h,i;
    double factor = 1.0;
    double echelleFactor;
};

#endif // MAINWINDOW_H
