/*
** Copyright (c) 2007, TUBITAK/UEKAE
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 2 of the License, or (at your
** option) any later version. Please read the COPYING file.
*/

#include "python_macros.h"
#include "catbox.h"
#include <structmember.h>



typedef struct {
	PyObject_HEAD
	int code;
	PyObject *violations;
} RetVal;

static PyMemberDef members[] = {
	{ "code", T_INT, offsetof(RetVal, code), 0, NULL },
	{ "violations", T_OBJECT, offsetof(RetVal, violations), 0, NULL },
	{ NULL, 0, 0, 0, NULL }
};


static PyTypeObject RetVal_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
};


static int is_initialized = 0;

int
catbox_retval_init(struct trace_context *ctx)
{
	RetVal *ret;

	if (!is_initialized) {
		RetVal_type.tp_name = "catbox.RetVal";
		RetVal_type.tp_basicsize = sizeof(RetVal);
		RetVal_type.tp_itemsize = 0;
		RetVal_type.tp_flags = Py_TPFLAGS_DEFAULT;
		RetVal_type.tp_doc = "Sandbox log";
		RetVal_type.tp_members = members;
		RetVal_type.tp_new = PyType_GenericNew;

		if (PyType_Ready(&RetVal_type) < 0)
			return -1;

		Py_INCREF(&RetVal_type);
		is_initialized = 1;
	}

	ret = PyObject_New(RetVal, &RetVal_type);
	if (ret == NULL)
		return -1;

	ret->code = 0;
	ret->violations = PyList_New(0);

	if (ret->violations == NULL) {
		Py_DECREF(ret);
		return -1;
	}

	ctx->retval = (PyObject *)ret;

	return 0;
}

void
catbox_retval_set_exit_code(struct trace_context *ctx, int retcode)
{
	RetVal *ret = (RetVal *) ctx->retval;

	ret->code = retcode;
}

void
catbox_retval_add_violation(struct trace_context *ctx, const char *operation, const char *path, const char *canonical)
{
	RetVal *ret = (RetVal *) ctx->retval;
	PyObject *item;

	item = PyTuple_New(3);
	PyTuple_SetItem(item, 0, PY_STRING_FROM_STRING(operation));
	PyTuple_SetItem(item, 1, PY_STRING_FROM_STRING(path));
	PyTuple_SetItem(item, 2, PY_STRING_FROM_STRING(canonical));
	PyList_Append(ret->violations, item);

	if (ctx->logger) {
		PyObject_Call(ctx->logger, item, NULL);
	}
}
