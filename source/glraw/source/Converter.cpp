
#include <glraw/Converter.h>

#include <glraw/AssetInformation.h>


namespace glraw
{

Converter::Converter()
:   m_format(GL_RGBA)
,   m_type(GL_UNSIGNED_BYTE)
{
}

Converter::~Converter()
{
}

QByteArray Converter::convert(QImage & image, AssetInformation & info)
{
    m_canvas.loadTextureFromImage(image);
    
    if (hasFragmentShader() && !m_canvas.process(m_fragmentShader, m_uniforms))
        return QByteArray();
    
    info.setProperty("format", QVariant(static_cast<int>(m_format)));
    info.setProperty("type", QVariant(static_cast<int>(m_type)));
    info.setProperty("mipmapLevel", QVariant(static_cast<int>(m_mipmapLevel)));

    QByteArray imageFromCanvas = m_canvas.imageFromTexture(m_format, m_type, m_mipmapLevel);

    if (m_mipmapLevel != 0)
    {
	    GLint width, height;
	    m_canvas.getImageDimensions(width, height);
	    info.setProperty("width", width);
	    info.setProperty("height", height);
    }
    
    return imageFromCanvas;
}

void Converter::setFormat(GLenum format)
{
    m_format = format;
}
    
void Converter::setType(GLenum type)
{
    m_type = type;
}

} // namespace glraw
