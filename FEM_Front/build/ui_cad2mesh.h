/********************************************************************************
** Form generated from reading UI file 'cad2mesh.ui'
**
** Created by: Qt User Interface Compiler version 5.15.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAD2MESH_H
#define UI_CAD2MESH_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glviewer.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionLoad;
    QAction *actionView_mesh_edges;
    QAction *actionView_mesh_normals;
    QAction *actionClear_data;
    QAction *actionMesh_options;
    QAction *actionView_information_bar;
    QAction *actionView_options;
    QAction *actionView_mesh_facets;
    QAction *actionSave_camera;
    QAction *actionLoad_camera;
    QAction *actiontest;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    GlViewer *viewer;
    QStatusBar *statusbar;
    QDockWidget *dockWidget_infos;
    QWidget *dockWidgetContents;
    QTextBrowser *textBrowser;
    QDockWidget *dockWidget_options;
    QWidget *dockWidgetContents_3;
    QTabWidget *tabWidget_options;
    QWidget *tab_solver;
    QWidget *tab_view;
    QLabel *label_52;
    QWidget *horizontalLayoutWidget_14;
    QHBoxLayout *horizontalLayout_14;
    QVBoxLayout *verticalLayout_30;
    QLabel *label_53;
    QLabel *label_54;
    QVBoxLayout *verticalLayout_31;
    QDoubleSpinBox *spinInput_point_size;
    QDoubleSpinBox *spinColor_point_size;
    QWidget *horizontalLayoutWidget_15;
    QHBoxLayout *horizontalLayout_15;
    QVBoxLayout *verticalLayout_32;
    QLabel *label_56;
    QLabel *label_59;
    QVBoxLayout *verticalLayout_33;
    QDoubleSpinBox *spinMesh_edge_width;
    QDoubleSpinBox *spinVector_ratio;
    QLabel *label_48;
    QMenuBar *menubar;
    QMenu *menuData;
    QMenu *menuView;
    QMenu *menuFunction;
    QMenu *menuWindow;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(2000, 1200);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(2000, 1200));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QString::fromUtf8("actionLoad"));
        actionView_mesh_edges = new QAction(MainWindow);
        actionView_mesh_edges->setObjectName(QString::fromUtf8("actionView_mesh_edges"));
        actionView_mesh_edges->setCheckable(true);
        actionView_mesh_edges->setChecked(true);
        actionView_mesh_normals = new QAction(MainWindow);
        actionView_mesh_normals->setObjectName(QString::fromUtf8("actionView_mesh_normals"));
        actionView_mesh_normals->setCheckable(true);
        actionClear_data = new QAction(MainWindow);
        actionClear_data->setObjectName(QString::fromUtf8("actionClear_data"));
        actionMesh_options = new QAction(MainWindow);
        actionMesh_options->setObjectName(QString::fromUtf8("actionMesh_options"));
        actionView_information_bar = new QAction(MainWindow);
        actionView_information_bar->setObjectName(QString::fromUtf8("actionView_information_bar"));
        actionView_information_bar->setCheckable(true);
        actionView_information_bar->setChecked(true);
        actionView_options = new QAction(MainWindow);
        actionView_options->setObjectName(QString::fromUtf8("actionView_options"));
        actionView_options->setCheckable(true);
        actionView_options->setChecked(true);
        actionView_mesh_facets = new QAction(MainWindow);
        actionView_mesh_facets->setObjectName(QString::fromUtf8("actionView_mesh_facets"));
        actionView_mesh_facets->setCheckable(true);
        actionView_mesh_facets->setChecked(true);
        actionSave_camera = new QAction(MainWindow);
        actionSave_camera->setObjectName(QString::fromUtf8("actionSave_camera"));
        actionLoad_camera = new QAction(MainWindow);
        actionLoad_camera->setObjectName(QString::fromUtf8("actionLoad_camera"));
        actiontest = new QAction(MainWindow);
        actiontest->setObjectName(QString::fromUtf8("actiontest"));
        actiontest->setCheckable(false);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        viewer = new GlViewer(centralwidget);
        viewer->setObjectName(QString::fromUtf8("viewer"));
        sizePolicy.setHeightForWidth(viewer->sizePolicy().hasHeightForWidth());
        viewer->setSizePolicy(sizePolicy);
        viewer->setMinimumSize(QSize(0, 0));
        viewer->setAutoFillBackground(false);
        viewer->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));

        verticalLayout->addWidget(viewer);

        verticalLayout->setStretch(0, 6);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        dockWidget_infos = new QDockWidget(MainWindow);
        dockWidget_infos->setObjectName(QString::fromUtf8("dockWidget_infos"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dockWidget_infos->sizePolicy().hasHeightForWidth());
        dockWidget_infos->setSizePolicy(sizePolicy1);
        dockWidget_infos->setMinimumSize(QSize(545, 360));
        dockWidget_infos->setMaximumSize(QSize(524287, 524287));
        dockWidget_infos->setFloating(false);
        dockWidget_infos->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        dockWidgetContents->setMaximumSize(QSize(16777215, 16777215));
        textBrowser = new QTextBrowser(dockWidgetContents);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(0, 0, 545, 336));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy2);
        textBrowser->setAutoFillBackground(false);
        dockWidget_infos->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget_infos);
        dockWidget_options = new QDockWidget(MainWindow);
        dockWidget_options->setObjectName(QString::fromUtf8("dockWidget_options"));
        sizePolicy.setHeightForWidth(dockWidget_options->sizePolicy().hasHeightForWidth());
        dockWidget_options->setSizePolicy(sizePolicy);
        dockWidget_options->setMinimumSize(QSize(545, 793));
        dockWidget_options->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea|Qt::TopDockWidgetArea);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
        tabWidget_options = new QTabWidget(dockWidgetContents_3);
        tabWidget_options->setObjectName(QString::fromUtf8("tabWidget_options"));
        tabWidget_options->setGeometry(QRect(10, 20, 800, 1400));
        tab_solver = new QWidget();
        tab_solver->setObjectName(QString::fromUtf8("tab_solver"));
        tabWidget_options->addTab(tab_solver, QString());
        tab_view = new QWidget();
        tab_view->setObjectName(QString::fromUtf8("tab_view"));
        label_52 = new QLabel(tab_view);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setGeometry(QRect(240, 10, 101, 20));
        QFont font;
        font.setPointSize(9);
        label_52->setFont(font);
        horizontalLayoutWidget_14 = new QWidget(tab_view);
        horizontalLayoutWidget_14->setObjectName(QString::fromUtf8("horizontalLayoutWidget_14"));
        horizontalLayoutWidget_14->setGeometry(QRect(10, 40, 511, 61));
        horizontalLayout_14 = new QHBoxLayout(horizontalLayoutWidget_14);
        horizontalLayout_14->setObjectName(QString::fromUtf8("horizontalLayout_14"));
        horizontalLayout_14->setContentsMargins(0, 0, 0, 0);
        verticalLayout_30 = new QVBoxLayout();
        verticalLayout_30->setObjectName(QString::fromUtf8("verticalLayout_30"));
        label_53 = new QLabel(horizontalLayoutWidget_14);
        label_53->setObjectName(QString::fromUtf8("label_53"));

        verticalLayout_30->addWidget(label_53);

        label_54 = new QLabel(horizontalLayoutWidget_14);
        label_54->setObjectName(QString::fromUtf8("label_54"));

        verticalLayout_30->addWidget(label_54);


        horizontalLayout_14->addLayout(verticalLayout_30);

        verticalLayout_31 = new QVBoxLayout();
        verticalLayout_31->setObjectName(QString::fromUtf8("verticalLayout_31"));
        spinInput_point_size = new QDoubleSpinBox(horizontalLayoutWidget_14);
        spinInput_point_size->setObjectName(QString::fromUtf8("spinInput_point_size"));
        spinInput_point_size->setDecimals(6);
        spinInput_point_size->setMinimum(0.000000000000000);
        spinInput_point_size->setMaximum(100000.000000000000000);
        spinInput_point_size->setSingleStep(0.500000000000000);
        spinInput_point_size->setValue(2.500000000000000);

        verticalLayout_31->addWidget(spinInput_point_size);

        spinColor_point_size = new QDoubleSpinBox(horizontalLayoutWidget_14);
        spinColor_point_size->setObjectName(QString::fromUtf8("spinColor_point_size"));
        spinColor_point_size->setDecimals(6);
        spinColor_point_size->setMaximum(1000000.000000000000000);
        spinColor_point_size->setSingleStep(0.500000000000000);
        spinColor_point_size->setValue(5.000000000000000);

        verticalLayout_31->addWidget(spinColor_point_size);


        horizontalLayout_14->addLayout(verticalLayout_31);

        horizontalLayoutWidget_15 = new QWidget(tab_view);
        horizontalLayoutWidget_15->setObjectName(QString::fromUtf8("horizontalLayoutWidget_15"));
        horizontalLayoutWidget_15->setGeometry(QRect(10, 160, 511, 61));
        horizontalLayout_15 = new QHBoxLayout(horizontalLayoutWidget_15);
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        horizontalLayout_15->setContentsMargins(0, 0, 0, 0);
        verticalLayout_32 = new QVBoxLayout();
        verticalLayout_32->setObjectName(QString::fromUtf8("verticalLayout_32"));
        label_56 = new QLabel(horizontalLayoutWidget_15);
        label_56->setObjectName(QString::fromUtf8("label_56"));

        verticalLayout_32->addWidget(label_56);

        label_59 = new QLabel(horizontalLayoutWidget_15);
        label_59->setObjectName(QString::fromUtf8("label_59"));

        verticalLayout_32->addWidget(label_59);


        horizontalLayout_15->addLayout(verticalLayout_32);

        verticalLayout_33 = new QVBoxLayout();
        verticalLayout_33->setObjectName(QString::fromUtf8("verticalLayout_33"));
        spinMesh_edge_width = new QDoubleSpinBox(horizontalLayoutWidget_15);
        spinMesh_edge_width->setObjectName(QString::fromUtf8("spinMesh_edge_width"));
        spinMesh_edge_width->setDecimals(6);
        spinMesh_edge_width->setMinimum(0.000000000000000);
        spinMesh_edge_width->setMaximum(100000.000000000000000);
        spinMesh_edge_width->setSingleStep(0.100000000000000);
        spinMesh_edge_width->setValue(0.500000000000000);

        verticalLayout_33->addWidget(spinMesh_edge_width);

        spinVector_ratio = new QDoubleSpinBox(horizontalLayoutWidget_15);
        spinVector_ratio->setObjectName(QString::fromUtf8("spinVector_ratio"));
        spinVector_ratio->setDecimals(6);
        spinVector_ratio->setMaximum(1000000.000000000000000);
        spinVector_ratio->setSingleStep(0.100000000000000);
        spinVector_ratio->setValue(0.100000000000000);

        verticalLayout_33->addWidget(spinVector_ratio);


        horizontalLayout_15->addLayout(verticalLayout_33);

        label_48 = new QLabel(tab_view);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        label_48->setGeometry(QRect(220, 130, 181, 20));
        label_48->setFont(font);
        tabWidget_options->addTab(tab_view, QString());
        dockWidget_options->setWidget(dockWidgetContents_3);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget_options);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 2000, 22));
        menuData = new QMenu(menubar);
        menuData->setObjectName(QString::fromUtf8("menuData"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuFunction = new QMenu(menubar);
        menuFunction->setObjectName(QString::fromUtf8("menuFunction"));
        menuWindow = new QMenu(menubar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuData->menuAction());
        menubar->addAction(menuFunction->menuAction());
        menubar->addAction(menuView->menuAction());
        menubar->addAction(menuWindow->menuAction());
        menuData->addAction(actionLoad);
        menuData->addSeparator();
        menuData->addAction(actionClear_data);
        menuData->addSeparator();
        menuData->addAction(actionQuit);
        menuView->addAction(actionView_mesh_facets);
        menuView->addAction(actionView_mesh_edges);
        menuView->addAction(actionView_mesh_normals);
        menuView->addSeparator();
        menuView->addAction(actionSave_camera);
        menuView->addAction(actionLoad_camera);

        retranslateUi(MainWindow);

        tabWidget_options->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "3D Viewer", nullptr));
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
#if QT_CONFIG(shortcut)
        actionQuit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionLoad->setText(QCoreApplication::translate("MainWindow", "Load...", nullptr));
#if QT_CONFIG(statustip)
        actionLoad->setStatusTip(QCoreApplication::translate("MainWindow", "Load PSLG", nullptr));
#endif // QT_CONFIG(statustip)
#if QT_CONFIG(shortcut)
        actionLoad->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionView_mesh_edges->setText(QCoreApplication::translate("MainWindow", "Mesh Edges", nullptr));
#if QT_CONFIG(shortcut)
        actionView_mesh_edges->setShortcut(QCoreApplication::translate("MainWindow", "E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionView_mesh_normals->setText(QCoreApplication::translate("MainWindow", "Mesh Normals", nullptr));
        actionClear_data->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        actionMesh_options->setText(QCoreApplication::translate("MainWindow", "Options...", nullptr));
#if QT_CONFIG(shortcut)
        actionMesh_options->setShortcut(QCoreApplication::translate("MainWindow", "Shift+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionView_information_bar->setText(QCoreApplication::translate("MainWindow", "Information Bar", nullptr));
        actionView_options->setText(QCoreApplication::translate("MainWindow", "Options", nullptr));
        actionView_mesh_facets->setText(QCoreApplication::translate("MainWindow", "Mesh Facets", nullptr));
        actionSave_camera->setText(QCoreApplication::translate("MainWindow", "Save Camera", nullptr));
        actionLoad_camera->setText(QCoreApplication::translate("MainWindow", "Load Camera", nullptr));
        actiontest->setText(QCoreApplication::translate("MainWindow", "test", nullptr));
        dockWidget_infos->setWindowTitle(QCoreApplication::translate("MainWindow", "Information Bar", nullptr));
        dockWidget_options->setWindowTitle(QCoreApplication::translate("MainWindow", "Options", nullptr));
        tabWidget_options->setTabText(tabWidget_options->indexOf(tab_solver), QCoreApplication::translate("MainWindow", "Solver", nullptr));
        label_52->setText(QCoreApplication::translate("MainWindow", "Point Sizes", nullptr));
        label_53->setText(QCoreApplication::translate("MainWindow", "Input Point Size", nullptr));
        label_54->setText(QCoreApplication::translate("MainWindow", "Color Point Size", nullptr));
        label_56->setText(QCoreApplication::translate("MainWindow", "Mesh Edge Width", nullptr));
        label_59->setText(QCoreApplication::translate("MainWindow", "Vector Ratio", nullptr));
        label_48->setText(QCoreApplication::translate("MainWindow", "Segment Width", nullptr));
        tabWidget_options->setTabText(tabWidget_options->indexOf(tab_view), QCoreApplication::translate("MainWindow", "View", nullptr));
        menuData->setTitle(QCoreApplication::translate("MainWindow", "&Data", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "View", nullptr));
        menuFunction->setTitle(QCoreApplication::translate("MainWindow", "Function", nullptr));
        menuWindow->setTitle(QCoreApplication::translate("MainWindow", "Window", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAD2MESH_H
