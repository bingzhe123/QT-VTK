/*----------------------------------------------------------------------------
   Component       : IPS - View
   Name            :
   Author          :
   Creation Date   : 16. MAY 2021
   Description     : ��ͼ��������ʱ��
   Copy Right      : 2021 All Rights Reserved
-----------------------------------------------------------------------------*/
#pragma once

#include <QRadioButton>
#include <QPainter>
#include <QBrush>
#include <QtWidgets/QWidget>
#include <QButtonGroup>
#include <QVector>

#include "ui_IPSToolsBar.h"
#include "BaseView.h"
#include "gespips.h"
#include "Cell3D.h"
#include "SliceCell.h"
#include "DRCell.h"

class IPSToolsBar : public BaseView
{
    Q_OBJECT
signals:
	void ChangeNegtiveStatus();
	void High();
	void Low();
	void Color();
	void Gray();
	void Edge();
	void Organic();
	void IC();
	
public:
    IPSToolsBar(std::shared_ptr< Container> spContainer, QWidget* parent = Q_NULLPTR);
	IPSToolsBar(std::shared_ptr<Container> spContainer,  Cell3D * a,
		DRCell *celldr,SliceCell *cellSlice, DataModule *pDataModule, QWidget * parent = Q_NULLPTR);
	QRadioButton *radio[9];
	QPushButton*pushButton[2];
	QCheckBox *checkBox[8];
	QButtonGroup *group[2];
	void InitImageString();
	QVector <QString> imagestring;
	QVector <QString> tipstring;
	void SetEnable();
	void SetDisable();
	/*-----------------------------------------------------------
	***   Description:���òü���ť״̬
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
    void SetClippingState(bool bClipping);
	/*-----------------------------------------------------------
	***   Description:���ò���״̬
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void SetMeasureState(bool bMeasure);

	/*-----------------------------------------------------------
	***   Description:���õ�ǰ����ͼ״̬
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void SetViewState(ViewDirection viewDirection);
	void EdgeDo();
	void OrgDo();
	void ICDo();
private slots:

    void OnChecked(bool bChecked);


	/*-----------------------------------------------------------
	***   Description:���α���㷨
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void OnColor();

	/*-----------------------------------------------------------
	***   Description:����Ҷ��㷨
	***   OUTPUT     :
	***   INPUT      :
	------------------------------------------------------------*/
	void OnGray();

private:
	bool toolsout;
	bool widgetinit;
    Ui::IPSToolsBarClass ui;
	
	void HideToolsBar();
	void EdgeEnhence();
	void AdjustDesity();

	/*-----------------------------------------------------------
	***   Description : �л����޳�
	***   OUTPUT      :
	***   INPUT       :
	------------------------------------------------------------*/
	void RemoveOrg();

	/*-----------------------------------------------------------
	***   Description : �޻����޳�
	***   OUTPUT      :
	***   INPUT       :
	------------------------------------------------------------*/
	void RemoveInorg();

	/*-----------------------------------------------------------
	***   Description : ���Ӷ���������
	***   OUTPUT      :
	***   INPUT       :
	------------------------------------------------------------*/
	void IncreaseMultiAbsorb();

	/*-----------------------------------------------------------
	***   Description : ���Ͷ���������
	***   OUTPUT      :
	***   INPUT       :
	------------------------------------------------------------*/
	void DecreaseMultiAbsorb();

	void Brightness();
	void ContrastControl();
	void NegativeProcess();
	bool shown;
	Cell3D *m_p3DCell;
	DRCell *m_pDRCell;
	SliceCell* m_pSliceCell;
	DataModule *m_pDataModule = nullptr;

protected:
	void paintEvent(QPaintEvent*event);

};
