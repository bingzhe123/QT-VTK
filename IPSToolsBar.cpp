#include "IPSToolsBar.h"
#include "IPSImageAlgo.h"
#include "Cell2D.h"
#include "commondef.h"

extern int MachineMode;
/*-----------------------------------------------------------
***   Description:     工具栏构造
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
IPSToolsBar::IPSToolsBar(std::shared_ptr< Container> spContainer, QWidget* parent)
	: BaseView(spContainer, parent)
{}

IPSToolsBar::IPSToolsBar(std::shared_ptr< Container> spContainer,
	Cell3D *cell3d,
	DRCell *celldr,
	SliceCell *cellSlice,
	DataModule *pDataModule,
	QWidget* parent)
	: BaseView(spContainer, parent)
{
	InitImageString();
	m_p3DCell = cell3d;
	m_pDRCell = celldr;
	m_pSliceCell = cellSlice;
	m_pDataModule = pDataModule;
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSToolsBar::TPSToolsBar");
	
#pragma region 初始化控件

	radio[0] = ui.rcolor;
	radio[1] = ui.rblack;
	radio[2] = ui.rhigh;
	radio[3] = ui.rlow;
	radio[4] = ui.rresume;
	radio[5] = ui.rscale;
	radio[6] = ui.rtop;
	radio[7] = ui.rleft;
	radio[8] = ui.rright;

	checkBox[0] = ui.rneg;
	checkBox[1] = ui.ror;
	checkBox[2] = ui.ric;
	checkBox[3] = ui.ren;
	checkBox[4] = ui.rcut;
	checkBox[5] = ui.rrec;
	checkBox[6] = ui.rmeasure;
	checkBox[7] = ui.rsus;

	pushButton[0] = ui.rde;
	pushButton[1] = ui.rin;

	for (int i = 0; i < 19; i++)
	{
        if (i<9)
        {        
			radio[i]->setStyleSheet((const QString)QString("QRadioButton::indicator{image:url(:/img/res/toolbar/" + imagestring[i * 3] + ");}"
				"QRadioButton::indicator:hover{image:url(:/img/res/toolbar/" + imagestring[i * 3 + 2] + ");}"
				"QRadioButton::indicator:checked{image:url(:/img/res/toolbar/" + imagestring[i * 3 + 1] + ");}"));
			radio[i]->setCheckable(true);
        }
		else if (i < 17)
		{
			checkBox[i-9]->setStyleSheet((const QString)QString("QCheckBox::indicator{image:url(:/img/res/toolbar/" + imagestring[i * 3] + ");}"
				"QCheckBox::indicator:hover{image:url(:/img/res/toolbar/" + imagestring[i * 3 + 2] + ");}"
				"QCheckBox::indicator:checked{image:url(:/img/res/toolbar/" + imagestring[i * 3 + 1] + ");}"));
			checkBox[i-9]->setCheckable(true);
		}
		else
		{
			pushButton[i-17]->setStyleSheet((const QString)QString("QPushButton{border-image:url(:/img/res/toolbar/" + imagestring[i * 3] + ");}"
				"QPushButton::hover{border-image:url(:/img/res/toolbar/" + imagestring[i * 3 + 2] + ");}"
				"QPushButton::pressed{border-image:url(:/img/res/toolbar/" + imagestring[i * 3 + 1] + ");}"));
		}

	}     
#pragma endregion
#pragma region ButtonGroup
	group[0] = new QButtonGroup(this);
	group[0]->addButton(radio[0]);
	group[0]->addButton(radio[1]);
	group[0]->addButton(radio[2]);
	group[0]->addButton(radio[3]);
	group[1] = new QButtonGroup(this);
	group[1]->addButton(radio[4]);
	group[1]->addButton(radio[5]);
	group[1]->addButton(radio[6]);
	group[1]->addButton(radio[7]);
	group[1]->addButton(radio[8]);
	radio[0]->setChecked(true);
	radio[4]->setChecked(true);
	checkBox[7]->setChecked(false);
#pragma endregion
	if (widgetinit == false)
	{
		ui.pushButton->setStyleSheet("QPushButton{background:#642AB5 url(:/img/res/toolbar/fold.png) center no-repeat;border-radius:4px;}"
			"QPushButton:hover{background:#9254DE url(:/img/res/toolbar/fold.png) center no-repeat;border-radius:4px;}"
			"QPushButton:pressed{background:#391085 url(:/img/res/toolbar/fold.png) center no-repeat;border-radius:4px;}");
		widgetinit = true;
		toolsout = true;
	}

	for (int i = 0; i < 19; i++)
	{
		if (i < 9) radio[i]->setToolTip(tipstring[i]);
		else if (i < 17) checkBox[i-9]->setToolTip(tipstring[i]);
		else pushButton[i-17]->setToolTip(tipstring[i]);
	}

    

	setStyleSheet("QToolTip{font-size:14px;color:white;background:#484251;border-style: none;}");
#pragma region Connect
	connect(ui.pushButton, &QPushButton::clicked, this, &IPSToolsBar::HideToolsBar);

	//connect(radio[0], &QRadioButton::clicked, m_p3DCell, &Cell3D::Color3D);//伪彩
	//connect(radio[0], &QRadioButton::clicked, m_pDRCell, &DRCell::ShowDR);//伪彩
	//connect(radio[0], &QRadioButton::clicked, m_pSliceCell, &SliceCell::Page);//伪彩
	connect(radio[0], &QRadioButton::clicked, this, &IPSToolsBar::OnColor);//伪彩

	//connect(radio[1], &QRadioButton::clicked, m_p3DCell, &Cell3D::Gray3D);//灰度
	//connect(radio[1], &QRadioButton::clicked, m_pDRCell, &DRCell::ShowDRGray);//灰度
	//connect(radio[1], &QRadioButton::clicked, m_pSliceCell, &SliceCell::PageGray);//灰度
	connect(radio[1], &QRadioButton::clicked, this, &IPSToolsBar::OnGray);//灰度

	connect(radio[2], &QRadioButton::clicked, m_p3DCell, &Cell3D::HavyThrough3D);//高穿
	connect(radio[2], &QRadioButton::clicked, m_pDRCell, &DRCell::HavyThroughDR);//高穿
	connect(radio[2], &QRadioButton::clicked, m_pSliceCell, &SliceCell::HavyThroughCT);//高穿
	//connect(radio[2], &QRadioButton::clicked, );//高穿
	//connect(radio[3], &QRadioButton::clicked, );//低穿

	connect(radio[3], &QRadioButton::clicked, m_p3DCell, &Cell3D::LightThrough3D);//低穿
	connect(radio[3], &QRadioButton::clicked, m_pDRCell, &DRCell::LightThroughDR);//低穿
	connect(radio[3], &QRadioButton::clicked, m_pSliceCell, &SliceCell::LightThroughCT);//低穿

	connect(radio[0], &QRadioButton::clicked, this, &IPSToolsBar::Color);//伪彩
	connect(radio[1], &QRadioButton::clicked, this, &IPSToolsBar::Gray);//灰度

	connect(radio[2], &QRadioButton::clicked, this, &IPSToolsBar::High);//高穿
	connect(radio[3], &QRadioButton::clicked, this, &IPSToolsBar::Low);//低穿

	connect(radio[4], &QRadioButton::clicked, m_p3DCell, &Cell3D::ResumeView);
	connect(radio[5], &QRadioButton::clicked, m_p3DCell, &Cell3D::ResumeView);
	connect(radio[6], &QRadioButton::clicked, m_p3DCell, &Cell3D::TopView);
	connect(radio[7], &QRadioButton::clicked, m_p3DCell, &Cell3D::LeftView);
	connect(radio[8], &QRadioButton::clicked, m_p3DCell, &Cell3D::RightView);

	connect(pushButton[0], &QPushButton::clicked, m_p3DCell, &Cell3D::DecreaseMultiAbsorb);
	connect(pushButton[1], &QPushButton::clicked, m_p3DCell, &Cell3D::IncreaseMultiAbsorb);

	connect(pushButton[0], &QPushButton::clicked, m_pDRCell, &DRCell::DecreaseMultiAbsorb);
	connect(pushButton[1], &QPushButton::clicked, m_pDRCell, &DRCell::IncreaseMultiAbsorb);

	connect(pushButton[0], &QPushButton::clicked, this, &IPSToolsBar::DecreaseMultiAbsorb);
	connect(pushButton[1], &QPushButton::clicked, this, &IPSToolsBar::IncreaseMultiAbsorb);
	//connect(radio[1], &QRadioButton::clicked, this, &IPSToolsBar::Brightness);
	//connect(radio[2], &QRadioButton::clicked, this, &IPSToolsBar::ContrastControl);
	//connect(radio[7], &QRadioButton::clicked, this, &IPSToolsBar::AdjustDesity);

	connect(checkBox[0], &QRadioButton::clicked, m_p3DCell, &Cell3D::NegativeProcess3D);// 反色
	connect(checkBox[0], &QRadioButton::clicked, this, &IPSToolsBar::NegativeProcess);// 反色

	connect(checkBox[1], &QRadioButton::clicked, this, &IPSToolsBar::OrgDo);
	connect(checkBox[2], &QRadioButton::clicked, this, &IPSToolsBar::ICDo);
	
	
	//connect(checkBox[1], &QRadioButton::clicked, m_p3DCell, &Cell3D::RemoveOrg3D);  // 有机物剔除
	//connect(checkBox[2], &QRadioButton::clicked, m_p3DCell, &Cell3D::RemoveInorg3D); // 无机物剔除

	connect(checkBox[1], &QRadioButton::clicked, this, &IPSToolsBar::RemoveOrg);  // 有机物剔除
	connect(checkBox[2], &QRadioButton::clicked, this, &IPSToolsBar::RemoveInorg); // 无机物剔除
	// 边缘增强
	connect(checkBox[3], &QRadioButton::clicked, m_p3DCell, &Cell3D::EdgeEnhance3D);
	connect(checkBox[3], &QRadioButton::clicked, this, &IPSToolsBar::EdgeEnhence);

	connect(checkBox[3], &QRadioButton::clicked, this, &IPSToolsBar::EdgeDo);
	connect(checkBox[4], &QRadioButton::clicked, m_p3DCell, &Cell3D::ScreenCut);
	connect(checkBox[5], &QRadioButton::clicked, m_p3DCell, &Cell3D::ScreenCapBtn);
	connect(checkBox[6], &QCheckBox::clicked, m_p3DCell, &Cell3D::SetMeasure3D);
    connect(checkBox[7], SIGNAL(clicked(bool)), (Cell3D*)m_p3DCell, SLOT(OnCropping(bool)));
#pragma endregion
	if (!MachineMode)SetDisable();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSToolsBar::IPSToolsBar");
}
void IPSToolsBar::InitImageString()
{
	imagestring.append("color.png");
	imagestring.append("colorc.png");
	imagestring.append("colorh.png");

	imagestring.append("black.png");
	imagestring.append("blackc.png");
	imagestring.append("blackh.png");

	imagestring.append("highprocess.png");
	imagestring.append("highprocessc.png");
	imagestring.append("highprocessh.png");

	imagestring.append("lowprocess.png");
	imagestring.append("lowprocessc.png");
	imagestring.append("lowprocessh.png");

	imagestring.append("resume.png");
	imagestring.append("resumec.png");
	imagestring.append("resumeh.png");

	imagestring.append("scale.png");
	imagestring.append("scalec.png");
	imagestring.append("scaleh.png");

	imagestring.append("topview.png");
	imagestring.append("topviewc.png");
	imagestring.append("topviewh.png");

	imagestring.append("leftview.png");
	imagestring.append("leftviewc.png");
	imagestring.append("leftviewh.png");

	imagestring.append("rightview.png");
	imagestring.append("rightviewc.png");
	imagestring.append("rightviewh.png");

	imagestring.append("neg.png");
	imagestring.append("negc.png");
	imagestring.append("negh.png");

	imagestring.append("or.png");
	imagestring.append("orc.png");
	imagestring.append("orh.png");

	imagestring.append("ic.png");
	imagestring.append("icc.png");
	imagestring.append("ich.png");

	imagestring.append("edgeenhence.png");
	imagestring.append("edgeenhencec.png");
	imagestring.append("edgeenhenceh.png");

	imagestring.append("cut.png");
	imagestring.append("cutc.png");
	imagestring.append("cuth.png");

	imagestring.append("rec.png");
	imagestring.append("recc.png");
	imagestring.append("rech.png");

	imagestring.append("measure.png");
	imagestring.append("measurec.png");
	imagestring.append("measureh.png");

	imagestring.append("sus.png");
	imagestring.append("susc.png");
	imagestring.append("sush.png");

	imagestring.append("demulti.png");
	imagestring.append("demultic.png");
	imagestring.append("demultih.png");

	imagestring.append("inmulti.png");
	imagestring.append("inmultic.png");
	imagestring.append("inmultih.png");

	tipstring.append(QString::fromLocal8Bit("彩色图像"));
	tipstring.append(QString::fromLocal8Bit("灰度图像"));
	tipstring.append(QString::fromLocal8Bit("高穿透性图像处理"));
	tipstring.append(QString::fromLocal8Bit("低穿透性图像处理"));
	tipstring.append(QString::fromLocal8Bit("3D旋转复位"));
	tipstring.append(QString::fromLocal8Bit("图像缩放"));
	tipstring.append(QString::fromLocal8Bit("顶视图"));
	tipstring.append(QString::fromLocal8Bit("左视图"));
	tipstring.append(QString::fromLocal8Bit("右视图"));
	tipstring.append(QString::fromLocal8Bit("反色"));
	tipstring.append(QString::fromLocal8Bit("有机物剔除"));
	tipstring.append(QString::fromLocal8Bit("无机物剔除"));
	tipstring.append(QString::fromLocal8Bit("边缘增强"));
	tipstring.append(QString::fromLocal8Bit("截图"));
	tipstring.append(QString::fromLocal8Bit("屏幕录制"));
	tipstring.append(QString::fromLocal8Bit("尺寸测量"));
	tipstring.append(QString::fromLocal8Bit("可疑物标记"));
	tipstring.append(QString::fromLocal8Bit("降低多重吸收率"));
	tipstring.append(QString::fromLocal8Bit("增加多重吸收率"));
}
void IPSToolsBar::SetEnable()
{
	for (int i = 0; i < 9; i++)
		radio[i]->setEnabled(true);
	for (int i = 0; i < 4; i++)
		checkBox[i]->setEnabled(true);
	checkBox[7]->setEnabled(true);
	for (int i = 0; i < 2; i++)
		pushButton[i]->setEnabled(true);
}
void IPSToolsBar::SetDisable()
{
	for (int i = 0; i < 9; i++)
		radio[i]->setEnabled(false);
	for (int i = 0; i < 4; i++)
		checkBox[i]->setDisabled(true);
	checkBox[7]->setDisabled(true);
	for (int i = 0; i < 2; i++)
		pushButton[i]->setEnabled(false);
}
void IPSToolsBar::SetClippingState(bool bClipping)
{
    ui.rsus->setChecked(bClipping);
}
void IPSToolsBar::SetMeasureState(bool bMeasure)
{
	ui.rmeasure->setChecked(bMeasure);
}
void IPSToolsBar::SetViewState(ViewDirection viewDirection)
{
}



void IPSToolsBar::OrgDo()
{
	if (checkBox[1]->isChecked())
	{
		checkBox[1]->setChecked(true);
		checkBox[2]->setChecked(false);
		checkBox[3]->setChecked(false);
		emit Organic();
	}
}

void IPSToolsBar::ICDo()
{
	if (checkBox[2]->isChecked())
	{
		checkBox[1]->setChecked(false);
		checkBox[2]->setChecked(true);
		checkBox[3]->setChecked(false);
		emit IC();
	}
}

void IPSToolsBar::OnColor()
{
	m_pDataModule->SetAlgo(Algorithm::Algo_Color);
	m_p3DCell->Color3D();
	m_pDRCell->ShowDR();
	m_pSliceCell->Page();
}

void IPSToolsBar::OnGray()
{
	m_pDataModule->RemoveAlgo(Algorithm::Algo_Color);
	m_p3DCell->Gray3D();
	m_pDRCell->ShowDRGray();
	m_pSliceCell->PageGray();
}

void IPSToolsBar::EdgeDo()
{
	if (checkBox[3]->isChecked())
	{
		checkBox[1]->setChecked(false);
		checkBox[2]->setChecked(false);
		checkBox[3]->setChecked(true);
		emit Edge();
	}
}

void IPSToolsBar::OnChecked(bool bChecked)
{
}

/*-----------------------------------------------------------
***   Description:     隐藏工具栏
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSToolsBar::HideToolsBar()
{

	if (!toolsout)
	{
		ui.pushButton->setStyleSheet("QPushButton{background:#642AB5 url(:/img/res/toolbar/fold.png) center no-repeat;border-radius:4px;}"
			"QPushButton:hover{background:#9254DE url(:/img/res/toolbar/fold.png) center no-repeat;border-radius:4px;}"
			"QPushButton:pressed{background:#391085 url(:/img/res/toolbar/fold.png) center no-repeat;border-radius:4px;}");
		for (int i = 12; i < 19; i++)
			radio[i]->setVisible(true);
		ui.line->setVisible(true);
		ui.line_2->setVisible(true);
		ui.widget->setFixedSize(48,732);
		toolsout = true;
	}
	else
	{
		ui.pushButton->setStyleSheet("QPushButton{background:#642AB5 url(:/img/res/toolbar/unfold.png) center no-repeat;border-radius:4px;}"
			"QPushButton:hover{background:#9254DE url(:/img/res/toolbar/unfold.png) center no-repeat;border-radius:4px;}"
			"QPushButton:pressed{background:#391085 url(:/img/res/toolbar/unfold.png) center no-repeat;border-radius:4px;}");
		for (int i = 12; i < 19; i++)
			radio[i]->setVisible(false);
		ui.line->setVisible(false);
		ui.line_2->setVisible(false);
		ui.widget->setFixedSize(48,472);
		toolsout = false;
	}
}

/*-----------------------------------------------------------
***   Description : 有机物剔除
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void IPSToolsBar::RemoveOrg()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSToolsBar::RemoveOrg");
	if (m_pSliceCell == nullptr)
	{
		return;
	}
	auto spData = m_pSliceCell->GetOriginalDataPixel();
	if (spData == nullptr)
	{
		return;
	}
	int w, h;
	int channel = 4;
	m_pSliceCell->GetDimention(w, h);
	std::shared_ptr<short> tempData = std::shared_ptr<short>(new short[w*h], std::default_delete<short[]>());
	auto spDes = std::shared_ptr<unsigned char>(new unsigned char[w*h*channel], std::default_delete<unsigned char[]>());
	CTRemoveOrg((const unsigned short*)tempData.get(), w, h, spDes);
	m_pSliceCell->RefreshImage(spDes.get(), w, h);

	// dr图像边缘增强
	if (m_pDRCell == nullptr)
	{
		return;
	}
	spData = m_pDRCell->GetOriginalDataPixel();
	if (spData == nullptr)
	{
		return;
	}
	//int w, h;
	//int channel = 3;
	m_pDRCell->GetDimention(w, h);
	spDes = std::shared_ptr<unsigned char>(new unsigned char[w*h*channel], std::default_delete<unsigned char[]>());
	// color(tempData.get(), w, h, spDes);
	DrRemoveOrg((const unsigned short*)spData.get(), w, h, spDes);
	m_pDRCell->RefreshImage(spDes.get(), w, h);

	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSToolsBar::RemoveInorg");
}

/*-----------------------------------------------------------
***   Description : 无机物剔除
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void IPSToolsBar::RemoveInorg()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSToolsBar::RemoveInorg");
	if (m_pSliceCell == nullptr)
	{
		return;
	}
	auto spData = m_pSliceCell->GetOriginalDataPixel();
	if (spData == nullptr)
	{
		return;
	}
	int w, h;
	int channel = 4;
	m_pSliceCell->GetDimention(w, h);
	std::shared_ptr<short> tempData = std::shared_ptr<short>(new short[w*h], std::default_delete<short[]>());
	auto spDes = std::shared_ptr<unsigned char>(new unsigned char[w*h*channel], std::default_delete<unsigned char[]>());
	CTRemoveInorg((const unsigned short*)tempData.get(), w, h, spDes);
	m_pSliceCell->RefreshImage(spDes.get(), w, h);

	// dr图像边缘增强
	if (m_pDRCell == nullptr)
	{
		return;
	}

	spData = m_pDRCell->GetOriginalDataPixel();
	if (spData == nullptr)
	{
		return;
	}
	//int w, h;
	//int channel = 3;
	m_pDRCell->GetDimention(w, h);
	spDes = std::shared_ptr<unsigned char>(new unsigned char[w*h*channel], std::default_delete<unsigned char[]>());
	// color(tempData.get(), w, h, spDes);
	DrRemoveInorg((const unsigned short*)spData.get(), w, h, spDes);
	m_pDRCell->RefreshImage(spDes.get(), w, h);

	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSToolsBar::RemoveInorg");
}

/*-----------------------------------------------------------
***   Description:     边缘增强
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSToolsBar::EdgeEnhence()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSToolsBar::EdgeEnhence");
    if (m_pSliceCell == nullptr)
    {
        return;
    }
    auto spData = m_pSliceCell->GetOriginalDataPixel();
    if (spData == nullptr)
    {
        return;
    }
    int w, h;
	int channel = 3;
	m_pSliceCell->GetDimention(w, h);
	std::shared_ptr<short> tempData = std::shared_ptr<short>(new short[w*h], std::default_delete<short[]>());
    edgeEnhance(spData.get(), h, w, tempData.get());
	auto spDes = std::shared_ptr<unsigned char>(new unsigned char[w*h*channel], std::default_delete<unsigned char[]>());
	//color(tempData.get(), w, h, spDes);
	CTColor((const unsigned short*)tempData.get(), w, h, spDes);
	m_pSliceCell->RefreshImage(spDes.get(), w, h);

	// dr图像边缘增强
	if (m_pDRCell == nullptr)
	{
		return;
	}
	spData = m_pDRCell->GetOriginalDataPixel();
	if (spData == nullptr)
	{
		return;
	}
	//int w, h;
	//int channel = 3;
	m_pDRCell->GetDimention(w, h);
	int max = INT_MIN;
	int min = INT_MAX;
	short* data = spData.get();
	tempData = std::shared_ptr<short>(new short[w*h], std::default_delete<short[]>());
	for (int j = 0; j < h; ++j)
	{
		for (int i = 0; i < w; ++i)
		{
			if (*data > max)
			{
				max = *data;
			}

			if (*data < min)
			{
				min = *data;
			}

			++data;
		}
	}

	data = spData.get();
	short* temp = tempData.get();
	for (int j = 0; j < h; ++j)
	{
		for (int i = 0; i < w; ++i)
		{
			*temp = (*data - min) * 5120 / (max - min);

			++temp;
			++data;
		}
	}

	edgeEnhance(tempData.get(), h, w, tempData.get());
	spDes = std::shared_ptr<unsigned char>(new unsigned char[w*h*channel], std::default_delete<unsigned char[]>());
	// color(tempData.get(), w, h, spDes);
	CTColor((const unsigned short*)tempData.get(), w, h, spDes);
	m_pDRCell->RefreshImage(spDes.get(), w, h);

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSToolsBar::EdgeEnhence");
}


/*-----------------------------------------------------------
***   Description:     密度调节
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSToolsBar::AdjustDesity()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"IPSToolsBar::AdjustDesity");
	if (m_pSliceCell == nullptr)
	{
		return;
	}
	auto spData = m_pSliceCell->GetOriginalDataPixel();
	if (spData == nullptr)
	{
		return;
	}
	int w, h;
	int channel = 3;
	m_pSliceCell->GetDimention(w, h);
	auto spDes = std::shared_ptr<unsigned char>(new unsigned char[w*h], std::default_delete<unsigned char[]>());
	adjustDensity(spData.get(), w, h, 0, 500, spDes);
	m_pSliceCell->RefreshImage(spDes.get(), w, h);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"IPSToolsBar::AdjustDesity");
}

/*-----------------------------------------------------------
***   Description : 增加多重吸收
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void IPSToolsBar::IncreaseMultiAbsorb()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"IPSToolsBar::IncreaseMultiAbsorb");
	if (m_pSliceCell == nullptr)
	{
		return;
	}
	auto spData = m_pSliceCell->GetOriginalDataPixel();
	if (spData == nullptr)
	{
		return;
	}
	int w, h;
	m_pSliceCell->GetDimention(w, h);
	std::map<short, short> absorb;
	for (int i = 0; i < 500;++i)
	{
		absorb[i] = i;
	}
	auto spDes = std::shared_ptr<unsigned char>(new unsigned char[w*h], std::default_delete<unsigned char[]>());
	adjustMultiAbsorb((unsigned short*)spData.get(), w, h, absorb, spDes);
	m_pSliceCell->RefreshImage(spDes.get(), w, h);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"IPSToolsBar::IncreaseMultiAbsorb");
}

/*-----------------------------------------------------------
***   Description : 降低多重吸收
***   OUTPUT      :
***   INPUT       :
------------------------------------------------------------*/
void IPSToolsBar::DecreaseMultiAbsorb()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSToolsBar::DecreaseMultiAbsorb");
	if (m_pSliceCell == nullptr)
	{
		return;
	}
	auto spData = m_pSliceCell->GetOriginalDataPixel();
	if (spData == nullptr)
	{
		return;
	}
	int w, h;
	m_pSliceCell->GetDimention(w, h);
	std::map<short, short> absorb;
	for (int i = 1000; i < 1500; ++i)
	{
		absorb[i] = i;
	}
	auto spDes = std::shared_ptr<unsigned char>(new unsigned char[w*h], std::default_delete<unsigned char[]>());
	adjustMultiAbsorb((unsigned short*)spData.get(), w, h, absorb, spDes);
	m_pSliceCell->RefreshImage(spDes.get(), w, h);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(), "IPSToolsBar::DecreaseMultiAbsorb");
}


/*-----------------------------------------------------------
***   Description:     亮度调节
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSToolsBar::Brightness()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSToolsBar::Brightness");
	if (m_pSliceCell == nullptr)
	{
		return;
	}
	auto spData = m_pSliceCell->GetOriginalDataPixel();
	if (spData == nullptr)
	{
		return;
	}
	int w, h;
	m_pSliceCell->GetDimention(w, h);
	int channel = 3;
	auto spDes = std::shared_ptr<unsigned char>(new unsigned char[w*h*channel], std::default_delete<unsigned char[]>());
	adjustContrastAndBright(spData.get(), h, w, 2.0,30.0,spDes.get());
	m_pSliceCell->RefreshImage(spDes.get(), w, h);

	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSToolsBar::Brightness");
}

/*-----------------------------------------------------------
***   Description:     对比度调节
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSToolsBar::ContrastControl()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSToolsBar::ContrastControl");
	if (m_pSliceCell == nullptr)
	{
		return;
	}
	auto spData = m_pSliceCell->GetOriginalDataPixel();
	if (spData == nullptr)
	{
		return;
	}
	int w, h;
	m_pSliceCell->GetDimention(w, h);
	auto spDes = std::shared_ptr<unsigned char>(new unsigned char[w*h], std::default_delete<unsigned char[]>());
	adjustContrastAndBright(spData.get(), h, w, 0.5, 80.0, spDes.get());
	m_pSliceCell->RefreshImage(spDes.get(), w, h);
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),  "IPSToolsBar::ContrastControl");
}

/*-----------------------------------------------------------
***   Description:     反色
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSToolsBar::NegativeProcess()
{
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"IPSToolsBar::Negative");
	if (m_pSliceCell == nullptr)
	{
		return;
	}
	auto spData = m_pSliceCell->GetOriginalDataPixel();
	if (spData == nullptr)
	{
		return;
	}
	int w, h;
	
	m_pSliceCell->GetDimention(w, h);
	auto spDes = std::shared_ptr<unsigned char>(new unsigned char[w*h], std::default_delete<unsigned char[]>());
	negative(spData.get(), w, h, spData.get());
	CTColor((const unsigned short*)spData.get(), w, h, spDes);
	m_pSliceCell->RefreshImage(spDes.get(), w, h);
	m_pSliceCell->FitWindow();
	// dr图像反色
	if (m_pDRCell == nullptr)
	{
		return;
	}
	spData = m_pDRCell->GetOriginalDataPixel();
	if (spData == nullptr)
	{
		return;
	}
	//int w, h;

	m_pDRCell->GetDimention(w, h);
	spDes = std::shared_ptr<unsigned char>(new unsigned char[w*h], std::default_delete<unsigned char[]>());
	negative(spData.get(), w, h, spData.get());
	CTColor((const unsigned short*)spData.get(), w, h, spDes);
	m_pDRCell->RefreshImage(spDes.get(), w, h);
	m_pDRCell->FitWindow();
	emit ChangeNegtiveStatus();
	LOG_DEBUG(GESP::DebugLogger::GetInstance(),"IPSToolsBar::Negative");
}

/*-----------------------------------------------------------
***   Description:     重绘事件响应
***   OUTPUT     :
***   INPUT      :
------------------------------------------------------------*/
void IPSToolsBar::paintEvent(QPaintEvent * event)
{
}
