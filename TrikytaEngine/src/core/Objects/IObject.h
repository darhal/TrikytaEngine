#pragma once

class IObject // Uncopyable atm!
{
public:
	IObject(){};
	virtual ~IObject() {};
protected:

private:
	IObject(const IObject&) = delete;
	IObject& operator=(IObject&&) = delete;
};
