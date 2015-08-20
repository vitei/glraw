#pragma once

#include <QtGui/qopengl.h>

#include <QString>


#include <glraw/glraw_api.h>


namespace glraw
{


class GLRAW_API FileNameSuffix
{
public:
	FileNameSuffix(const QString & fileName);

	FileNameSuffix(
		int width,
		int height,
		GLenum format,
		GLenum type,
		GLint mipmapLevel);

	FileNameSuffix(
		int width,
		int height,
		GLenum compressedType,
		GLint mipmapLevel);

	/** \return Returns true, if width, height, type, and format if not compressed have valid values.
	*/
	bool isValid() const;

	/** \return Returns width of raw texture file name suffix in pixel;
				Is -1 when no width is provided.
	*/
	int width()  const;

	/** \return Returns height of raw texture file name suffix in pixel; 
				Is -1 when no height is provided.
	*/
	int height() const;

	/** \return Returns mipmap level (where 0 is the main image) of raw texture file name suffix;
				Is -1 when no count is provided.
	*/
	GLint mipmapLevel() const;

	/** \return Returns type of raw texture file name suffix; 
				Is GL_INVALID_ENUM when type is not unknown or not provided.
	*/
	GLenum type() const;

	/** \return Returns format of raw texture file name suffix; 
				Is GL_INVALID_ENUM when format is not unknown or not provided.
	*/
	GLenum format() const;

	/** \return Returns true if texture type is compressed and no extra format is given; false otherwise.
	    \remark When type is of GL_INVALID_ENUM, returns false.
	*/
	bool compressed() const;

	/** \return raw texture file name suffix as QString.
	*/
	const QString & get() const;

protected:
	static const QString formatSuffix(GLenum format);
	static const QString typeSuffix(GLenum type);

	static GLenum format(const QString & suffix);
	static GLenum type(const QString & suffix);

protected:
	QString m_suffix;

	bool m_compressed;

	int m_width;
	int m_height;
	
	GLenum m_format;
	GLenum m_type;
	GLint m_mipmapLevel;
};

} // namespace glraw
