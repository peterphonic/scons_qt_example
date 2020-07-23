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
#pragma once
#include <QProcess>
#include <QTimer>


enum PROCESS_STATE {
    PROCESS_NOT_STARTED= 0,
    PROCESS_STARTING,
    PROCESS_FAILED_TO_START,
    PROCESS_RUNNING,
    PROCESS_STOPPED
};

//---------------------------------------------------------------------------
/*!
  @class ASREngineSpawner
  @brief This class handles the process related stuff.
*///-------------------------------------------------------------------------
class ProcessManager : public QObject
{
    Q_OBJECT
public:
    ProcessManager(const QString& pathToApp, const QString& appName, int normalExitCode);
    virtual ~ProcessManager(void);

    //------------------------------------------------------------------------------
    /*!
        @brief start application
    *///----------------------------------------------------------------------------
    void start();

    //------------------------------------------------------------------------------
    /*!
        @brief get the current process state
    *///----------------------------------------------------------------------------
    PROCESS_STATE getProcessState() { return m_processState; }

Q_SIGNALS:
    //------------------------------------------------------------------------------
    /*!
        @brief notification that the process state changed
        @param[in] process state
    *///----------------------------------------------------------------------------
    void stateChanged(const PROCESS_STATE);

    //------------------------------------------------------------------------------
    /*!
        @brief send notification that output text was read.
        @param[in] output text
    *///----------------------------------------------------------------------------
    void outputText(const QString& outText);

private Q_SLOTS:
    //------------------------------------------------------------------------------
    /*!
        @brief receive notification when error happens
        @param[in] process error
    *///----------------------------------------------------------------------------
    void onError(QProcess::ProcessError error);

    //------------------------------------------------------------------------------
    /*!
        @brief receive notification when process is started
    *///----------------------------------------------------------------------------
    void onStarted();

    //------------------------------------------------------------------------------
    /*!
        @brief receive notification when process is finished
        @param[in] exit code
        @param[in] exit status
    *///----------------------------------------------------------------------------
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

    //------------------------------------------------------------------------------
    /*!
        @brief receive notification when ready to read standard input
    *///----------------------------------------------------------------------------
    void onReadyToReadStandardInput();
private:

    //------------------------------------------------------------------------------
    /*!
        @brief change current process state
        @param[in] new state
    *///----------------------------------------------------------------------------
    void setState(PROCESS_STATE newState);


    QTimer*  m_pTimer;
    QProcess m_process;
    
    QString m_applicationName;
    QString m_applicationPath;
    int     m_msecCheckTime;
    int     m_normalExitCode;
    PROCESS_STATE m_processState;
};

