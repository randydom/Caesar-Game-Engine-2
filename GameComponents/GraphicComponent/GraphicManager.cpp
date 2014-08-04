#include "GraphicManager.h"

#include <EntityList.h>
#include "3DMath.h"
#include "XNAConverter.h"
#include "DX11Helper.h"
#include <algorithm>
#include <CameraEntity.h>
#include <ObjectEntity.h>
#include "Scene.h"
#include <Logger.h>
#include <sstream>
#include "ResourceManager.h"

GraphicManager::GraphicManager()
{
	this->D3DStuff.IsInitialized = false;
}

void GraphicManager::Init()
{
	this->InitWindow();
	this->InitDevice();
}

void GraphicManager::Work(double realTime, double deltaTime)
{
	MSG msg = {0};
	BOOL result = PeekMessage(&msg, this->window.hWnd, 0, 0, PM_REMOVE);
	if(result == TRUE)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	this->ProcessDrawing();
}

void GraphicManager::Shutdown()
{

}

void GraphicManager::ProcessDrawing()
{
	auto camera = Scene::GetCamera(this->DefaultCamera);
	std::hash_map<std::string, GraphicObjectEntity> objects = Scene::GetAllObjectEntities();

	camera.UpdateView();
	camera.UpdatePerspective();
	camera.UpdateOrthogonal();

	Scene::ClearScreen(camera);
	Scene::DrawObjects(camera, objects);

	// Present the information rendered to the back buffer to the front buffer (the screen)
	this->D3DStuff.pSwapChain->Present(0, 0);
}

void GraphicManager::RunAllCapture(std::hash_map<std::string, SP_INFO>& objects)
{
	auto& screenCaptures = ResourceManager::ScreenCaptureList.All();
	for(auto iter = screenCaptures.begin();
		iter != screenCaptures.end();
		++iter)
	{
		iter->second->Snap(objects);
	}
}

void GraphicManager::InitWindow()
{
	Logger::AddErrorLogger(MesageBoxError);

	this->window.hInst = GetModuleHandle(NULL);
	this->window.height = 768;
	this->window.width = 2048;

	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = GraphicManager::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = this->window.hInst;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"DirectX11Basic";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_APPLICATION);
	// Try a "register" this type of window... so that we can create it later
	if(!RegisterClassEx(&wcex))
	{
		Logger::LogError("Failed at registering Window");
	}

	// Create window
	this->window.hInst = this->window.hInst;
	RECT rc = {0, 0, this->window.width, this->window.height};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	this->window.hWnd = 0;
	this->window.hWnd = CreateWindow(L"DirectX11Basic", L"Caesar Game Engine", WS_OVERLAPPEDWINDOW,
									 CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, this->window.hInst,
									 NULL);

	// Try to create the window...
	if(!(this->window.hWnd))
	{
		Logger::LogError("Failed at creating window");
	}

	ShowWindow(this->window.hWnd, SW_SHOWNORMAL);
}
void GraphicManager::InitDevice()
{
	unsigned int& Width = this->window.width;
	unsigned int& Height = this->window.height;
	HWND& hwnd = this->window.hWnd;

	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Width;
	sd.BufferDesc.Height = Height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	ID3D11Device* pd3dDevice = nullptr;
	ID3D11DeviceContext* pImmediateContext = nullptr;
	IDXGISwapChain* pSwapChain = nullptr;
	for(UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		this->D3DStuff.driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, this->D3DStuff.driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
										   D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &this->D3DStuff.featureLevel, &pImmediateContext);
		if(SUCCEEDED(hr))
			break;
	}
	if(FAILED(hr)){ Logger::LogError("Failed at creating device and SwapChain"); }
	this->D3DStuff.pd3dDevice = pd3dDevice;
	this->D3DStuff.pImmediateContext = pImmediateContext;
	this->D3DStuff.pSwapChain = pSwapChain;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = this->D3DStuff.pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if(FAILED(hr))
		Logger::LogError("Failed at creating back buffer");

	ID3D11RenderTargetView*	pRenderTargetView = nullptr;
	hr = this->D3DStuff.pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	pBackBuffer->Release();
	if(FAILED(hr)){ Logger::LogError("Failed at creating Render Target view"); }
	this->D3DStuff.pRenderTargetView = pRenderTargetView;

	// Set up depth & stencil buffer
	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = Width;
	depthBufferDesc.Height = Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	ID3D11Texture2D* pDepthStencilBuffer = nullptr;
	// Create the texture for the depth buffer using the filled out description.
	hr = this->D3DStuff.pd3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &pDepthStencilBuffer);
	if(FAILED(hr)){ Logger::LogError("Failed at creating dept buffer"); }
	this->D3DStuff.pDepthStencilBuffer = pDepthStencilBuffer;

	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	ID3D11DepthStencilState* pDepthStencilState = nullptr;
	// Create the depth stencil state.
	hr = this->D3DStuff.pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
	if(FAILED(hr)) { Logger::LogError("Failed at creating Dept stencil State"); }
	this->D3DStuff.pDepthStencilState = pDepthStencilState;

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Set up the depth stencil view description.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	ID3D11DepthStencilState* pDepthDisabledStencilState = nullptr;
	// Create the depth stencil view.
	hr = this->D3DStuff.pd3dDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &pDepthDisabledStencilState);
	if(FAILED(hr)){ Logger::LogError("Failed at creating no depth state"); }
	this->D3DStuff.pDepthDisabledStencilState = pDepthDisabledStencilState;

	// Set the depth stencil state.
	this->D3DStuff.pImmediateContext->OMSetDepthStencilState(this->D3DStuff.pDepthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	ID3D11DepthStencilView* pDepthStencilView = nullptr;
	// Create the depth stencil view.
	hr = this->D3DStuff.pd3dDevice->CreateDepthStencilView(this->D3DStuff.pDepthStencilBuffer, &depthStencilViewDesc, &pDepthStencilView);
	if(FAILED(hr)){ Logger::LogError("Failed at creating depth stencil view"); }
	this->D3DStuff.pDepthStencilView = pDepthStencilView;

	// Set the depth stencil state.
	this->D3DStuff.pImmediateContext->OMSetDepthStencilState(this->D3DStuff.pDepthStencilState, 1);

	this->D3DStuff.pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	// Setup the viewport
	this->D3DStuff.vp.Width = (FLOAT)Width;
	this->D3DStuff.vp.Height = (FLOAT)Height;
	this->D3DStuff.vp.MinDepth = 0.0f;
	this->D3DStuff.vp.MaxDepth = 1.0f;
	this->D3DStuff.vp.TopLeftX = 0;
	this->D3DStuff.vp.TopLeftY = 0;
	this->D3DStuff.pImmediateContext->RSSetViewports(1, &(this->D3DStuff.vp));

	this->D3DStuff.IsInitialized = true;
}

void GraphicManager::MesageBoxError(std::string s)
{
	std::wstringstream wss;
	wss << "Exception: Something went wrong: " << std::endl
		<< s.c_str();
	std::wstring wStringError = wss.str();

	MessageBox(NULL, wStringError.c_str(), L"Exception", MB_ICONERROR);
}

LRESULT CALLBACK GraphicManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			std::exit(0);
			break;
		case WM_SIZE:
		{
			int width = 0;
			int height = 0;
			RECT rect;
			if(GetClientRect(hWnd, &rect))
			{
				width = rect.right - rect.left;
				height = rect.bottom - rect.top;
			}
			if(width != 0 || height != 0)
			{
				GraphicManager::GetInstance().Resize(width, height);
			}
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void GraphicManager::Resize(unsigned int width, unsigned int height)
{
	GraphicManager& graphic = GraphicManager::GetInstance();

	if(graphic.D3DStuff.IsInitialized == true)
	{
		graphic.D3DStuff.pRenderTargetView.reset();
		graphic.D3DStuff.pDepthStencilBuffer.reset();
		graphic.D3DStuff.pDepthStencilView.reset();

		// Resize the swap chain and recreate the render target view.
		HRESULT hr = graphic.D3DStuff.pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		if(FAILED(hr)){ Logger::LogError("Failed at resizing swap chain buffer"); }

		ID3D11Texture2D* pBuffer = NULL;
		hr = graphic.D3DStuff.pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
		if(FAILED(hr)){ Logger::LogError("Failed at creating back buffer"); }

		ID3D11RenderTargetView*	pRenderTargetView = nullptr;
		hr = graphic.D3DStuff.pd3dDevice->CreateRenderTargetView(pBuffer, NULL, &pRenderTargetView);
		pBuffer->Release();
		if(FAILED(hr)){ Logger::LogError("Failed at creating Render Target view"); }
		graphic.D3DStuff.pRenderTargetView = pRenderTargetView;

		// Create the depth/stencil buffer and view.
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = width;
		depthBufferDesc.Height = height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		ID3D11Texture2D* pDepthStencilBuffer = nullptr;
		// Create the texture for the depth buffer using the filled out description.
		hr = graphic.D3DStuff.pd3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &pDepthStencilBuffer);
		if(FAILED(hr)) { Logger::LogError("Failed at creating dept buffer"); }
		graphic.D3DStuff.pDepthStencilBuffer = pDepthStencilBuffer;

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		// Initialize the depth stencil view.
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		ID3D11DepthStencilView* pDepthStencilView = nullptr;
		// Create the depth stencil view.
		hr = graphic.D3DStuff.pd3dDevice->CreateDepthStencilView(graphic.D3DStuff.pDepthStencilBuffer, &depthStencilViewDesc, &pDepthStencilView);
		if(FAILED(hr)) { Logger::LogError("Failed at creating depth stencil view"); }
		graphic.D3DStuff.pDepthStencilView = pDepthStencilView;

		graphic.D3DStuff.pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);


		// Set the viewport transform.

		// Setup the viewport
		graphic.D3DStuff.vp.Width = (FLOAT)width;
		graphic.D3DStuff.vp.Height = (FLOAT)height;
		graphic.D3DStuff.vp.MinDepth = 0.0f;
		graphic.D3DStuff.vp.MaxDepth = 1.0f;
		graphic.D3DStuff.vp.TopLeftX = 0;
		graphic.D3DStuff.vp.TopLeftY = 0;
		graphic.D3DStuff.pImmediateContext->RSSetViewports(1, &(graphic.D3DStuff.vp));

		graphic.window.width = width;
		graphic.window.height = height;
	}
}
