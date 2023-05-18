#pragma once
#include <Python.h>
#include <string>
#include <iostream>
inline std::string getMove(std::string fen)
{
	PyObject* pInt;

	Py_Initialize();

	PyObject* myModuleString = PyUnicode_FromString((char*)"polyglot");
	PyObject* myModule = PyImport_Import(myModuleString);

	PyObject* myFunction = PyObject_GetAttrString(myModule, (char*)"getMove");
	PyObject* args = PyTuple_Pack(1, PyUnicode_FromString(fen.c_str()));

	PyObject* myResult = PyObject_CallObject(myFunction, args);
	auto kazkas = Py_TYPE(myResult);
	std::string moves = PyUnicode_AsUTF8(myResult);

	Py_Finalize();
	return moves;
}