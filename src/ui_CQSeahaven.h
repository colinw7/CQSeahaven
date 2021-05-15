/********************************************************************************
** Form generated from reading UI file 'CQSeahaven.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CQSEAHAVEN_H
#define UI_CQSEAHAVEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CQSeahaven
{
public:
    QAction *action_Quit;
    QAction *action_Solve;
    QAction *action_Help;
    QAction *action_New_Game;
    QWidget *centralwidget;
    QVBoxLayout *vboxLayout;
    QWidget *canvas;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menu_Move;
    QMenu *menu_Help;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CQSeahaven)
    {
        if (CQSeahaven->objectName().isEmpty())
            CQSeahaven->setObjectName(QString::fromUtf8("CQSeahaven"));
        CQSeahaven->resize(800, 600);
        action_Quit = new QAction(CQSeahaven);
        action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
        action_Solve = new QAction(CQSeahaven);
        action_Solve->setObjectName(QString::fromUtf8("action_Solve"));
        action_Help = new QAction(CQSeahaven);
        action_Help->setObjectName(QString::fromUtf8("action_Help"));
        action_New_Game = new QAction(CQSeahaven);
        action_New_Game->setObjectName(QString::fromUtf8("action_New_Game"));
        centralwidget = new QWidget(CQSeahaven);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        vboxLayout = new QVBoxLayout(centralwidget);
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        canvas = new QWidget(centralwidget);
        canvas->setObjectName(QString::fromUtf8("canvas"));

        vboxLayout->addWidget(canvas);

        CQSeahaven->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CQSeahaven);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 33));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menu_Move = new QMenu(menubar);
        menu_Move->setObjectName(QString::fromUtf8("menu_Move"));
        menu_Help = new QMenu(menubar);
        menu_Help->setObjectName(QString::fromUtf8("menu_Help"));
        CQSeahaven->setMenuBar(menubar);
        statusbar = new QStatusBar(CQSeahaven);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        CQSeahaven->setStatusBar(statusbar);

        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menu_Move->menuAction());
        menubar->addAction(menu_Help->menuAction());
        menu_File->addAction(action_New_Game);
        menu_File->addSeparator();
        menu_File->addAction(action_Quit);
        menu_Move->addAction(action_Solve);
        menu_Help->addAction(action_Help);

        retranslateUi(CQSeahaven);
        QObject::connect(action_Quit, SIGNAL(triggered()), CQSeahaven, SLOT(close()));

        QMetaObject::connectSlotsByName(CQSeahaven);
    } // setupUi

    void retranslateUi(QMainWindow *CQSeahaven)
    {
        CQSeahaven->setWindowTitle(QApplication::translate("CQSeahaven", "MainWindow", nullptr));
        action_Quit->setText(QApplication::translate("CQSeahaven", "&Quit", nullptr));
        action_Solve->setText(QApplication::translate("CQSeahaven", "&Solve", nullptr));
        action_Solve->setIconText(QApplication::translate("CQSeahaven", "Solve", nullptr));
#ifndef QT_NO_TOOLTIP
        action_Solve->setToolTip(QApplication::translate("CQSeahaven", "Solve", nullptr));
#endif // QT_NO_TOOLTIP
        action_Help->setText(QApplication::translate("CQSeahaven", "&Help", nullptr));
        action_New_Game->setText(QApplication::translate("CQSeahaven", "&New Game", nullptr));
        menu_File->setTitle(QApplication::translate("CQSeahaven", "&File", nullptr));
        menu_Move->setTitle(QApplication::translate("CQSeahaven", "&Move", nullptr));
        menu_Help->setTitle(QApplication::translate("CQSeahaven", "&Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CQSeahaven: public Ui_CQSeahaven {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CQSEAHAVEN_H
