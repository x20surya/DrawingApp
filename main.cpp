#include <raylib.h>
#include <string>

#define MAX_COLORS_COUNT 23

int main(void)
{

	int screenWidth = 1200;
	int screenHeight = 720;

	Image icon = LoadImage("./assets/icon.png");

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(screenWidth + 100, screenHeight + 100, "Drawing app");

	SetWindowIcon(icon);

	Color colors[MAX_COLORS_COUNT] = {
		RAYWHITE, YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, DARKGREEN,
		SKYBLUE, BLUE, DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN,
		LIGHTGRAY, GRAY, DARKGRAY, BLACK
	};

	Rectangle colorRectangles[MAX_COLORS_COUNT] = { 0 };

	for (int i = 0; i < MAX_COLORS_COUNT; i++)
	{
		colorRectangles[i].x = 10 + 30.0f * i + 2 * i;
		colorRectangles[i].y = 10;
		colorRectangles[i].width = 30;
		colorRectangles[i].height = 30;
	}

	int colorSelected = 0;
	int colorSelectedPrev = colorSelected;
	int colorMouseHover = 0;
	float brushSize = 10.0f;
	bool mouseWasPressed = false;

	Rectangle btnSaveRec = { 750, 10, 40, 30 };
	bool btnSaveMouseHover = false;
	bool showSaveMessage = false;
	int saveMessageCounter = 0;

	RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);

	BeginTextureMode(target);
	ClearBackground(RAYWHITE);
	EndTextureMode();

	SetTargetFPS(144);

	Vector2 prevMousePos = { 0,0 };

	while (!WindowShouldClose())
	{
		screenHeight = GetScreenHeight();
		screenWidth = GetScreenWidth();
		
		Vector2 mousePos = GetMousePosition();

		if (IsKeyPressed(KEY_RIGHT)) colorSelected++;
		else if (IsKeyPressed(KEY_LEFT)) colorSelected--;

		if (colorSelected >= MAX_COLORS_COUNT) colorSelected = 0;
		else if (colorSelected < 0) colorSelected = MAX_COLORS_COUNT - 1;

		for (int i = 0; i < MAX_COLORS_COUNT; i++)
		{
			if (CheckCollisionPointRec(mousePos, colorRectangles[i]))
			{
				colorMouseHover = i;
				break;
			}
			else colorMouseHover = -1;
		}

		if ((colorMouseHover >= 0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			colorSelected = colorMouseHover;
			colorSelectedPrev = colorSelected;
		}

		brushSize += GetMouseWheelMove() * 5;
		if (brushSize < 2) brushSize = 2;
		if (brushSize > 50) brushSize = 50;

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			BeginTextureMode(target);
            if (prevMousePos.x == 0 && prevMousePos.y == 0)
			{
				prevMousePos = mousePos;
			}
			if (mousePos.y > 50)
			{
				DrawLineEx({ prevMousePos.x,prevMousePos.y }, { mousePos.x, mousePos.y }, brushSize*2, colors[colorSelected]);
				DrawCircle((int)mousePos.x, (int)mousePos.y, brushSize, colors[colorSelected]);
			}
			EndTextureMode();

			prevMousePos = mousePos;
		}
		else 
		{
			prevMousePos = { 0,0 };
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			if (!mouseWasPressed)
			{
				colorSelectedPrev = colorSelected;
				colorSelected = 0;
			}

			mouseWasPressed = true;

			BeginTextureMode(target);
			if (mousePos.y > 50) DrawCircle((int)mousePos.x, (int)mousePos.y, brushSize, colors[0]);
			EndTextureMode();
		}
		else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && mouseWasPressed)
		{
			colorSelected = colorSelectedPrev;
			mouseWasPressed = false;
		}

		if (CheckCollisionPointRec(mousePos, btnSaveRec)) btnSaveMouseHover = true;
		else btnSaveMouseHover = false;

		if (btnSaveMouseHover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_S))
		{
			Image image = LoadImageFromTexture(target.texture);
			ImageFlipVertical(&image);
			ExportImage(image, "painting.png");
			UnloadImage(image);
			showSaveMessage = true;
		}
		if (showSaveMessage)
		{
			saveMessageCounter++;
			if (saveMessageCounter > 240)
			{
				showSaveMessage = false;
				saveMessageCounter = 0;
			}
		}
		if (IsKeyPressed(KEY_C))
		{
			BeginTextureMode(target);
			ClearBackground(RAYWHITE);
			EndTextureMode();
		}

		BeginDrawing();

		ClearBackground(BLACK);

		DrawTextureRec(target.texture, { 0, 0, (float)target.texture.width, -(float)target.texture.height }, { 0, 0 }, WHITE);

		if (mousePos.y > 50)
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) DrawCircleLines((int)mousePos.x, (int)mousePos.y, brushSize, GRAY);
			else DrawCircle(GetMouseX(), GetMouseY(), brushSize, colors[colorSelected]);
		}

		DrawRectangle(0, 0, GetScreenWidth(), 50, RAYWHITE);
		DrawLine(0, 50, GetScreenWidth(), 50, LIGHTGRAY);

		for (int i = 0; i < MAX_COLORS_COUNT; i++) DrawRectangleRec(colorRectangles[i], colors[i]);
		DrawRectangleLines(10, 10, 30, 30, LIGHTGRAY);

		if (colorMouseHover >= 0) DrawRectangleRec(colorRectangles[colorMouseHover], Fade(WHITE, 0.6f));

		DrawRectangleLinesEx({
			colorRectangles[colorSelected].x - 2, colorRectangles[colorSelected].y - 2,
				colorRectangles[colorSelected].width + 4, colorRectangles[colorSelected].height + 4
		}, 2, BLACK);

		DrawRectangleLinesEx(btnSaveRec, 2, btnSaveMouseHover ? RED : BLACK);
		DrawText("SAVE!", 755, 20, 10, btnSaveMouseHover ? RED : BLACK);

		if (showSaveMessage)
		{
			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.8f));
			DrawRectangle(0, 150, GetScreenWidth(), 80, BLACK);
			DrawText("IMAGE SAVED:  painting.png", 150, 180, 20, RAYWHITE);
		}

		DrawFPS(10, 10);

		std::string clearScreenText = "Press 'C' to clear the screen";
		std::string brushSizeControlText = "Use Mouse Wheel to change brush size";

		DrawText(clearScreenText.c_str(), screenWidth - (clearScreenText.length() * 12), screenHeight - 25, 20, RAYWHITE);
		DrawText(brushSizeControlText.c_str(), screenWidth - (brushSizeControlText.length() * 12 + clearScreenText.length() * 12), screenHeight - 25, 20, RAYWHITE);
		EndDrawing();
	}

	UnloadRenderTexture(target);

	CloseWindow();

	return 0;
}