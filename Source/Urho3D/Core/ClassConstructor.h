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

#ifndef U3D_CLASSCONSTRUCTOR_H_
#define U3D_CLASSCONSTRUCTOR_H_
#pragma once

#include "../Core/Context.h"
#include "../Core/Object.h"
#include "../Core/Attribute.h"
#include "../Core/ClassDef.h"

#include "../Scene/Serializable.h"

#include "../Container/Ptr.h"
#include "../Container/HashMap.h"
#include "../Container/RefCounted.h"

#include "../Math/StringHash.h"

#include <cstdio>

namespace Urho3D
{


template <class T>
class ClassConstructor
{
public:
	ClassConstructor(Context* context, WeakPtr<ClassDef> info);

	template <class U> 
	ClassConstructor<T>& Implements();

	template <class U> 
	ClassConstructor<T>& Base(bool copy = false);

	ClassConstructor<T>& Factory();
	ClassConstructor<T>& Factory(const char* category);
    
    template <class U>
    ClassConstructor<T>& Attribute(const char* name, size_t offset, U defaultValue, unsigned mode = AM_DEFAULT)
    {
        context_->RegisterAttribute<T>(Urho3D::AttributeInfo(GetVariantType<U>(), name, offset, defaultValue, mode));
        return *this;
    }

	template <class U> 
	ClassConstructor<T>& Attribute(const char* name, U (T::*getter)() const, void (T::*setter)(U), U defaultValue, unsigned mode = AM_DEFAULT);
    
    template <class U>
    ClassConstructor<T>& Attribute(const char* name, U (T::*getter)() const, void (T::*setter)(const U&), U defaultValue, unsigned mode = AM_DEFAULT);
    
    template <class U>
    ClassConstructor<T>& Attribute(const char* name, const U& (T::*getter)() const, void (T::*setter)(const U&), U defaultValue, unsigned mode = AM_DEFAULT);

	template <class U> 
	ClassConstructor<T>& Prop();

	template <class U> 
	ClassConstructor<T>& Prop(const VariantMap& constructorData);
    
    ClassConstructor<T>& Remove(const char* attrib)
    {
        context_->RemoveAttribute<T>(attrib);
        return *this;
    }
    
    ClassConstructor<T>& UpdateDefault(const char* name, const Variant& newValue)
    {
        context_->UpdateAttributeDefaultValue<T>(name, newValue);
        return *this;
    }

	ClassConstructor<T>& operator[](SharedPtr<AttributeProperty> property)
	{
		if(!lastAttribute_)
			classDef_->AddProperty(property);
		else
			classDef_->AddProperty(lastAttribute_,property);
		return *this;
	}
    
    Context* GetContext() const { return context_; }

private:
	/// Runtime context.
	Context* context_;
	/// The last attribute added to Context.
	AttributeInfo* lastAttribute_;
	/// The class info we will be returning.
	WeakPtr<ClassDef> classDef_;
};

template <class T>
ClassConstructor<T>::ClassConstructor(Context* context, WeakPtr<ClassDef> classDef)
: context_(context)
, lastAttribute_(0)
, classDef_(classDef)
{
	assert(classDef_);
}

template <class T>
template <class U>
ClassConstructor<T>& ClassConstructor<T>::Implements()
{
	static_assert(U3D_Traits::is_interface<U>::value == true, "Attempted to register interface that is not an interface.");
	classDef_->AddInterface(U::GetInterfaceTypeStatic());
	return *this;
}

template <class T>
template <class U> 
ClassConstructor<T>& ClassConstructor<T>::Base(bool copy)
{
    if(copy && context_)
    {
        context_->CopyBaseAttributes<U,T>();
    }
    classDef_->AddBase(WeakPtr<ClassDef>(U::GetClassDefStatic()));
    
	return *this;
}

template <class T>
ClassConstructor<T>& ClassConstructor<T>::Factory()
{
	if(context_)
	{
		context_->RegisterFactory<T>();
	}
	return *this;
}

template <class T>
ClassConstructor<T>& ClassConstructor<T>::Factory(const char* category)
{
	if(context_)
	{
		context_->RegisterFactory<T>(category);
	}
	return *this;
}

template <class T>
template <class U> 
ClassConstructor<T>& ClassConstructor<T>::Attribute(const char* name, U (T::*getter)() const, void (T::*setter)(U), U defaultValue, unsigned mode)
{
    if(context_)
    {
        context_->RegisterAttribute<T>(AttributeInfo(GetVariantType<U>(), name, new AttributeAccessorImpl<T, U, AttributeTrait<U> >(getter, setter), defaultValue, mode));
        lastAttribute_ = context_->GetAttribute<T>(name);
    }
	return *this;
}
    
template <class T>
template <class U>
ClassConstructor<T>& ClassConstructor<T>::Attribute(const char* name, U (T::*getter)() const, void (T::*setter)(const U&), U defaultValue, unsigned mode)
{
    if(context_)
    {
        context_->RegisterAttribute<T>(AttributeInfo(GetVariantType<U>(), name, new AttributeAccessorImpl<T, U, MixedAttributeTrait<U> >(getter, setter), defaultValue, mode));
        lastAttribute_ = context_->GetAttribute<T>(name);
    }
    return *this;
}

template <class T>
template <class U>
ClassConstructor<T>& ClassConstructor<T>::Attribute(const char* name, const U& (T::*getter)() const, void (T::*setter)(const U&), U defaultValue, unsigned mode)
{
    if(context_)
    {
        context_->RegisterAttribute<T>(AttributeInfo(GetVariantType<U>(), name, new AttributeAccessorImpl<T, U, AttributeTrait<U> >(getter, setter), defaultValue, mode));
        lastAttribute_ = context_->GetAttribute<T>(name);
    }
    return *this;
}

template <class T>
template <class U> 
ClassConstructor<T>& ClassConstructor<T>::Prop()
{
	if(lastAttribute_)
	{
		// classDef_->AddProperty(lastAttribute_, new U());
	}
	else
	{
		// classDef_->AddProperty(new U());
	}
	return *this;
}

template <class T>
template <class U> 
ClassConstructor<T>& ClassConstructor<T>::Prop(const VariantMap& constructorData)
{
	if(lastAttribute_)
	{
		// classDef_->AddProperty(lastAttribute_, new U(constructorData));
	}
	else
	{
		// classDef_->AddProperty(new U(constructorData));
	}
	return *this;
}

}

#endif
