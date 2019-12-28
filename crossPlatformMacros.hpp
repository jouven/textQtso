//because windows sucks...

#ifndef TEXTQTSO_CROSSPLATFORMMACROS_HPP
#define TEXTQTSO_CROSSPLATFORMMACROS_HPP

#include <QtCore/QtGlobal>

//remember to define this variable in the .pro file
#if defined(TEXTQTSO_LIB)
#  define EXPIMP_TEXTQTSO Q_DECL_EXPORT
#else
#  define EXPIMP_TEXTQTSO Q_DECL_IMPORT
#endif

#endif // TRANSLATORJSONQTSO_CROSSPLATFORMMACROS_HPP
