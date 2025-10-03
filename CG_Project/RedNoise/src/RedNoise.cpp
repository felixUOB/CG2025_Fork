#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>

#define WIDTH 320
#define HEIGHT 240

std::vector<float> interpolateSingleFloats(const float from, const float to, const int numberOfValues)
{
	std::vector<float> result;
	const float step = (to - from) / (static_cast<float>(numberOfValues) - 1.0f);

	for (int i = 0; i < numberOfValues; i++)
	{
		result.push_back(from + static_cast<float>(i) * step);
	}
	return result;
}

std::vector<glm::vec3> interpolateThreeElementValues(const glm::vec3& vec1, const glm::vec3& vec2, const int numberOfElements) {
	std::vector<glm::vec3> result(numberOfElements);

	for (int i = 0; i < 3; i++) {
		const float step = (vec2[i] - vec1[i]) / (static_cast<float>(numberOfElements) - 1.0f);

		for (int j = 0; j <= numberOfElements; j++) {
			result[j][i] = vec1[i] + static_cast<float>(j) * step;
		}
	}

	// for (auto & i : result) {
	// 	for (int j = 0; j < 3; j++) {
	// 		std::cout << i[j] << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
	return result;
}

void draw(DrawingWindow &window, std::vector<std::vector<glm::vec3>>& pixelValues)
{
	window.clearPixels();
	for (size_t y = 0; y < window.height; y++)
	{
		for (size_t x = 0; x < window.width; x++)
		{
			const float red = pixelValues[y][x][0];
			const float green = pixelValues[y][x][1];
			const float blue = pixelValues[y][x][2];
			const uint32_t colour = (255 << 24) + (static_cast<int>(red) << 16) + (static_cast<int>(green) << 8) + static_cast<int>(blue);
			window.setPixelColour(x, y, colour);
		}
	}
}

void handleEvent(SDL_Event event, DrawingWindow &window) {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

int main(int argc, char *argv[]) {
	// glm::vec3 topLeft(255, 0, 0);        // red
	// glm::vec3 topRight(0, 0, 255);       // blue
	//
	// glm::vec3 bottomRight(0, 255, 0);    // green
	// glm::vec3 bottomLeft(255, 255, 0);   // yellow
	//
	// const std::vector<glm::vec3> leftCol = interpolateThreeElementValues(topLeft, bottomLeft, HEIGHT);
	// const std::vector<glm::vec3> rightCol = interpolateThreeElementValues(topRight, bottomRight, HEIGHT);
	//
	//
	// for (int y = 0; y < HEIGHT; y++) {
	// 	result[y] = interpolateThreeElementValues(leftCol[y], rightCol[y], WIDTH);
	// }

	std::vector<std::vector<glm::vec3>> pixelValues(HEIGHT, std::vector<glm::vec3>(WIDTH));

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			bool inTri = true;
			glm::vec3 weighting = convertToBarycentricCoordinates( glm::vec2(WIDTH / 2, 0.0f), glm::vec2(WIDTH, HEIGHT), glm::vec2(0.0f, HEIGHT),glm::vec2(j, i));
			// Check if any weighting values are negative -> not in triangle
			for (int k = 0; k < 3; k++) {
				if (weighting[k] <= 0) inTri = false;
			}
			if (inTri) {
				pixelValues[i][j] = glm::vec3(255, 255, 255) * weighting;
			}
		}
	}

	auto window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	while (true)
	{
		// We MUST poll for events - otherwise the window will freeze !
		if (window.pollForInputEvents(event))
			handleEvent(event, window);
		draw(window, pixelValues);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}

}
