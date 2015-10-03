//
// Copyright (c) 2008-2015 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "../Precompiled.h"
#include "../Core/ClassDef.h"

namespace Urho3D
{

ClassDef::ClassDef(Context* context, StringHash classId):
    context_(context),
    classId_(classId),
    closed_(false)
{
}

void ClassDef::AddInterface(WeakPtr<ClassDef> interface)
{
	if(!closed_)
	{
		if(interfaces_.Find(interface) == interfaces_.End())
		{
			interfaces_.Push(interface);
		}
	}
}

void ClassDef::AddBase(WeakPtr<ClassDef> classInfo)
{
	if(!closed_)
	{
		if(bases_.Find(classInfo) == bases_.End())
		{
			bases_.Push(classInfo);
		}
	}
}

void ClassDef::AddProperty(SharedPtr<Property> prop)
{
	if(!closed_)
	{
		if(classProperties_.Find(prop) == classProperties_.End())
		{
			classProperties_.Push(prop);
		}
	}
}

void ClassDef::AddProperty(AttributeInfo* attrib, SharedPtr<Property> prop)
{
	if(!closed_)
	{
		if(classProperties_.Find(prop) == classProperties_.End())
		{
			classProperties_.Push(prop);
		}
	}
}

void ClassDef::Close()
{
	closed_ = true;
    
}

Property::Property(Context* context)
: Object(context)
{
}

REGISTER_OBJECT(Property)
{
	Definition
        .Base<Object>()
    ;
}

}
