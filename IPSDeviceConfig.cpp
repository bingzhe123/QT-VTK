#include "IPSDeviceConfig.h"
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include "gespips.h"
extern QString ScreenCapPath, ScreenShotPath;
IPSDeviceConfig::IPSDeviceConfig(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	ui.label_shot->setText(ScreenShotPath);
	ui.label_cap->setText(ScreenCapPath);
	connect(ui.pushButtoncap, &QPushButton::clicked, this, &IPSDeviceConfig::GetScreenCapDir);
	connect(ui.pushButtonshot, &QPushButton::clicked, this, &IPSDeviceConfig::GetScreenShotDir);
}
void IPSDeviceConfig::GetScreenShotDir()
{
	QString pathname = GetDir(QString::fromLocal8Bit("Ñ¡Ôñ½ØÍ¼±£´æÂ·¾¶"), ScreenShotPath);
	if (pathname.size() > 0)
	{
		ScreenShotPath = pathname;
		QSettings iniFile("./IPS.ini", QSettings::IniFormat);
		iniFile.setIniCodec("utf-8");

		iniFile.beginGroup("SaveConfig");
		iniFile.setValue("ScreenShotPath", ScreenShotPath);
		iniFile.endGroup();
		ui.label_shot->setText(ScreenShotPath);
		LOG_INFO(GESP::DebugLogger::GetInstance(), "Change ScreenShot direction Successed");
	}
}
void IPSDeviceConfig::GetScreenCapDir()
{

	QString pathname = GetDir(QString::fromLocal8Bit("Ñ¡ÔñÂ¼ÆÁ±£´æÂ·¾¶"), ScreenCapPath);
	if (pathname.size() > 0)
	{
		ScreenCapPath = pathname;
		QSettings iniFile("./IPS.ini", QSettings::IniFormat);
		iniFile.setIniCodec("utf-8");

		iniFile.beginGroup("SaveConfig");
		iniFile.setValue("ScreenCapPath", ScreenCapPath);
		iniFile.endGroup();
		ui.label_cap->setText(ScreenCapPath);
		LOG_INFO(GESP::DebugLogger::GetInstance(), "Change ScreenCap direction Successed");
	}

}
QString IPSDeviceConfig::GetDir(QString tips,QString dir)
{

	return QString(QFileDialog::getExistingDirectory(this, tips, dir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}