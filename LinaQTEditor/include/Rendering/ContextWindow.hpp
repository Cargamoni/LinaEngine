/*
Author: Inan Evin
www.inanevin.com
https://github.com/inanevin/LinaEngine

Copyright 2020~ Inan Evin

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions
and limitations under the License.

Class: ContextWindow
Timestamp: 10/6/2020 6:36:04 PM

*/
#pragma once

#ifndef ContextWindow_HPP
#define ContextWindow_HPP

// Headers here.
#include "qopenglwindow.h"
#include "Rendering/Window.hpp"

namespace LinaEditor::Graphics
{
	class ContextWindow : public QOpenGLWindow, public LinaEngine::Graphics::Window
	{
		
	public:
		
		ContextWindow() {}
		~ContextWindow() {};
	
	private:
	

		// Inherited via Window
		virtual bool CreateContext(LinaEngine::Graphics::WindowProperties& propsIn) override;

		virtual void Tick() override;

		virtual void* GetNativeWindow() const override;

		virtual double GetTime() override;

	};
}

#endif
