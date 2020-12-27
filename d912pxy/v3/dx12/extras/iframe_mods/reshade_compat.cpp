#include "stdafx.h"
/*
MIT License

Copyright(c) 2020 megai2

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/////////reshade ct interface

static const GUID reshade_ct_fake_guid = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
static const uint64_t reshade_ct_magic = 0x505670b7c18ff478;

enum reshade_ct_res_names
{
	COLOR = 0,
	DEPTH = 1,
	ZPREPASS = 2,
	GBUF_0 = 3,
	GBUF_1 = 4,
	GBUF_2 = 5,
	GBUF_3 = 6,
	GBUF_4 = 7,
	OVERLAY_0 = 8,
	OVERLAY_1 = 9,
	OVERLAY_2 = 10,
	OVERLAY_3 = 11,
	OPAQUE_GEOMETRY = 12,
	TRANSP_GEOMETRY = 13,
	COUNT = 14
};

struct reshade_ct_entry
{
	uint64_t magic;
	uint64_t ct_idx;
	ID3D12Resource* res;
};

/////////

using namespace d912pxy::extras::IFrameMods;

ReshadeCompat::ReshadeCompat()
{
	wchar_t* preUiPass = d912pxy_s.iframeMods.configVal(L"pre_ui_pass").raw;
	wchar_t* uiPassInitial = d912pxy_s.iframeMods.configVal(L"ui_pass_initial").raw;
	uint32_t uiPassRTDSmask = d912pxy_s.iframeMods.configVal(L"ui_pass_RTDS_mask").ui32();
	uint32_t uiPassTargetLock = d912pxy_s.iframeMods.configVal(L"ui_pass_target_lock").ui32();

	uiPass = new PassDetector(preUiPass, uiPassInitial, uiPassRTDSmask, uiPassTargetLock);
	d912pxy_s.iframeMods.pushMod(uiPass);

	d912pxy_s.iframeMods.pushMod(this);
}

void ReshadeCompat::RP_RTDSChange(d912pxy_replay_item::dt_om_render_targets* rpItem, d912pxy_replay_thread_context* rpContext)
{
	//detect any copy needed frames to ReShade as external resource
	if (uiPass->entered())
	{
		if (colorCopy.syncFrom(uiPass->getSurf()))
		{
			reshade_ct_entry color_tex_ct { reshade_ct_magic, reshade_ct_res_names::COLOR, colorCopy.ptr()->GetD12Obj() };
			rpContext->cl->SetPrivateData(reshade_ct_fake_guid, sizeof(reshade_ct_entry), (const void*)(&color_tex_ct));
		}
		uiPass->getSurf()->BCopyToWStates(colorCopy.ptr(), 3, rpContext->cl, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
}