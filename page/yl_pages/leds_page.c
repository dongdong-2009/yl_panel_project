
#include <config.h>
#include <render.h>
#include <stdlib.h>

/* 定义panel_page对应的icons的存储位置 */
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
 * 函数名称： LedsPageGetInputEvent
 * 功能描述： 为"led灯操作页面"获得输入数据,判断输入事件位于哪一个图标上
 * 输入参数： ptPageLayout - 内含多个图标的显示区域
 * 输出参数： ptInputEvent - 内含得到的输入数据
 * 返 回 值： -1     - 输入数据不位于任何一个图标之上
 *            其他值 - 输入数据所落在的图标(PageLayout->atLayout数组的哪一项)
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2017/10/10		 V1.0     TECH-PRO		  创建
 ***********************************************************************/
static int LedsPageGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent)
{
	return GenericGetInputEvent(ptPageLayout, ptInputEvent);
}


/**********************************************************************
 * 函数名称： ShowLedsPage
 * 功能描述： 显示"led灯操作页面"
 * 输入参数： ptPageLayout - 内含多个图标的文件名和显示区域
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2017/10/10	     V1.0	  TECH-PRO	      创建
 ***********************************************************************/
static void ShowLedsPage(PT_PageLayout ptPageLayout)
{
	PT_VideoMem ptVideoMem;
	int iXres, iYres, iBpp;
	int iError;

	PT_Layout atLayout = ptPageLayout->atLayout;
	GetDispResolution(&iXres, &iYres, &iBpp);
	ptPageLayout->iBpp = iBpp;
		
	/* 1. 获得显存 */
	ptVideoMem = GetVideoMem(ID("leds"), 1);
	if (ptVideoMem == NULL)
	{
		DBG_PRINTF("can't get video mem for leds page!\n");
		return;
	}

	/* 2. 描画数据 */
	
	/* 确定每个图标所占的显存大小，因为图标大小一样，所以直接计算任意一个图标的大小即可 */
	ptPageLayout->iMaxTotalBytes = (atLayout[0].iBotRightX - atLayout[0].iTopLeftX + 1)
								   * (atLayout[0].iBotRightY - atLayout[0].iTopLeftY + 1)
								   * ptPageLayout->iBpp / 8;

	iError = GeneratePageAndMidPath(ptPageLayout, ptVideoMem, LEDS_PAGE_ICONS_PATH);	

	/* 3. 刷到设备上去 */
	FlushVideoMemToDev(ptVideoMem);

	/* 4. 解放显存 */
	PutVideoMem(ptVideoMem);
}


/**********************************************************************
 * 函数名称： LedsPageRun
 * 功能描述： "led灯操作页面"的运行函数: 显示菜单图标,根据用户输入作出反应
 * 输入参数： ptParentPageParams - 未用
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2017/10/10	     V1.0	  TECH-PRO	      创建
 ***********************************************************************/
static void LedsPageRun(PT_PageParams ptParentPageParams)
{
	int iIndex;
	T_InputEvent tInputEvent;
	int bPressed = 0;
	int iIndexPressed = -1;
    T_PageParams tPageParams;

    tPageParams.iPageID = ID("leds");
	
	/* 1. 显示页面 */
	ShowLedsPage(&g_tLedsPageLayout);

	/* 2. 创建Prepare线程 */

	/* 3. 调用GetInputEvent获得输入事件，进而处理 */
	while (1)
	{
		iIndex = LedsPageGetInputEvent(&g_tLedsPageLayout, &tInputEvent);
		if (tInputEvent.iPressure == 0)
		{
			/* 如果是松开 */
			if (bPressed)
			{
				/* 曾经有按钮被按下 */
				ReleaseButton(&g_atLedsPageIconsLayout[iIndexPressed]);
				bPressed = 0;

				if (iIndexPressed == iIndex) /* 按下和松开都是同一个按钮 */
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
						case 10:	/* 返回按钮，返回到上一级页面 */
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
			/* 按下状态 */
			if (iIndex != -1)
			{
				if (!bPressed)
				{
					/* 未曾按下按钮 */
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
 * 函数名称： LedsPageInit
 * 功能描述： 注册"led灯的操作页面"
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2017/10/10		 V1.0     TECH-PRO		  创建
 ***********************************************************************/
int LedsPageInit(void)
{
	return RegisterPageAction(&g_tLedsPageAction);
}

