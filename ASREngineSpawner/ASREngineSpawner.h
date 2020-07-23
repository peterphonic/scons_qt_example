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

#include "ui_ASREngineWidget.h"
#include "ProcessManager.h"

//---------------------------------------------------------------------------
/*!
  @class ASREngineSpawner
  @brief This class handles the start and/or restart of the ASREngineApp
*///-------------------------------------------------------------------------
class ASREngineSpawner : public QDialog
{
    Q_OBJECT

public:
    ASREngineSpawner(const QString pathToApplication, QWidget* pParent= NULL);
    virtual ~ASREngineSpawner(void);

private Q_SLOTS:
    //------------------------------------------------------------------------------
    /*!
        @brief notification that timer timeout was triggered.
    *///----------------------------------------------------------------------------
    void onTimer();

    //------------------------------------------------------------------------------
    /*!
        @brief notification that the process state changed
        @param[in] text : new state.
    *///----------------------------------------------------------------------------
    void onStateChanged(PROCESS_STATE newState);

    //------------------------------------------------------------------------------
    /*!
        @brief notification that the process is outputting text
        @param[in] text
    *///----------------------------------------------------------------------------
    void onProcessOutputText(const QString& text);

private:
    //------------------------------------------------------------------------------
    /*!
        @brief start the application
    *///----------------------------------------------------------------------------
    void startApplication();

    //------------------------------------------------------------------------------
    /*!
        @brief append string to window (will add date and time)
        @param[in] text : the string content to add.
    *///----------------------------------------------------------------------------
    void logString(const QString& text);

    Ui::ASREngineSpawnerWindow m_ui;
    ProcessManager* m_pProcessManager;
    PROCESS_STATE m_currentProcessState;
    QTimer* m_pTimer;
    int m_currentRetry;
    bool m_needToClose;
};

