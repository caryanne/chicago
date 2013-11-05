#ifndef _MOVEABLEOBJECT_H_
#define _MOVEABLEOBJECT_H_

#include <string>
using namespace std;

class MoveableObject {
private:
	bool mVisible;
	string mName;

public:
	MoveableObject();
	MoveableObject(const string& name, bool visible) { mName = name; mVisible = visible; }
	virtual const string& getName() { return mName; }
	virtual void setName(const string& name) { mName = name; }
	virtual bool getVisible() { return mVisible; }
	virtual void setVisible(bool visible) { mVisible = visible; }

};

#endif