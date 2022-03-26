// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "../include/Game.h"
#include "stdio.h"
#include <iostream>
#include <random>
#include <Vector>
using namespace std;


const int thickness = 15;//sera usado para setar a altura de alguns objetos
const float paddleHeight = 15.0f;//tamanho da raquete
const float WINDOW_WIDTH = 1024.0f;
const float WINDOW_HEIGHT = 600.0f;
const unsigned int MAXIMUM_NUMBER_OF_BALLS = 5;

Game::Game()
:mWindow(nullptr)//para criar uma janela
,mRenderer(nullptr)//para fins de renderiza��o na tela
,mTicksCount(0)//para guardar o tempo decorrido no jogo
,mIsRunning(true)//verificar se o jogo ainda deve continuar sendo executado
,mPaddleDir1(0)//direcao da bolinha
,mPaddleDir2(0)
,isMultiplayerActive(false)
{
	
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		static_cast<int>(WINDOW_WIDTH),	// Width of window
		static_cast<int>(WINDOW_HEIGHT),	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	float paddlePositionY = WINDOW_HEIGHT - 15.0f;
	float paddlePositionX = (WINDOW_WIDTH / 4.0f) - 50;
	
	paddleFirstPlayer.Initialize(paddlePositionX, paddlePositionY, 100.0f, paddleHeight, 0);



	ball.position.x = 1024.0f / 2.0f;
	ball.position.y = WINDOW_HEIGHT / 2.0f;
	ball.velocity.x = -200.0f;
	ball.velocity.y = 500.0f;

	balls.push_back(ball);

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			Block bloco;
			if (bloco.Initialize(100.0f + (i * 30.0f), 100.0f + (j * 50.0f))) {
				blocos.push_back(bloco);
			}
		}
	}

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;//evento, inputs do jogador s�o armazenados aqui
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	// Get state of keyboard - podemos buscar por alguma tecla espec�fica pressionada no teclado, nesse caso, Escape
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	// Update paddle direction based on W/S keys - atualize a dire��o da raquete com base na entrada do jogador
	// W -> move a raquete para cima, S -> move a raquete para baixo
	mPaddleDir1 = 0;
	if (state[SDL_SCANCODE_A])
	{
		mPaddleDir1 -= 3;
	}
	else if (state[SDL_SCANCODE_D])
	{
		mPaddleDir1 += 3;
	}
	paddleFirstPlayer.changePaddleDirection(mPaddleDir1);


	mPaddleDir2 = 0;
	if (isMultiplayerActive) {
		if (state[SDL_SCANCODE_UP])
		{
			mPaddleDir2 -= 3;
		}
		if (state[SDL_SCANCODE_DOWN])
		{
			mPaddleDir2 += 3;
		}
		paddleSecondPlayer.changePaddleDirection(mPaddleDir1);
	}

	// Ativando multiplayer
	if (state[SDL_SCANCODE_2])
	{
		if (!isMultiplayerActive) {
			float paddlePositionX = 1000.0f;
			float paddlePositionY = WINDOW_HEIGHT / 2.0f;

			paddleSecondPlayer.Initialize(paddlePositionX, paddlePositionY, thickness, paddleHeight, 0);
			

			isMultiplayerActive = true;
		}
	}

	// Ativando singleplayer
	if (state[SDL_SCANCODE_1])
	{
		if (isMultiplayerActive) {


			isMultiplayerActive = false;
		}
	}
}

void Game::UpdateGame()
{

	// Espere que 16ms tenham passado desde o �ltimo frame - limitando os frames
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time � a diferen�a em ticks do �ltimo frame
	// (convertido pra segundos) - calcula o delta time para atualiza��o do jogo
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	
	// "Clamp" (lima/limita) valor m�ximo de delta time
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// atualize a contagem de ticks par ao pr�ximo frame
	mTicksCount = SDL_GetTicks();

	if (!isMultiplayerActive)
		updateSingleGame(deltaTime);
	else
		updateMultiplayerGame(deltaTime);

}

void Game::updateSingleGame(float deltaTime)
{
	//atualiza a posi��o da raquete
	if (paddleFirstPlayer.isPaddleMoving(mPaddleDir1))
	{
		paddleFirstPlayer.updatePaddlePosition(mPaddleDir1, deltaTime, thickness);
	}

	int ballsSize = balls.size();

	for (int i = 0; i < ballsSize; i++) {

		// atualiza a posi��o da bola com base na sua velocidade
		balls[i].position.x += balls[i].velocity.x * deltaTime;
		balls[i].position.y += balls[i].velocity.y * deltaTime;

		float diff = paddleFirstPlayer.getY() - balls[i].position.y;
		diff = (diff > 0.0f) ? diff : -diff;

		if (balls[i].collidesWithPaddle(diff, paddleFirstPlayer))
		{

			if (balls.size() < MAXIMUM_NUMBER_OF_BALLS) {
				if (generateRandomBool())
					ball.velocity.x = 1 * 200.0f;
				else
					ball.velocity.x = -1 * 200.0f;

				balls.push_back(ball);
			}

			balls[i].updateBallDirection();


		}
		else if (balls[i].isBallOutOfLeftSide())
		{
			// mIsRunning = false;
		}
		else if (balls[i].isBallOutOfRightSide())
		{
			balls[i].reverseBallXDirection();
		}
		else if (balls[i].collidesWithTopWall())
		{
			balls[i].reverseBallYDirection();
		}
		else if (balls[i].collidesWithBottomWall(WINDOW_HEIGHT))
		{
			balls[i].reverseBallYDirection();
		}
	}
}

void Game::updateMultiplayerGame(float deltaTime)
{
	//atualiza a posi��o da raquete
	if (paddleFirstPlayer.isPaddleMoving(mPaddleDir1))
	{
		paddleFirstPlayer.updatePaddlePosition(mPaddleDir1, deltaTime, thickness);
	}

	//atualiza a posi��o da raquete
	if (paddleSecondPlayer.isPaddleMoving(mPaddleDir2))
	{
		paddleSecondPlayer.updatePaddlePosition(mPaddleDir2, deltaTime, thickness);
	}

	int ballsSize = balls.size();

	for (int i = 0; i < ballsSize; i++) {

		// atualiza a posi��o da bola com base na sua velocidade
		balls[i].position.x += balls[i].velocity.x * deltaTime;
		balls[i].position.y += balls[i].velocity.y * deltaTime;

		// atualiza a posi��o da bola se ela colidiu com a raquete
		float diff = paddleFirstPlayer.getY() - balls[i].position.y;
		float diff2 = paddleSecondPlayer.getY() - balls[i].position.y;

		diff = (diff > 0.0f) ? diff : -diff;
		diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
		if (balls[i].collidesWithPaddle(diff, paddleFirstPlayer) || balls[i].collidesWithPaddle(diff2, paddleSecondPlayer))
		{
			if (balls.size() < MAXIMUM_NUMBER_OF_BALLS) {
				if (generateRandomBool())
					ball.velocity.x = 1 * 200.0f;
				else
					ball.velocity.x = -1 * 200.0f;
				balls.push_back(ball);
			}

			balls[i].updateBallDirection();
		}
		else if (balls[i].collidesWithTopWall())
		{
			balls[i].reverseBallYDirection();
		}
		else if (balls[i].collidesWithBottomWall(WINDOW_HEIGHT))
		{
			balls[i].reverseBallYDirection();
		}
	}

	auto it = balls.begin();
	int j = 0;
	while (it != balls.end() && j < ballsSize)
	{
		if (balls[j].isBallOutOfLeftSide())
		{
			it = balls.erase(it);
			ballsSize -= 1;

		}
		else if (balls[j].isBallOutOfRightSide(0))
		{
			it = balls.erase(it);
			ballsSize -= 1;


		}
		else {
			j++;
			it++;
		}
	}
		
}

//Desenhando a tela do jogo
void Game::GenerateOutput()
{
	// Setamos a cor de fundo par azul
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);
	
	// limpa o back buffer
	SDL_RenderClear(mRenderer);

	// Desenha as paredes - mudamos a cor de mRenderer para o desenho dos retangulos que formaram as paredes
	SDL_SetRenderDrawColor(mRenderer, 255, 0, 255, 255);
	
	// Desenha a parede de cima - desenhando um retangulo no topo da tela, coordenada x e y = 0, 0 representa o topo esquerdo
	//primeiro definimos as coordenadas e tamanhos do triangulo
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);//finalmente, desenhamos um retangulo no topo
	
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);

	//desenhamos as outras paredes apenas mudando as coordenadas de wall

	// parede de baixo
	wall.y = static_cast<int>(WINDOW_HEIGHT) - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);

	// parede da direita
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = 1024;
	wall.h = thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	//como as posi��es da raquete e da bola ser�o atualizadas a cada itera��o do game loop, criamos "membros" na classe
	//Game.h para tal

	//mudar a cor da raquete
	SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);

	//desenhando a raquete - usando mPaddlePos que � uma struc de coordenada que foi definida em Game.h
	 
	SDL_Rect paddleFirstPlayerRender{
		static_cast<int>(paddleFirstPlayer.getX()),//static_cast converte de float para inteiros, pois SDL_Rect trabalha com inteiros
		static_cast<int>(paddleFirstPlayer.getY() - paddleHeight / 2),
		static_cast<int>(paddleFirstPlayer.getWidth()),
		static_cast<int>(paddleFirstPlayer.getHeight())
	};
	SDL_RenderFillRect(mRenderer, &paddleFirstPlayerRender);

	SDL_Rect paddleSecondPlayerRender{
		static_cast<int>(paddleSecondPlayer.getX()),//static_cast converte de float para inteiros, pois SDL_Rect trabalha com inteiros
		static_cast<int>(paddleSecondPlayer.getY() - paddleHeight / 2),
		static_cast<int>(paddleSecondPlayer.getWidth()),
		static_cast<int>(paddleSecondPlayer.getHeight())
	};

	if(isMultiplayerActive)
		SDL_RenderFillRect(mRenderer, &paddleSecondPlayerRender);


	//mudar a cor do renderizador para a bola
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 255);

	int ballsSize = balls.size();

	for (int i = 0; i < ballsSize; i++) {
		// Draw ball
		SDL_Rect ball{	
			static_cast<int>(balls[i].position.x - thickness/2),
			static_cast<int>(balls[i].position.y - thickness/2),
			thickness,
			thickness
		};
		SDL_RenderFillRect(mRenderer, &ball);

	}

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 255);
	int blockSize = blocos.size();
	//cout<<blockSize<<endl;
	for (int i = 0; i < blockSize; i++) {

		cout << i << blocos[i].position.x << endl;
		cout << i << blocos[i].position.y << endl;
		// Draw ball
		SDL_Rect block{
			static_cast<int>(blocos[i].position.x),
			static_cast<int>(blocos[i].position.y),
			50,
			50
		};
		SDL_RenderFillRect(mRenderer, &block);

	}


	SDL_SetRenderDrawColor(mRenderer, 255, 255, 0, 255);	

	
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

bool Game::generateRandomBool()
{
	int randomNumber = rand() % 2;
	
	return !!randomNumber;
}

//Para encerrar o jogo
void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);//encerra o renderizador
	SDL_DestroyWindow(mWindow);//encerra a janela aberta
	SDL_Quit();//encerra o jogo
}