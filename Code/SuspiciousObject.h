#pragma once
#include <QString>
#include <iostream>
#include <string>
class SuspiciousObject
{
public:

	int X, Y, Z, length, width, high, weight;
	int Code;
	QString type;
	char*types;
	QString uid;//ø…“…ŒÔµƒUID	
	SuspiciousObject(int code,int x, int y, int z, int l, int w, int h, int we, QString t) :Code(code),X(x), Y(y), Z(z), length(l), width(w), high(h), weight(we), type(t) 
	{
		QByteArray arr = type.toLocal8Bit();
		types = new char[arr.size() + 1];
		strcpy_s(types, arr.size() + 1, arr.data());
	};
	~SuspiciousObject();
};
