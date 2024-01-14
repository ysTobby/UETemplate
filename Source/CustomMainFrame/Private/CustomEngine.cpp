#include "CustomEngine.h"
#include "CustomStyleSet.h"

TSharedRef<SWindow> UCustomEngine::CreateCustomWindow()
{
	FString DeviceLocalizedName;
	int32 ResX = GSystemResolution.ResX;
	int32 ResY = GSystemResolution.ResY;
	EWindowMode::Type WindowMode = GSystemResolution.WindowMode;

	{
		ConditionallyOverrideSettings(ResX, ResY, WindowMode);

		// If the current settings have been overridden, apply them back into the system
		if (ResX != GSystemResolution.ResX || ResY != GSystemResolution.ResY || WindowMode != GSystemResolution.WindowMode)
		{
			FSystemResolution::RequestResolutionChange(ResX, ResY, WindowMode);
			IConsoleManager::Get().CallAllConsoleVariableSinks();
		}
	}

	FText WindowTitleOverride = FText();
	GConfig->GetText(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectDisplayedTitle"), WindowTitleOverride, GGameIni);
	const FText WindowTitleComponent = WindowTitleOverride.IsEmpty() ? NSLOCTEXT("UnrealEd", "GameWindowTitle", "{GameName}") : WindowTitleOverride;

	FText WindowDebugInfoComponent = FText();
#if !UE_BUILD_SHIPPING
	FText WindowDebugInfoOverride = FText();
	GConfig->GetText(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("ProjectDebugTitleInfo"), WindowDebugInfoOverride, GGameIni);
	WindowDebugInfoComponent = WindowDebugInfoOverride.IsEmpty() ? NSLOCTEXT("UnrealEd", "GameWindowTitleDebugInfo", "({PlatformArchitecture}-bit {BuildConfiguration} {RHIName})") : WindowDebugInfoOverride;
#endif

#if PLATFORM_64BITS
	//These are invariant strings so they don't need to be localized
	const FText PlatformBits = FText::FromString(TEXT("64"));
#else	//PLATFORM_64BITS
	const FText PlatformBits = FText::FromString(TEXT("32"));
#endif	//PLATFORM_64BITS

	// Note: If these parameters are updated or renamed, please update the tooltip on the ProjectDisplayedTitle and ProjectDebugTitleInfo properties
	FFormatNamedArguments Args;
	Args.Add(TEXT("GameName"), FText::FromString(FApp::GetProjectName()));
	Args.Add(TEXT("PlatformArchitecture"), PlatformBits);
	Args.Add(TEXT("RHIName"), FText::FromName(LegacyShaderPlatformToShaderFormat(GMaxRHIShaderPlatform)));
	Args.Add(TEXT("BuildConfiguration"), FText::FromString(LexToString(FApp::GetBuildConfiguration())));

	/************************************************************************/
	/************************ Add device name to window title****************/
	/************************************************************************/
	const FText WindowTitleVar = FText::Format(FText::FromString(TEXT("{0} {1} {2}")), WindowTitleComponent, WindowDebugInfoComponent, FGlobalTabmanager::Get()->GetApplicationTitle());
	const FText WindowTitle = FText::Format(WindowTitleVar, Args);

	auto GetProjectSettingBool = [](const FString& ParamName, bool Default) -> bool
		{
			bool Temp = Default;
			GConfig->GetBool(TEXT("/Script/EngineSettings.GeneralProjectSettings"), *ParamName, Temp, GGameIni);
			return Temp;
		};

	auto GetProjectSettingInt = [](const FString& ParamName, int Default) -> int32
		{
			int32 Temp = Default;
			GConfig->GetInt(TEXT("/Script/EngineSettings.GeneralProjectSettings"), *ParamName, Temp, GGameIni);
			return Temp;
		};

	const bool bShouldPreserveAspectRatio = GetProjectSettingBool(TEXT("bShouldWindowPreserveAspectRatio"), true);
	const bool bUseBorderlessWindow = GetProjectSettingBool(TEXT("bUseBorderlessWindow"), false) && PLATFORM_SUPPORTS_BORDERLESS_WINDOW;
	const bool bAllowWindowResize = GetProjectSettingBool(TEXT("bAllowWindowResize"), true);
	const bool bAllowClose = GetProjectSettingBool(TEXT("bAllowClose"), true);
	const bool bAllowMaximize = GetProjectSettingBool(TEXT("bAllowMaximize"), true);
	const bool bAllowMinimize = GetProjectSettingBool(TEXT("bAllowMinimize"), true);

	const int32 MinWindowWidth = GetProjectSettingInt(TEXT("MinWindowWidth"), 640);
	const int32 MinWindowHeight = GetProjectSettingInt(TEXT("MinWindowHeight"), 480);

	// Allow optional winX/winY parameters to set initial window position
	EAutoCenter AutoCenterType = EAutoCenter::PrimaryWorkArea;
	int32 WinX = 0;
	int32 WinY = 0;
	if (FParse::Value(FCommandLine::Get(), TEXT("WinX="), WinX) && FParse::Value(FCommandLine::Get(), TEXT("WinY="), WinY))
	{
		AutoCenterType = EAutoCenter::None;
	}

	// @ByteDance-BIM-Client Modification Start: Set Window Postion In Center
	bool bWindownInCenter = false;
	GConfig->GetBool(TEXT("BDBim"), TEXT("WindownInCenter"), bWindownInCenter, GGameIni);
	if (bWindownInCenter)
	{
		FDisplayMetrics DisplayMetrics;
		if (FSlateApplication::IsInitialized())
		{
			FSlateApplication::Get().GetInitialDisplayMetrics(DisplayMetrics);
		}
		else
		{
			if (FApp::CanEverRender())
			{
				FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);
			}
			else
			{
				// assume headless
				DisplayMetrics.PrimaryDisplayWidth = 0;
				DisplayMetrics.PrimaryDisplayHeight = 0;
			}
		}

		FIntPoint DesktopRes = FIntPoint(DisplayMetrics.PrimaryDisplayWidth, DisplayMetrics.PrimaryDisplayHeight);
		WinX = (DesktopRes.X - MinWindowWidth) / 2;
		WinY = (DesktopRes.Y - MinWindowHeight) / 2;
		AutoCenterType = EAutoCenter::None;
	}
	// @ByteDance-BIM-Client Modification End

	// Give the window the max width/height of either the requested resolution, or your available desktop resolution
	// We need to do this as we request some 4K windows when rendering sequences, and the OS may try and clamp that
	// window to your available desktop resolution
	TOptional<float> MaxWindowWidth;
	TOptional<float> MaxWindowHeight;
	if (WindowMode == EWindowMode::Windowed)
	{
		// Get available desktop area
		FDisplayMetrics DisplayMetrics;
		if (FSlateApplication::IsInitialized())
		{
			FSlateApplication::Get().GetInitialDisplayMetrics(DisplayMetrics);
		}
		else
		{
			FDisplayMetrics::RebuildDisplayMetrics(DisplayMetrics);
		}

		MaxWindowWidth = FMath::Max(DisplayMetrics.VirtualDisplayRect.Right - DisplayMetrics.VirtualDisplayRect.Left, ResX);
		MaxWindowHeight = FMath::Max(DisplayMetrics.VirtualDisplayRect.Bottom - DisplayMetrics.VirtualDisplayRect.Top, ResY);
	}

	static FWindowStyle BorderlessStyle = FWindowStyle::GetDefault();
	BorderlessStyle
		.SetActiveTitleBrush(FSlateNoResource())
		.SetInactiveTitleBrush(FSlateNoResource())
		.SetFlashTitleBrush(FSlateNoResource())
		.SetOutlineBrush(FSlateNoResource())
		.SetBorderBrush(FSlateNoResource())
		.SetBackgroundBrush(FSlateNoResource())
		.SetChildBackgroundBrush(FSlateNoResource());

	FString WindowTypeConfigStr = TEXT("GameWindow");
	EWindowType WindowType = EWindowType::GameWindow;
	GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"), TEXT("WindowType"), WindowTypeConfigStr, GGameIni);
	if (WindowTypeConfigStr == TEXT("Normal"))
		WindowType = EWindowType::Normal;

	TSharedRef<SWindow> Window = SNew(SWindow)
		.Type(WindowType)
		.Style(bUseBorderlessWindow ? &BorderlessStyle : &FCoreStyle::Get().GetWidgetStyle<FWindowStyle>("Window"))
		.ClientSize(FVector2D(ResX, ResY))
		.AdjustInitialSizeAndPositionForDPIScale(false)
		.Title(WindowTitle)
		.AutoCenter(AutoCenterType)
		.ScreenPosition(FVector2D(WinX, WinY))
		.MinWidth(MinWindowWidth)
		.MinHeight(MinWindowHeight)
		.MaxWidth(MaxWindowWidth)
		.MaxHeight(MaxWindowHeight)
		.FocusWhenFirstShown(true)
		.SaneWindowPlacement(AutoCenterType == EAutoCenter::None)
		.UseOSWindowBorder(!bUseBorderlessWindow)
		.CreateTitleBar(!bUseBorderlessWindow)
		.ShouldPreserveAspectRatio(bShouldPreserveAspectRatio)
		.LayoutBorder(bUseBorderlessWindow ? FMargin(0) : FMargin(5, 5, 5, 5))
		.SizingRule(bAllowWindowResize ? ESizingRule::UserSized : ESizingRule::FixedSize)
		.HasCloseButton(bAllowClose)
		.SupportsMinimize(bAllowMinimize)
		.SupportsMaximize(bAllowMaximize);

	Window->SetAllowFastUpdate(true);

	const bool bShowImmediately = false;

	FSlateApplication::Get().AddWindow(Window, bShowImmediately);

	if (WindowMode == EWindowMode::Fullscreen)
	{
		Window->SetWindowMode(EWindowMode::WindowedFullscreen);
	}
	else
	{
		Window->SetWindowMode(WindowMode);
	}

	// Tick now to force a redraw of the window and ensure correct fullscreen application
	FSlateApplication::Get().Tick();

	return Window;
}

UCustomEngine::UCustomEngine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCustomEngine::Init(IEngineLoop* InEngineLoop)
{
	//这里可以做一些控制台参数的初始化工作
	/*static IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	CVar->Set(200);*/

	if (GameViewportWindow.IsValid())
	{
		GameViewportWindow = CreateCustomWindow();
	}

	UGameEngine::Init(InEngineLoop);

	GameViewport->AddViewportWidgetContent(SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SButton)
						.Text(FText::FromString("map1"))
						.OnClicked_Lambda([this]()->FReply {
						FURL DefaultURL;
						DefaultURL.LoadURLConfig(TEXT("DefaultPlayer"), GGameIni);
						//这种写法可以指定到对应的Map和Gamemode信息，至于使用什么PC和State由GameMode去控制
						FString PackageName2 = "/Game/Maps/Map_Test1?GAME=/Script/CustomEngineExtend.CustomGameModeBaseTest";
						FURL URL2(&DefaultURL, *PackageName2, TRAVEL_Partial);
						FString Error;
						if (URL2.Valid)
						{
							auto BrowseRet = Browse(*GameInstance->GetWorldContext(), URL2, Error);
						}
							return FReply::Handled();
						})
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SButton)
						.Text(FText::FromString("map2"))
						.OnClicked_Lambda([this]()->FReply {
						FURL DefaultURL;
						DefaultURL.LoadURLConfig(TEXT("DefaultPlayer"), GGameIni);
						FString PackageName2 = "/Game/Maps/Map_Empty";
						FURL URL2(&DefaultURL, *PackageName2, TRAVEL_Partial);
						FString Error;
						if (URL2.Valid)
						{
							auto BrowseRet = Browse(*GameInstance->GetWorldContext(), URL2, Error);
						}
						return FReply::Handled();
							})
				]
		]);
}