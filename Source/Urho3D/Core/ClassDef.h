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

#ifndef U3D_CLASSDEF_H
#define U3D_CLASSDEF_H
#pragma once

#include "../Core/Attribute.h"

namespace Urho3D
{
    
class ClassDef : public RefCounted
{
    typedef WeakPtr<ClassDef> CDWeakPtr;
    
public:
    ClassDef(const String& name, StringHash classId);
    
    const String& GetName() const { return className_; }
    StringHash GetId() const { return classId_; }
    
    const Vector<CDWeakPtr>& GetBaseClasses() const { return bases_; }
    
    void AddInterface(StringHash interface);
    void AddBase(CDWeakPtr classInfo);
    //void AddProperty(SharedPtr<Property> prop);
    //void AddProperty(AttributeInfo* attrib, SharedPtr<Property> prop);
    
    void Close();
    
    unsigned GetBaseCount() const { return bases_.Size(); }
    
private:
    /// The name as a human-readable string.
    String className_;
    
    /// The class id.
    StringHash classId_;
    
    /// Properties for the whole class.
    //Vector<SharedPtr<Property> > classProperties_;
    
    /// The properties for each attribute.
    //HashMap<AttributeInfo*, Vector<SharedPtr<Property> > > properties_;
    
    /// Interfaces that this class implements.
    Vector<StringHash> interfaces_;
    
    /// Base classes of this class.
    Vector<CDWeakPtr> bases_;
    
    /// Can we still add to this?
    bool closed_;
};
    
/*class Property : public Object
{
    URHO_OBJECT(Property);
        
public:
    Property(Context* context);
};*/

}
#endif
