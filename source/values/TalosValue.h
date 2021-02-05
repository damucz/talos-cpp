#pragma once

namespace Talos
{

    class Value
    {
    public:
        virtual const char* GetValueType();
    	
        virtual ~Value();
        virtual void SetEmpty(bool isEmpty);
        bool IsEmpty() const;

        virtual void Set(Value* value) = 0;
    protected:
        bool _isEmpty = false;
    };

}