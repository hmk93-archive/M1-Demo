#include "pch.h"
#include "PostProcess.h"
#include "Device.h"
#include "RenderTarget.h"

PostProcess::PostProcess()
	: UIImage(L"PostProcess", UIImage::Center)
{
}

PostProcess::~PostProcess()
{
}

void PostProcess::Render()
{
	// MultiSampling RTV 에서 No MultiSampling RTV 로 복사
	Device::Get().GetDeviceContext()->ResolveSubresource(Device::Get().GetResolvedRT()->GetResource(), 0,
		Device::Get().GetFloatRT()->GetResource(), 0, DXGI_FORMAT_R16G16B16A16_FLOAT);

	ID3D11RenderTargetView* rtvs[] =
	{
		Device::Get().GetBackRT()
	};

	Device::Get().GetDeviceContext()->OMSetRenderTargets(_countof(rtvs), rtvs, nullptr);

	UIImage::Render();
}
