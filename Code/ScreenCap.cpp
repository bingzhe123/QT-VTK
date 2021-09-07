
#include "ScreenCap.h"
#include "QScreen.h"
#include "QPixmap.h"
#include "QBuffer.h"
#include "qguiapplication.h"
#include "commondef.h"
#include <QApplication>
#include <QDebug>
#include <QDateTime>
extern QString ScreenCapPath;
ScreenCap::ScreenCap()
{
	m_pRecordThread = NULL;
	m_iRecordTimer = 0;
}

ScreenCap::~ScreenCap()
{
}

void ScreenCap::StartCap()
{
	m_pRecordThread = new RecordThread();

	QScreen* screen = QGuiApplication::primaryScreen();
	m_pRecordThread->setWidgetSize(screen->size());
	bool bOk = connect(this, SIGNAL(sigSendPixData(const QByteArray&)),
		m_pRecordThread, SLOT(slotRcvData(const QByteArray&)));
	Q_ASSERT(bOk);

	m_pRecordThread->start();

	time = new QTimer();
	connect(time, SIGNAL(timeout()), this, SLOT(shotScreen()));

	time->start(50);
}

void ScreenCap::shotScreen()
{
	QScreen* screen = QGuiApplication::primaryScreen();
	QPixmap pix = screen->grabWindow(0);
    //pix.save("1.jpg");
	QByteArray pixData;
	pixData.clear();

	QBuffer buffer(&pixData);
	bool bOk = pix.save(&buffer, "jpg");

	if (!pixData.isEmpty() && bOk)
	{
		emit sigSendPixData(pixData);
	}
}

void ScreenCap::EndCap()
{
	m_pRecordThread->stopThread();
	m_pRecordThread = NULL;
	time->stop();
	m_iRecordTimer = 0;
}

RecordThread::RecordThread(QObject *parent)
	:QThread(parent)
{
	m_bStoped = false;
	m_pixByteArray.clear();

	/*
avi_t* AVI_open_output_file(char * filename);
void AVI_set_video(avi_t *AVI, int width, int height, double fps, char *compressor);
void AVI_set_audio(avi_t *AVI, int channels, long rate, int bits, int format, long mp3rate);
int  AVI_write_frame(avi_t *AVI, char *data, long bytes, int keyframe);
*/
}

void RecordThread::setWidgetSize(const QSize &size)
{
	m_size = size;
}

void RecordThread::stopThread()
{
	m_bStoped = true;
}

void RecordThread::slotRcvData(const QByteArray &data)
{
	if (data.isEmpty())
	{
		return;
	}

	m_pixByteArray = data;
}

void RecordThread::run()
{
	QDateTime datetime = QDateTime::currentDateTime();
	QString strAppDir = ScreenCapPath +'/'+ datetime.toString("yyMMddhhmmss") +".avi";

	avi_t *FD = AVI_open_output_file(strAppDir.toLocal8Bit().data());
	AVI_set_video(FD, m_size.width(), m_size.height(), 20, QByteArray("MJPG").data());

	while (!m_bStoped)
	{
		if (!m_pixByteArray.isEmpty())
		{
			int iRet = AVI_write_frame(FD, m_pixByteArray.data(), m_pixByteArray.length(), 1);
			qDebug() << iRet;
			m_pixByteArray.clear();
		}
	}
	AVI_close(FD);

	this->exec();
	this->deleteLater();
}
