#pragma once

#include "Styling/SlateStyle.h"

class CUSTOMUI_API FCustomUIStyleSet : public FSlateStyleSet
{
public:
	static const ISlateStyle& Get();
private:
	FCustomUIStyleSet(const FName& InName);
	~FCustomUIStyleSet();

	void CreateUIResource();
};