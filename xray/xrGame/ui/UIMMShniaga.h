#pragma once

#include "UIWindow.h"

class CUIStatic;
class CUITextWnd;
class CUIXml;
class CUIScrollView;
class CMMSound;

class CUIMMShniaga :	public CUIWindow,
						public CDeviceResetNotifier
{
public:
	enum
	{
		fl_SoundFinalized	= 1,
		fl_MovingStoped		= 2
	};

	enum EVENT
	{
		E_Begin				= 0,
		E_Update,
		E_Finalize,
		E_Stop
	};
	struct SPage
	{
		xr_vector<CUITextWnd*>	m_buttons;
	};

public:
						CUIMMShniaga			();
	virtual				~CUIMMShniaga			();

			void 			InitShniaga				(CUIXml& xml_doc, LPCSTR path);
	virtual void 			Update					();
	virtual void 			Draw					();

	virtual bool 			OnMouseAction			(float x, float y, EUIMessages mouse_action);
	virtual bool 			OnKeyboardAction		(int dik, EUIMessages keyboard_action);
	virtual void 			SendMessage				(CUIWindow* pWnd, s16 msg, void* pData = 0);
			void 			SetVisibleMagnifier		(bool f);
	virtual void			OnDeviceReset			();

	int						AddPage					(CUIXml& xml_doc, LPCSTR path);
	void					ClearPages				();
	void					ShowPage				(int page_id);
	int						PageCount				();
	int						CurrentPageId			();

	int	 					BtnCount				();
	void 					SelectBtn				(int btn);
	void 					SelectBtn				(CUIWindow* btn);
protected:
	void 					OnBtnClick				();

	void 					ProcessEvent			(EVENT ev);

	bool 					IsButton				(CUIWindow* st);
	void 					CreateList				(xr_vector<CUITextWnd*>& lst, CUIXml& xml_doc, LPCSTR path);
	float					pos						(float x1, float x2, u32 t);

    CUIStatic*				m_shniaga;
	CUIStatic*				m_magnifier;
	CUIScrollView*			m_view;

    u32						m_start_time;
	u32						m_run_time;
    float					m_origin;
	float					m_destination;
	float					m_mag_pos;
	float					m_offset;

	xr_vector<SPage>		m_pages;

	int						m_selected_btn;
	int						m_page_id;
    CUIWindow*				m_selected;
	CMMSound*				m_sound;
	Flags32					m_flags;	
};