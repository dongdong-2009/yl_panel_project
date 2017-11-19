
#include <config.h>
#include <render.h>
#include <stdlib.h>

/* ����panel_page��Ӧ��icons�Ĵ洢λ�� */
#define PANEL_PAGE_ICONS_PATH	"panel_icons"

/* ���ݼ��������ͼ���λ��
 *		��Ļ�ֱ���:800*480
 *		ͼ������Ϊ5�ţ�ÿ��9��ͼ��
 *		ͼ��ķֱ���Ϊ72*72
 *		����ͼ����Ϊ15�����ص�
 *		����ͼ����Ϊ20�����ص�
 */
static T_Layout g_atPanelPageIconsLayout[] = {
	{16, 21, 87, 92, "file_finder.jpg"},
	{103, 21, 174, 92, "leds.jpg"},
	{0, 0, 0, 0, NULL},
};

static T_PageLayout g_tPanelPageLayout = {
	.iMaxTotalBytes = 0,
	.atLayout       = g_atPanelPageIconsLayout,
};

/**********************************************************************
 * �������ƣ� ShowPanelPage
 * ���������� ��ʾ"panelҳ��"
 * ��������� ptPageLayout - �ں����ͼ����ļ�������ʾ����
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2017/09/23	     V1.0	  TECH-PRO	      ����
 ***********************************************************************/
static void ShowPanelPage(PT_PageLayout ptPageLayout)
{
	PT_VideoMem ptVideoMem;
	int iXres, iYres, iBpp;
	int iError;

	PT_Layout atLayout = ptPageLayout->atLayout;
	GetDispResolution(&iXres, &iYres, &iBpp);
	ptPageLayout->iBpp = iBpp;
		
	/* 1. ����Դ� */
	ptVideoMem = GetVideoMem(ID("panel"), 1);
	if (ptVideoMem == NULL)
	{
		DBG_PRINTF("can't get video mem for panel page!\n");
		return;
	}

	/* 2. �軭���� */

	/* ȷ��ÿ��ͼ����ռ���Դ��С����Ϊͼ���Сһ��������ֱ�Ӽ�������һ��ͼ��Ĵ�С���� */
	ptPageLayout->iMaxTotalBytes = (atLayout[0].iBotRightX - atLayout[0].iTopLeftX + 1)
								   * (atLayout[0].iBotRightY - atLayout[0].iTopLeftY + 1)
								   * ptPageLayout->iBpp / 8;
	
	iError = GeneratePageAndMidPath(ptPageLayout, ptVideoMem, PANEL_PAGE_ICONS_PATH);	

	/* 3. ˢ���豸��ȥ */
	FlushVideoMemToDev(ptVideoMem);

	/* 4. ����Դ� */
	PutVideoMem(ptVideoMem);
}


/**********************************************************************
 * �������ƣ� PanelPageGetInputEvent
 * ���������� Ϊ"panelҳ��"�����������,�ж������¼�λ����һ��ͼ����
 * ��������� ptPageLayout - �ں����ͼ�����ʾ����
 * ��������� ptInputEvent - �ں��õ�����������
 * �� �� ֵ�� -1     - �������ݲ�λ���κ�һ��ͼ��֮��
 *            ����ֵ - �������������ڵ�ͼ��(PageLayout->atLayout�������һ��)
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2017/09/23	     V1.0	  TECH-PRO	      ����
 ***********************************************************************/
static int PanelPageGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent)
{
	return GenericGetInputEvent(ptPageLayout, ptInputEvent);
}

/**********************************************************************
 * �������ƣ� PanelPageRun
 * ���������� "panelҳ��"�����к���: ��ʾ�˵�ͼ��,�����û�����������Ӧ
 * ��������� ptParentPageParams - δ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2017/09/23	     V1.0	  TECH-PRO	      ����
 ***********************************************************************/
static void PanelPageRun(PT_PageParams ptParentPageParams)
{
	int iIndex;
	T_InputEvent tInputEvent;
	int bPressed = 0;
	int iIndexPressed = -1;
	T_PageParams tPageParams;

	tPageParams.iPageID = ID("panel");
	
	/* 1. ��ʾҳ�� */
	ShowPanelPage(&g_tPanelPageLayout);

	/* 2. ����Prepare�߳� */

	/* 3. ����GetInputEvent��������¼����������� */
	while (1)
	{
		iIndex = PanelPageGetInputEvent(&g_tPanelPageLayout, &tInputEvent);
		if (tInputEvent.iPressure == 0)
		{
			/* ������ɿ� */
			if (bPressed)
			{
				/* �����а�ť������ */
				ReleaseButton(&g_atPanelPageIconsLayout[iIndexPressed]);
				bPressed = 0;

				if (iIndexPressed == iIndex) /* ���º��ɿ�����ͬһ����ť */
				{
					switch (iIndexPressed)
					{
						case 0: /* �����ļ�������� */
						{
							Page("main")->Run(&tPageParams);

							/* ������ҳ�淵�غ���ʾ���׵�panelҳ�� */
							ShowPanelPage(&g_tPanelPageLayout);
							break;
						}
						case 1: /* ����LED�Ʋ������� */
						{
							Page("leds")->Run(&tPageParams);

							/* ������ҳ�淵�غ���ʾ���׵�panelҳ�� */
							ShowPanelPage(&g_tPanelPageLayout);
							break;
						}
						default:
						{
							break;
						}
					}
				}
				
				iIndexPressed = -1;
			}
		}
		else
		{
			/* ����״̬ */
			if (iIndex != -1)
			{
				if (!bPressed)
				{
					/* δ�����°�ť */
					bPressed = 1;
					iIndexPressed = iIndex;
					PressButton(&g_atPanelPageIconsLayout[iIndexPressed]);
				}
			}
		}		
	}
}

/* panel�������к������Ϣ */
static T_PageAction g_tPanelPageAction = {
	.name          = "panel",
	.Run           = PanelPageRun,
	.GetInputEvent = PanelPageGetInputEvent,
};

/**********************************************************************
 * �������ƣ� PanelPageInit
 * ���������� ע��"panelҳ��"
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� 0 - �ɹ�, ����ֵ - ʧ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2017/09/23	     V1.0	  TECH-PRO	      ����
 ***********************************************************************/
int PanelPageInit(void)
{
	return RegisterPageAction(&g_tPanelPageAction);
}

