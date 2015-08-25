#pragma once

#include <QWindow>
#include <QOpenGLFunctions_3_2_Core>

#include <glraw/glraw_api.h>

class QImage;
class QByteArray;

class QOpenGLFunctions_3_2_Core;


namespace glraw
{

class GLRAW_API Canvas : public QWindow
{
public:
    Canvas();
    virtual ~Canvas();

    void initializeGL();

    void loadTextureFromImage(const QImage & image);
    QByteArray imageFromTexture(GLenum format, GLenum type, GLint mipmapLevel);
    QByteArray compressedImageFromTexture(GLenum compressedInternalFormat, GLint mipmapLevel);

    bool process(
        const QString & fragmentShader
    ,   const QMap<QString, QString> & uniforms);

    bool textureLoaded() const;
    void getImageDimensions(GLint & width, GLint & height) const;

protected:
    static int byteSizeOf(GLenum type);
    static int numberOfElementsFor(GLenum format);
    
    QOpenGLContext m_context;
    GLuint m_texture;   

    // These are the dimensions of the image that we actually load.
    // They will differ from the main image if a mipmap level greater
    // than 0 is specified.
    GLint m_width;
    GLint m_height;

    // using gl as a memeber instead of inheritance 
    // probably resolves an deinitialization issue.
    QOpenGLFunctions_3_2_Core * m_gl;
};

} // namespace glraw
