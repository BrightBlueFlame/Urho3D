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
#ifndef URHO3D_ATTRIBUTEPROPERTY_H
#define URHO3D_ATTRIBUTEPROPERTY_H
#pragma once

#include "../Core/Object.h"

namespace Urho3D
{

class AttributeProperty : public Object
{
	URHO3D_OBJECT(AttributeProperty, Object);

public:
	AttributeProperty(Context* context);

private:
	Context* context_;
};

class EasyStringProperty : public AttributeProperty
{
	URHO3D_OBJECT(EasyStringProperty, AttributeProperty);

public:
	EasyStringProperty(Context* context,const String& value);

	const String& GetValue() const { return value_; }

private:
	String value_;
};

namespace P {
	static SharedPtr<AttributeProperty> EasyString(Context* context, const String& value)
	{
		return SharedPtr<AttributeProperty>(new EasyStringProperty(context, value));
	}
} // namespace P
} // namespace Urho3D

#endif
