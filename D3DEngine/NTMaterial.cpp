#include "PreCom.h"
#include "NTMaterial.h"
#include "NTPixelShader.h"
#include "NTVertexShader.h"
#include "ResourceSystem.h"
#include "NTSampler.h"
#include "NTMultiTexture.h"
#include "NTRenderTarget.h"


NTMaterial::NTMaterial() : VertexShader(nullptr), PixelShader(nullptr), IsOriginal(false)
{
}


NTMaterial::~NTMaterial()
{
}

NTMaterial::NTMaterial(const NTMaterial & _Other) : NTResource(_Other), VertexShader(_Other.VertexShader), PixelShader(_Other.PixelShader), Blend(_Other.Blend)
, TexData(_Other.TexData), TextureMap(_Other.TextureMap), SamplerMap(_Other.SamplerMap), IsOriginal(false), HullShader(_Other.HullShader), GeometryShader(_Other.GeometryShader),
DomainShader(_Other.DomainShader)
{
	

}

bool NTMaterial::SetVertexShader(const wchar_t* _VertexName)
{
	VertexShader = ResourceSystem<NTVertexShader>::Find(_VertexName);
	tassert(nullptr == VertexShader);

	if (nullptr == VertexShader)
	{
		return false;
	}

	return true;
}

bool NTMaterial::SetPixelShader(const wchar_t* _PixelName)
{
	PixelShader = ResourceSystem<NTPixelShader>::Find(_PixelName);
	tassert(nullptr == VertexShader);

	if (nullptr == PixelShader)
	{
		return false;
	}

	return true;
}

bool NTMaterial::SetDomainShader(const wchar_t * _DomainName)
{
	DomainShader = ResourceSystem<NTDomainShader>::Find(_DomainName);
	tassert(nullptr == DomainShader);

	if (nullptr == DomainShader)
	{
		return false;
	}

	return true;
}

bool NTMaterial::SetGeometryShader(const wchar_t * _GeometryName)
{
	GeometryShader = ResourceSystem<NTGeometryShader>::Find(_GeometryName);
	tassert(nullptr == GeometryShader);

	if (nullptr == GeometryShader)
	{
		return false;
	}

	return true;
}

bool NTMaterial::SetHullShader(const wchar_t * _HullName)
{
	HullShader = ResourceSystem<NTHullShader>::Find(_HullName);
	tassert(nullptr == HullShader);

	if (nullptr == HullShader)
	{
		return false;
	}

	return true;
}

bool NTMaterial::SetBlend(const wchar_t* _BlendName)
{
	Blend = ResourceSystem<NTBlend>::Find(_BlendName);
	tassert(nullptr == Blend);

	if (nullptr == Blend)
	{
		return false;
	}

	return true;
}

void NTMaterial::Update()
{
	if (nullptr != VertexShader)
	{
		VertexShader->Update();
		VertexShader->SetLayout();
	}

	if (nullptr != PixelShader)
	{
		PixelShader->Update();
	}

	if (nullptr != HullShader)
	{
		HullShader->Update();
	}

	if (nullptr != GeometryShader)
	{
		GeometryShader->Update();
	}

	if (nullptr != DomainShader)
	{
		DomainShader->Update();
	}

	if (nullptr != Blend)
	{
		Blend->Update();
	}
}

void NTMaterial::AddTextureData(TEXTYPE _Type, UINT _TexSlot, const wchar_t * _TexName, UINT _SmpSlot, const wchar_t * _SmpName)
{
	TextureData TD;
	TD.Type = (int)_Type;
	TD.Tex_Idx = _TexSlot;
	TD.Tex_Smp = _SmpSlot;
	TexData.push_back(TD);

	if (_Type == TEXTYPE::TT_TARGET)
	{
		SetTargetTexture(_TexSlot, _TexName);
	}
	else if (_Type == TEXTYPE::TT_MULTI)
	{
		SetMultiTexture(_TexSlot, _TexName);
	}
	else
	{
		SetTexture(_TexSlot, _TexName);
	}

	SetSampler(_SmpSlot, _SmpName);
}

bool NTMaterial::Create()
{
	IsOriginal = true;
	return true;
}

void NTMaterial::SetTexture(unsigned int _Slot, const wchar_t * _TexName)
{
	Autoptr<NTTexture> Tex = ResourceSystem<NTTexture>::Find(_TexName);

	if (nullptr == Tex)
	{
		tassert(true);
		return;
	}

	std::unordered_map<unsigned int, Autoptr<NTTexture>>::iterator FindIter = TextureMap.find(_Slot);

	if (FindIter == TextureMap.end())
	{
		TextureMap.insert(std::unordered_map<unsigned int, Autoptr<NTTexture>>::value_type(_Slot, Tex));
	}
	else
	{
		FindIter->second = Tex;
	}
}

void NTMaterial::SetTargetTexture(unsigned int _Slot, const wchar_t * _TexName)
{
	Autoptr<NTRenderTarget> Target = ResourceSystem<NTRenderTarget>::Find(_TexName);

	if (nullptr == Target)
	{
		tassert(true);
		return;
	}

	std::unordered_map<unsigned int, Autoptr<NTTexture>>::iterator FindIter = TextureMap.find(_Slot);

	if (FindIter == TextureMap.end())
	{
		TextureMap.insert(std::unordered_map<unsigned int, Autoptr<NTTexture>>::value_type(_Slot, Target->GetTexture()));
	}
	else
	{
		FindIter->second = Target->GetTexture();
	}
}

void NTMaterial::SetMultiTexture(unsigned int _Slot, const wchar_t * _TexName)
{
	Autoptr<NTMultiTexture> Find = ResourceSystem<NTMultiTexture>::Find(_TexName);

	if (nullptr == Find)
	{
		tassert(true);
		return;
	}

	std::unordered_map<unsigned int, Autoptr<NTMultiTexture>>::iterator FindIter = MultiTextureMap.find(_Slot);

	if (FindIter == MultiTextureMap.end())
	{
		MultiTextureMap.insert(std::unordered_map<unsigned int, Autoptr<NTMultiTexture>>::value_type(_Slot, Find));
	}
	else
	{
		FindIter->second = Find;
	}
}

void NTMaterial::SetSampler(unsigned int _Slot, const wchar_t * _SmpName)
{
	Autoptr<NTSampler> Smp = ResourceSystem<NTSampler>::Find(_SmpName);

	if (nullptr == Smp)
	{
		tassert(true);
		return;
	}

	std::unordered_map<unsigned int, Autoptr<NTSampler>>::iterator FindIter = SamplerMap.find(_Slot);

	if (FindIter == SamplerMap.end())
	{
		SamplerMap.insert(std::unordered_map<unsigned int, Autoptr<NTSampler>>::value_type(_Slot, Smp));
	}
	else
	{
		FindIter->second = Smp;
	}
}

unsigned int NTMaterial::SetTextureData(TextureData * _Data)
{
	for (size_t i = 0; i < TexData.size(); i++)
	{
		_Data[i] = TexData[i];
	}

	return (unsigned int)TexData.size();
}

void NTMaterial::TextureUpdate()
{
	TexStartIter = TextureMap.begin();
	TexEndIter = TextureMap.end();
	for (; TexStartIter != TexEndIter; ++TexStartIter)
	{
		TexStartIter->second->Update(TexStartIter->first);
	}

	MultiTexStartIter = MultiTextureMap.begin();
	MultiTexEndIter = MultiTextureMap.end();
	for (; MultiTexStartIter != MultiTexEndIter; ++MultiTexStartIter)
	{
		MultiTexStartIter->second->Update(MultiTexStartIter->first);
	}
}

void NTMaterial::ResetTexture()
{
	TexStartIter = TextureMap.begin();
	TexEndIter = TextureMap.end();

	for (; TexStartIter != TexEndIter; ++TexStartIter)
	{
		TexStartIter->second->Reset(TexStartIter->first);
	}

	MultiTexStartIter = MultiTextureMap.begin();
	MultiTexEndIter = MultiTextureMap.end();

	for (; MultiTexStartIter != MultiTexEndIter; ++MultiTexStartIter)
	{
		MultiTexStartIter->second->Reset(MultiTexStartIter->first);
	}
}

void NTMaterial::SamplerUpdate()
{
	SmpStartIter = SamplerMap.begin();
	SmpEndIter = SamplerMap.end();
	for (; SmpStartIter != SmpEndIter; ++SmpStartIter)
	{
		SmpStartIter->second->Update(SmpStartIter->first);
	}
}

Autoptr<NTMaterial> NTMaterial::Clone()
{
	return new NTMaterial(*this);
}
