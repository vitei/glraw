#pragma once

#include <QStringList>
#include <QMap>
#include <QCommandLineParser>
#include <QScopedPointer>

#include <glraw/ConvertManager.h>


class QCoreApplication;

namespace glraw
{
    class ImageEditorInterface;
    class FileWriter;
    class AbstractConverter;
}

struct CommandLineOption;

class Builder
{
public:
    using ConfigureMethod = bool (Builder::*)(const QString &);
    
    Builder();
    ~Builder();

    void process(const QCoreApplication & app);
    
protected:
    static QList<CommandLineOption> commandLineOptions();

protected:
    void initialize();
    
    bool help(const QString & name);
    bool outputPath(const QString & name);
    bool quiet(const QString & name);
    bool noSuffixes(const QString & name);
    bool format(const QString & name);
    bool type(const QString & name);
    bool compressedFormat(const QString & name);
    bool raw(const QString & name);
    bool mirrorVertical(const QString & name);
    bool mirrorHorizontal(const QString & name);
    bool scale(const QString & name);
    bool widthScale(const QString & name);
    bool heightScale(const QString & name);
    bool width(const QString & name);
    bool height(const QString & name);
    bool transformMode(const QString & name);
    bool aspectRatioMode(const QString & name);
    bool shader(const QString & name);
    bool uniform(const QString & name);
    bool mipmapLevel(const QString & name);

protected:
    bool editorExists(const QString & key);
    void appendEditor(const QString & key, glraw::ImageEditorInterface * editor);
    
    template <class Editor>
    Editor * editor(const QString & key);

    bool configureShader();
    
    void showHelp() const;

protected:
    QCommandLineParser m_parser;
    QMap<QString, ConfigureMethod> m_configureMethods;

    QString m_shaderSource;
    QStringList m_uniformList;

    QMap<QString, glraw::ImageEditorInterface *> m_editors;
    glraw::AbstractConverter * m_converter;
    glraw::FileWriter * m_writer;
    glraw::ConvertManager m_manager;
    int m_mipmapLevel;

};

#include "Builder.hpp"
