#ifndef PYTHON_MACROS_H
#define PYTHON_MACROS_H

#include <Python.h>


#if PY_MAJOR_VERSION >= 3

#define PY_STRING_AS_STRING(obj) PyUnicode_AsUTF8(obj)
#define PY_STRING_FROM_STRING(obj) PyUnicode_FromString(obj)
#define PY_STRING_FROM_FORMAT PyUnicode_FromFormat

#define PY_INT_FROM_LONG PyLong_FromLong
#define PY_INT_AS_LONG PyLong_AsLong
#define PY_INT_CHECK(obj) PyLong_Check(obj)

#else

#define PY_STRING_AS_STRING(obj) PyString_AsString(obj)
#define PY_STRING_FROM_STRING(obj) PyString_FromString(obj)
#define PY_STRING_FROM_FORMAT PyString_FromFormat

#define PY_INT_FROM_LONG PyInt_FromLong
#define PY_INT_AS_LONG PyInt_AsLong
#define PY_INT_CHECK(obj) PyInt_Check(obj)

#endif

#endif
