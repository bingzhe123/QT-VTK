#include "ManageCheckViewWidget.h"

#include "QFileDialog.h"
#include "QDateTime.h"




extern QString userID;
extern std::queue<int> commandQueue;
extern QMutex commandQueueMutex;
extern std::queue<std::string> commandContainer;
extern QMutex commandContainerMutex;





ManageCheckViewWidget::ManageCheckViewWidget(std::shared_ptr< Container> spContainer, QWidget *parent)
	: BaseView(spContainer, parent)
{
    ui.setupUi(this);
	exportButton = ui._export;
	deleteButton = ui._delete;
	lockButton = ui.lock;
	vertiRadio = ui.radio_verti;
	horiRadio = ui.radio_hori;
	printButton = ui.print;
	QPrinter print;
	

	vertiRadio->setChecked(true);
	connect(exportButton, &QPushButton::clicked, this, &ManageCheckViewWidget::exportImage);
	connect(lockButton, &QPushButton::clicked, this, &ManageCheckViewWidget::LockUnlock);
	connect(deleteButton, &QPushButton::clicked, this, &ManageCheckViewWidget::DeleteImage);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "ManageCheckViewWidget::ManageCheckViewWidget");
	connect(printButton, &QPushButton::clicked, this, &ManageCheckViewWidget::Print);
	

	
}

/*-----------------------------------------------------------
***   Description : ��Դ�ļ��п���ͼ��Ŀ���ļ��У�Ŀ���ļ����Ե�ǰʱ�䴴��
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
bool copyDirectory(const QString& srcPath, const QString& dstPath, bool coverFileIfExist)
{
	QDateTime current_date_time = QDateTime::currentDateTime();
	QString current_time = current_date_time.toString("yyyy_MM_dd_hh_mm_ss");

	QString tempPath(dstPath);
	tempPath.append("//");
	tempPath.append(current_time);

	QDir srcDir(srcPath);
	QDir dstDir(tempPath);
	if (!dstDir.exists()) { //Ŀ���ļ�Ŀ¼�������򴴽��ļ�Ŀ¼
		if (!dstDir.mkdir(dstDir.absolutePath()))
			return false;
	}
	QFileInfoList fileInfoList = srcDir.entryInfoList();
	foreach(QFileInfo fileInfo, fileInfoList) {
		if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
			continue;

		if (fileInfo.isDir()) {    // ��ΪĿ¼ʱ���ݹ�Ľ���copy 
			if (!copyDirectory(fileInfo.filePath(), dstDir.filePath(fileInfo.fileName()), coverFileIfExist))
				return false;
		}
		else {            //�������ǲ���ʱ�������ļ�����ɾ������ 
			if (coverFileIfExist && dstDir.exists(fileInfo.fileName())) {
				dstDir.remove(fileInfo.fileName());
			}
			/// �����ļ�copy
			if (!QFile::copy(fileInfo.filePath(), dstDir.filePath(fileInfo.fileName()))) {
				return false;
			}
		}
	}
	return true;
}

/*-----------------------------------------------------------
***   Description : �������ص�ͼ���ļ�
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void ManageCheckViewWidget::exportImage()
{
	QString dest_path = QFileDialog::getExistingDirectory(this, "��ѡ���ļ���·��...", "./");
	if (dest_path.isEmpty())
	{
		return;
	}
	else
	{
		QString src_path("..//code//test_data//test");
		if (copyDirectory(src_path,dest_path,true))
		{
			LOG_INFO(GESP::DebugLogger::GetInstance(), "ͼ�񱣴�ɹ�");
		}
		else
		{
			LOG_INFO(GESP::DebugLogger::GetInstance(), "ͼ�񱣴�ʧ��");
		}
	}
}

void ManageCheckViewWidget::ChangeLockStatus(int a)
{
	lock = a;
	if (a)
	{
		lockButton->setText(QString::fromLocal8Bit("����"));
		deleteButton->setEnabled(!true);
	}
	else
	{
		lockButton->setText(QString::fromLocal8Bit("����"));
		deleteButton->setEnabled(true);
	}
}

void ManageCheckViewWidget::LockUnlock()
{

	GESP::JsonObject lockdata;
	int lockop;
	if (lock)lockop = 0;
	else lockop = 1;
	lockdata.Set(ksID,sessionID.toStdString());
	lockdata.Set(ksLock, lockop);
	lockdata.Set(ksName, userID.toStdString());
	QMutexLocker locker(&commandQueueMutex);
	QMutexLocker locker2(&commandContainerMutex);
	commandQueue.push(12);
	commandContainer.push(lockdata.Stringify());
	QString lockstring = "lock image";
	if (lock)lockstring = "unlock image";
	QString logstring = "lock user:" + userID+"   sessionID:"+sessionID+lockstring;
	LOG_INFO(GESP::DebugLogger::GetInstance(), logstring.toStdString());
	//QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}

void ManageCheckViewWidget::ChangeID(QString a )
{
	sessionID = a;
}

void ManageCheckViewWidget::DeleteImage()
{
	GESP::JsonObject deletedata;
	deletedata.Set(ksID, sessionID.toStdString());
	deletedata.Set(ksName, userID.toStdString());
	QMutexLocker locker(&commandQueueMutex);
	QMutexLocker locker2(&commandContainerMutex);
	commandQueue.push(11);
	commandContainer.push(deletedata.Stringify());
	QString deletestring = "delete image";

	QString logstring = " user:" + userID + "   sessionID:" + sessionID + deletestring;
	emit DeleteOne();
	LOG_INFO(GESP::DebugLogger::GetInstance(), logstring.toStdString());
}


void ManageCheckViewWidget::PrintPaint(QPrinter *printer)
{
	QPainter painter(printer);


	//����ҳ���С
	printer->setPageSize(QPagedPaintDevice::A4);
	QRect pageSize = painter.viewport();
	int width = (double)(pageSize.width()*widthside);
	int height =(double)(pageSize.height()*heightside);

	int dx =(0.3028) / 2 * pageSize.width();
	int dy = (0.171) / 2 * pageSize.height();
	//����ҳ�߾�
	QPagedPaintDevice::Margins marg;
	marg.left = 0;
	marg.right = 0;
	marg.top = 0;
	marg.bottom = 0;
	printer->setMargins(marg);

	

	//����ҳ����Ⱦģʽ
	painter.setRenderHint(QPainter::Antialiasing, true);
	printer->setOrientation(QPrinter::Portrait);


	//���û�ˢ��ɫ
	//painter.setBrush(Qt::white);


	//����ɫ����ɫ
	painter.fillRect(pageSize, Qt::white);
	//painter.drawRect(QRect(dx,0,width,height));


	//���������
	//���û�����ɫ,���
	painter.setPen(QPen(Qt::black, 2));
	QVector<QLine> tableline;
	tableline.append(QLine(QPoint(dx, dy), QPoint(dx, height+dy)));
	tableline.append(QLine(QPoint(dx, height+dy), QPoint(width+dx, height+dy)));
	tableline.append(QLine(QPoint(dx, dy), QPoint(width+dx, dy)));
	tableline.append(QLine(QPoint(width+dx, dy), QPoint(width+dx, height+dy)));
	tableline.append(QLine(QPoint(dx, dy+(double)width / 16 * 9), QPoint(width+dx, dy+(double)width / 16 * 9)));
	painter.drawLines(tableline);
	QFont font;
	font.setPointSize(13);
	font.setFamily("����");
	font.setBold(true);
	painter.setFont(font);
	painter.drawText(2*dx, dy+(double)width / 16 * 9 + 100, width*0.5, 100, Qt::AlignLeft, QString::fromLocal8Bit("��  ��  ID : ") + sessionID);
	painter.drawText(2*dx, dy+(double)width / 16 * 9 + 200, width*0.5, 100, Qt::AlignLeft, QString::fromLocal8Bit("��  ͼ  Ա : ") + userID);
	painter.drawText(2*dx, dy+(double)width / 16 * 9 + 300, width*0.5, 100, Qt::AlignLeft, QString::fromLocal8Bit("��      �� : ") + QDateTime().currentDateTime().toString("yyyy MM dd"));
	painter.drawText(2*dx, dy+(double)width / 16 * 9 + 400, width*0.5, 100, Qt::AlignLeft, QString::fromLocal8Bit("ʱ      �� : ") + QTime().currentTime().toString("hh:mm:ss"));
	painter.drawText(2*dx, dy+(double)width / 16 * 9 + 500, width*0.5, 100, Qt::AlignLeft, QString::fromLocal8Bit("��  ͼ  Ա : ") + sessionID);
	painter.drawText(2*dx, dy+(double)width / 16 * 9 + 600, width*0.5, 100, Qt::AlignLeft, QString::fromLocal8Bit("��  ͼ  Ա : ") + sessionID);
	painter.drawText(2*dx, dy+(double)width / 16 * 9 + 700, width*0.5, 100, Qt::AlignLeft, QString::fromLocal8Bit("��  ͼ  Ա : ") + sessionID);
	QScreen* screen = QGuiApplication::primaryScreen();
	QPixmap pix = screen->grabWindow(0);
	pix = pix.scaled(QSize(width-2*dx, height / 16 * 9-2*dy),Qt::KeepAspectRatio);
	painter.drawImage(dx+dx,dy+dy,pix.toImage());
	painter.end();
}

void ManageCheckViewWidget::Print()
{
	connect(&printDialog, &QPrintPreviewDialog::paintRequested, this, &ManageCheckViewWidget::PrintPaint);
	printDialog.setFixedSize(2000, 1000);
	printDialog.exec();
}