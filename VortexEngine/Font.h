#pragma once

typedef struct Character {
	int codePoint, x, y, width, height, originX, originY;
} Character;


typedef struct FontType {
	const char *name;
	int size, bold, italic, width, height, characterCount;
	Character *characters;
} FontType;


static Character characters_Arial[] = {
	{ ' ', 59, 36, 3, 3, 1, 1 },
	{ '!', 60, 25, 5, 11, 1, 10 },
	{ '"', 7, 36, 7, 5, 1, 10 },
	{ '#', 50, 14, 10, 11, 2, 10 },
	{ '$', 62, 0, 9, 12, 1, 10 },
	{ '%', 85, 0, 13, 11, 1, 10 },
	{ '&', 0, 14, 10, 11, 1, 10 },
	{ '\'', 19, 36, 4, 5, 1, 10 },
	{ '(', 46, 0, 6, 13, 1, 10 },
	{ ')', 34, 0, 6, 13, 1, 10 },
	{ '*', 0, 36, 7, 6, 1, 10 },
	{ '+', 227, 25, 9, 7, 1, 8 },
	{ ',', 14, 36, 5, 5, 1, 2 },
	{ '-', 48, 36, 6, 3, 1, 5 },
	{ '.', 54, 36, 5, 3, 1, 2 },
	{ '/', 40, 25, 7, 11, 2, 10 },
	{ '0', 226, 14, 9, 11, 1, 10 },
	{ '1', 54, 25, 6, 11, 0, 10 },
	{ '2', 181, 14, 9, 11, 1, 10 },
	{ '3', 172, 14, 9, 11, 1, 10 },
	{ '4', 100, 14, 9, 11, 1, 10 },
	{ '5', 244, 14, 9, 11, 1, 10 },
	{ '6', 190, 14, 9, 11, 1, 10 },
	{ '7', 163, 14, 9, 11, 1, 10 },
	{ '8', 109, 14, 9, 11, 1, 10 },
	{ '9', 208, 14, 9, 11, 1, 10 },
	{ ':', 204, 25, 5, 9, 1, 8 },
	{ ';', 80, 25, 5, 11, 1, 8 },
	{ '<', 218, 25, 9, 7, 1, 8 },
	{ '=', 244, 25, 9, 6, 1, 7 },
	{ '>', 209, 25, 9, 7, 1, 8 },
	{ '?', 199, 14, 9, 11, 1, 10 },
	{ '@', 0, 0, 14, 14, 1, 10 },
	{ 'A', 221, 0, 11, 11, 1, 10 },
	{ 'B', 10, 14, 10, 11, 1, 10 },
	{ 'C', 210, 0, 11, 11, 1, 10 },
	{ 'D', 199, 0, 11, 11, 1, 10 },
	{ 'E', 20, 14, 10, 11, 1, 10 },
	{ 'F', 30, 14, 10, 11, 1, 10 },
	{ 'G', 188, 0, 11, 11, 1, 10 },
	{ 'H', 40, 14, 10, 11, 1, 10 },
	{ 'I', 70, 25, 5, 11, 1, 10 },
	{ 'J', 25, 25, 8, 11, 1, 10 },
	{ 'K', 177, 0, 11, 11, 1, 10 },
	{ 'L', 136, 14, 9, 11, 1, 10 },
	{ 'M', 110, 0, 12, 11, 1, 10 },
	{ 'N', 243, 0, 10, 11, 1, 10 },
	{ 'O', 166, 0, 11, 11, 1, 10 },
	{ 'P', 60, 14, 10, 11, 1, 10 },
	{ 'Q', 98, 0, 12, 11, 1, 10 },
	{ 'R', 155, 0, 11, 11, 1, 10 },
	{ 'S', 70, 14, 10, 11, 1, 10 },
	{ 'T', 80, 14, 10, 11, 1, 10 },
	{ 'U', 90, 14, 10, 11, 1, 10 },
	{ 'V', 144, 0, 11, 11, 1, 10 },
	{ 'W', 71, 0, 14, 11, 1, 10 },
	{ 'X', 122, 0, 11, 11, 2, 10 },
	{ 'Y', 133, 0, 11, 11, 2, 10 },
	{ 'Z', 232, 0, 11, 11, 2, 10 },
	{ '[', 28, 0, 6, 13, 1, 10 },
	{ '\\', 47, 25, 7, 11, 2, 10 },
	{ ']', 52, 0, 5, 13, 1, 10 },
	{ '^', 236, 25, 8, 7, 1, 10 },
	{ '_', 37, 36, 11, 3, 2, 0 },
	{ '`', 32, 36, 5, 4, 1, 10 },
	{ 'a', 136, 25, 9, 9, 1, 8 },
	{ 'b', 235, 14, 9, 11, 1, 10 },
	{ 'c', 118, 25, 9, 9, 1, 8 },
	{ 'd', 217, 14, 9, 11, 1, 10 },
	{ 'e', 154, 25, 9, 9, 1, 8 },
	{ 'f', 9, 25, 8, 11, 2, 10 },
	{ 'g', 145, 14, 9, 11, 1, 8 },
	{ 'h', 118, 14, 9, 11, 1, 10 },
	{ 'i', 65, 25, 5, 11, 1, 10 },
	{ 'j', 40, 0, 6, 13, 2, 10 },
	{ 'k', 127, 14, 9, 11, 1, 10 },
	{ 'l', 75, 25, 5, 11, 1, 10 },
	{ 'm', 97, 25, 12, 9, 1, 8 },
	{ 'n', 172, 25, 9, 9, 1, 8 },
	{ 'o', 163, 25, 9, 9, 1, 8 },
	{ 'p', 154, 14, 9, 11, 1, 8 },
	{ 'q', 0, 25, 9, 11, 1, 8 },
	{ 'r', 197, 25, 7, 9, 1, 8 },
	{ 's', 181, 25, 8, 9, 1, 8 },
	{ 't', 33, 25, 7, 11, 2, 10 },
	{ 'u', 127, 25, 9, 9, 1, 8 },
	{ 'v', 189, 25, 8, 9, 1, 8 },
	{ 'w', 85, 25, 12, 9, 2, 8 },
	{ 'x', 109, 25, 9, 9, 2, 8 },
	{ 'y', 17, 25, 8, 11, 1, 8 },
	{ 'z', 145, 25, 9, 9, 2, 8 },
	{ '{', 21, 0, 7, 13, 2, 10 },
	{ '|', 57, 0, 5, 13, 1, 10 },
	{ '}', 14, 0, 7, 13, 1, 10 },
	{ '~', 23, 36, 9, 4, 1, 6 },
};


static Character characters_Times[] = {
	{ ' ', 179, 77, 3, 3, 1, 1 },
	{ '!', 205, 54, 7, 23, -2, 22 },
	{ '"', 66, 77, 13, 11, 0, 22 },
	{ '#', 471, 31, 18, 23, 1, 22 },
	{ '$', 141, 0, 16, 26, 0, 23 },
	{ '%', 363, 0, 28, 23, 1, 22 },
	{ '&', 418, 0, 27, 23, 1, 22 },
	{ '\'', 87, 77, 6, 11, 0, 22 },
	{ '(', 96, 0, 12, 29, 0, 22 },
	{ ')', 108, 0, 12, 29, 1, 22 },
	{ '*', 35, 77, 14, 15, -1, 23 },
	{ '+', 233, 54, 20, 19, 1, 20 },
	{ ',', 79, 77, 8, 11, 0, 5 },
	{ '-', 148, 77, 11, 5, 0, 9 },
	{ '.', 142, 77, 6, 6, -1, 5 },
	{ '/', 251, 0, 13, 24, 2, 23 },
	{ '0', 489, 31, 18, 23, 1, 22 },
	{ '1', 193, 54, 12, 23, -2, 22 },
	{ '2', 70, 54, 17, 23, 1, 22 },
	{ '3', 135, 54, 15, 23, 0, 22 },
	{ '4', 453, 31, 18, 23, 1, 22 },
	{ '5', 103, 54, 16, 23, 0, 22 },
	{ '6', 53, 54, 17, 23, 0, 22 },
	{ '7', 36, 54, 17, 23, 1, 22 },
	{ '8', 87, 54, 16, 23, 0, 22 },
	{ '9', 119, 54, 16, 23, 0, 22 },
	{ ':', 28, 77, 7, 16, -1, 15 },
	{ ';', 224, 54, 9, 21, 0, 15 },
	{ '<', 273, 54, 20, 17, 1, 19 },
	{ '=', 93, 77, 20, 9, 1, 15 },
	{ '>', 253, 54, 20, 17, 1, 19 },
	{ '?', 165, 54, 15, 23, 0, 22 },
	{ '@', 0, 0, 30, 31, 0, 23 },
	{ 'A', 445, 0, 27, 23, 2, 22 },
	{ 'B', 271, 31, 22, 23, 1, 22 },
	{ 'C', 226, 31, 23, 23, 1, 22 },
	{ 'D', 177, 31, 25, 23, 1, 22 },
	{ 'E', 249, 31, 22, 23, 1, 22 },
	{ 'F', 434, 31, 19, 23, 1, 22 },
	{ 'G', 152, 31, 25, 23, 1, 22 },
	{ 'H', 77, 31, 25, 23, 1, 22 },
	{ 'I', 180, 54, 13, 23, 1, 22 },
	{ 'J', 150, 54, 15, 23, 1, 22 },
	{ 'K', 26, 31, 26, 23, 1, 22 },
	{ 'L', 293, 31, 22, 23, 1, 22 },
	{ 'M', 332, 0, 31, 23, 1, 22 },
	{ 'N', 0, 31, 26, 23, 2, 22 },
	{ 'O', 52, 31, 25, 23, 1, 22 },
	{ 'P', 396, 31, 19, 23, 1, 22 },
	{ 'Q', 47, 0, 25, 29, 1, 22 },
	{ 'R', 202, 31, 24, 23, 1, 22 },
	{ 'S', 18, 54, 18, 23, 0, 22 },
	{ 'T', 336, 31, 21, 23, 1, 22 },
	{ 'U', 391, 0, 27, 23, 2, 22 },
	{ 'V', 127, 31, 25, 23, 1, 22 },
	{ 'W', 299, 0, 33, 23, 1, 22 },
	{ 'X', 472, 0, 26, 23, 2, 22 },
	{ 'Y', 102, 31, 25, 23, 1, 22 },
	{ 'Z', 315, 31, 21, 23, 1, 22 },
	{ '[', 131, 0, 10, 29, -1, 22 },
	{ '\\', 264, 0, 13, 24, 2, 23 },
	{ ']', 120, 0, 11, 29, 1, 22 },
	{ '^', 49, 77, 17, 13, 1, 22 },
	{ '_', 159, 77, 20, 4, 2, -4 },
	{ '`', 133, 77, 9, 7, 0, 22 },
	{ 'a', 442, 54, 17, 16, 1, 15 },
	{ 'b', 215, 0, 19, 24, 2, 23 },
	{ 'c', 475, 54, 16, 16, 1, 15 },
	{ 'd', 196, 0, 19, 24, 1, 23 },
	{ 'e', 459, 54, 16, 16, 1, 15 },
	{ 'f', 234, 0, 17, 24, 1, 23 },
	{ 'g', 0, 54, 18, 23, 1, 15 },
	{ 'h', 157, 0, 20, 24, 2, 23 },
	{ 'i', 288, 0, 11, 24, 1, 23 },
	{ 'j', 30, 0, 12, 31, 4, 23 },
	{ 'k', 177, 0, 19, 24, 1, 23 },
	{ 'l', 277, 0, 11, 24, 1, 23 },
	{ 'm', 293, 54, 28, 16, 1, 15 },
	{ 'n', 348, 54, 20, 16, 2, 15 },
	{ 'o', 424, 54, 18, 16, 1, 15 },
	{ 'p', 377, 31, 19, 23, 2, 15 },
	{ 'q', 415, 31, 19, 23, 1, 15 },
	{ 'r', 0, 77, 15, 16, 2, 15 },
	{ 's', 15, 77, 13, 16, 0, 15 },
	{ 't', 212, 54, 12, 21, 1, 20 },
	{ 'u', 368, 54, 20, 16, 2, 15 },
	{ 'v', 406, 54, 18, 16, 1, 15 },
	{ 'w', 321, 54, 27, 16, 2, 15 },
	{ 'x', 388, 54, 18, 16, 1, 15 },
	{ 'y', 357, 31, 20, 23, 2, 15 },
	{ 'z', 491, 54, 16, 16, 1, 15 },
	{ '{', 84, 0, 12, 29, -3, 22 },
	{ '|', 42, 0, 5, 30, -1, 23 },
	{ '}', 72, 0, 12, 29, -1, 22 },
	{ '~', 113, 77, 20, 7, 1, 12 },
};
Character GetCharacter(int ASCII);
class Font
{
public:

	Font();
	~Font();
};

static FontType Arial = { "Arial", 12, 0, 0, 256, 64, 95, characters_Arial };
static FontType Times = { "Times", 32, 0, 0, 512, 128, 95, characters_Times };