#ifndef __FRAMEWORK_GRAPHICS_GRAPHICSCONTEXTRESOURCE_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_GRAPHICSCONTEXTRESOURCE_H_INCLUDED__

#include "../common.h"

class GraphicsDevice;

/**
 * Base class for objects whose lifecycle is tied to the current graphics
 * context and should be made aware when the context is lost and/or
 * recreated.
 */
class GraphicsContextResource
{
public:
	GraphicsContextResource();
	GraphicsContextResource(GraphicsDevice *graphicsDevice);
	
	virtual ~GraphicsContextResource();
	
	/**
	 * New OpenGL graphics context creation callback.
	 */
	virtual void OnNewContext()                                                 {}

	/**
	 * Lost OpenGL graphics context callback.
	 */
	virtual void OnLostContext()                                                {}
	
	GraphicsDevice* GetGraphicsDevice() const                                   { return m_graphicsDevice; }
	
private:
	GraphicsDevice *m_graphicsDevice;
};

#endif
