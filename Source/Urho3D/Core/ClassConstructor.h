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

#include "../Scene/Serializable.h"

#include "../Container/Ptr.h"
#include "../Container/HashMap.h"
#include "../Container/RefCounted.h"

#include "../Math/StringHash.h"

#include <cstdio>

#define URHO_REGISTER_DECLARE \
    public: \
        static void RegisterObject(Urho3D::Context* context); \
        static void DefineMyAttributes(Urho3D::ClassConstructor<ClassName>& Definition); \

#define URHO_REGISTER_OBJECT(typeName) \
	void typeName::RegisterObject(Urho3D::Context* context) \
	{ \
		const Urho3D::TypeInfo* typeInfo = typeName::GetTypeInfoStatic(); \
		Urho3D::ClassConstructor<typeName> cc(context, const_cast<Urho3D::TypeInfo*>(typeInfo)); \
        typeName::DefineMyAttributes(cc); \
	} \
    void typeName::DefineMyAttributes(Urho3D::ClassConstructor<typeName>& Definition) \

namespace Urho3D
{

class TypeInfo;

template <class T>
class ClassConstructor
{
public:
	ClassConstructor(Context* context, TypeInfo* info);

	template <class U>
	ClassConstructor<T>& Implements();
    
    template <class U>
    ClassConstructor<T>& CopyBase();

	ClassConstructor<T>& Factory();
	ClassConstructor<T>& Factory(const char* category);

    template <class U>
    ClassConstructor<T>& Attribute(const char* name, size_t offset, U defaultValue, unsigned mode = AM_DEFAULT);

	template <class U>
	ClassConstructor<T>& Attribute(const char* name, U (T::*getter)() const, void (T::*setter)(U), U defaultValue, unsigned mode = AM_DEFAULT);

    template <class U>
    ClassConstructor<T>& Attribute(const char* name, U (T::*getter)() const, void (T::*setter)(const U&), U defaultValue, unsigned mode = AM_DEFAULT);

    template <class U>
    ClassConstructor<T>& Attribute(const char* name, const U& (T::*getter)() const, void (T::*setter)(const U&), U defaultValue, unsigned mode = AM_DEFAULT);

    ClassConstructor<T>& Remove(const char* attrib);

    ClassConstructor<T>& UpdateDefault(const char* name, const Variant& newValue);

	ClassConstructor<T>& operator[](AttributeProperty* property);

    Context* GetContext() const { return context_; }

private:
	/// Runtime context.
	Context* context_;
	/// The last attribute added to Context.
	AttributeInfo* lastAttribute_;
	/// The class info we will be returning.
	TypeInfo* typeInfo_;
};

template <class T>
ClassConstructor<T>::ClassConstructor(Context* context, TypeInfo* typeInfo)
: context_(context)
, lastAttribute_(0)
, typeInfo_(typeInfo)
{
	assert(context_);
	assert(typeInfo_);
}

template <class T>
template <class U>
ClassConstructor<T>& ClassConstructor<T>::Implements()
{
	typeInfo_->AddInterface(U::GetInterfaceTypeInfoStatic());
	return *this;
}
    
template <class T>
template <class U>
ClassConstructor<T>& ClassConstructor<T>::CopyBase()
{
    context_->CopyBaseAttributes<T,U>();
    return *this;
}

template <class T>
ClassConstructor<T>& ClassConstructor<T>::Factory()
{
	
	context_->RegisterFactory<T>();
	return *this;
}

template <class T>
ClassConstructor<T>& ClassConstructor<T>::Factory(const char* category)
{
	context_->RegisterFactory<T>(category);
	return *this;
}

template <class T>
template <class U>
ClassConstructor<T>& ClassConstructor<T>::Attribute(const char* name, size_t offset, U defaultValue, unsigned mode)
{
    context_->RegisterAttribute<T>(AttributeInfo(GetVariantType<U>(), name, offset, defaultValue, mode));
    return *this;
}

template <class T>
template <class U>
ClassConstructor<T>& ClassConstructor<T>::Attribute(const char* name, U (T::*getter)() const, void (T::*setter)(U), U defaultValue, unsigned mode)
{
    context_->RegisterAttribute<T>(AttributeInfo(GetVariantType<U>(), name, new AttributeAccessorImpl<T, U, AttributeTrait<U> >(getter, setter), defaultValue, mode));
    lastAttribute_ = context_->GetAttribute<T>(name);
	return *this;
}

template <class T>
template <class U>
ClassConstructor<T>& ClassConstructor<T>::Attribute(const char* name, U (T::*getter)() const, void (T::*setter)(const U&), U defaultValue, unsigned mode)
{
    context_->RegisterAttribute<T>(AttributeInfo(GetVariantType<U>(), name, new AttributeAccessorImpl<T, U, MixedAttributeTrait<U> >(getter, setter), defaultValue, mode));
	lastAttribute_ = context_->GetAttribute<T>(name);
    return *this;
}

template <class T>
template <class U>
ClassConstructor<T>& ClassConstructor<T>::Attribute(const char* name, const U& (T::*getter)() const, void (T::*setter)(const U&), U defaultValue, unsigned mode)
{
    context_->RegisterAttribute<T>(AttributeInfo(GetVariantType<U>(), name, new AttributeAccessorImpl<T, U, AttributeTrait<U> >(getter, setter), defaultValue, mode));
	lastAttribute_ = context_->GetAttribute<T>(name);
    return *this;
}

template <class T>
ClassConstructor<T>& ClassConstructor<T>::Remove(const char* attrib)
{
    context_->RemoveAttribute<T>(attrib);
    return *this;
}

template <class T>
ClassConstructor<T>& ClassConstructor<T>::UpdateDefault(const char* name, const Variant& newValue)
{
    context_->UpdateAttributeDefaultValue<T>(name, newValue);
    return *this;
}

template <class T>
ClassConstructor<T>& ClassConstructor<T>::operator[](AttributeProperty* property)
{
	if(!lastAttribute_)
		typeInfo_->AddProperty(property);
	else
		typeInfo_->AddProperty(lastAttribute_,property);
	return *this;
}

}

#endif
