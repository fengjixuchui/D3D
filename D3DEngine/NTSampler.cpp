#include "PreCom.h"
#include "NTSampler.h"
#include "NTWinShortCut.h"


NTSampler::NTSampler() : State(nullptr)
{
}


NTSampler::~NTSampler()
{
	if (nullptr != State)
	{
		State->Release();
	}
}

bool NTSampler::Create()
{
	if (false == Set())
	{
		return false;
	}

	return true;
}

bool NTSampler::Create(D3D11_FILTER _Filter, D3D11_TEXTURE_ADDRESS_MODE AddressU, D3D11_TEXTURE_ADDRESS_MODE AddressV, D3D11_TEXTURE_ADDRESS_MODE AddressW, FLOAT MipLodBias, UINT MaxAnisotropy, D3D11_COMPARISON_FUNC ComparisonFunc, NTCOLOR BorderColor, FLOAT MinLOD, FLOAT MaxLOD)
{
	if (false == Set(_Filter, AddressU, AddressV, AddressW, MipLodBias, MaxAnisotropy, ComparisonFunc, BorderColor, MinLOD, MaxLOD))
	{
		return false;
	}

	return true;
}

bool NTSampler::Set(D3D11_FILTER Filter,
	D3D11_TEXTURE_ADDRESS_MODE AddressU,
	D3D11_TEXTURE_ADDRESS_MODE AddressV,
	D3D11_TEXTURE_ADDRESS_MODE AddressW,
	FLOAT MipLODBias,
	UINT MaxAnisotropy,
	D3D11_COMPARISON_FUNC ComparisonFunc,
	NTCOLOR BorderColor,
	FLOAT MinLOD,
	FLOAT MaxLOD)
{
	if (nullptr != State)
	{
		State->Release();
	}

	Desc.Filter = Filter;
	Desc.AddressU = AddressU;
	Desc.AddressV = AddressV;
	Desc.AddressW = AddressW;
	memcpy_s(Desc.BorderColor, sizeof(NTCOLOR), BorderColor.s, sizeof(NTCOLOR));
	Desc.ComparisonFunc = ComparisonFunc;
	Desc.MaxAnisotropy = MaxAnisotropy;
	Desc.MipLODBias = MipLODBias;
	Desc.MaxLOD = MaxLOD;
	Desc.MinLOD = MinLOD;

	if (S_OK != NTWinShortCut::GetDevice()->CreateSamplerState(&Desc, &State))
	{
		return false;
	}

	return true;
}

void NTSampler::Update(unsigned int _Slot)
{
	NTWinShortCut::GetContext()->VSSetSamplers(_Slot, 1, &State);
	NTWinShortCut::GetContext()->PSSetSamplers(_Slot, 1, &State);
}