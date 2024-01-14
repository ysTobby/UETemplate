#include "CustomStyleSet.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/CoreStyle.h"

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BORDER_BRUSH(RelativePath, ...) FSlateBorderBrush(RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define TTF_FONT(RelativePath, ...) FSlateFontInfo(RootToContentDir(RelativePath, TEXT(".ttf")), __VA_ARGS__)
#define OTF_FONT(RelativePath, ...) FSlateFontInfo(RootToContentDir(RelativePath, TEXT(".otf")), __VA_ARGS__)

FCustomUIStyleSet::FCustomUIStyleSet(const FName& InName)
    : FSlateStyleSet(InName)
{
    // UI文件放在Slate下
    SetContentRoot(FPaths::ProjectContentDir() / TEXT("Slate"));
    CreateUIResource();
    FSlateStyleRegistry::RegisterSlateStyle(*this);
}

FCustomUIStyleSet::~FCustomUIStyleSet()
{
    FSlateStyleRegistry::UnRegisterSlateStyle(*this);
}

const ISlateStyle& FCustomUIStyleSet::Get()
{
    static FCustomUIStyleSet Style(FName("FCustomUIStyle"));
    return Style;
}

void FCustomUIStyleSet::CreateUIResource()
{
    /*FVector2D Icon16x16(16, 16);
    FColor Color_0xFF93949D(0xFF93949D);
    Set("Color.0xFF93949D", Color_0xFF93949D);

    Set("ClassIcon.CompositingElement", new IMAGE_BRUSH("Editor/Slate/Icons/icon_CompShot_16px", Icon16x16));

    Get().GetBrush("ClassIcon.CompositingElement");*/
    //Set("checkbox_checked", new IMAGE_BRUSH("checkbox_checked", FVector2D(32, 32)));
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT