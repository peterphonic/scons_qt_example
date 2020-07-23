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
#include "ProcessManager.h"
#include <QDir>
#include <QDebug>

//-----------------------------------------------------------------------------
//
ProcessManager::ProcessManager(const QString& pathToApp, const QString& appName, int normalExitCode) : m_applicationName(appName),
                                                                                                       m_applicationPath(pathToApp),
                                                                                                       m_normalExitCode(normalExitCode),
                                                                                                       m_processState(PROCESS_NOT_STARTED)
{
    connect(&m_process, SIGNAL(error(QProcess::ProcessError)), SLOT(onError(QProcess::ProcessError)));
    connect(&m_process, SIGNAL(started()), SLOT(onStarted()));
    connect(&m_process, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(onFinished(int, QProcess::ExitStatus)));

    connect (&m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyToReadStandardInput()));
    connect (&m_process, SIGNAL(readyReadStandardError()), this, SLOT(onReadyToReadStandardInput()));
}

//-----------------------------------------------------------------------------
//
ProcessManager::~ProcessManager(void)
{
}

//-----------------------------------------------------------------------------
//
void ProcessManager::start()
{
    setState(PROCESS_STARTING);
    if (m_applicationPath.isEmpty() == false)
    {
        QDir::setCurrent(m_applicationPath);
    }
    m_process.start(m_applicationName);
}

//-----------------------------------------------------------------------------
//
void ProcessManager::setState(PROCESS_STATE newState)
{
    m_processState= newState;
    Q_EMIT(stateChanged(m_processState));
}

//-----------------------------------------------------------------------------
//
void ProcessManager::onStarted()
{
    setState(PROCESS_RUNNING);
}

//-----------------------------------------------------------------------------
//
void ProcessManager::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode == m_normalExitCode)
    {
        setState(PROCESS_STOPPED);
    }
}

//-----------------------------------------------------------------------------
//
void ProcessManager::onError(QProcess::ProcessError error)
{
    switch (error)
    {
    case QProcess::FailedToStart:
        setState(PROCESS_FAILED_TO_START);
        break;
    case QProcess::Crashed:
        setState(PROCESS_STOPPED);
        break;
    case QProcess::Timedout:
        setState(PROCESS_STOPPED);
        break;
    case QProcess::WriteError:
        setState(PROCESS_STOPPED);
        break;
    case QProcess::UnknownError:
        setState(PROCESS_STOPPED);
        break;
    }
}

//-----------------------------------------------------------------------------
//
void ProcessManager::onReadyToReadStandardInput()
{
    QString outText = m_process.readAllStandardOutput();
    Q_EMIT(outputText(outText));
}