#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScrollBar>
#include <QScreen>
#include <QMessageBox>
#include <QTimer>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    zoneImage = new QWidget;
    layout = new QGridLayout;
    scrollArea = new QScrollArea;
    image = new QLabel(zoneImage);

    layout->addWidget(image,0,0,5,20);

    createActions();
    createImage();
    createMenus();

     //resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

void MainWindow::createMenus()
{
    menuFichier = menuBar()->addMenu(tr("&Fichier"));
    menuFichier->addAction(openAct);
    menuFichier->addAction(openActDir);
    menuFichier->addAction(saveAsAct);
    menuFichier->addAction(renameAct);

    menuEdition = menuBar()->addMenu("&Edition");
    menuEdition->addAction(tailleNormalAct);

    menuAffichage = menuBar()->addMenu("&Affichage");
    menuAffichage->addAction(metaAct);
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open"),this);
    openActDir =new QAction(tr("open Dir"), this);

    openAct->setShortcut(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));

    connect(openAct, &QAction::triggered, this, &MainWindow::ouvrirDialog);
    connect(openActDir, &QAction::triggered, this, &MainWindow::openDir);

    saveAsAct = new QAction(tr("&Save as"),this);
    saveAsAct->setShortcuts(QKeySequence::Save);
    saveAsAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);

    metaAct = new QAction(tr("&Detail image"),this);
    connect(metaAct, &QAction::triggered, this, &MainWindow::meta);

    renameAct = new QAction(tr("&Renomer"),this);
    connect(renameAct, &QAction::triggered,this, &MainWindow::rename);

    tailleNormalAct = new QAction(tr("Taille de depart"),this);
    connect(tailleNormalAct, &QAction::triggered,this,&MainWindow::tailleNormal);
}

void MainWindow::createButton()
{
    button1 = new QPushButton("<<");
    button2 = new QPushButton(">>");
    button3 = new QPushButton("Quitter");
    button4 = new QPushButton("+");
    button5 = new QPushButton("-");
    button6 = new QPushButton("rot G");
    button7 = new QPushButton("rot D");
    button8 = new QPushButton("Timer :");
    button9 = new QPushButton("supprimer");

    QObject::connect(button1, SIGNAL(clicked()),this, SLOT(buttonLeft()));
    QObject::connect(button2, SIGNAL(clicked()),this, SLOT(buttonRight()));
    QObject::connect(button3, SIGNAL(clicked()), qApp, SLOT(quit()));
    QObject::connect(button4, SIGNAL(clicked()),this, SLOT(zoomIn()));
    QObject::connect(button5, SIGNAL(clicked()),this, SLOT(zoomOut()));
    QObject::connect(button6, SIGNAL(clicked()),this, SLOT(rotationG()));
    QObject::connect(button7, SIGNAL(clicked()),this, SLOT(rotationD()));
    QObject::connect(button8, SIGNAL(clicked()),this, SLOT(buttonTime()));
    QObject::connect(button9, SIGNAL(clicked()),this, SLOT(supp()));

    layout->addWidget(button1,1,8);
    layout->addWidget(button2,3,10);
    layout->addWidget(button3,4,18);
    layout->addWidget(button4,3,2);
    layout->addWidget(button5,3,4);
    layout->addWidget(button6,4,2);
    layout->addWidget(button7,4,4);
    layout->addWidget(button8,3,14);
    layout->addWidget(button9,3,18);
}

void MainWindow::createImage()
{
    echelleFactor = 1.0;

    /*image->setBackgroundRole(QPalette::Base);*/
    w = image->width();
    h = image->height();
    image->setPixmap(fichier);

    image->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    image->setScaledContents(true);

    zoneImage->setLayout(layout);
    image->setStatusTip(fichier + " " + QString::number(image->width()) + "x" +
                        QString::number(image->height()) + " , " + QString::number(image->width()*image->height()/1024) + "ko");
    setCentralWidget(zoneImage);
    createButton();
}

void MainWindow::openDir()
{
    QString selectDir = QFileDialog::getExistingDirectory
        (
            this,
            tr("Ouvrir un répertoire"),
            "",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

        QStringList listFilter;
        listFilter << "*.jpg";
        listFilter << "*.png";

        QDirIterator dirIterator(selectDir, listFilter ,QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

        text=new QTextEdit;
        layout=new QGridLayout;
        layout->addWidget(text);

        i=0;
        /*long taille;
        taille =sizeof (fileList);*/
        while(dirIterator.hasNext())
        {
            QString fileName;
            fileName=dirIterator.next();
            fileList << fileName;
            /*text->append(fileName);
            text->append(fileList[i++]);
            text->show();*/
        }

        fichier = fileList[i];
        createImage();

        delete text;
}

void MainWindow::ouvrirDialog()
{
    fichier = QFileDialog::getOpenFileName(this, "ouvrir un fichier", QString(), "Images(*.png *.jpg *,gif *.jpeg *.JPG)");
    createImage();
}

void MainWindow::buttonRight()
{
    i++;
    if(i>=fileList.size()) i=0;
    fichier = fileList[i];
    createImage();
}

void MainWindow::buttonLeft()
{
    i--;
    if(i<0) i = fileList.size()-1;
    fichier = fileList[i];
    createImage();
}

void MainWindow::buttonTime()
{
    QTimer *timer = new QTimer(this);
    int tim = QInputDialog::getInt(this, "temps", "minuteur(sec):");

    connect(timer, SIGNAL(timeout()), this, SLOT(buttonRight()));
    timer->start(tim*1000);

    delete timer;
}

void MainWindow::echelleImage(double factor)
{
    Q_ASSERT(image->pixmap());
      echelleFactor *= factor;
      /*double w = (image->width())*echelleFactor;
      double h = (image->height())*echelleFactor;

      image->resize(static_cast<int>(w), static_cast<int>(h));*/
      image->resize(echelleFactor * image->pixmap()->size());

      adjustScrollBar(scrollArea->horizontalScrollBar(), echelleFactor);
      adjustScrollBar(scrollArea->verticalScrollBar(), echelleFactor);

     // resize(QGuiApplication::primaryScreen()->availableSize() * 3/5);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}
void MainWindow::zoomIn()
{
    echelleImage(1.25);
}

void MainWindow::zoomOut()
{
    echelleImage(0.8);
}

void MainWindow::rotationD()
{
    image->setPixmap(image->pixmap()->transformed(QMatrix().rotate(90)));
   // image->adjustSize();
}

void MainWindow::rotationG()
{
    image->setPixmap(image->pixmap()->transformed(QMatrix().rotate(-90)));
}

void MainWindow::tailleNormal()
{
    image->adjustSize();
    echelleFactor = 1.0;
}

void MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save image"), "", tr("image (*.jpg,*.png);;All Files (*)"));
    if (fileName.isEmpty())
         return;
     else
     {
         QFile file(fileName);
         if (!file.open(QIODevice::WriteOnly))
         {
             QMessageBox::information(this, tr("Impossible !"),file.errorString());
             return;
         }
         image->pixmap()->save(&file,"jpg");
     }
}

void MainWindow::meta()
{
    QMessageBox::information(this, "info image", "largeur x hauteur: " + QString::number(image->width()) + "x" +
                                                                         QString::number(image->height()));
}

void MainWindow::rename()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("rename image"), "", tr("image"));
    if (fileName.isEmpty())
         return;
     else
     {
         QFile file2(fichier);
         if(!file2.rename(fileName)){
                QMessageBox::information(this, tr("Impossible !"),file2.errorString());
         }
     }
}

void MainWindow::supp()
{
    QFile *file = new QFile(fichier);

    if(!fichier.isEmpty()){
            file->remove();
            QMessageBox::information(this, "le fichier", " " + file->fileName() + "a ete supprimer");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete image;
    delete label;

    delete button1;
    delete button2;
    delete button3;
    delete button4;
    delete button5;
    delete button6;
    delete button7;
    delete button8;
    delete button9;

    delete image;
    delete layout;
    delete zoneImage;
    delete label;

    delete menuFichier;
}
