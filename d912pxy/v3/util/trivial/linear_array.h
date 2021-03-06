/*
MIT License

Copyright(c) 2020 megai2

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#pragma once
#include "stdafx.h"

namespace d912pxy
{
	namespace Trivial
	{
		template<typename ElementType>
		class LinearArray : private MemoryBlock
		{
		public:
			LinearArray() = default;

			intptr_t byteSize(intptr_t elements)
			{
				return elements * sizeof(ElementType);
			}

			void init(intptr_t elements)
			{
				alloc(byteSize(elements));
			}

			void resize(intptr_t elements)
			{
				realloc(byteSize(elements));
			}

			void clear()
			{
				free();
			}

			void zeroMem(uintptr_t offset, uintptr_t elements)
			{
				memset((void*)&(c_arr<ElementType>()[offset]), 0, byteSize(elements));
			}

			ElementType& get(uintptr_t idx) { return c_arr<ElementType>()[idx]; }
			ElementType& operator[](uintptr_t idx) { return c_arr<ElementType>()[idx]; }
		};
	}
}