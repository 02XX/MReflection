#include "gtest/gtest.h"

class Int
{
  public:
    int mValue;
    Int(int value) : mValue(value)
    {
    }
    Int(const Int &other) : mValue(other.mValue)
    {
        GTEST_LOG_(INFO) << "Int Copy Constructor";
    }
    Int(Int &&other) noexcept : mValue(other.mValue)
    {
        GTEST_LOG_(INFO) << "Int Move Constructor";
    }
    Int &operator=(const Int &other)
    {
        GTEST_LOG_(INFO) << "Int Copy Assignment";
        mValue = other.mValue;
        return *this;
    }
    Int &operator=(Int &&other) noexcept
    {
        GTEST_LOG_(INFO) << "Int Move Assignment";
        mValue = other.mValue;
        return *this;
    }

    bool operator==(const Int &other) const
    {
        return mValue == other.mValue;
    }
};
