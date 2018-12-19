/*
 * z80-board-utilities
 * 
 * Copyright (c) 2018 Michael Cullen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "stringutils.h"

namespace z80board
{

std::string convertString(LPCWSTR wideString, int count)
{
    int requiredLength = ::WideCharToMultiByte(CP_UTF8, 0, wideString, count, nullptr, 0, nullptr, nullptr);
    if(requiredLength == 0)
    {
        //TODO: Handle error
        return std::string("Conversion Error!");
    }
    std::string outVal(requiredLength, '\0');
    if(::WideCharToMultiByte(CP_UTF8, 0, wideString, count, &outVal[0], requiredLength, nullptr, nullptr) != requiredLength)
    {
        //HUH?!
        return std::string("Conversion Error!!");
    }
    return outVal;
}

std::wstring convertString(const std::string& str)
{
    int requiredLength = ::MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), nullptr, 0);
    if(requiredLength == 0)
    {
        //TODO: Handle error
        return std::wstring(L"Conversion Error!");
    }
    std::wstring outVal(requiredLength, '\0');
    if(::MultiByteToWideChar(CP_UTF8, 0, str.data(), str.size(), &outVal[0], requiredLength) != requiredLength)
    {
        //HUH?!
        return std::wstring(L"Conversion Error!!");
    }
    return outVal;
}

}
