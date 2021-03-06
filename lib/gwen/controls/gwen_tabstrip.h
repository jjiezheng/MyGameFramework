/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_TABSTRIP_H
#define GWEN_CONTROLS_TABSTRIP_H

#include "gwen_base.h"
#include "gwen_button.h"
#include "gwen_tabbutton.h"

namespace Gwen 
{
	namespace Controls
	{
		//class TabControl;

		class GWEN_EXPORT TabStrip : public Base
		{
			public:

				GWEN_CONTROL( TabStrip, Base );

				virtual void Layout( Skin::Base* skin );

				virtual bool DragAndDrop_HandleDrop( Gwen::DragAndDrop::Package* pPackage, int x, int y );
				virtual bool DragAndDrop_CanAcceptPackage( Gwen::DragAndDrop::Package* pPackage );

				virtual void DragAndDrop_HoverEnter( Gwen::DragAndDrop::Package* pPackage, int x, int y );
				virtual void DragAndDrop_HoverLeave( Gwen::DragAndDrop::Package* pPackage );
				virtual void DragAndDrop_Hover( Gwen::DragAndDrop::Package* pPackage, int x, int y );

				virtual void SetTabPosition( int iPos );

				virtual void SetAllowReorder( bool b ){ m_bAllowReorder = b; }
				virtual bool AllowsTabReorder() { return m_bAllowReorder; }

				virtual bool ShouldClip(){ return false; }

			private:

				Base*	m_TabDragControl;
				bool	m_bAllowReorder;

		};
	}
}
#endif
