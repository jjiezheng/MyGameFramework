#include "gwen_inputprocessor.h"
#include <controls/gwen_canvas.h>
#include "../basegameapp.h"
#include "../input/keyboard.h"
#include "../input/keysyms.h"
#include "../input/mouse.h"
#include "../input/mousebuttons.h"
#include "../input/touchscreen.h"

namespace Gwen
{
	namespace Input
	{
		InputProcessor::InputProcessor(BaseGameApp *gameApp, Gwen::Controls::Canvas *canvas)
		{
			m_gameApp = gameApp;
			m_canvas = canvas;
			m_enabled = false;

			Enable(true);
		}

		InputProcessor::~InputProcessor()
		{
			Enable(false);
		}

		bool InputProcessor::OnKeyDown(KEYS key)
		{
			unsigned char gwenKey = ConvertToGwenKey(key);
			return (bool)m_canvas->InputKey(gwenKey, true);
		}

		bool InputProcessor::OnKeyUp(KEYS key)
		{
			unsigned char gwenKey = ConvertToGwenKey(key);
			return (bool)m_canvas->InputKey(gwenKey, false);
		}

		bool InputProcessor::OnMouseButtonDown(MOUSE_BUTTONS button, uint x, uint y)
		{
			int gwenButton = ConvertToGwenButton(button);
			
			int scaledX = (float)x / m_canvas->Scale();
			int scaledY = (float)y / m_canvas->Scale();

			// trigger mouse move event for button events to ensure GWEN
			// knows where the button event occured at
			bool movedResult = (bool)m_canvas->InputMouseMoved(scaledX, scaledY, 0, 0);
			bool clickResult = (bool)m_canvas->InputMouseButton(gwenButton, true);

			// TODO: is this really the right way to do this .. ?
			return (movedResult || clickResult);
		}

		bool InputProcessor::OnMouseButtonUp(MOUSE_BUTTONS button, uint x, uint y)
		{
			int gwenButton = ConvertToGwenButton(button);

			int scaledX = (float)x / m_canvas->Scale();
			int scaledY = (float)y / m_canvas->Scale();

			// trigger mouse move event for button events to ensure GWEN
			// knows where the button event occured at
			bool movedResult = (bool)m_canvas->InputMouseMoved(scaledX, scaledY, 0, 0);
			bool clickResult = (bool)m_canvas->InputMouseButton(gwenButton, false);

			// TODO: is this really the right way to do this .. ?
			return (movedResult || clickResult);
		}

		bool InputProcessor::OnMouseMove(uint x, uint y, int deltaX, int deltaY)
		{
			// Gwen's input handling only processes coordinates in terms of scale = 1.0f
			int scaledX = (float)x / m_canvas->Scale();
			int scaledY = (float)y / m_canvas->Scale();
			int scaledDeltaX = (float)deltaX / m_canvas->Scale();
			int scaledDeltaY = (float)deltaY / m_canvas->Scale();

			return (bool)m_canvas->InputMouseMoved(scaledX, scaledY, scaledDeltaX, scaledDeltaY);
		}

		bool InputProcessor::OnTouchDown(int id, uint x, uint y, bool isPrimary)
		{
			if (!isPrimary)
				return false;

			// Gwen's input handling only processes coordinates in terms of scale = 1.0f
			int scaledX = (float)x / m_canvas->Scale();
			int scaledY = (float)y / m_canvas->Scale();

			bool movedResult = (bool)m_canvas->InputMouseMoved(scaledX, scaledY, 0, 0);
			bool clickResult = (bool)m_canvas->InputMouseButton(0, true);

			// TODO: is this really the right way to do this .. ?
			return (movedResult || clickResult);
		}

		bool InputProcessor::OnTouchUp(int id, bool isPrimary)
		{
			if (!isPrimary)
				return false;

			bool clickResult = (bool)m_canvas->InputMouseButton(0, false);

			// we do this so that GWEN isn't left thinking that the "mouse" is
			// hovering over whatever we were just clicking/touching. This is
			// done because obviously with a touchscreen, you don't hover over
			// anything unless you are clicking/touching...
			bool movedResult = (bool)m_canvas->InputMouseMoved(-1, -1, 0, 0);

			// TODO: is this really the right way to do this .. ?
			return (movedResult || clickResult);
		}

		bool InputProcessor::OnTouchMove(int id, uint x, uint y, int deltaX, int deltaY, bool isPrimary)
		{
			if (!isPrimary)
				return false;

			// Gwen's input handling only processes coordinates in terms of scale = 1.0f
			int scaledX = (float)x / m_canvas->Scale();
			int scaledY = (float)y / m_canvas->Scale();
			int scaledDeltaX = (float)deltaX / m_canvas->Scale();
			int scaledDeltaY = (float)deltaY / m_canvas->Scale();
			
			bool movedResult = (bool)m_canvas->InputMouseMoved(scaledX, scaledY, scaledDeltaX, scaledDeltaY);
			bool clickResult = (bool)m_canvas->InputMouseButton(0, true);

			// TODO: is this really the right way to do this .. ?
			return (movedResult || clickResult);
		}

		void InputProcessor::Enable(bool enable)
		{
			if (IsEnabled() == enable)
				return;

			Keyboard *keyboard = m_gameApp->GetKeyboard();
			Mouse *mouse = m_gameApp->GetMouse();
			Touchscreen *touchscreen = m_gameApp->GetTouchscreen();

			if (enable)
			{
				if (keyboard != NULL)
					keyboard->RegisterListener(this);
				if (mouse != NULL)
					mouse->RegisterListener(this);
				if (touchscreen != NULL)
					touchscreen->RegisterListener(this);
			}
			else
			{
				if (keyboard != NULL)
					keyboard->UnregisterListener(this);
				if (mouse != NULL)
					mouse->UnregisterListener(this);
				if (touchscreen != NULL)
					touchscreen->UnregisterListener(this);
			}

			m_enabled = enable;
		}

		unsigned char InputProcessor::ConvertToGwenKey(KEYS key) const
		{
			switch (key)
			{
			case KSYM_BACKSPACE: return Gwen::Key::Backspace;
			case KSYM_ESCAPE:    return Gwen::Key::Escape;
			case KSYM_END:       return Gwen::Key::End;
			case KSYM_LCTRL:     return Gwen::Key::Control;
			case KSYM_RCTRL:     return Gwen::Key::Control;
			case KSYM_RETURN:    return Gwen::Key::Return;
			case KSYM_TAB:       return Gwen::Key::Tab;
			case KSYM_SPACE:     return Gwen::Key::Space;
			case KSYM_UP:        return Gwen::Key::Up;
			case KSYM_DOWN:      return Gwen::Key::Down;
			case KSYM_LEFT:      return Gwen::Key::Left;
			case KSYM_RIGHT:     return Gwen::Key::Right;
			case KSYM_HOME:      return Gwen::Key::Home;
			case KSYM_DELETE:    return Gwen::Key::Delete;
			case KSYM_LALT:      return Gwen::Key::Alt;
			case KSYM_LSHIFT:    return Gwen::Key::Shift;
			case KSYM_RALT:      return Gwen::Key::Alt;
			case KSYM_RSHIFT:    return Gwen::Key::Shift;
			default:             return Gwen::Key::Invalid;
			}
		}

		int InputProcessor::ConvertToGwenButton(MOUSE_BUTTONS button) const
		{
			switch (button)
			{
			case MOUSE_LEFT:   return 0;
			case MOUSE_MIDDLE: return 2;
			case MOUSE_RIGHT:  return 1;
			default:           return -1;   // TODO: is this ok for an invalid value? would it be better to return some other default ... ?
			}
		}
	}
}

