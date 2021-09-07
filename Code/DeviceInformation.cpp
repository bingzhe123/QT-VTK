#include "DeviceInformation.h"

static const QString GetSystemInformation()
{
	QString info = QSysInfo::productType();
	info += QSysInfo::productVersion();
	return info;
}

DeviceInformation::DeviceInformation(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	QSettings iniFile("./IPS.ini", QSettings::IniFormat);
	iniFile.setIniCodec("utf-8");
	iniFile.childGroups();
	iniFile.beginGroup("MachineInfomation");
	ui.labelDeviceType->setText(iniFile.value("DeviceType").toString());
	ui.labelID->setText(iniFile.value("DeviceID").toString());
	iniFile.endGroup();

	ui.labelSoftSys->setText(GetSystemInformation());

	iniFile.beginGroup("VersionInfomation");
	ui.labelSoftVersion->setText(iniFile.value("SoftwareVersion").toString());
	ui.labelHardSN->setText(iniFile.value("HardwareSN").toString());
	iniFile.endGroup();

	iniFile.beginGroup("ManufacturerInfomation");
	ui.labelManufacturer->setText(iniFile.value("Manufacturer").toString());
	ui.labelManufacturerAddr->setText(iniFile.value("ManufacturerAddress").toString());
	iniFile.endGroup();

	iniFile.beginGroup("DispositionInfomation");
	ui.labelDispositionAddr->setText(iniFile.value("DispositionAddress").toString());
	iniFile.endGroup();
}