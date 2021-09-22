#ifndef KIJANGPROTOCOL_GLOBAL_H
#define KIJANGPROTOCOL_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QBuffer>

#if defined(KIJANGPROTOCOL_LIBRARY)
#  define KIJANGPROTOCOL_EXPORT Q_DECL_EXPORT
#else
#  define KIJANGPROTOCOL_EXPORT Q_DECL_IMPORT
#endif

#endif // KIJANGPROTOCOL_GLOBAL_H