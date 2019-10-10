#include "..\includes\Graphics.h"

Graphics::Graphics(HWND hWnd)
{
	//Descriptor for swap chain
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.Width = 0; //Looks at the window and figure it out
	scd.BufferDesc.Height = 0;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0; //pick whatever freshrate you need to
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hWnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;

	//create device and front/back buffers, swapchain and rendering context
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&_pSwapChain,
		&_pDevice,
		nullptr,
		&_pContext
	);

	//gain access to texture subresource in swapchain (back buffer)
	ID3D11Texture2D* pBackBuffer = nullptr;
	_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	_pDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr,
		&_pTarget
	);
	pBackBuffer->Release();
}

Graphics::~Graphics()
{
	if (_pTarget != nullptr)
	{
		_pTarget->Release();
	}

	if (_pContext != nullptr)
	{
		_pContext->Release();
	}

	if (_pSwapChain != nullptr)
	{
		_pSwapChain->Release();
	}

	if (_pDevice != nullptr)
	{
		_pDevice->Release();
	}
}

void Graphics::EndFrame()
{
	_pSwapChain->Present(1u, 0u);
}

void Graphics::ClearBuffer(float r, float g, float b) noexcept
{
	const float color[] = { r,g,b,1.0f };
	_pContext->ClearRenderTargetView(_pTarget, color);
}
