#include "qscilexerbin.h"

#include <QColor>
#include <QFont>
#include <QSettings>

QsciLexerBin::QsciLexerBin(QObject */*parent*/)
{
#ifdef Q_WS_MAC
   setDefaultFont(QFont("Monaco", 11));
#endif
#ifdef Q_WS_X11
   setDefaultFont(QFont("Monospace", 10));
#endif
#ifdef Q_WS_WIN
   setDefaultFont(QFont("Consolas", 11));
#endif
}

QsciLexerBin::~QsciLexerBin()
{
}

QString QsciLexerBin::description(int style) const
{
   // Note: this function MUST return a non-empty string
   //       for a style otherwise that style is ignored!
   switch ( style )
   {
      case Bin_Default:
         return "Default";
      break;
   }
}

void QsciLexerBin::styleText(int start, int end)
{
   // Reset line styling.
   startStyling(start,0xFF);
   setStyling(end-start,Bin_Default);
}

QColor QsciLexerBin::defaultColor() const
{
   return QColor(0,0,0);
}

QColor QsciLexerBin::defaultColor(int style) const
{
   switch ( style )
   {
      case Bin_Default:
         return QColor(50,50,50);
      break;
      default:
         return defaultColor();
      break;
   }
}

QColor QsciLexerBin::defaultPaper() const
{
   return QColor(255,255,255);
}

QColor QsciLexerBin::defaultPaper(int /*style*/) const
{
   return QsciLexer::defaultPaper();
}

QColor QsciLexerBin::paper(int /*style*/) const
{
   return QsciLexer::defaultPaper();
}

QFont QsciLexerBin::defaultFont() const
{
#ifdef Q_WS_MAC
   return QFont("Monaco", 11);
#endif
#ifdef Q_WS_X11
   return QFont("Monospace", 10);
#endif
#ifdef Q_WS_WIN
   return QFont("Consolas", 11);
#endif
}

QFont QsciLexerBin::defaultFont(int style) const
{
   QFont font = QsciLexerBin::defaultFont();

   return font;
}
