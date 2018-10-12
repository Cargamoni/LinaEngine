/*
Author: Inan Evin
www.inanevin.com


BSD 2-Clause License
Lina Engine Copyright (c) 2018, Inan Evin All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.

-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO
-- THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
-- BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
-- GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
-- STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
-- OF SUCH DAMAGE.


*/

#pragma once

#ifndef LINA_CORE_H
#define LINA_CORE_H

#include "Lina_RenderingEngine.h"
#include "Lina_GameCore.h"
#include "Lina_InputEngine.h"
#include "Lina_EventHandler.h"

extern const double FRAME_CAP;
extern const long SECOND;

class Lina_Core {

public:

    Lina_Core();
	~Lina_Core();

private:

	void Wake();
	void Start();
	void Stop();
	void Run();
	void Render();
	void CleanUp();
	bool isRunning;

	Lina_InputEngine inputEngine;
	Lina_RenderingEngine renderingEngine;
	Lina_GameCore gameCore;
	Lina_SDLHandler sdlHandler;

	// Object Specific
	Lina_EventHandler eventHandler;

	Lina_Core(const Lina_Core& r) = delete;


};

#endif