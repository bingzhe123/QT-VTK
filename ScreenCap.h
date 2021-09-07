#pragma once

#include "QObject.h"

#include"avilib.h"

#include <QSize>
#include <QThread>
#include <QByteArray>

#include "QTimer.h"

class RecordThread : public QThread
{
	Q_OBJECT
public:
	RecordThread(QObject* parent = NULL);

public:
	void setWidgetSize(const QSize& size);
	void stopThread();

private slots:
	void slotRcvData(const QByteArray& data);

protected:
	void run();

private:
	bool                m_bStoped;
	QByteArray          m_pixByteArray;
	QSize               m_size;
};


class ScreenCap : public QObject
{
	Q_OBJECT

public:
	ScreenCap();
	~ScreenCap();

	void StartCap();

	void EndCap();

signals:
	void sigSendPixData(const QByteArray& data);

private slots:
	void shotScreen();

private:
	RecordThread* m_pRecordThread;
	int            m_iRecordTimer;
	int count;
	QTimer* time;
};

