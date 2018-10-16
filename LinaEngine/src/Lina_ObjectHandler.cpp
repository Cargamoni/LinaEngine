/*
Author: Inan Evin
www.inanevin.com

MIT License

Lina Engine, Copyright (c) 2018 Inan Evin

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

4.0.30319.42000
10/9/2018 5:25:08 AM

*/

#include "pch.h"
#include "Lina_ObjectHandler.h"  

void Lina_ObjectHandler::Initialize()
{
	// Retrieve the input engine pointer from the message bus.
	inputEngine = Lina_CoreMessageBus::Instance().GetInputEngine();
	inputDispatcher = inputEngine->GetInputDispatcher();
}

/* ACITON SUBSCRIPTION OVERLOADS */




// Returns the smoothed delta mouse x value.
float Lina_ObjectHandler::GetMouseX()
{
	return inputEngine->GetMouseX();
}

// Returns the smoothed delta mouse y value.
float Lina_ObjectHandler::GetMouseY()
{
	return inputEngine->GetMouseY();
}

// Returns the raw delta mouse x value.
float Lina_ObjectHandler::GetRawMouseX()
{
	return inputEngine->GetRawMouseX();
}

// Returns the raw delta mouse y value.
float Lina_ObjectHandler::GetRawMouseY()
{
	return inputEngine->GetRawMouseY();
}

bool Lina_ObjectHandler::GetMouseButton(int button)
{
	return inputEngine->GetMouseButton(button);
}

bool Lina_ObjectHandler::GetMouseButtonDown(int button)
{
	return inputEngine->GetMouseButtonDown(button);
}

bool Lina_ObjectHandler::GetMouseButtonUp(int button)
{
	return inputEngine->GetMouseButtonUp(button);
}