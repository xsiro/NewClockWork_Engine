#include "About.h"
#include "imgui.h"
#include "Application.h"
#include "glew/include/glew.h"
#include "Assimp/include/version.h"
#include "FileSys.h"

About::About() : Window()
{
	type = WindowType::ABOUT_WINDOW;
}

About::~About() {}

void About::Draw()
{
	if (ImGui::BeginMenu("About", &visible))
	{
		
			ImGui::Text("ClockWorkEngine is developed by Daniel Ruiz & Pol Cortes");
			ImGui::Text("This engine has been coded in C++");
			ImGui::Text("Libraries:");
			ImGui::Text("OpenGL v2.1");
			ImGui::Text("Glew v7.0");
			ImGui::Text("MathGeoLib v1.5");
			ImGui::Text("PhysFS v3.0.2");
			ImGui::Text("Assimp v3.1.1");

			ImGui::Text("");

			ImGui::Text("LICENSE:");
			ImGui::Text("");
			ImGui::Text("MIT License");
			ImGui::Text("");
			ImGui::Text("Copyright (c) 2020 [Daniel Ruiz & Pol Cortes]");
			ImGui::Text("");
			ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
			ImGui::Text("of this software and associated documentation files (the 'Software'), to deal");
			ImGui::Text("in the Software without restriction, including without limitation the rights");
			ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell");
			ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
			ImGui::Text("furnished to do so, subject to the following conditions:");
			ImGui::Text("");
			ImGui::Text("The above copyright notice and this permission notice shall be included in all");
			ImGui::Text("copies or substantial portions of the Software.");
			ImGui::Text("");
			ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
			ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
			ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE");
			ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
			ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
			ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN ");
			ImGui::Text("THE SOFTWARE.");
			ImGui::End();



		


	}
	ImGui::End();
}