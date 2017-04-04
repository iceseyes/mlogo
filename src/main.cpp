/**
 * @file: main.cc
 *
 *  Created on: 15 mar 2017
 *      Author: massimo Bianchi
 */

#include <cmath>
#include <iostream>

#include <SDL2/SDL.h>

#include "parser.hpp"
#include "memory.hpp"
#include "eval.hpp"

//Screen dimension constants
constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

using namespace std;

namespace mlogo { namespace builtin {

void initBuiltInProcedures();

}}

SDL_Window* window = nullptr;
SDL_Renderer *renderer = nullptr;

struct {
	std::vector<SDL_Point> points;
	double angle { M_PI };
} track;

void initSDL() {
	SDL_Point o = { 310, 230 };
	track.points.push_back(o);

    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		return;
    }

	window = SDL_CreateWindow( "mLogo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_ALWAYS_ON_TOP );
	if(window == NULL) {
		cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
		SDL_Quit();
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr){
		SDL_DestroyWindow(window);
		cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
		SDL_Quit();

		return;
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderDrawLine(renderer, 310, 230, 315, 240);
	SDL_RenderDrawLine(renderer, 305, 240, 310, 230);
	SDL_RenderDrawLine(renderer, 305, 240, 315, 240);
	SDL_RenderPresent(renderer);
}

void closeSDL() {
	//Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_Quit();
}

struct Forward : mlogo::types::BasicProcedure {
	Forward() : mlogo::types::BasicProcedure(1) {}
	void operator()() const override {
		int arg = std::stoi(fetchArg(0));
		SDL_Point o = track.points.back();
		SDL_Point d = { (int)(o.x + sin(track.angle) * arg), (int)(o.y +  cos(track.angle) * arg) };

		track.points.push_back(d);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		SDL_RenderDrawLines(renderer, track.points.data(), track.points.size());
		SDL_RenderPresent(renderer);
	}
};

struct Right : mlogo::types::BasicProcedure {
	Right() : mlogo::types::BasicProcedure(1) {}
	void operator()() const override {
		double arg = std::stod(fetchArg(0));

		track.angle -= arg*0.0174533;
	}
};

int main(int argc, char **argv) {
	mlogo::builtin::initBuiltInProcedures();
	mlogo::memory::Stack::instance().globalFrame().setProcedure<Forward>("forward");
	mlogo::memory::Stack::instance().globalFrame().setProcedure<Forward>("fd");
	mlogo::memory::Stack::instance().globalFrame().setProcedure<Right>("right");
	mlogo::memory::Stack::instance().globalFrame().setProcedure<Right>("rt");
	initSDL();

	string str;
	cout << endl << "? ";
	while(std::getline(std::cin, str)) {
		if(str.empty() || str[0] == 'q' || str[0] == 'Q') break;

		auto stmt = mlogo::parser::parse(str);
		auto s = mlogo::eval::make_statement(stmt);

		(*s)();

		delete s;
		cout << endl << "? ";
	}

	closeSDL();
	return 0;
}

