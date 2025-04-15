#include <raylib.h>
#include <string>

int main(void)
{

	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Drawing app");

	RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

	BeginTextureMode(target);
	ClearBackground(RAYWHITE);
	EndTextureMode();

	SetTargetFPS(144);

	Vector2 prevMousePos = { 0,0 };

	while (!WindowShouldClose())
	{
		
		Vector2 mousePos = GetMousePosition();

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			BeginTextureMode(target);
            if (prevMousePos.x == 0 && prevMousePos.y == 0)
			{
				prevMousePos = mousePos;
			}
			DrawLineEx({ prevMousePos.x,prevMousePos.y }, { mousePos.x, mousePos.y }, 10.0f, BLUE);
			DrawCircle((int)mousePos.x, (int)mousePos.y, 5.0f, BLUE);
			EndTextureMode();

			prevMousePos = mousePos;
		}
		else 
		{
			prevMousePos = { 0,0 };
		}
		
		if (IsKeyPressed(KEY_C))
		{
			BeginTextureMode(target);
			ClearBackground(RAYWHITE);
			EndTextureMode();
		}

		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawTextureRec(target.texture, { 0, 0, (float)target.texture.width, -(float)target.texture.height }, { 0, 0 }, WHITE);

		

		DrawCircle(GetMouseX(), GetMouseY(), 5.0f, BLUE);

		DrawFPS(10, 10);

		std::string text = "Press 'C' to clear the screen";

		DrawText(text.c_str(), screenWidth - (text.length() * 12), screenHeight - 20, 20, DARKGRAY);

		EndDrawing();
	}

	UnloadRenderTexture(target);

	CloseWindow();

	return 0;
}