#include "..\includes\Graphics.h"
#include "dxerr.h"
#include <sstream>

#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib, "D3Dcompiler.lib")

#define GFX_THROW_FAILED(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )

namespace wrl = Microsoft::WRL;

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

	HRESULT hr;

	//create device and front/back buffers, swapchain and rendering context
	GFX_THROW_FAILED( D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&_pSwapChain,
		&_pDevice,
		nullptr,
		&_pContext
	));

	//gain access to texture subresource in swapchain (back buffer)
	wrl::ComPtr<ID3D11Texture2D> pBackBuffer;
	GFX_THROW_FAILED( _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer));
	GFX_THROW_FAILED( _pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&_pTarget
	));

	//Create z buffer / depth buffer
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDDState;
	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthEnable = TRUE;
	dsd.StencilEnable = FALSE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS;

	GFX_THROW_FAILED(_pDevice->CreateDepthStencilState(&dsd, &pDDState));

	//bind depth state
	_pContext->OMSetDepthStencilState(pDDState.Get(), 1u);

	//Create depth stensil texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC td = {};
	td.Width = 800u;
	td.Height = 600u;
	td.MipLevels = 1u;
	td.ArraySize = 1u;
	td.Format = DXGI_FORMAT_D32_FLOAT;
	td.SampleDesc.Count = 1u;			//SampleDesc values must match swapchains values!!!
	td.SampleDesc.Quality = 0u;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_FAILED(_pDevice->CreateTexture2D(&td, nullptr, &pDepthStencil));

	//Create view of depth stencil texture
	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd = {};
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0u;
	GFX_THROW_FAILED(_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &dsvd, &_pDepthStencilView));

	//Bind the depth stencil view to OM
	_pContext->OMSetRenderTargets(1u, _pTarget.GetAddressOf(), _pDepthStencilView.Get());
}

void Graphics::EndFrame()
{
	HRESULT hr;

	if (FAILED(hr = _pSwapChain->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(_pDevice->GetDeviceRemovedReason());
		}
		else
		{
			GFX_THROW_FAILED(hr);
		}
	}
}

void Graphics::ClearBuffer(float r, float g, float b) noexcept
{
	const float color[] = { r,g,b,1.0f };
	_pContext->ClearRenderTargetView(_pTarget.Get(), color);
	_pContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawTestCube(float angle, float x, float z)
{
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
	};

	//Create verticies for a cube
	const Vertex vertices[] = {
		{-1.0f, -1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f},
		{-1.0f, 1.0f, -1.0f},
		{1.0f, 1.0f, -1.0f},
		{-1.0f, -1.0f, 1.0f},
		{1.0f, -1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
	};

	D3D11_SUBRESOURCE_DATA verDataDesc = {};
	verDataDesc.pSysMem = vertices;

	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.CPUAccessFlags = 0u;
	vertexBufDesc.MiscFlags = 0u;
	vertexBufDesc.ByteWidth = sizeof(vertices);
	vertexBufDesc.StructureByteStride = sizeof(Vertex);


	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

	HRESULT hr;
	GFX_THROW_FAILED(_pDevice->CreateBuffer(&vertexBufDesc, &verDataDesc, &pVertexBuffer) );

	//Bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	_pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	//Create indices for those vertices
	const unsigned short indices[] =
	{
		0,2,1,	2,3,1,
		1,3,5,	3,7,5,
		2,6,3,	3,6,7,
		4,5,7,	4,7,6,
		0,4,2,	2,4,6,
		0,1,4,	1,5,4
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	GFX_THROW_FAILED(_pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

	//Bind indices buffer to pipeline
	_pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	//create constant buffer for transformation matrix
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};
	//This matrix is a concatination of rotation plus a squeeze in the X to compensate for 4:3 ratio
	const ConstantBuffer cb =
	{
		{
			//Matrixies are right multiplied so put modifiers on the right ALL THE TIME, same in shader
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationZ(angle) *								//Rotate around z
				DirectX::XMMatrixRotationX(angle) *								//Rotate around x as well
				DirectX::XMMatrixTranslation(x,0.0f,z + 4.0f) *					//Move to where the mouse is
				DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f/4.0f, 0.5f, 10.0f)	//Projection matrix that conpensates for unmatching aspect ratio
			)				
		}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	GFX_THROW_FAILED(_pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	//Bind constant buffer to vertex shader
	_pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	//Second constant buffer to hold colors for the lookup 
	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	//One colour for each face of the cube
	const ConstantBuffer2 cb2 =
	{
		{
			{1.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 1.0f},
		}
	};

	//Create second constant buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(ConstantBuffer2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;
	GFX_THROW_FAILED(_pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));

	//Bind constant buffer to pixel shader
	_pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

	//Create Pixel shader
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	GFX_THROW_FAILED(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	GFX_THROW_FAILED(_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	//Bind pixel shader to pipeline
	_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	//Create vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_FAILED(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	GFX_THROW_FAILED(_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	//Bind vertex shader to pipeline
	_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	//Input (vertex) layout (2d position only)
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	GFX_THROW_FAILED(_pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));

	//Bind input layout to pipeline
	_pContext->IASetInputLayout(pInputLayout.Get());

	//Create viewport
	D3D11_VIEWPORT vp = {};
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_pContext->RSSetViewports(1u, &vp);

	//Set primitive topology to triangle list
	_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw vertices in pipeline
	_pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}


//Exception handling below here
Graphics::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	_hr(hr)
{
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return _hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString(_hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(_hr, buf, sizeof(buf));
	return buf;
}


const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}