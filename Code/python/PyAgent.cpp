#include <Python.h>
#include "PyAgent.h"
#include <QString>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include "moduleBase/messageWindowBase.h"
#include "mainWindow/mainWindow.h"
#include "PyInterpreter.h"
#include "ScriptReader.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <wchar.h>

#ifdef Q_OS_WIN32
#include <atlconv.h>
#endif

#include <locale.h>

namespace Py
{
	PythonAagent* PythonAagent::_instance = nullptr;

	PythonAagent* PythonAagent::getInstance()
	{
		if (_instance == nullptr)
			_instance = new PythonAagent;

		return _instance;
	}

	void PythonAagent::connectSignals()
	{
		connect(this, SIGNAL(printInfo(int, QString)), _mainWindow, SIGNAL(printMessageSig(int, QString)));
	}

	void PythonAagent::appCodeList(QString code)
	{
		if (!_append) return;
		emit printInfo(ModuleBase::Python, code);
		_interpreter->codeListAppend(code);
	}

	PythonAagent::PythonAagent()
	{
		_interpreter = new PyInterpreter;
	}

	void PythonAagent::initialize(GUI::MainWindow* m)
	{
		_mainWindow = m;
		connectSignals();

		QString path = qApp->applicationDirPath() + "/../python37";
		path = QDir::cleanPath(path);
		char*  ch;
		QByteArray ba = path.toLocal8Bit();
		ch = ba.data();
		wchar_t* wc;

#ifdef Q_OS_WIN32
 		USES_CONVERSION;
 		 wc = A2W(ch);
#endif

#ifdef Q_OS_LINUX
		setlocale(LC_CTYPE,"zh_CN.utf8");
		int w_size = mbstowcs(NULL, ba, 0) + 1;
		wc = new wchar_t[w_size];
		mbstowcs(wc, ba, strlen(ba) + 1);
#endif
		

		QDir d(path);
		if (d.exists())
		{
#ifdef Q_OS_WIN32
			Py_SetPythonHome(wc);
#endif
			Py_Initialize();
			if (!_interpreter->init(this))
				emit printInfo((int)ModuleBase::Error_Message, tr("Python Initialize failed!"));
			else
				emit printInfo((int)ModuleBase::Normal_Message, tr("Python Initialized"));
		}
	}

	void PythonAagent::finalize()
	{
		if (Py_IsInitialized())
			Py_Finalize();
		
		if (_reader != nullptr)
		{
			if (_reader->isRunning())
				_reader->quit();
			delete _reader;
			_reader = nullptr;
		}
		if (_interpreter != nullptr) delete _interpreter;

	}

	void PythonAagent::submit(QString code, bool s)
	{
//		this->lock();
		qDebug() << "submit: " << code;
		emit printInfo(ModuleBase::Python, code);
//		lock();
		int ok = _interpreter->execCode(code,s);
		if (ok == -1)
		{
			if (_reader != nullptr)
				_reader->restart();
		}
		
	}	

	void PythonAagent::submit(QStringList codes, bool save /*= true*/)
	{
		const int n = codes.size();
		for (int i = 0; i < n; ++i)
		{
			this->submit(codes.at(i), save);
		}
	}

	void PythonAagent::saveScript(QString filename)
	{
		QFile file(filename);
		if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
		{
			emit printInfo(ModuleBase::Error_Message, tr("Script open failed"));
			return;
		}
		QTextStream stream(&file);
		const int n = _interpreter->getCodeCount();
		for (int i = 0; i < n; ++i)
		{
			QString s = _interpreter->getCodeAt(i);
			stream << s << endl;
		}
		file.close();
		emit printInfo(ModuleBase::Normal_Message, tr("Script Saved %1").arg(filename));
	}

	void PythonAagent::execScript(QString filename)
	{
		if (_reader != nullptr) return;
		_reader = new ScriptReader(filename, this);
		connect(_reader, SIGNAL(finished()), this, SLOT(readerFinished()));
		_reader->start();
	}

	void PythonAagent::readerFinished()
	{
		if (_reader != nullptr)
			delete _reader;
		_reader = nullptr;
	}

	void PythonAagent::lock()
	{
		if (_reader != nullptr)
			_reader->pause();
	}

	void PythonAagent::unLock()
	{
		if (_reader != nullptr)
			_reader->restart();
	}

	bool PythonAagent::isLocked()
	{
		return _islock;
	}

	QStringList PythonAagent::getcodelist()
	{
		return _interpreter->getCode();
	}


	void PythonAagent::appendOn()
	{
		_append = true;
	}
	void PythonAagent::appendOff()
	{
		_append = false;
	}

	void PythonAagent::execMessWinCode(QString code)
	{
		_interpreter->execCode(code);
	}

	void PythonAagent::backstageExec(QString code)
	{
		_interpreter->execCode(code, false);
	}

}