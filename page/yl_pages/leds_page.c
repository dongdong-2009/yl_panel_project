
#include <config.h>
#include <render.h>
#include <stdlib.h>

/* ����panel_page��Ӧ��icons�Ĵ洢λ�� */
#define LEDS_PAGE_ICONS_PATH	"led_icons"

static T_Layout g_atLedsPageIconsLayout[] = {
	{31,  21,  174, 92,  "led1_on.jpg"},
	{205, 21,  348, 92,  "led1_off.jpg"},
	{31,  113, 174, 184, "led2_on.jpg"},
	{205, 113, 348, 184, "led2_off.jpg"},
	{31,  205, 174, 276, "led3_on.jpg"},
	{205, 205, 348, 276, "led3_off.jpg"},
	{31,  297, 174, 368, "led4_on.jpg"},
	{205, 297, 348, 368, "led4_off.jpg"},
	{31,  389, 174, 460, "leds_on.jpg"},
	{205, 389, 348, 460, "leds_off.jpg"},
	{699, 389, 770, 460, "return.jpg"},
	{0,   0,   0,   0,   NULL},
};

static T_PageLayout g_tLedsPageLayout = {
	.iMaxTotalBytes = 0,
	.atLayout       = g_atLedsPageIconsLayout,
};

/**********************************************************************
 * �������ƣ� LedsPageGetInputEvent
 * ���������� Ϊ"led�Ʋ���ҳ��"�����������,�ж������¼�λ����һ��ͼ����
 * ��������� ptPageLayout - �ں����ͼ�����ʾ����
 * ��������� ptInputEvent - �ں��õ�����������
 * �� �� ֵ�� -1     - �������ݲ�λ���κ�һ��ͼ��֮��
 *            ����ֵ - �������������ڵ�ͼ��(PageLayout->atLayout�������һ��)
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2017/10/10		 V1.0     TECH-PRO		  ����
 ***********************************************************************/
static int LedsPageGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent)
{
	return GenericGetInputEvent(ptPageLayout, ptInputEvent);
}


/**********************************************************************
 * �������ƣ� ShowLedsPage
 * ���������� ��ʾ"led�Ʋ���ҳ��"
 * ��������� ptPageLayout - �ں����ͼ����ļ�������ʾ����
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2017/10/10	     V1.0	  TECH-PRO	      ����
 ***********************************************************************/
static void ShowLedsPage(PT_PageLayout ptPageLayout)
{
	PT_VideoMem ptVideoMem;
	int iXres, iYres, iBpp;
	int iError;

	PT_Layout atLayout = ptPageLayout->atLayout;
	GetDispResolution(&iXres, &iYres, &iBpp);
	ptPageLayout->iBpp = iBpp;
		
	/* 1. ����Դ� */
	ptVideoMem = GetVideoMem(ID("leds"), 1);
	if (ptVideoMem == NULL)
	{
		DBG_PRINTF("can't get video mem for leds page!\n");
		return;
	}

	/* 2. �軭���� */
	
	/* ȷ��ÿ��ͼ����ռ���Դ��С����Ϊͼ���Сһ��������ֱ�Ӽ�������һ��ͼ��Ĵ�С���� */
	ptPageLayout->iMaxTotalBytes = (atLayout[0].iBotRightX - atLayout[0].iTopLeftX + 1)
								   * (atLayout[0].iBotRightY - atLayout[0].iTopLeftY + 1)
								   * ptPageLayout->iBpp / 8;

	iError = GeneratePageAndMidPath(ptPageLayout, ptVideoMem, LEDS_PAGE_ICONS_PATH);	

	/* 3. ˢ���豸��ȥ */
	FlushVideoMemToDev(ptVideoMem);

	/* 4. ����Դ� */
	PutVideoMem(ptVideoMem);
}


/**********************************************************************
 * �������ƣ� LedsPageRun
 * ���������� "led�Ʋ���ҳ��"�����к���: ��ʾ�˵�ͼ��,�����û�����������Ӧ
 * ��������� ptParentPageParams - δ��
 * ��������� ��
 * �� �� ֵ�� ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2017/10/10	     V1.0	  TECH-PRO	      ����
 ***********************************************************************/
static void LedsPageRun(PT_PageParams ptParentPageParams)
{
	int iIndex;
	T_InputEvent tInputEvent;
	int bPressed = 0;
	int iIndexPressed = -1;
    T_PageParams tPageParams;

    tPageParams.iPageID = ID("leds");
	
	/* 1. ��ʾҳ�� */
	ShowLedsPage(&g_tLedsPageLayout);

	/* 2. ����Prepare�߳� */

	/* 3. ����GetInputEvent��������¼����������� */
	while (1)
	{
		iIndex = LedsPageGetInputEvent(&g_tLedsPageLayout, &tInputEvent);
		if (tInputEvent.iPressure == 0)
		{
			/* ������ɿ� */
			if (bPressed)
			{
				/* �����а�ť������ */
				ReleaseButton(&g_atLedsPageIconsLayout[iIndexPressed]);
				bPressed = 0;

				if (iIndexPressed == iIndex) /* ���º��ɿ�����ͬһ����ť */
				{
					switch (iIndexPressed)
					{
						case 0: 
						{
							break;
						}
						case 1:
						{
							break;
						}
						case 2: 
						{
							break;
						}
						case 3: 
						{
							break;
						}
						case 4: 
						{
							break;
						}
						case 5:
						{
							break;
						}
						case 6: 
						{
							break;
						}
						case 7: 
						{
							break;
						}
						case 8:
						{
							break;
						}
						case 9: 
						{
							break;
						}
						case 10:	/* ���ذ�ť�����ص���һ��ҳ�� */
						{
							return;
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
					PressButton(&g_atLedsPageIconsLayout[iIndexPressed]);
				}
			}
		}		
	}
}


static T_PageAction g_tLedsPageAction = {
	.name          = "leds",
	.Run           = LedsPageRun,
	.GetInputEvent = LedsPageGetInputEvent,
};


/**********************************************************************
 * �������ƣ� LedsPageInit
 * ���������� ע��"led�ƵĲ���ҳ��"
 * ��������� ��
 * ��������� ��
 * �� �� ֵ�� 0 - �ɹ�, ����ֵ - ʧ��
 * �޸�����        �汾��     �޸���	      �޸�����
 * -----------------------------------------------
 * 2017/10/10		 V1.0     TECH-PRO		  ����
 ***********************************************************************/
int LedsPageInit(void)
{
	return RegisterPageAction(&g_tLedsPageAction);
}

