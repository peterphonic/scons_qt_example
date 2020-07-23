//*****************************************************************************
//
// Copyright 2015 by Adacel Inc.
//
// Possession and use of this software must conform strictly to the license
// agreement between licensee and Adacel Inc.
//
// Any use of this software contrary to the above mentioned license agreement
// is thus strictly prohibited by Adacel Inc.
// as it is the sole owner of all copyrights pertaining to this software.
//
//*****************************************************************************
#include <QApplication>
#include <QSharedMemory>
#include <QString>
#include "ASREngineSpawner.h"
#include <windows.h>

int main(int argc, char *argv[])
{
    QSharedMemory sharedMemory;
    sharedMemory.setKey("ASREngineApp");
    if (!sharedMemory.create(1))
    {
        return 0;
    }
    QApplication app(argc, argv);
    QString pathToApplication;
    if (argc > 1)
    {
        pathToApplication= argv[1];
    }
    FreeConsole(); //hide console window
    ASREngineSpawner spawner(pathToApplication);
    spawner.show();

    app.setQuitOnLastWindowClosed(true);
    return app.exec();
}
