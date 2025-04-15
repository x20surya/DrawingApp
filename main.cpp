#include <raylib.h>

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

	while (!WindowShouldClose())
	{
		Vector2 mousePos = GetMousePosition();

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			BeginTextureMode(target);
			DrawCircle((int)mousePos.x, (int)mousePos.y, 5.0f, BLUE);
			EndTextureMode();
		}

		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawTextureRec(target.texture, { 0, 0, (float)target.texture.width, -(float)target.texture.height }, { 0, 0 }, WHITE);

		DrawCircle(GetMouseX(), GetMouseY(), 5.0f, BLUE);

		DrawFPS(10, 10);

		EndDrawing();
	}

	UnloadRenderTexture(target);

	CloseWindow();

	return 0;
}