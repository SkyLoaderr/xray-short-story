#include "StdAfx.h"

#include "UIMMShniaga.h"
#include "UICursor.h"
#include "UIStatic.h"
#include "UIScrollView.h"
#include "UIXmlInit.h"
#include "MMsound.h"
#include "game_base_space.h"
#include "../level.h"
#include "object_broker.h"
#include <math.h>
#include "../Actor.h"
#include "../saved_game_wrapper.h"
#include "MainMenu.h"

extern string_path g_last_saved_game;

CUIMMShniaga::CUIMMShniaga()
{
	m_sound			= xr_new<CMMSound>();

	m_view			= xr_new<CUIScrollView>();	AttachChild(m_view);
	m_shniaga		= xr_new<CUIStatic>();	AttachChild(m_shniaga);
	m_magnifier		= xr_new<CUIStatic>();	m_shniaga->AttachChild(m_magnifier);	m_magnifier->SetPPMode();
	m_mag_pos		= 0;

	m_selected		= nullptr;

	m_start_time	= 0;
	m_origin		= 0;
	m_destination	= 0;
	m_run_time		= 0;

	m_flags.zero	();	

	m_selected_btn	= -1;
	m_page_id		= 0;
}

CUIMMShniaga::~CUIMMShniaga()
{
	xr_delete(m_magnifier);
	xr_delete(m_shniaga);
	xr_delete(m_view);
	xr_delete(m_sound);

	for (u32 i = 0; i < m_pages.size(); i++)
		delete_data(m_pages[i].m_buttons);
	m_pages.clear();
}

extern CActor* g_actor;

void CUIMMShniaga::InitShniaga(CUIXml& xml_doc, LPCSTR path)
{
	string256 _path;

	CUIXmlInit::InitWindow	(xml_doc, path, 0, this);

	strconcat				(sizeof(_path), _path, path, ":shniaga:magnifire");
	CUIXmlInit::InitStatic	(xml_doc, _path, 0, m_magnifier); 

	m_mag_pos				= m_magnifier->GetWndPos().x;
	strconcat				(sizeof(_path), _path, path, ":shniaga");
	CUIXmlInit::InitStatic	(xml_doc, _path, 0, m_shniaga);

	strconcat				(sizeof(_path), _path, path, ":buttons_region");
	CUIXmlInit::InitScrollView(xml_doc, _path, 0, m_view);

	strconcat				(sizeof(_path), _path, path, ":shniaga:magnifire:y_offset");
	m_offset				= xml_doc.ReadFlt(_path, 0, 0);

	m_sound->Init			(xml_doc, "menu_sound");
	m_sound->music_Play		();
}

void CUIMMShniaga::OnDeviceReset()
{
}

void CUIMMShniaga::CreateList(xr_vector<CUITextWnd*>& lst, CUIXml& xml_doc, LPCSTR path)
{
	CGameFont* pF					= nullptr;
	u32	color						= 0;

	CUIXmlInit::InitFont			(xml_doc, path, 0, color, pF);
	R_ASSERT3						(pF, "Can't find 'font' attribute", path);

	int nodes_num					= xml_doc.GetNodesNum(path, 0, "btn");

	XML_NODE* tab_node				= xml_doc.NavigateToNode(path,0);
	xml_doc.SetLocalRoot			(tab_node);

	CUITextWnd* st					= nullptr;

	for (int i = 0; i < nodes_num; ++i)
	{		
		st							= xr_new<CUITextWnd>();
		st->SetWndPos				(Fvector2().set(0.0f,0.0f));
		st->SetWndSize				(Fvector2().set(m_view->GetDesiredChildWidth(), xml_doc.ReadAttribFlt("btn", i, "btn_height", 25.f)));
		st->SetFont					(pF);
		st->SetTextComplexMode		(false);
		st->SetTextST				(xml_doc.ReadAttrib	("btn", i, "caption"));

//		float font_height			= st->GetFont()->GetHeight();
//		UI().ClientToScreenScaledHeight(font_height);

//.		st->SetTextOffset			(0, (button_height-font_height)/2.0f);
		st->SetTextColor			(color);
		st->SetTextAlignment		(CGameFont::alCenter);
		st->SetVTextAlignment		(valCenter);
		st->SetWindowName			(xml_doc.ReadAttrib("btn", i, "name"));
		st->SetMessageTarget		(this);


		lst.push_back(st);
	}
	xml_doc.SetLocalRoot(xml_doc.GetRoot());

}

int CUIMMShniaga::AddPage(CUIXml& xml_doc, LPCSTR path)
{
	SPage				m_new_page;
	CreateList			(m_new_page.m_buttons, xml_doc, path);
	m_pages.push_back	(m_new_page);

	return (int)m_pages.size()-1;
}

void CUIMMShniaga::ClearPages()
{
	for (u32 i = 0; i < m_pages.size(); i++)
		delete_data(m_pages[i].m_buttons);
	m_pages.clear();

	m_page_id	= -1;
	ShowPage	(m_page_id);
}

void CUIMMShniaga::ShowPage(int page_id)
{
	R_ASSERT(page_id >= -1 && page_id < (int)m_pages.size());

	m_page_id = page_id;
	m_view->Clear();

	if (page_id == -1) return;

	for (u32 i = 0; i<m_pages[m_page_id].m_buttons.size(); i++)
		m_view->AddWindow(m_pages[m_page_id].m_buttons[i], false);

	SelectBtn(m_pages[m_page_id].m_buttons[0]);
}

bool CUIMMShniaga::IsButton(CUIWindow* st)
{
	for (u32 i = 0; i<m_pages[m_page_id].m_buttons.size(); ++i)
		if (m_pages[m_page_id].m_buttons[i] == st)
			return true;

	return false;
}

void CUIMMShniaga::SendMessage(CUIWindow* pWnd, s16 msg, void* pData)
{
	CUIWindow::SendMessage(pWnd, msg, pData);
	if (IsButton(pWnd))
	{
		switch (msg)
		{
			case WINDOW_FOCUS_RECEIVED:
				SelectBtn(pWnd);
				break;
		}

	}
}

void CUIMMShniaga::SelectBtn(int btn)
{
	m_view->Update		();

	if (m_page_id == -1) return;

	R_ASSERT(btn >= 0 && btn < (int)m_pages[m_page_id].m_buttons.size());
	m_selected = m_pages[m_page_id].m_buttons[btn];
	m_selected_btn = btn;

	ProcessEvent(E_Begin);
}

void CUIMMShniaga::SelectBtn(CUIWindow* btn)
{
	R_ASSERT(m_page_id >= 0);
	for (int i = 0; i<(int)m_pages[m_page_id].m_buttons.size(); ++i)
	{
		if (m_pages[m_page_id].m_buttons[i] == btn)
		{
			SelectBtn(i);
			return;
		}
	}	
}

void CUIMMShniaga::Draw()
{
	CUIWindow::Draw();
}

void CUIMMShniaga::Update()
{
	if (m_start_time > Device.dwTimeContinual - m_run_time)
	{
		Fvector2 pos = m_shniaga->GetWndPos();
		pos.y = this->pos(m_origin, m_destination, Device.dwTimeContinual - m_start_time);
		m_shniaga->SetWndPos(pos);		
	} else
		ProcessEvent(E_Stop);

	if (m_start_time > Device.dwTimeContinual - m_run_time*10/100)
		ProcessEvent(E_Finalize);

	ProcessEvent(E_Update);
		
	CUIWindow::Update();
}


bool CUIMMShniaga::OnMouseAction(float x, float y, EUIMessages mouse_action)
{
	
	Fvector2 pos = UI().GetUICursor().GetCursorPosition();
    Frect r;
	m_magnifier->GetAbsoluteRect(r);
	if (WINDOW_LBUTTON_DOWN == mouse_action && r.in(pos.x, pos.y))
	{
		OnBtnClick();
	}

	return CUIWindow::OnMouseAction(x,y,mouse_action);
}

void CUIMMShniaga::OnBtnClick()
{
	GetMessageTarget()->SendMessage(m_selected, BUTTON_CLICKED);
}

#include <dinput.h>
bool CUIMMShniaga::OnKeyboardAction(int dik, EUIMessages keyboard_action)
{
	if (WINDOW_KEY_PRESSED == keyboard_action)
	{
		switch (dik)
		{
			case DIK_UP:
				if (m_selected_btn > 0)
					SelectBtn(m_selected_btn - 1);
				return true;
			case DIK_DOWN:
				if (m_selected_btn < BtnCount() - 1)
					SelectBtn(m_selected_btn + 1);
				return true;
			case DIK_RETURN:
				OnBtnClick();
				return true;
		}
	}

	return CUIWindow::OnKeyboardAction(dik, keyboard_action);
}

int CUIMMShniaga::BtnCount()
{
	if (PageCount() && m_page_id >= 0)
		return (int)m_pages[m_page_id].m_buttons.size();
	else
		return 0;
}

int CUIMMShniaga::PageCount()
{
	return (int)m_pages.size();
}

int CUIMMShniaga::CurrentPageId()
{
	return m_page_id;
}

float CUIMMShniaga::pos(float x1, float x2, u32 t)
{
	float x = 0;

    if (t>=0 && t<=m_run_time)
        x = log(1 + (t*10.0f)/m_run_time)/log(11.0f);
	else if (t<=0)
		x = 0;
	else if (t>m_run_time)
		x = 1;

	x*=abs(x2 - x1);

	if (x2 - x1 < 0)
		return x1 - x;
	else
        return x1 + x;
}

bool b_shniaganeed_pp = true;
void CUIMMShniaga::SetVisibleMagnifier(bool f)
{
	b_shniaganeed_pp = f;
	Fvector2 pos = m_magnifier->GetWndPos();
	if (f)
		pos.x = m_mag_pos;
	else
		pos.x = 1025;
	m_magnifier->SetWndPos(pos);
}

void CUIMMShniaga::ProcessEvent(EVENT ev)
{
	switch (ev){
		case E_Begin:
			{
				// init whell sound
				m_sound->whell_Play();

                // calculate moving params
				m_start_time = Device.dwTimeContinual;
				m_origin = m_shniaga->GetWndPos().y;
				m_destination = m_selected->GetWndPos().y - m_magnifier->GetWndPos().y;
				m_destination += m_offset;
				m_run_time = u32((log(1 + abs(m_origin - m_destination))/log(GetHeight()))*300);
				if (m_run_time < 100)
					m_run_time = 100;

                // reset flags
				m_flags.set(fl_SoundFinalized,	FALSE);
				m_flags.set(fl_MovingStoped,	FALSE);
			}	break;
		case E_Finalize:
			if (!m_flags.test(fl_SoundFinalized))
			{
				m_sound->whell_Click();
				
				m_flags.set(fl_SoundFinalized, TRUE);
			}	break;
		case E_Stop:		
			if (!m_flags.test(fl_MovingStoped))
			{
				m_sound->whell_Stop();	


				Fvector2 pos = m_shniaga->GetWndPos();
				pos.y = m_destination;
				m_shniaga->SetWndPos(pos);		

                m_flags.set(fl_MovingStoped, TRUE);
			}	break;
		case E_Update:		m_sound->music_Update();
			break;
									
	}
}
