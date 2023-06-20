#pragma once

namespace renderer {

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void setResolution(int new_width, int new_height);

		void render();

	private:
	};

}