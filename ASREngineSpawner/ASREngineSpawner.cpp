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
#include "ASREngineSpawner.h"
#include <QDateTime>
#include <QString>
#include <QTextEdit>
#include <QProcessEnvironment>
#include <QDir>

namespace 
{
    const int MSEC_APP_CHECK_TIMER =     2000;
    const int NORMAL_EXIT =              1;
    const QString APPLICATION_NAME =     "ASREngineApp.exe";
    const int MAX_RETRY =                10;
}

//-----------------------------------------------------------------------------
//
ASREngineSpawner::ASREngineSpawner(const QString pathToApplication, QWidget* pParent) : QDialog(pParent),
                                   m_pProcessManager(NULL),
                                   m_pTimer(NULL),
                                   m_currentRetry(0),
                                   m_needToClose(false)
{
    m_ui.setupUi(this);
    QString pathToVRX= "..\\..\\3rdParty\\vrx";
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    pathToVRX = env.value("VRX_HOME");
    logString("VRX_HOME = " + pathToVRX);
    logString("Complete path to application = [" + pathToApplication + APPLICATION_NAME + "]");
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    
    m_pProcessManager = new ProcessManager(pathToApplication, APPLICATION_NAME, NORMAL_EXIT);

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(onTimer()));
    connect(m_pProcessManager, SIGNAL(stateChanged(PROCESS_STATE)), this, SLOT(onStateChanged(PROCESS_STATE)));
    connect(m_pProcessManager, SIGNAL(outputText(const QString&)), this, SLOT(onProcessOutputText(const QString&)));
    
    
    onTimer(); //call it immediately, then start timer...
    m_pTimer->start(MSEC_APP_CHECK_TIMER);
    QWidget::showMinimized ();
}


//-----------------------------------------------------------------------------
//
ASREngineSpawner::~ASREngineSpawner()
{
    if (m_pProcessManager)
    {
        delete m_pProcessManager;
        m_pProcessManager= NULL;
    }
}

void ASREngineSpawner::onStateChanged(PROCESS_STATE newState)
{
    switch (newState)
    {
    case PROCESS_FAILED_TO_START:
        logString(APPLICATION_NAME + " failed to start"); //will be retried on timer...
        break;
    case PROCESS_STARTING:
        logString(APPLICATION_NAME + " starting");
        break;
    case PROCESS_STOPPED:
        logString(APPLICATION_NAME + " stopped, restarting...");
        m_pProcessManager->start();
        break;
    case PROCESS_RUNNING:
        logString(APPLICATION_NAME + " Started...");
        break;
    case PROCESS_NOT_STARTED: //do nothing, will be started on timer...
        break;
    }
    
}

void ASREngineSpawner::onTimer()
{
    if (m_needToClose)
    {
        close();
    }
    switch (m_pProcessManager->getProcessState())
    {
    case PROCESS_FAILED_TO_START:
        if (m_currentRetry < MAX_RETRY)
        {
            m_currentRetry++;
            logString("retry #" + QString::number(m_currentRetry));
            m_pProcessManager->start();
        }
        else
        {
            logString("Unable to start " + APPLICATION_NAME + " closing...");
            m_needToClose= true;
        }
        break;
    case PROCESS_STARTING:
        break;
    case PROCESS_STOPPED:
        break;
    case PROCESS_RUNNING:
        m_currentRetry= 0;
        break;
    case PROCESS_NOT_STARTED:
        m_pProcessManager->start();
        break;
    }
}

//-----------------------------------------------------------------------------
//
void ASREngineSpawner::onProcessOutputText(const QString& text)
{
    m_ui.textEdit->setTextColor(Qt::blue);
    m_ui.textEdit->append(text);
    m_ui.textEdit->setTextColor(Qt::black);
}

//-----------------------------------------------------------------------------
//
void ASREngineSpawner::logString(const QString& text)
{
    QString textLine = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss : ") + text;
    m_ui.textEdit->append(textLine);
}
