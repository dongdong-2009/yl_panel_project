
#include <config.h>
#include <render.h>
#include <stdlib.h>

/* 定义panel_page对应的icons的存储位置 */
#define PANEL_PAGE_ICONS_PATH	"panel_icons"

/* 根据计算来获得图标的位置
 *		屏幕分辨率:800*480
 *		图标排列为5排，每排9个图标
 *		图标的分辨率为72*72
 *		左右图标间隔为15个像素点
 *		上下图标间隔为20个像素点
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
 * 函数名称： ShowPanelPage
 * 功能描述： 显示"panel页面"
 * 输入参数： ptPageLayout - 内含多个图标的文件名和显示区域
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2017/09/23	     V1.0	  TECH-PRO	      创建
 ***********************************************************************/
static void ShowPanelPage(PT_PageLayout ptPageLayout)
{
	PT_VideoMem ptVideoMem;
	int iXres, iYres, iBpp;
	int iError;

	PT_Layout atLayout = ptPageLayout->atLayout;
	GetDispResolution(&iXres, &iYres, &iBpp);
	ptPageLayout->iBpp = iBpp;
		
	/* 1. 获得显存 */
	ptVideoMem = GetVideoMem(ID("panel"), 1);
	if (ptVideoMem == NULL)
	{
		DBG_PRINTF("can't get video mem for panel page!\n");
		return;
	}

	/* 2. 描画数据 */

	/* 确定每个图标所占的显存大小，因为图标大小一样，所以直接计算任意一个图标的大小即可 */
	ptPageLayout->iMaxTotalBytes = (atLayout[0].iBotRightX - atLayout[0].iTopLeftX + 1)
								   * (atLayout[0].iBotRightY - atLayout[0].iTopLeftY + 1)
								   * ptPageLayout->iBpp / 8;
	
	iError = GeneratePageAndMidPath(ptPageLayout, ptVideoMem, PANEL_PAGE_ICONS_PATH);	

	/* 3. 刷到设备上去 */
	FlushVideoMemToDev(ptVideoMem);

	/* 4. 解放显存 */
	PutVideoMem(ptVideoMem);
}


/**********************************************************************
 * 函数名称： PanelPageGetInputEvent
 * 功能描述： 为"panel页面"获得输入数据,判断输入事件位于哪一个图标上
 * 输入参数： ptPageLayout - 内含多个图标的显示区域
 * 输出参数： ptInputEvent - 内含得到的输入数据
 * 返 回 值： -1     - 输入数据不位于任何一个图标之上
 *            其他值 - 输入数据所落在的图标(PageLayout->atLayout数组的哪一项)
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2017/09/23	     V1.0	  TECH-PRO	      创建
 ***********************************************************************/
static int PanelPageGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent)
{
	return GenericGetInputEvent(ptPageLayout, ptInputEvent);
}

/**********************************************************************
 * 函数名称： PanelPageRun
 * 功能描述： "panel页面"的运行函数: 显示菜单图标,根据用户输入作出反应
 * 输入参数： ptParentPageParams - 未用
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2017/09/23	     V1.0	  TECH-PRO	      创建
 ***********************************************************************/
static void PanelPageRun(PT_PageParams ptParentPageParams)
{
	int iIndex;
	T_InputEvent tInputEvent;
	int bPressed = 0;
	int iIndexPressed = -1;
	T_PageParams tPageParams;

	tPageParams.iPageID = ID("panel");
	
	/* 1. 显示页面 */
	ShowPanelPage(&g_tPanelPageLayout);

	/* 2. 创建Prepare线程 */

	/* 3. 调用GetInputEvent获得输入事件，进而处理 */
	while (1)
	{
		iIndex = PanelPageGetInputEvent(&g_tPanelPageLayout, &tInputEvent);
		if (tInputEvent.iPressure == 0)
		{
			/* 如果是松开 */
			if (bPressed)
			{
				/* 曾经有按钮被按下 */
				ReleaseButton(&g_atPanelPageIconsLayout[iIndexPressed]);
				bPressed = 0;

				if (iIndexPressed == iIndex) /* 按下和松开都是同一个按钮 */
				{
					switch (iIndexPressed)
					{
						case 0: /* 进入文件浏览界面 */
						{
							Page("main")->Run(&tPageParams);

							/* 从设置页面返回后显示当首的panel页面 */
							ShowPanelPage(&g_tPanelPageLayout);
							break;
						}
						case 1: /* 进入LED灯操作界面 */
						{
							Page("leds")->Run(&tPageParams);

							/* 从设置页面返回后显示当首的panel页面 */
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
			/* 按下状态 */
			if (iIndex != -1)
			{
				if (!bPressed)
				{
					/* 未曾按下按钮 */
					bPressed = 1;
					iIndexPressed = iIndex;
					PressButton(&g_atPanelPageIconsLayout[iIndexPressed]);
				}
			}
		}		
	}
}

/* panel界面运行和相关信息 */
static T_PageAction g_tPanelPageAction = {
	.name          = "panel",
	.Run           = PanelPageRun,
	.GetInputEvent = PanelPageGetInputEvent,
};

/**********************************************************************
 * 函数名称： PanelPageInit
 * 功能描述： 注册"panel页面"
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2017/09/23	     V1.0	  TECH-PRO	      创建
 ***********************************************************************/
int PanelPageInit(void)
{
	return RegisterPageAction(&g_tPanelPageAction);
}

