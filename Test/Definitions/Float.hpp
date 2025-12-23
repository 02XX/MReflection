#include "gtest/gtest.h"
class Float
{
  public:
    float mValue;
    Float(float value) : mValue(value)
    {
    }
    Float(const Float &other) : mValue(other.mValue)
    {
        GTEST_LOG_(INFO) << "Float Copy Constructor";
    }
    Float(Float &&other) noexcept : mValue(other.mValue)
    {
        GTEST_LOG_(INFO) << "Float Move Constructor";
    }
    Float &operator=(const Float &other)
    {
        GTEST_LOG_(INFO) << "Float Copy Assignment";
        mValue = other.mValue;
        return *this;
    }
    bool operator==(const Float &other) const
    {
        return mValue == other.mValue;
    }
};