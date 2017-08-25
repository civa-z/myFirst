#include <Python.h>
#include <iostream>
#include <vector>

int main(){
	PyObject* pyName;
	PyObject* pyModule;
	PyObject* pyDict;
	PyObject* pyFunc;
	PyObject* pyArgs;

	//初始化python解释器
	Py_Initialize();
	if (!Py_IsInitialized()) {
		return false;
	}

	//开始使用PyRun_SimpleString 运行python语句， 设置被调用的python脚本的全路径
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");

	//导入python脚本文件testpy.py，此处的脚本文件名不需要加扩展名
	pyName = PyString_FromString("mypy");

	//加载python模型,作用类似python的 import
	pyModule = PyImport_Import(pyName); if (!pyModule) {
		printf("can't find mypy.py\n");
		return false;
	}

	//取得模块接口字典信息
	pyDict = PyModule_GetDict(pyModule);
	if (!pyDict) {
		return false;
	}

	//获取需要使用的接口句柄
	pyFunc = PyDict_GetItemString(pyDict, "doubleArray");
	if (!pyFunc || !PyCallable_Check(pyFunc)) {
		printf("can't find function [doubleArray]\n");
		return false;
	}

	//开始准备调用python函数, 接口的输入和输出均为size为5的Tuple类型, Tuple元素均为double
	PyObject* pytuple = PyTuple_New(5);
	for (int i = 0; i < 5; ++i) {
		PyTuple_SetItem(pytuple, i, Py_BuildValue("d", (double)i));
	}
	pyArgs = PyTuple_New(1);
	PyTuple_SetItem(pyArgs, 0, pytuple);

	//调用接口，取得返回值
	PyObject* pyResult = PyObject_CallObject(pyFunc, pyArgs);
	
	//解析结果
	int size = PyList_Size(pyResult);
	std::vector<double> result;
	double tmp;
	for (int i = 0; i < size; ++i) {
		PyArg_Parse(PyList_GET_ITEM(pyResult, i), "d", &tmp);
		result.push_back(tmp);
	}

	//打印处理结果
	for (auto item : result) {
		std::cout << item << " ";
	}
	std::cout<<std::endl;

	//删除python object变量
	Py_DECREF(pyArgs);
	Py_DECREF(pyFunc);
	Py_DECREF(pyDict);
	Py_DECREF(pyModule);
	Py_DECREF(pyName);

	return 0;
}
