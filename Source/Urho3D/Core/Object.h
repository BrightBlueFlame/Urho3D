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

#pragma once

#include "../Container/LinkedList.h"
#include "../Core/Variant.h"
#include "../Core/ClassDef.h"

namespace U3D_Traits {
    template <class T>
    struct is_base
    {
        static const bool value = false;
    };
    
    template <class T>
    struct is_interface
    {
        static const bool value = false;
    };
}

namespace Urho3D
{

class Context;
class EventHandler;
    
template <class T> class ClassConstructor;

#define __CLASSINFOHOLDERGETTER_(TYPE_NAME) \
	static const ClassInfoHolder& __GetClassInfoHolderStatic() \
	{ \
		static const ClassInfoHolder infoStatic(#TYPE_NAME); \
		return infoStatic; \
	} \

#define __INTERFACEINFOHOLDERGETTER_(TYPE_NAME) \
	static const InterfaceInfoHolder& __GetInterfaceInfoHolderStatic() \
	{ \
		static const InterfaceInfoHolder infoStatic(#TYPE_NAME); \
		return infoStatic; \
	} \

#define __DEFINTERFACEINFOHOLDER__(TYPE_NAME) \
private: \
	struct InterfaceInfoHolder \
	{ \
		InterfaceInfoHolder(const char* tn): \
			typeId_(tn), \
			typeName_(tn), \
			classDef_(new Urho3D::ClassDef(typeName_, typeId_)) \
		{\
			classDef_->Close(); \
		}\
		Urho3D::StringHash           typeId_; \
		Urho3D::String               typeName_; \
		Urho3D::SharedPtr<Urho3D::ClassDef>  classDef_; \
	}; \
	__INTERFACEINFOHOLDERGETTER_(TYPE_NAME); \

#define __DEFCLASSINFOHOLDER__(TYPE_NAME) \
private: \
	struct ClassInfoHolder \
	{ \
		ClassInfoHolder(const char* tn): \
			typeId_(tn), \
			typeName_(tn), \
			classDef_(new Urho3D::ClassDef(typeName_, typeId_)) \
		{\
			classDef_->Close(); \
		}\
		Urho3D::StringHash           typeId_; \
		Urho3D::String               typeName_; \
		Urho3D::SharedPtr<Urho3D::ClassDef>  classDef_; \
	}; \
	__CLASSINFOHOLDERGETTER_(TYPE_NAME); \

#define __DEFCLASSINFOHOLDERCLOSED0__(TYPE_NAME) \
private: \
	struct ClassInfoHolder \
	{ \
		ClassInfoHolder(const char* tn): \
			typeId_(tn), \
			typeName_(tn), \
			classDef_(new Urho3D::ClassDef(typeName_, typeId_)) \
		{\
			classDef_->Close(); \
		}\
		Urho3D::StringHash           typeId_; \
		Urho3D::String               typeName_; \
		Urho3D::SharedPtr<Urho3D::ClassDef>  classDef_; \
	}; \
	__CLASSINFOHOLDERGETTER_(TYPE_NAME); \

#define __DEFCLASSINFOHOLDERCLOSED1__(TYPE_NAME, BASE_TYPE) \
private: \
	struct ClassInfoHolder \
	{ \
		ClassInfoHolder(const char* tn): \
			typeId_(tn), \
			typeName_(tn), \
			classDef_(new Urho3D::ClassDef(typeName_, typeId_)) \
		{\
			classDef_->AddBase(WeakPtr<Urho3D::ClassDef>(BASE_TYPE::GetClassDefStatic())); \
			classDef_->Close(); \
		}\
		Urho3D::StringHash           typeId_; \
		Urho3D::String               typeName_; \
		Urho3D::SharedPtr<Urho3D::ClassDef>  classDef_; \
	}; \
	__CLASSINFOHOLDERGETTER_(TYPE_NAME); \

#define URHO_INTERFACE(typeName) \
	__DEFINTERFACEINFOHOLDER__(typeName); \
    public: \
		static Urho3D::ClassDef*     GetInterfaceDefStatic() { return __GetInterfaceInfoHolderStatic().classDef_.Get(); } \
        static Urho3D::StringHash    GetInterfaceTypeStatic() { return __GetInterfaceInfoHolderStatic().typeId_; } \
        static const Urho3D::String& GetInterfaceTypeNameStatic() { return __GetInterfaceInfoHolderStatic().typeName_; } \
    
#define URHO_OBJECT(typeName) \
	__DEFCLASSINFOHOLDER__(typeName); \
    public: \
        typedef typeName ClassName; \
		static Urho3D::ClassDef*      GetClassDefStatic() { return __GetClassInfoHolderStatic().classDef_.Get(); } \
		static Urho3D::StringHash     GetTypeStatic()     { return __GetClassInfoHolderStatic().typeId_; } \
        static const Urho3D::String&  GetTypeNameStatic() { return __GetClassInfoHolderStatic().typeName_; } \
        virtual Urho3D::ClassDef*     GetClassDef() const { return GetClassDefStatic(); } \
        virtual Urho3D::StringHash    GetType() const     { return GetTypeStatic(); } \
        virtual Urho3D::StringHash    GetBaseType() const { return GetBaseTypeStatic(); } \
        virtual const Urho3D::String& GetTypeName() const { return GetTypeNameStatic(); } \
		static void                   RegisterObject(Urho3D::Context* context); \
        static void                   DefineMyAttributes(Urho3D::ClassConstructor<typeName>& Definition); \

/*
#define URHO_OBJECT_NOREGISTER(typeName) \
    private: \
        static SharedPtr<Urho3D::ClassDef> s_classDef; \
    public: \
        typedef typeName ClassName; \
        static Urho3D::ClassDef* GetClassDefStatic(); \
        static void DefineMyAttributes(Urho3D::ClassConstructor<typeName>& Definition); \
        virtual Urho3D::ClassDef* GetClassDef() const { return GetClassDefStatic(); } \
        virtual Urho3D::StringHash GetType() const { return GetTypeStatic(); } \
        virtual Urho3D::StringHash GetBaseType() const { return GetBaseTypeStatic(); } \
        virtual const Urho3D::String& GetTypeName() const { return GetTypeNameStatic(); } \
        static Urho3D::StringHash GetTypeStatic() { static const Urho3D::StringHash typeStatic(#typeName); return typeStatic; } \
        static const Urho3D::String& GetTypeNameStatic() { static const Urho3D::String typeNameStatic(#typeName); return typeNameStatic; } \
*/

#define URHO_OBJECT_AUTO0(typeName) \
	__DEFCLASSINFOHOLDERCLOSED0__(typeName); \
    public: \
        typedef typeName ClassName; \
        static Urho3D::ClassDef*      GetClassDefStatic() { return __GetClassInfoHolderStatic().classDef_.Get(); } \
		static Urho3D::StringHash     GetTypeStatic()     { return __GetClassInfoHolderStatic().typeId_; } \
        static const Urho3D::String&  GetTypeNameStatic() { return __GetClassInfoHolderStatic().typeName_; } \
        virtual Urho3D::ClassDef*     GetClassDef() const { return GetClassDefStatic(); } \
        virtual Urho3D::StringHash    GetType() const     { return GetTypeStatic(); } \
        virtual Urho3D::StringHash    GetBaseType() const { return GetBaseTypeStatic(); } \
        virtual const Urho3D::String& GetTypeName() const { return GetTypeNameStatic(); } \

#define URHO_OBJECT_AUTO1(typeName, baseType0) \
    __DEFCLASSINFOHOLDERCLOSED1__(typeName, baseType0); \
    public: \
        typedef typeName ClassName; \
        static Urho3D::ClassDef*      GetClassDefStatic() { return __GetClassInfoHolderStatic().classDef_.Get(); } \
		static Urho3D::StringHash     GetTypeStatic()     { return __GetClassInfoHolderStatic().typeId_; } \
        static const Urho3D::String&  GetTypeNameStatic() { return __GetClassInfoHolderStatic().typeName_; } \
        virtual Urho3D::ClassDef*     GetClassDef() const { return GetClassDefStatic(); } \
        virtual Urho3D::StringHash    GetType() const     { return GetTypeStatic(); } \
        virtual Urho3D::StringHash    GetBaseType() const { return GetBaseTypeStatic(); } \
        virtual const Urho3D::String& GetTypeName() const { return GetTypeNameStatic(); } \

#define URHO_BASEOBJECT(typeName) \
    public: \
        static Urho3D::StringHash GetBaseTypeStatic() { static const Urho3D::StringHash baseTypeStatic(#typeName); return baseTypeStatic; } \
    
/// THIS MUST BE CALLED IN THE GLOBAL NAMESPACE!!!
#define URHO_REGISTER_BASEOBJECT_TRAITS(typeName) \
    namespace U3D_Traits { \
        template <> \
        struct is_base<typeName> \
        { \
            static const bool value = true; \
        }; \
    }

#define URHO_REGISTER_INTERFACE_TRAITS(typeName) \
    namespace U3D_Traits { \
        template <> \
        struct is_interface<typeName> \
        { \
            static const bool value = true; \
        }; \
    }
    
#define URHO_REGISTER_OBJECT( typeName, ... ) \
    void typeName::RegisterObject(Urho3D::Context* context) \
    { \
        Urho3D::ClassDef* classDef = typeName::GetClassDefStatic(); \
        context->RegisterFactory< typeName >(__VA_ARGS__); \
        Urho3D::ClassConstructor< typeName > ccc(context, ::Urho3D::WeakPtr< ::Urho3D::ClassDef>(classDef)); \
        typeName::DefineMyAttributes(ccc); \
        classDef->Close(); \
    } \
    void typeName::DefineMyAttributes(Urho3D::ClassConstructor<typeName>& Definition) \
    

#define URHO_REGISTER_OBJECT_NO_FACTORY(typeName) \
    void typeName::RegisterObject(Urho3D::Context* context) \
    { \
        Urho3D::ClassDef* classDef = typeName::GetClassDefStatic(); \
        Urho3D::ClassConstructor< typeName > ccc(context, ::Urho3D::WeakPtr<Urho3D::ClassDef>(classDef)); \
        typeName::DefineMyAttributes(ccc); \
        classDef->Close(); \
    } \
    void typeName::DefineMyAttributes(Urho3D::ClassConstructor<typeName>& Definition) \

/// Base class for objects with type identification, subsystem access and event sending/receiving capability.
class URHO3D_API Object : public RefCounted
{
    URHO_OBJECT_AUTO0(Object);
    URHO_BASEOBJECT(Object);

    friend class Context;

public:
    /// Construct.
    Object(Context* context);
    /// Destruct. Clean up self from event sender & receiver structures.
    virtual ~Object();
    
    /// Handle event.
    virtual void OnEvent(Object* sender, StringHash eventType, VariantMap& eventData);

    /// Subscribe to an event that can be sent by any sender.
    void SubscribeToEvent(StringHash eventType, EventHandler* handler);
    /// Subscribe to a specific sender's event.
    void SubscribeToEvent(Object* sender, StringHash eventType, EventHandler* handler);
    /// Unsubscribe from an event.
    void UnsubscribeFromEvent(StringHash eventType);
    /// Unsubscribe from a specific sender's event.
    void UnsubscribeFromEvent(Object* sender, StringHash eventType);
    /// Unsubscribe from a specific sender's events.
    void UnsubscribeFromEvents(Object* sender);
    /// Unsubscribe from all events.
    void UnsubscribeFromAllEvents();
    /// Unsubscribe from all events except those listed, and optionally only those with userdata (script registered events.)
    void UnsubscribeFromAllEventsExcept(const PODVector<StringHash>& exceptions, bool onlyUserData);
    /// Send event to all subscribers.
    void SendEvent(StringHash eventType);
    /// Send event with parameters to all subscribers.
    void SendEvent(StringHash eventType, VariantMap& eventData);
    /// Return a preallocated map for event data. Used for optimization to avoid constant re-allocation of event data maps.
    VariantMap& GetEventDataMap() const;

    /// Return execution context.
    Context* GetContext() const { return context_; }

    /// Return subsystem by type.
    Object* GetSubsystem(StringHash type) const;
    /// Return active event sender. Null outside event handling.
    Object* GetEventSender() const;
    /// Return active event handler. Null outside event handling.
    EventHandler* GetEventHandler() const;
    /// Return whether has subscribed to an event without specific sender.
    bool HasSubscribedToEvent(StringHash eventType) const;
    /// Return whether has subscribed to a specific sender's event.
    bool HasSubscribedToEvent(Object* sender, StringHash eventType) const;

    /// Return whether has subscribed to any event.
    bool HasEventHandlers() const { return !eventHandlers_.Empty(); }

    /// Template version of returning a subsystem.
    template <class T> T* GetSubsystem() const;
    /// Return object category. Categories are (optionally) registered along with the object factory. Return an empty string if the object category is not registered.
    const String& GetCategory() const;

protected:
    /// Execution context.
    Context* context_;

private:
    /// Find the first event handler with no specific sender.
    EventHandler* FindEventHandler(StringHash eventType, EventHandler** previous = 0) const;
    /// Find the first event handler with specific sender.
    EventHandler* FindSpecificEventHandler(Object* sender, EventHandler** previous = 0) const;
    /// Find the first event handler with specific sender and event type.
    EventHandler* FindSpecificEventHandler(Object* sender, StringHash eventType, EventHandler** previous = 0) const;
    /// Remove event handlers related to a specific sender.
    void RemoveEventSender(Object* sender);

    /// Event handlers. Sender is null for non-specific handlers.
    LinkedList<EventHandler> eventHandlers_;
};

template <class T> T* Object::GetSubsystem() const { return static_cast<T*>(GetSubsystem(T::GetTypeStatic())); }

/// Base class for object factories.
class URHO3D_API ObjectFactory : public RefCounted
{
public:
    /// Construct.
    ObjectFactory(Context* context) :
        context_(context)
    {
        assert(context_);
    }

    /// Create an object. Implemented in templated subclasses.
    virtual SharedPtr<Object> CreateObject() = 0;

    /// Return execution context.
    Context* GetContext() const { return context_; }

    /// Return type hash of objects created by this factory.
    StringHash GetType() const { return type_; }

    /// Return base type hash of objects created by this factory.
    StringHash GetBaseType() const { return baseType_; }

    /// Return type name of objects created by this factory.
    const String& GetTypeName() const { return typeName_; }

protected:
    /// Execution context.
    Context* context_;
    /// Object type.
    StringHash type_;
    /// Object base type.
    StringHash baseType_;
    /// Object type name.
    String typeName_;
};

/// Template implementation of the object factory.
template <class T> class ObjectFactoryImpl : public ObjectFactory
{
public:
    /// Construct.
    ObjectFactoryImpl(Context* context) :
        ObjectFactory(context)
    {
        type_ = T::GetTypeStatic();
        baseType_ = T::GetBaseTypeStatic();
        typeName_ = T::GetTypeNameStatic();
    }

    /// Create an object of the specific type.
    virtual SharedPtr<Object> CreateObject() { return SharedPtr<Object>(new T(context_)); }
};

/// Internal helper class for invoking event handler functions.
class URHO3D_API EventHandler : public LinkedListNode
{
public:
    /// Construct with specified receiver.
    EventHandler(Object* receiver) :
        receiver_(receiver),
        sender_(0),
        userData_(0)
    {
        assert(receiver_);
    }

    /// Construct with specified receiver and userdata.
    EventHandler(Object* receiver, void* userData) :
        receiver_(receiver),
        sender_(0),
        userData_(userData)
    {
        assert(receiver_);
    }

    /// Destruct.
    virtual ~EventHandler() { }

    /// Set sender and event type.
    void SetSenderAndEventType(Object* sender, StringHash eventType)
    {
        sender_ = sender;
        eventType_ = eventType;
    }

    /// Invoke event handler function.
    virtual void Invoke(VariantMap& eventData) = 0;
    /// Return a unique copy of the event handler.
    virtual EventHandler* Clone() const = 0;

    /// Return event receiver.
    Object* GetReceiver() const { return receiver_; }

    /// Return event sender. Null if the handler is non-specific.
    Object* GetSender() const { return sender_; }

    /// Return event type.
    const StringHash& GetEventType() const { return eventType_; }

    /// Return userdata.
    void* GetUserData() const { return userData_; }

protected:
    /// Event receiver.
    Object* receiver_;
    /// Event sender.
    Object* sender_;
    /// Event type.
    StringHash eventType_;
    /// Userdata.
    void* userData_;
};

/// Template implementation of the event handler invoke helper (stores a function pointer of specific class.)
template <class T> class EventHandlerImpl : public EventHandler
{
public:
    typedef void (T::*HandlerFunctionPtr)(StringHash, VariantMap&);

    /// Construct with receiver and function pointers.
    EventHandlerImpl(T* receiver, HandlerFunctionPtr function) :
        EventHandler(receiver),
        function_(function)
    {
        assert(function_);
    }

    /// Construct with receiver and function pointers and userdata.
    EventHandlerImpl(T* receiver, HandlerFunctionPtr function, void* userData) :
        EventHandler(receiver, userData),
        function_(function)
    {
        assert(function_);
    }

    /// Invoke event handler function.
    virtual void Invoke(VariantMap& eventData)
    {
        T* receiver = static_cast<T*>(receiver_);
        (receiver->*function_)(eventType_, eventData);
    }

    /// Return a unique copy of the event handler.
    virtual EventHandler* Clone() const
    {
        return new EventHandlerImpl(static_cast<T*>(receiver_), function_, userData_);
    }

private:
    /// Class-specific pointer to handler function.
    HandlerFunctionPtr function_;
};

/// Describe an event's hash ID and begin a namespace in which to define its parameters.
#define EVENT(eventID, eventName) static const Urho3D::StringHash eventID(#eventName); namespace eventName
/// Describe an event's parameter hash ID. Should be used inside an event namespace.
#define PARAM(paramID, paramName) static const Urho3D::StringHash paramID(#paramName)
/// Convenience macro to construct an EventHandler that points to a receiver object and its member function.
#define HANDLER(className, function) (new Urho3D::EventHandlerImpl<className>(this, &className::function))
/// Convenience macro to construct an EventHandler that points to a receiver object and its member function, and also defines a userdata pointer.
#define HANDLER_USERDATA(className, function, userData) (new Urho3D::EventHandlerImpl<className>(this, &className::function, userData))

}

URHO_REGISTER_BASEOBJECT_TRAITS(Urho3D::Object);
URHO_REGISTER_INTERFACE_TRAITS(Urho3D::Object);
