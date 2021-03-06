#ifndef __FRAMEWORK_GRAPHICS_GRAPHICSDEVICE_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_GRAPHICSDEVICE_H_INCLUDED__

#include "../common.h"

#include "framebufferdatatypes.h"
#include "textureformats.h"
#include "textureparameters.h"
#include "../math/rect.h"

#include <stl/list.h>
#include <stl/vector.h>

class BufferObject;
class DebugShader;
class Framebuffer;
class GameWindow;
class GeometryDebugRenderer;
class GraphicsContextResource;
class Image;
class IndexBuffer;
class Renderbuffer;
class Shader;
class SimpleColorShader;
class SimpleColorTextureShader;
class SimpleTextureShader;
class SimpleTextureVertexLerpShader;
class SimpleTextureVertexSkinningShader;
class SolidColorTextureCache;
class Sprite2DShader;
class Sprite3DShader;
class Texture;
class VertexBuffer;
class ViewContext;
struct Color;

typedef stl::list<GraphicsContextResource*> ManagedResourceList;
typedef stl::vector<uint> EnabledVertexAttribList;

/**
 * Provides an abstraction over the underlying OpenGL context.
 */
class GraphicsDevice
{
public:
	GraphicsDevice();
	virtual ~GraphicsDevice()                                                   { Release(); }
	
	/**
	 * Releases all resources associated with this object.
	 */
	void Release();
	
	/**
	 * Initializes the graphics device object based on a parent window that is
	 * hosting the OpenGL context.
	 * @param window a window with an active OpenGL context associated with it
	 * @return true if successful, false if not
	 */
	bool Initialize(GameWindow *window);

	/**
	 * New OpenGL graphics context creation callback.
	 */
	void OnNewContext();

	/**
	 * Lost OpenGL graphics context callback.
	 */
	void OnLostContext();

	/**
	 * Viewport resize callback.
	 * @param size the new viewport size
	 */
	void OnResize(const Rect &size);

	/**
	 * Render callback.
	 */
	void OnRender();

	/**
	 * Clears the display.
	 * @param r red component of the color to clear the display to
	 * @param g green component of the color to clear the display to
	 * @param b blue component of the color to clear the display to
	 * @param a alpha component of the color to clear the display to
	 */
	void Clear(float r, float g, float b, float a);

	/**
	 * Clears the display.
	 * @param color the color to clear the display to
	 */
	void Clear(const Color &color);

	/**
	 * Gets a texture that is filled with a single solid color.
	 * @param the color to get a texture for
	 * @return the texture or NULL on failure
	 */
	Texture* GetSolidColorTexture(const Color &color);

	/**
	 * Binds a texture for rendering.
	 * @param texture the texture to bind
	 * @param unit the texture unit to bind to
	 */
	void BindTexture(const Texture *texture, uint unit = 0);

	/**
	 * Binds a texture that is filled with the specified solid color.
	 * @param color a color with which to find and bind a texture with
	 * @param unit the texture unit that the solid color texture should be bound to
	 */
	void BindSolidColorTexture(const Color &color, uint unit = 0);

	/**
	 * Unbinds the currently bound texture.
	 * @param unit the texture unit to be unbound
	 */
	void UnbindTexture(uint unit = 0);
	
	/**
	 * Unbinds the specified texture only if it's bound already. If it's not
	 * currently bound this method will do nothing.
	 * @param texture the texture to be unbound
	 */
	void UnbindTexture(const Texture *texture);

	/**
	 * Sets the current texture parameters which all subsequent
	 * texture management calls will use.
	 * @param params the texture parameters to set
	 */
	void SetTextureParameters(const TextureParameters &params);
	
	/**
	 * @return the currently applied texture parameters
	 */
	const TextureParameters* GetTextureParameters() const                       { return &m_currentTextureParams; }
		
	/**
	 * Binds a renderbuffer.
	 * @param renderbuffer the renderbuffer to bind
	 */
	void BindRenderbuffer(Renderbuffer *renderbuffer);
	
	/**
	 * Unbinds the currently bound renderbuffer.
	 */
	void UnbindRenderbuffer();
	
	/**
	 * Unbinds the specified renderbuffer only if it's bound already. If it's
	 * not currently bound this method will do nothing.
	 * @param renderbuffer the renderbuffer to be unbound
	 */
	void UnbindRenderBuffer(Renderbuffer *renderBuffer);
	
	/**
	 * Binds a framebuffer.
	 * @param framebuffer the framebuffer to bind
	 */
	void BindFramebuffer(Framebuffer *framebuffer);
	
	/**
	 * Unbinds the currently bound framebuffer
	 */
	void UnbindFramebuffer();
	
	/**
	 * Unbinds the specified framebuffer only if it's bound already. If it's
	 * not currently bound this method will do nothing.
	 * @param framebuffer the framebuffer to be unbound
	 */
	void UnbindFramebuffer(Framebuffer *framebuffer);

	/**
	 * @return the current ViewContext object
	 */
	ViewContext* GetViewContext() const                                         { return m_activeViewContext; }
	
	/**
	 * Sets a new active ViewContext which will be applied immediately.
	 * If a NULL pointer is given, the default ViewContext will be
	 * restored.
	 * @param viewContext the new ViewContext to apply, or NULL to restore
	 *                    the original one
	 */
	void SetViewContext(ViewContext *viewContext);

	/**
	 * Registers a resource to have it's lifecycle management events
	 * raised automatically.
	 * @param resource the resource to be registered
	 */
	void RegisterManagedResource(GraphicsContextResource *resource);

	/**
	 * Unregisters a resource that was previously registered. It's lifecycle
	 * management events will no longer be raised automatically.
	 * @param resource the resource to be unregistered
	 */
	void UnregisterManagedResource(GraphicsContextResource *resource);

	/**
	 * Unregisters all resources that were previously registered. Their
	 * lifecycle management events will no longer be raised automatically.
	 */
	void UnregisterAllManagedResources();

	/**
	 * Binds a vertex buffer for rendering. If the buffer has an associated
	 * VBO it's contents will be uploaded to video memory if necessary.
	 * @param buffer the vertex buffer to bind
	 */
	void BindVertexBuffer(VertexBuffer *buffer);

	/**
	 * Unbinds a vertex buffer.
	 */
	void UnbindVertexBuffer();

	/**
	 * Binds an index buffer for rendering. If the buffer has an associated
	 * IBO it's contents will be uploaded to video memory if necessary.
	 * @param buffer the index buffer to bind
	 */
	void BindIndexBuffer(IndexBuffer *buffer);

	/**
	 * Unbinds an index buffer.
	 */
	void UnbindIndexBuffer();

	/**
	 * Binds a shader for rendering.
	 * @param shader the shader to bind
	 */
	void BindShader(Shader *shader);

	/**
	 * Unbinds any currently bound shader.
	 */
	void UnbindShader();

	/**
	 * Renders the currently bound vertex buffer as triangles.
	 * @param buffer index buffer containing indices of the vertices to be rendered
	 */
	void RenderTriangles(const IndexBuffer *buffer);

	/**
	 * Renders the currently bound vertex buffer as triangles.
	 */
	void RenderTriangles();

	/**
	 * Renders the currently bound vertex buffer as triangles.
	 * @param startVertex the index of the first vertex to be rendered
	 * @param numTriangles the number of triangles to be rendered
	 */
	void RenderTriangles(uint startVertex, uint numTriangles);

	/**
	 * Renders the currently bound vertex buffer as lines.
	 * @param buffer index buffer containing indices of the vertices to be rendered
	 */
	void RenderLines(const IndexBuffer *buffer);

	/**
	 * Renders the currently bound vertex buffer as lines.
	 */
	void RenderLines();

	/**
	 * Renders the currently bound vertex buffer as lines.
	 * @param startVertex the index of the first vertex to be rendered
	 * @param numLines the number of lines to be rendered
	 */
	void RenderLines(uint startVertex, uint numLines);

	/**
	 * Renders the currently bound vertex buffer as points.
	 */
	void RenderPoints(const IndexBuffer *buffer);

	/**
	 * Renders the currently bound vertex buffer as points.
	 */
	void RenderPoints();

	/**
	 * Renders the currently bound vertex buffer as points.
	 * @param startVertex the index of the first vertex to be rendered
	 * @param numPoints the number of points to be rendered
	 */
	void RenderPoints(uint startVertex, uint numPoints);
	
	/**
	 * @return a debug geometry renderer
	 */
	GeometryDebugRenderer* GetDebugRenderer() const                             { return m_debugRenderer; }

	/**
	 * @return built-in shader
	 */
	SimpleColorShader* GetSimpleColorShader();

	/**
	 * @return built-in shader
	 */
	SimpleColorTextureShader* GetSimpleColorTextureShader();

	/**
	 * @return built-in shader
	 */
	SimpleTextureShader* GetSimpleTextureShader();

	/**
	 * @return built-in shader
	 */
	Sprite2DShader* GetSprite2DShader();

	/**
	 * @return built-in shader
	 */
	Sprite3DShader* GetSprite3DShader();

	/**
	 * @return built-in shader
	 */
	SimpleTextureVertexLerpShader* GetSimpleTextureVertexLerpShader();
	
	/**
	 * @return built-in shader
	 */
	SimpleTextureVertexSkinningShader* GetSimpleTextureVertexSkinningShader();

	/**
	 * @return built-in shader
	 */
	DebugShader* GetDebugShader();
	
	/**
	 * @return true if depth textures are supported
	 */
	bool IsDepthTextureSupported() const                                        { return m_isDepthTextureSupported; }
	
	/**
	 * @return true if textures with dimensions that are not a power of two
	 *         are supported
	 */
	bool IsNonPowerOfTwoTextureSupported() const                                { return m_isNonPowerOfTwoTextureSupported; }
	
	/**
	 * @return the parent window object that this graphics device is for
	 */
	GameWindow* GetWindow() const                                               { return m_window; }

private:
	void BindVBO(VertexBuffer *buffer);
	void BindClientBuffer(VertexBuffer *buffer);
	void BindIBO(IndexBuffer *buffer);
	void BindClientBuffer(IndexBuffer *buffer);

	bool IsReadyToRender() const;

	void SetShaderVertexAttributes();
	void ClearSetShaderVertexAttributes();

	ManagedResourceList m_managedResources;

	Framebuffer *m_boundFramebuffer;
	const Renderbuffer *m_boundRenderbuffer;
	const VertexBuffer *m_boundVertexBuffer;
	const IndexBuffer *m_boundIndexBuffer;
	const Texture **m_boundTextures;
	Shader *m_boundShader;
	bool m_shaderVertexAttribsSet;
	EnabledVertexAttribList m_enabledVertexAttribIndices;
	bool m_isDepthTextureSupported;
	bool m_isNonPowerOfTwoTextureSupported;

	GameWindow *m_window;
	ViewContext *m_defaultViewContext;
	ViewContext *m_activeViewContext;
	TextureParameters m_currentTextureParams;

	GeometryDebugRenderer *m_debugRenderer;
	SolidColorTextureCache *m_solidColorTextures;

	SimpleColorShader *m_simpleColorShader;
	SimpleColorTextureShader *m_simpleColorTextureShader;
	SimpleTextureShader *m_simpleTextureShader;
	SimpleTextureVertexLerpShader *m_simpleTextureVertexLerpShader;
	SimpleTextureVertexSkinningShader *m_simpleTextureVertexSkinningShader;
	Sprite2DShader *m_sprite2dShader;
	Sprite3DShader *m_sprite3dShader;
	DebugShader *m_debugShader;
	
	bool m_hasNewContextRunYet;
};

inline void GraphicsDevice::SetTextureParameters(const TextureParameters &params)
{
	m_currentTextureParams = params;
}

inline bool GraphicsDevice::IsReadyToRender() const
{
	if (m_boundShader != NULL && m_boundVertexBuffer != NULL && m_shaderVertexAttribsSet)
		return true;
	else
		return false;
}

#endif
